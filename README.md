# AUTOMATYCZNA TESTERKA DO ZADANIA MDIV

## Opis

W tym katalogu znajdują się narzędzia do automatycznego testowania rozwiązań do zadania MDIV.

* `tests\` zawiera testy w formacie test[0-9]+.cpp
* `build\` tutaj kompilują się kompilowane pliki

## Sposób użycia

1. Sklonuj to repozytorium:

    ```bash
    git clone
    ```

2. Dodaj swój plik `mdiv.asm`

3. Skompiluj testy:

    ```bash
    make all
    ```

4. Uruchom testy:

    ```bash
    make test
    ```

5. Uruchom konkretny test:

    ```bash
    make run-test1
    ```

## Modyfikowanie testów

W pliku `makefile` jest zmienna `tests` która zawiera wszystkie aktualnie wywoływane testy, można ją modyfikować aby uruchamiać tylko wybrane.

Można równierz, po dodaniu nowego pliku do folderu `tests\`, uruchamiać własne testy w `C\C++`.

### Jeśli zrobicie jakieś ciekawe testy, dajcie znać lub pull request'a

## Uwaga

1. Załączony biblioteka `gmp.h` sprawdza tylko poprawność wyniku operacji, nie sprawdza on czy nasz program wyrzuca poprawne błędy.
2. Testy nie sprawdzają ani szybkości naszego kodu (TODO), dodatkowej pamięci, wszystkich możliwych edge case'ów, ani rozmiaru sekcji `.text` programu, zwróć na to uwagę przed wysłaniem zadania.
3. Nie ma żadnej gwarancji, że program przechodzący wszystkie testy jest w 100% poprawny, ani że testerka jest wadliwa (jak coś znajdziecie dajcie znać). Zalecany jest zdrowy rozsądek.

### Autor : Konrad Kaczmarczyk
