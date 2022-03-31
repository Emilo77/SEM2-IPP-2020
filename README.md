# Duże zadanie, część 1

## Wielomiany

Tegoroczne duże zadanie polega na zaimplementowaniu operacji na wielomianach rzadkich wielu zmiennych o współczynnikach całkowitych. Zmienne wielomianu oznaczamy *x<sub>0</sub>*, *x<sub>1</sub>*, *x<sub>2</sub>* itd. Definicja wielomianu jest rekurencyjna. Wielomian jest sumą jednomianów postaci *px<sup>n</sup><sub>0</sub>*, gdzie *n* jest wykładnikiem tego jednomianu będącym nieujemną liczbą całkowitą, a *p* jest współczynnikiem, który jest wielomianem. Współczynnik w jednomianie zmiennej *x<sub>i</sub>* jest sumą jednomianów zmiennej *x<sub>i+1</sub>*.
Rekurencja kończy się, gdy współczynnik jest liczbą (czyli wielomianem stałym), a nie sumą kolejnych jednomianów. Wykładniki jednomianów w każdej z rozważanych sum są parami różne. Wielomiany są rzadkie, co oznacza, że stopień wielomianu może być znacznie większy niż liczba składowych jednomianów.

## Część 1 zadania

Jako pierwszą część zadania należy zaimplementować bibliotekę podstawowych operacji na wielomianach rzadkich wielu zmiennych. Opis funkcji znajduje się w pliku `poly.h` w formacie komentarzy dla programu `doxygen`.

## Dostarczamy

W repozytorium `https://git.mimuw.edu.pl/IPP-login.git` (gdzie `login` to identyfikator używany do logowania w laboratorium komputerowym) znajduje się szablon implementacji rozwiązania tego zadania. Znajdują się tam następujące pliki:

- `src/poly.h` – deklaracja interfejsu biblioteki wraz z jej dokumentacją w formacie doxygen,
- `src/poly_example.c` – przykłady użycia biblioteki,
- `CMakeLists.txt` – plik konfiguracyjny programu `cmake`,
- `Doxyfile.in`– plik konfiguracyjny programu `doxygen`,
- `MainPage.dox` – strona główna dokumentacji w formacie `doxygen`.

Zastrzegamy sobie możliwość nanoszenia poprawek do tego szablonu. Będziemy je umieszczać gałęzi `emplate/part1`.

## Wymagamy

Jako rozwiązanie części 1 zadania wymagamy:

- ewentualnego uzupełnienia implementacji lub rozszerzenia interfejsu biblioteki w pliku `src/poly.h`,
- stworzenia pliku `src/poly.c` z implementacją wymaganych funkcji,
- uzupełnienia dokumentacji w formacie `doxygen` tak, aby była przydatna dla programistów rozwijających moduł.

Powinna być możliwość skompilowania rozwiązania w dwóch wersjach: release i debug. Wersję release kompiluje się za pomocą sekwencji poleceń:
```c
mkdir release
cd release
cmake ..
make
make doc
```

Wersję debug kompiluje się za pomocą sekwencji poleceń:

```c
mkdir debug
cd debug
cmake -D CMAKE_BUILD_TYPE=Debug ..
make
make doc
```

W wyniku kompilacji odpowiednio w katalogu `release` lub `debug` powinien powstać plik wykonywalny `poly` oraz dokumentacja. W poleceniu `cmake` powinno być również możliwe jawne określenie wariantu release budowania pliku wynikowego:

```
cmake -D CMAKE_BUILD_TYPE=Release ..
```

Zawartość dostarczonych przez nas plików można modyfikować, o ile nie zmienia to interfejsu biblioteki i zachowuje wymagania podane w treści zadania, przy czym nie wolno usuwać opcji kompilacji: `-std=c11 -Wall -Wextra`. Zmiany mogą dotyczyć np. stylu, dokumentacji, deklaracji typedef, włączania plików nagłówkowych, implementacji funkcji jako `static inline`. Ewentualne dodatkowe pliki źródłowe, będące częścią rozwiązania, należy umieścić w katalogu src. Funkcja main programu musi znajdować się w pliku `src/poly_example.c`, ale zawartość tego pliku nie będzie oceniana w tej części zadania.

## Dodatkowe wymagania i ustalenia

Rozwiązanie zadania powinno być napisane w języku C i korzystać z dynamicznie alokowanych struktur danych. Implementacja powinna być jak najefektywniejsza. Należy unikać zbędnego alokowania pamięci i kopiowania danych.

W interfejsie zostały przyjęte pewne konwencje, które mają ułatwić zarządzanie pamięcią. Dzięki tym konwencjom wiadomo, co jest właścicielem obiektu. Bycie właścicielem obiektu implikuje odpowiedzialność za zwolnienie używanej przez niego pamięci. W przypadku struktur `Poly` i `Mono` zwalnianie pamięci uzyskuje się poprzez wywołania odpowiednio funkcji `PolyDestroy` i `MonoDestroy`.

Podstawową konwencją jest *przekazywanie argumentów przez zmienną*. W języku C do tego celu użyty jest typ wskaźnikowy (np. `const Poly *`). Kod wołający funkcję, której przekazujemy argument przez zmienną, odpowiada za utworzenie odpowiedniego wskaźnika. Może to być wskaźnik na lokalną zmienną, bądź też wskaźnik uzyskany w wyniku alokacji pamięci na stercie (np. przez `malloc`). W tym drugim wypadku trzeba pamiętać, aby zwolnić tę pamięć. Odpowiedzialność za zwolnienie tak uzyskanej pamięci nigdy nie przechodzi na wołaną funkcję.

Przy niektórych funkcjach *argumenty przechodzą* na własność funkcji wołanej. Jest to zaznaczone w komentarzu opisującym daną funkcję. Funkcja przejmuje zawartość pamięci wskazywanej przez przekazany wskaźnik. Zazwyczaj jest to pojedyncza struktura (np. `Poly`, `Mono`) bądź tablica struktur.

Wynikiem niektórych funkcji jest struktura (np. `Poly`, `Mono`). Przyjmujemy tu konwencję otrzymywania wyniku na własność, co oznacza, że kod wołający taką funkcję otrzymuję zwracaną wartość na własność.

Przykłady przekazywania własności i zwalniania pamięci przez ostatniego właściciela:
```c
{
  Poly p1 = ...
  PolyDestroy(&p1);
}
{
  Poly p1 = ...
  Mono m1 = MonoFromPoly(&p1, 7); // przekazanie własności p1
  MonoDestroy(&m1);
}
{
  Poly p1 = ...
  Mono m1 = MonoFromPoly(&p1, 7); // przekazanie własności p1
  Poly p2 = PolyAddMonos(1, &m1); // przekazanie własności m1
  PolyDestroy(&p2);
}
```

## Obsługa błędów krytycznych

Jeśli wystąpi błąd krytyczny, np. zabraknie pamięci, program powinien zakończyć się awaryjnie kodem 1. Niezmienniki i warunki wstępne należy sprawdzać za pomocą asercji.

## Oddawanie rozwiązania

Rozwiązanie należy oddawać przez wspomniane wyżej repozytorium `git`. W repozytorium mają się znaleźć wszystkie pliki niezbędne do zbudowania pliku wykonywalnego oraz dokumentacji. W repozytorium nie wolno umieszczać plików binarnych ani tymczasowych. W Moodle jako rozwiązanie należy umieścić tekst zawierający identyfikator commitu finalnej wersji rozwiązania, na przykład:
```
518507a7e9ea50e099b33cb6ca3d3141bc1d6638
```
Rozwiązanie należy zatwierdzić (`git commit`) i wysłać do repozytorium (`git push`) przed terminem podanym w Moodle.

## Punktacja

Za w pełni poprawne rozwiązanie zadania implementujące wszystkie wymagane funkcjonalności można zdobyć maksymalnie 20 punktów. Od tej oceny będą odejmowane punkty za poniższe uchybienia:

- Za problemy ze skompilowaniem rozwiązania można stracić wszystkie punkty.
- Za każdy test, którego program nie przejdzie, traci się do 1 punktu.
- Za problemy z zarządzaniem pamięcią można stracić do 6 punktów.
- Za niezgodną ze specyfikacją strukturę plików w rozwiązaniu, niezgodne ze specyfikacją nazwy plików w rozwiązaniu lub umieszczenie w repozytorium niepotrzebnych albo tymczasowych plików można stracić do 4 punktów.
- Za złą jakość kodu, brzydki styl kodowania można stracić do 4 punktów.
- Za ostrzeżenia wypisywane przez kompilator można stracić do 2 punktów.
- Za braki w dokumentacji można stracić do 2 punktów.

# Duże zadanie, część 2

Jako drugą część zadania należy zaimplementować kalkulator działający na wielomianach i stosujący odwrotną notację polską.

# Kalkulator

Program kalkulatora czyta dane wierszami ze standardowego wejścia. Wiersz zawiera wielomian lub polecenie do wykonania.

Wielomian reprezentujemy jako stałą, jednomian lub sumę jednomianów. Stała jest liczbą całkowitą. Jednomian reprezentujemy jako parę `(coeff,exp)`, gdzie współczynnik `coeff` jest wielomianem, a wykładnik `exp` jest liczbą nieujemną. Do wyrażenia sumy używamy znaku `+`. Jeśli wiersz zawiera wielomian, to program wstawia go na stos. Przykłady poprawnych wielomianów:

```
0
1
-2
(0,0)
(1,0)
(-2,0)
(1,1)
(1,0)+(1,2)
(1,2)+(1,0)
(1,2)+(-1,2)
(1,2)+(-2,2)
((1,2),15)+(-7,8)
(3,1)+(((4,4),100),2)
```
Kalkulator wykonuje następujące polecenia:

- `ZERO` – wstawia na wierzchołek stosu wielomian tożsamościowo równy zeru;
- `IS_COEFF` – sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem – wypisuje na standardowe wyjście `0` lub `1`;
- `IS_ZERO` – sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru – wypisuje na standardowe wyjście `0` lub `1`;
- `CLONE` – wstawia na stos kopię wielomianu z wierzchołka;
- `ADD` – dodaje dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich sumę;
- `MUL` – mnoży dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich iloczyn;
- `NEG` – neguje wielomian na wierzchołku stosu;
- `SUB` – odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem, usuwa je i wstawia na wierzchołek stosu różnicę;
- `IS_EQ` – sprawdza, czy dwa wielomiany na wierzchu stosu są równe – wypisuje na standardowe wyjście `0` lub `1`;
- `DEG` – wypisuje na standardowe wyjście stopień wielomianu (`−1` dla wielomianu tożsamościowo równego zeru);
- `DEG_BY idx` – wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną o numerze idx (`−1` dla wielomianu tożsamościowo równego zeru);
- `AT x` – wylicza wartość wielomianu w punkcie `x`, usuwa wielomian z wierzchołka i wstawia na stos wynik operacji;
- `PRINT` – wypisuje na standardowe wyjście wielomian z wierzchołka stosu;
- `POP` – usuwa wielomian z wierzchołka stosu.

Wypisywany poleceniem `PRINT` wielomian powinien mieć jak najprostszą postać. Wykładniki wypisywanych jednomianów nie powinny się powtarzać. Jednomiany powinny być posortowane rosnąco według wykładników. Podane wyżej wielomiany powinny zostać wypisane następująco:
```
0
1
-2
0
1
-2
(1,1)
(1,0)+(1,2)
(1,0)+(1,2)
0
(-1,2)
(-7,8)+((1,2),15)
(3,1)+(((4,4),100),2)
```

## Sprawdzanie poprawności danych wejściowych i obsługa błędów

Program nie powinien zakładać maksymalnej długości wiersza. Poprawny wiersz nie zawiera żadnych dodatkowych białych znaków oprócz pojedynczej spacji separującej parametr poleceń `AT` i `DEG_BY` od polecenia. Program wypisuje komunikaty o błędach na standardowe wyjście błędów. Poniżej w oznacza numer wiersza, a `\n` – znak przejścia do nowego wiersza. Wiersze i kolumny numerujemy od `1`.

Ignorujemy wiersze zaczynające się znakiem `#` i puste. Jeśli wiersz zaczyna się małą lub wielką literą alfabetu angielskiego, to uznajemy, że zawiera polecenie. W pozostałych przypadkach uznajemy, że wiersz opisuje wielomian.

Jeśli program wykryje niepoprawną nazwę polecenia, wypisuje:
```
ERROR w WRONG COMMAND\n
```
Jeśli w poleceniu `DEG_BY` nie podano parametru lub jest on niepoprawny, program wypisuje:
```
ERROR w DEG BY WRONG VARIABLE\n
```
Jeśli w poleceniu `AT` nie podano parametru lub jest on niepoprawny, program wypisuje:
```
ERROR w AT WRONG VALUE\n
```
Jeśli na stosie jest za mało wielomianów, aby wykonać polecenie, program wypisuje:
```
ERROR w STACK UNDERFLOW\n
```
Jeśli program wykryje błąd podczas parsowania wielomianu, wypisuje
```
ERROR w WRONG POLY\n
```
Wartość współczynnika jednomianu lub parametru polecenia `AT` uznajemy za niepoprawną, jeśli jest mniejsza od `-9223372036854775808` lub większa od `9223372036854775807`.

Wartość wykładnika jednomianu uznajemy za niepoprawną, jeśli jest mniejsza od `0` lub większa od `2147483647`.

Wartość parametru polecenia `DEG_BY` uznajemy za niepoprawną, jeśli jest mniejsza od 0 lub większa od `18446744073709551615`.

Program może, ale nie musi, akceptować liczby z dodatkowymi wiodącymi zerami. Znak `+` służy tylko do wyrażania sumy jednomianów i nie może poprzedzać liczby.

## Przykłady użycia

Do treści zadania dołączone jest archiwum z przykładami użycia.

## Rozwiązanie

Rozwiązanie części 2 zadania powinno korzystać z własnego rozwiązania części 1. Jako rozwiązanie części 2 zadania wymagamy:

- umieszczenia kodu źródłowego implementacji w katalogu `src`,
- uzupełnienia dokumentacji dla programu `doxygen`,
- dostosowania pliku konfiguracyjnego dla programu `cmake`.

Obowiązują wymagania sformułowane w punkcie „Wymagamy” w treści pierwszej części zadania. Zmieniamy jedno wymaganie: teraz funkcja `main` powinna być zawarta w pliku `src/calc.c`. Plik `src/poly_example.c` może pozostać niezmieniony. Pozostawiamy natomiast wymaganie, że w wyniku kompilacji powinien powstać plik wykonywalny `poly`.

Obowiązują wymagania sformułowane w punkcie „Obsługa błędów krytycznych” w treści pierwszej części zadania.

## Oddawanie rozwiązania

Rozwiązanie należy oddawać, podobnie jak część 1, przez repozytorium [git](https://moodle.mimuw.edu.pl/mod/page/view.php?id=45344). W repozytorium mają się znaleźć wszystkie pliki niezbędne do zbudowania pliku wykonywalnego oraz dokumentacji. **W repozytorium nie wolno umieszczać plików binarnych ani tymczasowych.** W Moodle jako rozwiązanie należy umieścić tekst zawierający identyfikator commitu finalnej wersji rozwiązania, na przykład:
```
518507a7e9ea50e099b33cb6ca3d3141bc1d6638
```
Rozwiązanie należy zatwierdzić (``git commit``) i wysłać do repozytorium (``git push``) przed terminem podanym w Moodle.

## Punktacja

Za w pełni poprawne rozwiązanie zadania implementujące wszystkie wymagane funkcjonalności można zdobyć maksymalnie 20 punktów. Od tej oceny będą odejmowane punkty za poniższe uchybienia:

- Za problemy ze skompilowaniem rozwiązania można stracić wszystkie punkty.
- Za każdy test, którego program nie przejdzie, traci się do 1 punktu.
- Za problemy z zarządzaniem pamięcią można stracić do 6 punktów.
- Za niezgodną ze specyfikacją strukturę plików w rozwiązaniu, niezgodne ze specyfikacją nazwy plików w rozwiązaniu lub umieszczenie w repozytorium niepotrzebnych albo tymczasowych plików można stracić do 4 punktów.
- Za złą jakość kodu, brzydki styl kodowania można stracić do 4 punktów.
- Za ostrzeżenia wypisywane przez kompilator można stracić do 2 punktów.
- Za braki w dokumentacji można stracić do 2 punktów.


# Duże zadanie, część 3

Celem trzeciej części zadania jest dokończenie programu kalkulatora działającego na wielomianach rzadkich wielu zmiennych. Oczekujemy poprawienia ewentualnych błędów z poprzednich części zadania oraz wprowadzenie opisanych poniżej modyfikacji i rozszerzeń. Obowiązują ustalenia z treści poprzednich części zadania i z forum dyskusyjnego dla studentów.

## Konstruowanie wielomianu z tablicy jednomianów

Do stworzonej w części 1 zadania biblioteki operacji na wielomianach poly (pliki `poly.h` i `poly.c`) należy dodać dwie funkcje działające podobnie jak funkcja `PolyAddMonos`, ale ze zmienionym sposobem przekazywania własności tablicy jednomianów:
```c
/**
 * Sumuje listę jednomianów i tworzy z nich wielomian. Przejmuje na własność
 * pamięć wskazywaną przez @p monos i jej zawartość. Może dowolnie modyfikować
 * zawartość tej pamięci. Zakładamy, że pamięć wskazywana przez @p monos
 * została zaalokowana na stercie. Jeśli @p count lub @p monos jest równe zeru
 * (NULL), tworzy wielomian tożsamościowo równy zeru.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return wielomian będący sumą jednomianów
 */
Poly PolyOwnMonos(size_t count, Mono *monos);
/**
 * Sumuje listę jednomianów i tworzy z nich wielomian. Nie modyfikuje zawartości
 * tablicy @p monos. Jeśli jest to wymagane, to wykonuje pełne kopie jednomianów
 * z tablicy @p monos. Jeśli @p count lub @p monos jest równe zeru (NULL),
 * tworzy wielomian tożsamościowo równy zeru.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return wielomian będący sumą jednomianów
 */
Poly PolyCloneMonos(size_t count, const Mono monos[]);
```
Przy implementowaniu tych funkcji należy unikać powtarzania kodu.

Użyte w poniższych przykładach makra `C` i `P` oraz funkcja `M` są zdefiniowane w testach do części 1 zadania, w pliku `poly_test.c`.

## Przykład 1

Funkcja `PolyOwnMonos` przejmuje na własność zarówno zawartość tablicy `monos`, jak i pamięć zajmowaną przez tę tablicę. Wywołanie funkcji `PolyDestroy` musi zwolnić wszystkie zasoby.

```c
Mono *monos = calloc(2, sizeof (Mono));
assert(monos);
monos[0] = M(P(C(-1), 1), 1);
monos[1] = M(P(C(1), 1), 2);
Poly p = PolyOwnMonos(2, monos);
PolyDestroy(&p);
```
## Przykład 2

Funkcja `PolyCloneMonos` nie przejmuje na własność żadnych zasobów i nie modyfikuje zawartości tablicy `monos`. Utworzone wielomiany `p1` i `p2` są takie same. Oprócz wywołań funkcji `PolyDestroy` konieczne są wywołania funkcji `MonoDestroy` i zwolnienie pamięci za pomocą funkcji `free`.

```c
Mono *monos = calloc(2, sizeof (Mono));
assert(monos);
monos[0] = M(P(C(-1), 1), 1);
monos[1] = M(P(C(1), 1), 2);
Poly p1 = PolyCloneMonos(2, monos);
Poly p2 = PolyCloneMonos(2, monos);
PolyDestroy(&p1);
PolyDestroy(&p2);
MonoDestroy(monos + 0);
MonoDestroy(monos + 1);
free(monos);
```

## Składanie wielomianów

Definiujemy operację składania wielomianów. Dany jest wielomian p
oraz k wielomianów _q<sub>0</sub>_, _q<sub>1</sub>_, _q<sub>2</sub>_, …, _q<sub>k - 1</sub>_. Niech l oznacza liczbę zmiennych wielomianu *p* i niech te zmienne są oznaczone odpowiednio _x<sub>0</sub>_, _x<sub>1</sub>_, _x<sub>2</sub>_, …, _x<sub>l−1</sub>_. Wynikiem złożenia jest wielomian _p(q<sub>0</sub>,q<sub>1</sub>,q<sub>2</sub>,…)_, czyli wielomian powstający przez podstawienie w wielomianie _p_ pod zmienną _x<sub>i</sub>_ wielomianu _q<sub>i</sub>_ dla _i=0,1,2,…,min(k,l)−1_. Jeśli _k<l_, to pod zmienne _x<sub>k</sub>, …, x<sub>l−1</sub>_ podstawiamy zera. Na przykład, jeśli _k=0_, to wynikiem złożenia jest liczba _p(0,0,0,…)_.

W celu realizacji operacji składanie wielomianów należy rozszerzyć bibliotekę `poly` o funkcję
```c
Poly PolyCompose(const Poly *p, size_t k, const Poly q[]);
```

Do interfejsu kalkulatora należy dodać polecenie
```
COMPOSE k
```
Polecenie to zdejmuje z wierzchołka stosu najpierw wielomian p, a potem kolejno wielomiany `q[k - 1], q[k - 2], … , q[0]` i umieszcza na stosie wynik operacji złożenia.

Jeśli w poleceniu `COMPOSE` nie podano parametru lub jest on niepoprawny, program powinien wypisać na standardowe wyjście diagnostyczne:

```
ERROR w COMPOSE WRONG PARAMETER\n
```

Wartość parametru polecenia `COMPOSE` uznajemy za niepoprawną, jeśli jest mniejsza od 0 lub większa od `18446744073709551615`.

Jeśli na stosie jest za mało wielomianów, aby wykonać polecenie, program powinien wypisać na standardowe wyjście diagnostyczne:
```
ERROR w STACK UNDERFLOW\n
```

Jak poprzednio w obu przypadkach `w` oznacza numer wiersza, a `\n` – znak przejścia do nowego wiersza.

## Przykład 1

Dla danych wejściowych:
```c
(1,2)
(2,0)+(1,1)
COMPOSE 1
PRINT
(1,3)
COMPOSE 1
PRINT
```

Jako wynik działania programu powinniśmy zobaczyć:
```c
(2,0)+(1,2)
(8,0)+(12,2)+(6,4)+(1,6)
```

Wyjaśnienie do przykładu:

- Pierwsze polecenie `COMPOSE` podstawia wielomian *x<sup>2</sup><sub>0</sub>* pod *x<sub>0</sub>* w wielomianie *(2+x0)*, więc w jego wyniku otrzymujemy wielomian *(2+*x<sup>2</sup><sub>0</sub>*)*.
- Drugie polecenie COMPOSE podstawia wielomian *(2+*x<sup>2</sup><sub>0</sub>*)*
pod *x<sub>0</sub>* w wielomianie *x<sup>3</sup><sub>0</sub>*, więc w jego wyniku otrzymujemy wielomian (*8 + 12x<sup>2</sup><sub>0</sub> + x<sup>4</sup><sub>0</sub>  + x<sup>6</sup><sub>0</sub>).

## Przykład 2

Dla danych wejściowych:
```
(1,4)
((1,0)+(1,1),1)
(((1,6),5),2)+((1,0)+(1,2),3)+(5,7)
COMPOSE 2
PRINT
```
Jako wynik działania programu powinniśmy zobaczyć:
```
(1,12)+((1,0)+(2,1)+(1,2),14)+(5,28)
```

## Przykład 3

Dla danych wejściowych:

```
((1,0)+(1,1),1)
(1,4)
COMPOSE -1
COMPOSE 18446744073709551615
```

Jako wynik działania programu powinniśmy zobaczyć:

```
ERROR 3 COMPOSE WRONG PARAMETER
ERROR 4 STACK UNDERFLOW
```

## Dokumentacja

Dodany kod należy udokumentować w formacie doxygen.

## Modyfikacja skryptu budującego

Należy dodać możliwość utworzenia pliku wykonywalnego z testami biblioteki poly. Czyli na przykład po wykonaniu:

```c
mkdir release
cd release
cmake ..
```
- polecenie `make` tworzy plik wykonywalny `poly` całego kalkulatora,
- polecenie `make` test tworzy plik wykonywalny `poly_test` z testami biblioteki `poly`,
- polecenie `make` doc tworzy dokumentację w formacie `doxygen`.

Funkcja `main` kalkulatora ma się znajdować w pliku `src/calc.c`. Funkcja main uruchamiająca testy biblioteki `poly` ma się znajdować w pliku `src/poly_test.c` – może to być plik z udostępnionymi testami do części 1 zadania i rozszerzony o własne testy. Zawartość tego pliku nie będzie oceniana.

Wskazówka: W pliku `CMakeList.txt` można dodać polecenia

```
# Wskazujemy plik wykonywalny testów biblioteki.
add_executable(test EXCLUDE_FROM_ALL ${TEST_SOURCE_FILES})
set_target_properties(test PROPERTIES OUTPUT_NAME poly_test)
```

definiując uprzednio zmienną `TEST_SOURCE_FILES`.

## Wymagania

Rozwiązanie części 3 zadania powinno korzystać z własnego rozwiązania poprzednich jego części. Obowiązują wszystkie wymagania z poprzednich części zadania, jeśli nie zostały zmienione w tym dokumencie.

**Uwaga: niezmiernie istotne jest, aby przestrzegać opisanej specyfikacji nazw plików.**

## Oddawanie rozwiązania

Rozwiązanie należy oddawać, podobnie jak części 1 i 2, przez repozytorium git. W repozytorium mają się znaleźć wszystkie pliki niezbędne do zbudowania plików wykonywalnych i dokumentacji, i tylko te pliki. **W repozytorium nie wolno umieszczać plików binarnych ani tymczasowych.** W Moodle jako rozwiązanie należy umieścić tekst zawierający identyfikator commitu finalnej wersji rozwiązania, na przykład:

```
518507a7e9ea50e099b33cb6ca3d3141bc1d6638
```

Rozwiązanie należy zatwierdzić (`git commit`) i wysłać do repozytorium (`git push`) przed terminem podanym w Moodle.

## Punktacja

Za w pełni poprawną implementację programu można zdobyć maksymalnie 20 punktów. Od tej oceny będą odejmowane punkty za poniższe uchybienia:

- Za problemy ze skompilowaniem rozwiązania można stracić wszystkie punkty.
- Za każdy test, którego program nie przejdzie, traci się do 1 punktu.
- Za problemy z zarządzaniem pamięcią można stracić do 6 punktów.
- Za niezgodną ze specyfikacją strukturę plików w rozwiązaniu, niezgodne ze specyfikacją nazwy plików w rozwiązaniu lub umieszczenie w repozytorium niepotrzebnych albo tymczasowych plików można stracić do 4 punktów.
- Za złą jakość kodu, brzydki styl kodowania można stracić do 4 punktów.
- Za ostrzeżenia wypisywane przez kompilator można stracić do 2 punktów.
- Za braki w dokumentacji można stracić do 2 punktów.
- Za niezaimplementowanie funkcji `PolyOwnMonos` lub `PolyCloneMonos` można stracić do 2 punktów.
- Za niezaimplementowanie funkcji `PolyCompose` lub polecenia `COMPOSE` można stracić do 2 punktów.

**Rozwiązania należy implementować samodzielnie pod rygorem niezaliczenia przedmiotu. Zarówno korzystanie z cudzego kodu, jak i prywatne lub publiczne udostępnianie własnego kodu jest zabronione.**
