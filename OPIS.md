Politechnika Śląska
Wydział Matematyki Stosowanej
Kierunek Informatyka
Gliwice, 15.1.2022

Programowanie I
projekt zaliczeniowy
"Snake"

Bartłomiej Pacia gr. lab. 3/6

1. Opis projektu.
   “Snake” jest klonem popularnej “gry w węża”. Rozgrywka toczy się na kwadratowej planszy. Na początku gracz startuje z wężem o długości 1 (tzn. zajmującym 1 kratkę). Jednocześnie w losowej kratce na mapie pojawia się punkt. Zadaniem gracza jest takie kierowanie swoim wężem przy użyciu klawiszy strzałek na klawiaturze, by zebrać punkt. Zebrany punkt znika, a długość węża gracza zwiększa się o 1. Następnie pojawia się nowy punkt i cały proces zaczyna się od nowa. Gracz przegra, jeśli uderzy głową węża w jego własne ciało lub wyjdzie poza mapę. Uniknięcie tej pierwszej sytuacji staje się coraz trudniejsze wraz ze wzrostem długości węża.
2. Wymagania
   Przekreślone elementy listy oznaczają wymagania zdefiniowane w pierwotnych Założeniach do Projektu, które ostatecznie nie zostały zrealizowane.
   Pogrubione elementy listy oznaczają nowe wymagania, niezdefiniowane w pierwotnych Założeniach do Projektu, które zostały zrealizowane.
   ekran menu z 2 przyciskami (“Graj” i “Wyjdź”)
   ekran gry z 2 przyciskami (“Pauza” i “Wyjdź”), licznikiem obecnej liczby punktów oraz najwyższej liczby punktów
   zbieranie punktów i w konsekwencji wydłużanie się węża
   zapisywanie najwyższej liczby punktów (high score) do pliku i możliwość pobicia tego rekordu
   możliwość podania przez gracza rozmiaru planszy i interwału czasowego między ruchami jako argumenty do programu
3. Przebieg realizacji
   Wykonawca opisuje wykonane przez siebie zadania. Należy zamieścić opis plików z których składa się projekt, opis algorytmu, gdy program jest związany z algorytmiką. W przypadku korzystania z zewnętrznych bibliotek należy je tu krótko opisać (do czego służą, z jakich funkcji się korzystało)

Starałem pisać się kod z użyciem nowych funkcjonalności udostępnianych przez standard C++11, C++14 i C++17. W szczególności użyłem smart pointers zamiast tzw. surowych wskaźników (ang. raw pointers) oraz przekazywania argumentów do funkcji/metod przez referencję. Użyłem też kontenera std::vector oraz funkcji z biblioteki algorithm.
Użyte biblioteki
Użyłem biblioteki standardowej C++ oraz biblioteki SFML, która udostępnia podstawowe funkcje do obsługi grafiki.
Skomplikowanie
W toku implementacji okazało się, że występuje sporo przypadków krańcowych, o których nie pomyślałem na początku. Od dawna nie zajmowałem się pisaniem gier, i przypomniało mi się, jak niesamowicie szybko rośnie poziom skomplikowania kodu gry. Nie mam w tym doświadczenia.
Z powodu ograniczonego czasu oraz wiedzy (i ograniczonego czasu na jej uzupełnienie) nie użyłem żadnych wzorców projektowych, które na pewno pomogłyby lepiej zdefiniować zależności między różnymi elementami gry i w konsekwencji zmniejszyć liczbę błędów oraz czasu wymaganego na testowanie.
Jestem świadom bardzo sporego miejsca na poprawę. 4. Instrukcja użytkownika
Program należy uruchamiać z linii komend. Pozwala to na wyświetlanie pomocy oraz przekazanie mu argumentów wpływających na wygląd i gameplay. 5. Podsumowanie i wnioski.
W miejscu tym piszemy co zrealizowaliśmy, z czym były problemy. Ewentualnie jakie są dalsze kierunki rozwoju programu, czego nie udało się zrealizować

Z ważniejszych rzeczy, których nie zaimplementowałem, trzeba wymienić interfejs użytkownika, np. w postaci menu.

Kiedy nabędę więcej doświadczenia z tzw. nowoczesnym C++ oraz przeczytam książkę Game Programming Patterns, planuję zrefaktoryzować kod gry, tak by nie był zagmatwaną plątaniną wzajemnych, nieoczywistych zależności i zachowań pomiędzy komponentami.
Chciałbym również zainteresować się tematem testów jednostkowych w C++ z użyciem biblioteki Google Test, i (gdy już zrefaktoryzuję kod i będzie on możliwy do testowania), planuję takowe napisać. Sprawi to, że będę pewniejszy tego, że gra działa poprawnie i nie będę testować wszystkich jej funkcji po wprowadzeniu jakiejś zmiany.
