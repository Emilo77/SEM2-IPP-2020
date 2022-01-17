# Testy do dużego zadania części 1

## Instalacja

Wchodzicie do folderu `src/`, później
```
git clone https://gitlab.com/mimuw-ipp-2021/testy-duze-zadanie-1
cp testy-duze-zadanie-1/CMakeLists.txt ..
cp testy-duze-zadanie-1/poly_example.c .
```

Na nowo wykonujecie `cmake` (ponieważ zmieniliście `CMakeLists.txt`), potem `make clean`.

Jak jest potrzeba to można zakomentować wywołanie danego testu w pliku `testy-duze-zadanie-1/test.h`.

## Drobne uwagi

Przypominam by nie zadawać pytań na forum jak nie trzeba (więcej informacji w pliku `LICENSE`).
Powinien gdzieś być docs z listą pytań i naszych odpowiedzi, tam można zadawać pytania zamiast na Moodlu.

Można kliknąć na dzwoneczek i ustawić powiadomienia by wysyłało maila jak coś w testach się zmieni.

## Dodawanie swoich testów

Najpierw forkujecie repozytorium przyciskiem `Fork`
(tworzy Wasze własne repo z własnym branchem, możecie robić co chcecie i jak coś zepsujecie to bez problemu można usunąć forka, nie ma to żadnego efektu na oryginalne repo z testami).
Tam edytujecie i commitujecie pliki 
(czyli zazwyczaj to będzie stworzenie odpowiedniego pliku `X.h` z funkcją o nazwie `void X()`, a potem wystarczy dodać wywołanie funkcji `X` w pliku `test.h`. Wszystkie inne funkcje w pliku `X.h` mają być `static`).
Jak plik jest za duży to dzielimy go na `X.h` oraz `X.c` i dodajemy odpowiedni plik do `CMakeExtensions.txt`.

Jak już wszystko będzie skończone to gdzieś na gitlabie powinien być przycisk `Merge Request`.
Klikacie go, wpisujecie jakiś sensowny tytuł i ja wtedy dostanę powiadomienie. 
Jak wszystko będzie wyglądać dobrze to zmerguję do głównego repo z testami.

Zawsze można na chacie się dopytać jak jakiś krok jest trudny.

Proponuję też, by nazwy plików mówiły jak najwięcej o teście (zawsze też można dodać komentarz w pliku `X.h` dokładniej wyjaśniający co dany test testuje i po co istnieje). 
Jak jakiś test ma testować jakiś corner case to może się np. zaczynać słowami `corner_case_`,
albo np. test wydajnościowy może być `performance_`. 
Przypominam, że testy nie mają odwoływać się do zmiennych w strukturach, mają tylko i wyłącznie korzystać z funkcji w pliku `poly.h`.


