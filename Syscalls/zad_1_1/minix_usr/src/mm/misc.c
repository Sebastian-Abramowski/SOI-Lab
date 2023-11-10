/* Miscellaneous system calls.				Author: Kees J. Bot
 *								31 Mar 2000
 *
 * The entry points into this file are:
 * 	 do_max_children: returns the maximum number of children of the process
 *   do_get_max_children_pid: returns the process id of the process with the most children
 *   do_reboot: kill all processes, then reboot system
 *   do_svrctl: memory manager control
 */

#include "mm.h"
#include <minix/callnr.h>
#include <signal.h>
#include <sys/svrctl.h>
#include "mproc.h"
#include "param.h"

/*=====================================================================*
 *			    do_max_children, do_get_max_children_pid               *
 *=====================================================================*/
int count_direct_children( int process_index ) 
{
	int children = 0;
	int i = 0;
	for (i = 0; i < NR_PROCS; ++i) {
		if ((mproc[i].mp_flags & IN_USE) && process_index != i) {
			if (mproc[i].mp_parent == process_index) {
				++children;
			}
		}
	}
		
	return(children);
}

void max_children( int * children, pid_t * who)
{
	int children_max = -1;
	pid_t max_children_pid = -1;
	int counted_children = 0;
	int proc_nr = 0;
	for (proc_nr = 0; proc_nr < NR_PROCS; ++proc_nr) {
		if (mproc[proc_nr].mp_flags & IN_USE) {
			counted_children = count_direct_children( proc_nr );
			if (counted_children > children_max) {
				children_max = counted_children;
				max_children_pid = mproc[proc_nr].mp_pid;
			}
		}
	}
	*children = children_max;
	*who = max_children_pid;
}

PUBLIC int do_max_children()
{
	int num_children = -1;
	pid_t parent_process_id = -1;
	max_children( & num_children, & parent_process_id );
	return(num_children);
}

PUBLIC int do_get_max_children_pid()
{
	int num_children = -1;
	pid_t parent_process_id = -1;
	max_children( & num_children, & parent_process_id );
	return((int)parent_process_id);
}


/*=====================================================================*
 *			    do_reboot				       *
 *=====================================================================*/
PUBLIC int do_reboot()
{
  register struct mproc *rmp = mp;
  char monitor_code[32*sizeof(char *)];

  if (rmp->mp_effuid != SUPER_USER) return(EPERM);

  switch (reboot_flag) {
  case RBT_HALT:
  case RBT_REBOOT:
  case RBT_PANIC:
  case RBT_RESET:
	break;
  case RBT_MONITOR:
	if (reboot_size >= sizeof(monitor_code)) return(EINVAL);
	if (sys_copy(who, D, (phys_bytes) reboot_code,
		MM_PROC_NR, D, (phys_bytes) monitor_code,
		(phys_bytes) (reboot_size+1)) != OK) return(EFAULT);
	if (monitor_code[reboot_size] != 0) return(EINVAL);
	break;
  default:
	return(EINVAL);
  }

  /* Kill all processes except init. */
  check_sig(-1, SIGKILL);

  tell_fs(EXIT, INIT_PROC_NR, 0, 0);	/* cleanup init */
  tell_fs(EXIT, MM_PROC_NR, 0, 0);	/* cleanup for myself */

  tell_fs(SYNC,0,0,0);

  sys_abort(reboot_flag, MM_PROC_NR, monitor_code, reboot_size);
  /* NOTREACHED */
}

/*=====================================================================*
 *			    do_svrctl				       *
 *=====================================================================*/
PUBLIC int do_svrctl()
{
  int req;
  vir_bytes ptr;

  req = svrctl_req;
  ptr = (vir_bytes) svrctl_argp;

  /* Is the request for the kernel? */
  if (((req >> 8) & 0xFF) == 'S') {
	return(sys_sysctl(who, req, mp->mp_effuid == SUPER_USER, ptr));
  }

  switch(req) {
  case MMSIGNON: {
	/* A user process becomes a task.  Simulate an exit by
	 * releasing a waiting parent and disinheriting children.
	 */
	struct mproc *rmp;
	pid_t pidarg;

	if (mp->mp_effuid != SUPER_USER) return(EPERM);

	rmp = &mproc[mp->mp_parent];
	tell_fs(EXIT, who, 0, 0);

	pidarg = rmp->mp_wpid;
	if ((rmp->mp_flags & WAITING) && (pidarg == -1
		|| pidarg == mp->mp_pid || -pidarg == mp->mp_procgrp))
	{
		/* Wake up the parent. */
		rmp->reply_res2 = 0;
		setreply(mp->mp_parent, mp->mp_pid);
		rmp->mp_flags &= ~WAITING;
	}

	/* Disinherit children. */
	for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++) {
		if (rmp->mp_flags & IN_USE && rmp->mp_parent == who) {
			rmp->mp_parent = INIT_PROC_NR;
		}
	}

	/* Become like MM and FS. */
	mp->mp_pid = mp->mp_procgrp = 0;
	mp->mp_parent = 0;
	return(OK); }

  case MMSWAPON: {
	struct mmswapon swapon;

	if (mp->mp_effuid != SUPER_USER) return(EPERM);

	if (sys_copy(who, D, (phys_bytes) ptr,
		MM_PROC_NR, D, (phys_bytes) &swapon,
		(phys_bytes) sizeof(swapon)) != OK) return(EFAULT);

	return(swap_on(swapon.file, swapon.offset, swapon.size)); }

  case MMSWAPOFF: {
	if (mp->mp_effuid != SUPER_USER) return(EPERM);

	return(swap_off()); }

  default:
	return(EINVAL);
  }
}
