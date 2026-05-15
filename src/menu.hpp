#pragma once
#include "benchmark.hpp"
#include "data_handler.hpp"
#include "IIList.hpp"
#include "priority_q.hpp"
#include <iostream>

class menu {
private:
    IIList<int>* _type;
    std::string  _dataset_name, _list_type;
    std::vector<int> _points;
    int          _num_files;
    unsigned int _main_seed;

public:
    menu()
        : _type(nullptr),
          _dataset_name("benchmark_test_dataset"),
          _list_type("pq_heap"),
          _points({1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, 512000}),
          _num_files(100),
          _main_seed(42) {}

    ~menu() { delete _type; }

    void display_menu() {
        std::cout << "\n===== MENU =====\n";
        std::cout << "1. Wybierz strukturę danych\n";
        std::cout << "2. Wybierz dataset\n";
        std::cout << "3. Generuj/przebuduj dataset\n";
        std::cout << "4. Parametry benchmarku\n";
        std::cout << "5. Uruchom benchmarki\n";
        std::cout << "6. Tryb manualny\n";
        std::cout << "7. Wyjście\n";
        std::cout << "Wybierz: ";
    }

    void choose_list_type() {
        std::cout << "Wybierz strukturę:\n";
        std::cout << "1. Kolejka priorytetowa (pairing heap)\n";
        std::cout << "2. Kolejka priorytetowa (posortowana lista)\n";
        std::cout << "Wybierz: ";
        int choice;
        std::cin >> choice;

        delete _type;
        switch (choice) {
            case 1:
                _type      = new pq_heap<int>();
                _list_type = "pq_heap";
                break;
            case 2:
                _type      = new pq_list<int>();
                _list_type = "pq_list";
                break;
            default:
                std::cout << "Nieprawidłowy wybór. Ustawiam pq_heap.\n";
                _type      = new pq_heap<int>();
                _list_type = "pq_heap";
        }
        std::cout << "Wybrano: " << _list_type << "\n";
    }

    void generate_dataset() {
        std::cout << "Generuję dataset '" << _dataset_name
                  << "' (" << _num_files << " plików)...\n";
        data_handler::delete_dataset(_dataset_name);
        data_handler::generate_dataset(_dataset_name, _points, _num_files, _main_seed);
        std::cout << "Gotowe.\n";
    }

    void run_benchmarks() {
        std::cout << "\nInicjalizuję benchmark dla '" << _dataset_name << "'...\n";
        benchmark bench(_dataset_name);
        std::cout << "Testuję: " << _list_type << "\n";

        if (_list_type == "pq_heap") {
            bench.run_structure_tests<pq_heap<int>>(
                "pq_heap", []() { return new pq_heap<int>(); });
        } else if (_list_type == "pq_list") {
            bench.run_structure_tests<pq_list<int>>(
                "pq_list", []() { return new pq_list<int>(); });
        } else {
            std::cout << "Nieznana struktura.\n";
            return;
        }
        std::cout << "\nBenchmark zakończony. Wyniki w results/" << _dataset_name << "/\n";
    }

    void parameters() {
        std::cout << "Liczba plików testowych: ";
        std::cin >> _num_files;
        std::cout << "Główny seed: ";
        std::cin >> _main_seed;
        std::cout << "Punkty pomiarowe (przecinkami, np. 1000,5000): ";
        std::string pts;
        std::cin >> pts;
        _points.clear();
        size_t pos = 0;
        while ((pos = pts.find(',')) != std::string::npos) {
            _points.push_back(std::stoi(pts.substr(0, pos)));
            pts.erase(0, pos + 1);
        }
        if (!pts.empty()) {
            _points.push_back(std::stoi(pts));
        }
    }

    void manual_mode() {
        if (!_type) {
            std::cout << "Najpierw wybierz strukturę (opcja 1).\n";
            return;
        }
        int choice;
        while (true) {
            std::cout << "\n--- Tryb manualny [" << _list_type << "] ---\n";
            std::cout << "1. Wstaw (insert)\n";
            std::cout << "2. Pobierz maksimum bez usuwania (peek)\n";
            std::cout << "3. Usuń maksimum (extract_max)\n";
            std::cout << "4. Zmień klucz (modify_key)\n";
            std::cout << "5. Wyświetl\n";
            std::cout << "6. Rozmiar\n";
            std::cout << "7. Wyczyść\n";
            std::cout << "0. Powrót\n";
            std::cout << "Wybierz: ";
            std::cin >> choice;
            if (choice == 0) break;

            int value, key;
            switch (choice) {
                case 1:
                    std::cout << "Wartość i klucz: ";
                    std::cin >> value >> key;
                    _type->insert(value, key);
                    std::cout << "Wstawiono.\n";
                    break;
                case 2:
                    try {
                        std::cout << "Maksimum: " << _type->peek() << "\n";
                    } catch (const std::exception& e) {
                        std::cout << "Błąd: " << e.what() << "\n";
                    }
                    break;
                case 3:
                    try {
                        _type->extract_max();
                        std::cout << "Usunięto maksimum.\n";
                    } catch (const std::exception& e) {
                        std::cout << "Błąd: " << e.what() << "\n";
                    }
                    break;
                case 4:
                    std::cout << "Wartość i nowy klucz: ";
                    std::cin >> value >> key;
                    try {
                        _type->modify_key(value, key);
                        std::cout << "Klucz zmieniony.\n";
                    } catch (const std::exception& e) {
                        std::cout << "Błąd: " << e.what() << "\n";
                    }
                    break;
                case 5:
                    _type->display();
                    break;
                case 6:
                    std::cout << "Rozmiar: " << _type->return_size() << "\n";
                    break;
                case 7:
                    _type->clear();
                    std::cout << "Wyczyszczono.\n";
                    break;
                default:
                    std::cout << "Nieprawidłowy wybór.\n";
            }
        }
    }

    void run() {
        std::cout << "=== Narzędzie do benchmarkowania kolejek priorytetowych ===\n";
        // Domyślna struktura
        _type = new pq_heap<int>();

        while (true) {
            display_menu();
            int choice;
            std::cin >> choice;
            switch (choice) {
                case 1: choose_list_type(); break;
                case 2:
                    std::cout << "Bieżący dataset: " << _dataset_name << "\n";
                    std::cout << "Nowa nazwa: ";
                    std::cin >> _dataset_name;
                    break;
                case 3: generate_dataset(); break;
                case 4: parameters(); break;
                case 5: run_benchmarks(); break;
                case 6: manual_mode(); break;
                case 7:
                    std::cout << "Zamykam...\n";
                    return;
                default:
                    std::cout << "Nieprawidłowy wybór.\n";
            }
        }
    }
};
