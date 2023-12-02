# SOI Lab

Zadania na SOI (Systemy operacyjne)

## Zad. 1 - wywołania systemowe

Zadanie:

- Wywołanie zwracające PID procesu, który ma najwięcej dzieci
- Wywołanie zwracające liczbę dzieci procesu, który ma ich najwięcej
- Wywołanie zwracające PID procesu, który ma największa liczbę potomków
- Wywołanie zwracające liczbę potomoków procesu, ktory ma ich najwięcej

Wskazówki:

- Procesu o PID 0 można nie brać pod uwagę, służy on do czegoś związanego z zarządzania pamięcią i zazwyczaj będzie miał najwięcej potomków
- Proces o PID 1 to INIT, trzeba pamiętać, że procesy, które straciły rodziców są do niego przypisywane, zazwyczaj będzie miał najwięcej dzieci

## Zad. 2 - szeregowanie procesów

Zadanie:

- podziel procesy użytkownika na dwie klasy: procesy wsadowe i normalne
- procesy wsadowe wykonują się tylko jak nie ma normalnych, w kolejności newest-first
- proces wsadowy wykonuje się do momentu zakończenia procesu (nie zmieniają się one co jakiś kwant czasu)
- procesy normalne mają pierwszeństwo przed wsadowymi, proces normalny traci i zyskuje dostęp do procesora co jakiś czas
- procesy normalne mają się zachowywać w podobny sposób do domyślnej implementacji szeregowania procesów użytkowych w Minixie

Uwagi do kodu:

- kod wkładający proces do kolejki procesów z funkcji 'ready()' z kernel/proc.c jest powtórzony w taskcallu 'do_setgroup' w pliku kernel/system.c,
  nie chciałem już tego dotykać póki wszystko działało
- testy zostać przeprowadzone przez robienie procesów poleceniem './seepid &' oraz './tstsetgroup pid_nr group_nr', te pliki wykonywalne są robione przez odpalenie './make_exe.sh'
