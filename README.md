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
- BATCH_GROUP (1) - grupa oznaczająca procesy wsadowe
- testy zostaną przeprowadzone przez robienie procesów poleceniem './seepid &' oraz './tstsetgroup pid_nr group_nr', te pliki wykonywalne są robione przez odpalenie './make_exe.sh'

Moje założenia:

- jeśli zmieniamy grupe procesu, to traktujemy go jako nowy proces

## Zad. 3 - synchronizacja procesów z wykorzystaniem semaforów

Zadanie:

- zasymuluj synchronizację pomiędzy klientami, który wytwarzają zlecenia i procesami wytwórczami, które te zlecenia wytwarzają
- poza tym jest także proces kuriera, który odbiera co określony czas przetworzone zlecenia z magazynu
- musisz korzystać z semaforów Linuxowych
- trzeba także w jakiś sposób mieć dostęp do czasu przetworzenia zlecenie (od momentu przyjęcia zlecenia do odebrania przez kuriera)
- przeprowadź odpowiednie testy, abyś był w stanie odpowiedzieć na trzy pytania dotyczące zadania znajdujące się pliku

Konkretnie do wykonanego zadania:

- zostały stworzone dodatkowe dwie dodatkowe kolejki:
- TIME_OF_ORDER_IN - kolejka o tym indeksie przetrzymuje czasy przyjęcia zlecenia (jest zajęta w jednakowym stopniu jak kolejka Orders)
- TIME_OF_ORDER_IN2 - kolejka o tym indeksie przetrzymuje czasy przyjęcia zlecenia, ale tych które został wyjęte z kolejki Orders (czyli są w trakcie przetwarzania) (jest zajęta w jednakowym stopniu jak kolejka Warehouse) - na jej podstawie można obliczyć całkowity czas przetwarzania zlecenia w sekundach, przykład wyników znajduje się w delivery_info.txt
- jeśli robimy kill na procesie, który ma otwarty plik, to system operacyjny zamknie ten plik i nie powinno być problemów (ewentualnie, że jakieś najnowsze dane nie zostaną zapisane, ale u nas to nie problem)

## Zad. 4 - synchronizacja procesów z wykorzystaniem monitorów

- TBA
