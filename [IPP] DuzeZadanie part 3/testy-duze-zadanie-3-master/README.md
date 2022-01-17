# Testy do dużego zadania części 3

## Instalacja

Są tutaj zarówno testy w postaci .in/.out/.err (używacie swojego `test.sh` z małego zadania),
jak i plik `poly_test.c` z części drugiej, ze zmodyfikowanymi testami do PolyAddMonos i dodanymi
testami do PolyCloneMonos oraz PolyOwnMonos. Nie jest pewne jak dokładnie wygląda sytuacja z
oddawaniem `poly_test.c`, tzn. czy może być pisany niesamodzielnie, więc na wszelki wypadek
zaleca się oddawanie niezmienionej wersji z drugiej części zadania.

Te testy testują tylko część 3,
dlatego upewnijcie się, że starsze testy też śmigają
(oficjalne z cz. 1, oficjalne z cz. 2, gitlabowe z cz. 1, gitlabowe z cz. 2).

## Drobne uwagi

Przypominam by nie zadawać pytań na forum jak nie trzeba.
Na grupie fb jest link do docsu z listą pytań i naszych odpowiedzi, tam można zadawać pytania zamiast na Moodlu.

Można kliknąć na dzwoneczek i ustawić powiadomienia by wysyłało maila jak coś w testach się zmieni.

## Dodawanie swoich testów

Najpierw forkujecie repozytorium przyciskiem `Fork`
(tworzy Wasze własne repo z własnym branchem, możecie robić co chcecie i jak coś zepsujecie to bez problemu można usunąć forka, nie ma to żadnego efektu na oryginalne repo z testami).
Tam edytujecie i commitujecie pliki. Staramy się, by pliki nie były zbyt duże.

Jak już wszystko będzie skończone to gdzieś na gitlabie powinien być przycisk `Merge Request`.
Klikacie go, wpisujecie jakiś sensowny tytuł i ja wtedy dostanę powiadomienie. 
Jak wszystko będzie wyglądać dobrze to zmerguję do głównego repo z testami.
Jak się okaże, że coś jest źle, to wystarczy zrobić nowego commita i Merge Request sam ogarnie, że coś się zmieniło
(więc nie trzeba robić nowego forka lub MR).

Zawsze można na chacie się dopytać jak jakiś krok jest nieoczywisty.

Proponuję też, by nazwy plików mówiły jak najwięcej o teście.
Jak jakiś test ma testować jakiś corner case to może się np. zaczynać słowami `corner_case_`,
albo np. test wydajnościowy może być `performance_`. 

