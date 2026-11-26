Czarna Skrzynka – Logger generyczny C++
1. Opis projektu

Projekt przedstawia generyczny system logowania napisany w języku C++17. Logger automatycznie formatuje przekazane dane w zależności od ich typu, wykorzystując szablony, SFINAE oraz metaprogramowanie.

Program rozpoznaje trzy rodzaje danych:

Typy posiadające metodę raport().
Kontenery posiadające metody begin() i end().
Zwykłe typy obsługujące operator <<.
2. Wykorzystane technologie
C++17
Templates (szablony)
Variadic Templates
SFINAE
std::void_t
std::enable_if_t
std::type_traits
std::remove_cvref_t (własny alias dla C++17)
Fold Expressions
std::ostringstream
3. Koncepty i sprawdzanie typów
Wypisywalny

Struktura Wypisywalny sprawdza, czy dany typ można wypisać za pomocą operatora << do strumienia std::ostream.

maRaport

Struktura maRaport sprawdza, czy obiekt posiada metodę raport() oraz czy zwracana przez nią wartość jest wypisywalna.

Kontener

Struktura Kontener sprawdza, czy typ posiada metody begin() i end().

Dodatkowo wyklucza typ std::string, wykorzystując remove_cvref_t.

4. Funkcja sformatujDane()

Funkcja posiada trzy przeciążenia:

Obiekty z metodą raport() – zwraca tekst wygenerowany przez metodę raport().
Kontenery – formatuje elementy w nawiasach kwadratowych [ ], oddzielając je przecinkami. Obsługuje również zagnieżdżone kontenery.
Zwykłe typy wypisywalne – wykorzystuje std::ostringstream i operator <<.

Wybór odpowiedniego przeciążenia odbywa się za pomocą std::enable_if_t.

5. Klasa czarnaSkrzynka

Klasa odpowiada za logowanie komunikatów.

Metoda:

loguj(poziom, komunikat, dane...)

Wykorzystuje:

Variadic Templates – umożliwiają przekazywanie dowolnej liczby argumentów.
Fold Expression – pozwala przetworzyć wszystkie przekazane argumenty.
sformatujDane() – automatycznie dobiera sposób formatowania każdego argumentu.
6. Testowanie programu

W funkcji main() przetestowano:

Logowanie prostych typów: int, double, const char*.
Logowanie kontenera std::vector<int>.
Logowanie obiektu klasy Student.
Jednoczesne logowanie różnych typów danych.
Logowanie std::string.
Logowanie bez dodatkowych argumentów.
Logowanie zagnieżdżonego wektora.
Logowanie wektora obiektów klasy Student.

Dodatkowo przygotowano strukturę UszkodzonyCzujnik, która nie spełnia żadnego z wymaganych warunków formatowania. Jej przekazanie do loggera powoduje błąd kompilacji.

7. Przykładowy wynik działania
text
[INFO] Dane: 123 45.67 tekst
[DEBUG] Zawartosc wektora: [1, 2, 3, 4, 5]
[INFO] Informacje o studencie: Student: Jan, wiek: 21
[WARNING] Test mieszany: 100 [1, 2, 3, 4, 5] Student: Jan, wiek: 21 koniec
[INFO] String: Przykladowy napis
[ERROR] Wystapil blad!
[INFO] Zagniezdzony wektor: [[1, 2], [3, 4]]
[INFO] Wektor studentow: [Student: Anna, wiek: 20, Student: Jan, wiek: 21]

8. Kompilacja

Projekt wymaga kompilatora obsługującego standard C++17.

Przykładowa komenda dla GCC:

bash
g++ -std=c++17 main.cpp -o logger


Uruchomienie:

bash
./logger

9. Podsumowanie

Projekt pokazuje zastosowanie programowania generycznego w C++. Dzięki szablonom i SFINAE logger automatycznie rozpoznaje typ przekazanych danych i dobiera odpowiedni sposób ich formatowania.

Rozwiązanie pozwala łatwo rozszerzać system o kolejne typy danych bez konieczności modyfikowania samej klasy loggera.
