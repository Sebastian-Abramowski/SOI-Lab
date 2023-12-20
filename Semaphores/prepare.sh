gcc -o test_queue test_queue.c msg_queue.c
gcc -o create_shared_memory put.c msg_queue.c
gcc -o show_queues show_queues.c msg_queue.c
gcc -o client client.c msg_queue.c utils.c
gcc -o creation creation.c msg_queue.c utils.c
gcc -o delivery delivery.c msg_queue.c utils.c
gcc -o delete_shared_memory clear_sm.c msg_queue.c
