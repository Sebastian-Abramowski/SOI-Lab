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

- to samo co w poprzednim, ale z wykorzystaniem monitorów
- jeden klient ma być priorytetowy
- porównaj wyniki z wynikami z semaforów
- zobacz różnice w obrabianiu zleceń z priorytetem
- porównać czasy przetwarzania zamówień zwykłych z priorytetowymi w trzech sytuacjach

Założenie:

- w monitorze będzie znajdowała się zmienna warunkowo o nazwie priority, która będzie pilnować, aby w kolejce w jednej chwili znajdowało się tylko jedno zlecenie priorytetowe, kiedy zlecenie priorytetowe zostanie wyciągniete z kolejki, następne będzie mogło do niej trafić

Wskazówki:

- gdy wywołujemy signal oraz jakiś wątek czeka na danym warunku, to ten oczekujący wątek od razu zaczyna kontynuować swoje
  wykonywanie, a my w naszym poprzendim wątku próbujemy drugi raz otrzymać pozwolenie wejścia do monitora
- gdy wątek, który został odblokowany, zakończy swoje działanie (przez leave() w monitorze), to wtedy w naszym poprzednim
  wątku drugi raz wchodzimy to monitora i od razu z niego wychodzimy
- czyli po wykonaniu signal, stan naszej kolejki może się zmienić
- gdy wywołujemy signal oraz żaden wątek nie czeka na danym warunku to po prostu lecimy dalej
- jeśli kilka wątków jest zablokowane na tym samym warunku, to nie ma pewności, który wątek zacznie się wykonywać jeśli warunek zostanie spełniony (to czy wstawianie z priorytetem działa, można zaobserwować, czy czas wstawiania jest regularnie mniejszy, gdy jeszcze nie dochodziło do zapełnienia kolejki)

Uwagi do kodu:

- określenie czy odbierany przedmiot przez kuriera był zleceniem priorytetowym, działa wtedy gdy kolejka zleceń jest trochę zapełniona, jeśli cały czas w kolejce zleceń byłyby 1-2 elementy, to wtedy może przy wyjęciu z magazynu zlecenia, może ono nie być określone jako priorytetowe - nie robi to w naszym przypadku problemu (wtedy i tak zlecenia niepriorytetowe mają podobny czas obrobienia do priorytetowych)
- idealnym rozwiązaniem byłyby kolejki, które przetrzymywałyby instancje jakieś klasy, a nie zwykłe inty, ale wtedy trzeba byłoby więcej zmieniać w porównaniu do poprzedniego zadania

## Zad. 5 - zarządzanie pamięcią

- zaimplementować algorytm przydziału pamięci worst fit
- dodać wywołanie systemowe zmieniające algorytm przydziału pamięci (albo worst fit albo first fit)
- dodaj wywołanie systemowe przedstawiające dziury w pamięci
- wykonaj podane w poleceniu testy
