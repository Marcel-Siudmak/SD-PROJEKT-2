# Miniprojekt 2 - Kolejki prioryTETOwe

Projekt realizuje własną implementację wybranych struktur danych w języku C++ oraz moduł badawczy do analizy ich złożoności czasowej. Program opiera się na paradygmacie obiektowym z wykorzystaniem interfejsów (szablonów) i jest podzielony na pliki nagłówkowe (`*.hpp`) oraz źródłowe (`*.cpp`). Podstawowym typem danych w strukturach jest 4-bajtowa liczba całkowita. 

## Spis treści
* [Wymagania](#1-wymagania)
* [Kompilacja i uruchomienie](#2-kompilacja-i-uruchomienie)
* [Struktura Projektu](#3-struktura-projektu)
* [Szczegółowy Opis Plików i Modułów](#4-szczegółowy-opis-plików-i-modułów)

## 1. Wymagania
* **Kompilator:** GCC 15+ / Clang 17+ (wymagane wsparcie dla C++20)
* **System budowania:** CMake 3.10+
* **System operacyjny:** macOS / Windows / Linux

## 2. Kompilacja i uruchomienie
Aby zbudować projekt, wykonaj poniższe kroki w terminalu:

1. Stwórz folder budowania:
   ```bash
   mkdir build && cd build
   ```

2. Skonfiguruj projekt za pomocą CMake:
   ```bash
   cmake ..
   ```

3. Skompiluj:
   ```bash
   cmake --build .
   ```

4. Uruchom program:
   ```bash
   ./mini_projekt_1
   ```

## 3. Struktura Projektu

```text
├── src/
│   ├── main.cpp
│   ├── IIList.hpp
│   ├── pairing_heap.hpp
│   ├── pairing_heap.cpp
│   ├── priority_q.hpp
│   ├── priority_q.cpp
│   ├── Menu.hpp
│   ├── Benchmark.hpp
│   ├── Benchmark.cpp
│   ├── DataHandler.hpp
│   └── DataHandler.cpp
├── data/
│   ├── random_data_5000.txt
│   ├── random_data_10000.txt
│   └── ... 
├── build/
│   ├── binary
│   └── ...
├── CMakeLists.txt
└── README.md
```

## 4. Szczegółowy Opis Plików i Modułów

### A. Abstrakcja i Interfejsy

#### `IIList.hpp` (Abstrakcja)
Abstrakcyjna klasa bazowa (czysty interfejs) wykorzystująca szablony (**template**). Definiuje kontrakt dla wszystkich struktur danych. Zawiera deklaracje metod wirtualnych do manipulacji danymi (dodawanie/usuwanie w różnych miejscach, wyszukiwanie) oraz metody pomocnicze do zarządzania rozmiarem i czyszczenia pamięci.

### B. Struktury Danych (Moduły implementacyjne)

#### `pairing_heap`
`pairing_heap.hpp` / `pairing_heap.cpp` – Implementacja kopca parującego, posiada metody z IIList.hpp (peek(), extract_max(), modify_key(), insert(), display(), clear()). Struktura leniwa o amortyzowanym czasie extract_max() oraz insert().

#### `priority_q`
`priority_q.hpp` / `priority_q.cpp` – Implementacja kolejki priorytetowej na liście, posiada metody z IIList.hpp (peek(), extract_max(), modify_key(), insert(), display(), clear()). Dodatkowo zawiera delegację do implementacji kopca parującego. 


### C. Logika Aplikacji i Interfejs Użytkownika

#### `Menu`
`Menu.hpp` / `Menu.cpp` – Warstwa prezentacji. Moduł odpowiada za wyświetlanie opcji w konsoli, walidację danych wprowadzanych przez użytkownika oraz wywoływanie odpowiednich metod z wybranych struktur danych. Pozwala na manualne testowanie funkcjonalności.

#### `main.cpp`
Minimalistyczny punkt startowy programu. Odpowiada za stworzenie instancji klasy `Menu` i uruchomienie jej głównej pętli. Nie zawiera logiki biznesowej ani zmiennych globalnych.

### D. Narzędzia Badawcze i Zarządzanie Danymi

#### `DataHandler`
`DataHandler.hpp` / `DataHandler.cpp` – Moduł narzędziowy do operacji na plikach. Zawiera logikę generowania dużych zbiorów liczb losowych i zapisywania ich do formatu `.txt`, a także funkcje parsujące te pliki w celu zasilenia struktur danych przed testami.

#### `Benchmark`
`Benchmark.hpp` / `Benchmark.cpp` – Serce części badawczej projektu. Wykorzystuje bibliotekę `<chrono>` do precyzyjnego pomiaru czasu (nanosekundy). Moduł automatyzuje proces testowy: przeprowadza tysiące powtórzeń dla różnych rozmiarów danych i oblicza uśrednione wyniki dla przypadku średniego (**average-case**).

### E. Pozostałe Elementy Projektu

#### `data/` (Katalog danych)
Folder przechowujący fizyczne pliki `.txt` (np. `random_data_5000.txt`). Pliki te stanowią stałą bazę testową, gwarantując, że każda struktura jest badana na dokładnie tym samym zestawie danych.

#### `build/` (Katalog kompilacji)
Katalog roboczy CMake. Przechowuje plik wykonywalny (**binary**) oraz pliki pośrednie (`.o`, `CMakeFiles`). Katalog ten jest zazwyczaj ignorowany przez system kontroli wersji Git.

#### `CMakeLists.txt` (Konfiguracja)
Plik tekstowy sterujący procesem kompilacji. Określa nazwę projektu, flagi kompilatora, standard **C++20** oraz instrukcje kopiowania folderu `data/` do lokalizacji pliku wykonywalnego.

#### `README.md` (Dokumentacja)
Główny plik informacyjny projektu. Zawiera instrukcje instalacji, kompilacji, opis struktury oraz cele badawcze projektu.
