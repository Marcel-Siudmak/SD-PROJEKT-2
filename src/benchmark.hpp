#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "IIList.hpp"
#include "data_handler.hpp"

using BenchmarkResults = std::map<std::string, std::map<int, double>>;

// ============================================================
//  Liczba kopii struktury tworzonych dla każdego seeda.
//  Większa wartość → dokładniejszy pomiar dla krótkich operacji
//  (np. peek, extract_max) kosztem pamięci.
// ============================================================
static constexpr int NUM_COPIES = 100;

// Zakres kluczy/priorytetów używanych przy generowaniu danych
// (zgodny z data_handler – klucze ∈ [1, 999'999])
static constexpr int KEY_MIN = 1;
static constexpr int KEY_MAX = 999'999;

// ============================================================
//  Eksport wyników – zapis przyrostowy (linia po linii),
//  dzięki czemu przerwanie benchmarku nie traci danych.
//
//  Format CSV:
//    operacja;n;czas_ns
//  Nagłówek jest zapisywany tylko raz (przy pierwszym otwarciu).
// ============================================================
class result_exporter {
public:
  // Otwiera (lub tworzy) plik CSV i zapisuje nagłówek, jeśli plik był pusty.
  // Zwraca otwarty strumień; wywołujący odpowiada za jego zamknięcie.
  static std::ofstream open_csv(const std::string &dataset_name,
                                const std::string &structure_name);

  // Dopisuje jeden wiersz wyników do już otwartego strumienia.
  static void append_row(std::ofstream &file, const std::string &operation,
                         int n, double time_ns);

  // Zachowana dla zgodności wstecznej – zapisuje całą mapę wyników naraz.
  static void export_to_csv(const BenchmarkResults &results,
                            const std::string &dataset_name,
                            const std::string &structure_name);
};

class benchmark {
public:
  explicit benchmark(const std::string &dataset_name);

  // ============================================================
  //  Główna metoda uruchamiająca testy dla jednej struktury.
  //  ListType     – konkretny typ (np. pq_heap<int>)
  //  list_factory – lambda tworząca pustą instancję
  // ============================================================
  template <typename ListType>
  void run_structure_tests(const std::string &structure_name,
                           std::function<ListType *()> list_factory) {

    // Otwórz plik CSV raz przed pętlą – wyniki trafiają do pliku
    // natychmiast po każdym zmierzonym punkcie.
    std::ofstream csv = result_exporter::open_csv(dataset_.get_name(),
                                                   structure_name);

    for (int n : dataset_.get_points()) {
      std::cout << "\n--- Testing " << structure_name << " for N=" << n
                << " ---\n";

      const auto &test_files = dataset_.get_test_files();
      const size_t num_seeds = test_files.size();

      // ----------------------------------------------------------
      // Bufory na wyniki cząstkowe (jeden pomiar na seed)
      // ----------------------------------------------------------
      std::vector<double> peek_times(num_seeds);
      std::vector<double> extract_times(num_seeds);
      std::vector<double> insert_times(num_seeds);
      std::vector<double> modify_times(num_seeds);

      for (size_t s = 0; s < num_seeds; ++s) {
        const std::string &file = test_files[s];
        const unsigned int seed = dataset_.get_file_seed(file);

        // --------------------------------------------------------
        // 1. peek() – operacja tylko-do-odczytu, nie zmienia stanu.
        //    Tworzymy NUM_COPIES identycznych kopii, mierzymy czas
        //    wykonania peek() na wszystkich łącznie, dzielimy / x.
        // --------------------------------------------------------
        {
          auto copies = make_copies(list_factory, file, n, NUM_COPIES);

          auto t0 = std::chrono::high_resolution_clock::now();
          for (auto *inst : copies) inst->peek();
          auto t1 = std::chrono::high_resolution_clock::now();

          peek_times[s] = ns(t0, t1) / NUM_COPIES;
          free_copies(copies);
        }

        // --------------------------------------------------------
        // 2. extract_max() – modyfikuje strukturę; każda kopia
        //    startuje z pełnym zestawem n elementów.
        // --------------------------------------------------------
        {
          auto copies = make_copies(list_factory, file, n, NUM_COPIES);

          auto t0 = std::chrono::high_resolution_clock::now();
          for (auto *inst : copies) inst->extract_max();
          auto t1 = std::chrono::high_resolution_clock::now();

          extract_times[s] = ns(t0, t1) / NUM_COPIES;
          free_copies(copies);
        }

        // --------------------------------------------------------
        // 3. insert() – wstawiamy element z deterministycznie
        //    wylosowanym priorytetem (kluczem).
        //    RNG seed = file_seed → pełna odtwarzalność.
        // --------------------------------------------------------
        {
          std::mt19937 rng(seed);
          std::uniform_int_distribution<int> key_dist(KEY_MIN, KEY_MAX);
          std::vector<int> insert_keys(NUM_COPIES);
          for (int &k : insert_keys) k = key_dist(rng);

          constexpr int INSERT_VALUE = 1'000'000;
          auto copies = make_copies(list_factory, file, n, NUM_COPIES);

          auto t0 = std::chrono::high_resolution_clock::now();
          for (size_t c = 0; c < copies.size(); ++c)
            copies[c]->insert(INSERT_VALUE, insert_keys[c]);
          auto t1 = std::chrono::high_resolution_clock::now();

          insert_times[s] = ns(t0, t1) / NUM_COPIES;
          free_copies(copies);
        }

        // --------------------------------------------------------
        // 4. modify_key(e, p)
        //
        //    e – element do modyfikacji: losowany deterministycznie
        //        spośród wartości faktycznie istniejących w danych
        //        (strumień RNG: mt19937(seed ^ 0xCAFEBABEu)).
        //
        //    p – nowy priorytet: losowany deterministycznie z innego
        //        strumienia (mt19937(seed ^ 0xDEADBEEFu)).
        //
        //    Oba losowania są niezależne i w pełni odtwarzalne
        //    dla każdego seeda.
        // --------------------------------------------------------
        {
          // Wczytaj pary (klucz, wartość) z pliku, żeby znać
          // jakie wartości faktycznie znajdują się w strukturze.
          auto pairs = load_pairs(file, n);  // vector<pair<int,int>>

          // Strumień RNG do losowania elementu e
          std::mt19937 rng_e(seed ^ 0xCAFEBABEu);
          std::uniform_int_distribution<int> elem_dist(0,
                                                       static_cast<int>(pairs.size()) - 1);

          // Strumień RNG do losowania nowego klucza p
          std::mt19937 rng_p(seed ^ 0xDEADBEEFu);
          std::uniform_int_distribution<int> key_dist(KEY_MIN, KEY_MAX);

          // Wylosuj NUM_COPIES par (e, p) deterministycznie
          std::vector<int> elems(NUM_COPIES);
          std::vector<int> new_keys(NUM_COPIES);
          for (int c = 0; c < NUM_COPIES; ++c) {
            elems[c]   = pairs[elem_dist(rng_e)].second; // wartość elementu e
            new_keys[c] = key_dist(rng_p);               // nowy priorytet p
          }

          // Przygotowanie kopii – elementy e już są w strukturze
          // (pochodzą z danych), nie trzeba nic dokładać poza pomiarem.
          auto copies = make_copies(list_factory, file, n, NUM_COPIES);

          auto t0 = std::chrono::high_resolution_clock::now();
          for (size_t c = 0; c < copies.size(); ++c)
            copies[c]->modify_key(elems[c], new_keys[c]);
          auto t1 = std::chrono::high_resolution_clock::now();

          modify_times[s] = ns(t0, t1) / NUM_COPIES;
          free_copies(copies);
        }

      } // koniec pętli po seedach

      // ----------------------------------------------------------
      // Uśrednij wyniki po wszystkich seedach i od razu zapisz
      // do pliku CSV (nie czekamy na koniec całego benchmarku).
      // ----------------------------------------------------------
      double avg_peek    = average(peek_times);
      double avg_extract = average(extract_times);
      double avg_insert  = average(insert_times);
      double avg_modify  = average(modify_times);

      result_exporter::append_row(csv, "peek",        n, avg_peek);
      result_exporter::append_row(csv, "extract_max", n, avg_extract);
      result_exporter::append_row(csv, "insert",      n, avg_insert);
      result_exporter::append_row(csv, "modify_key",  n, avg_modify);
      csv.flush(); // wymusz zapis na dysk

      std::cout << "  peek        avg = " << avg_peek    << " ns\n";
      std::cout << "  extract_max avg = " << avg_extract << " ns\n";
      std::cout << "  insert      avg = " << avg_insert  << " ns\n";
      std::cout << "  modify_key  avg = " << avg_modify  << " ns\n";

    } // koniec pętli po punktach pomiarowych

    csv.close();
    std::cout << "\nWyniki zapisane do results/" << dataset_.get_name()
              << "/" << structure_name << ".csv\n";
  }

private:
  data_set dataset_;

  // ----------------------------------------------------------
  //  Tworzy `count` identycznych kopii struktury załadowanych
  //  z pliku `file` (pierwsze `n` elementów).
  // ----------------------------------------------------------
  template <typename ListType>
  std::vector<IIList<int> *> make_copies(std::function<ListType *()> factory,
                                         const std::string &file, int n,
                                         int count) {
    std::vector<IIList<int> *> copies(count);
    for (int i = 0; i < count; ++i) {
      copies[i] = factory();
      dataset_.load_to_list(file, n, *copies[i]);
    }
    return copies;
  }

  // ----------------------------------------------------------
  //  Zwalnia wszystkie kopie struktury.
  // ----------------------------------------------------------
  static void free_copies(std::vector<IIList<int> *> &copies) {
    for (auto *ptr : copies) delete ptr;
    copies.clear();
  }

  // ----------------------------------------------------------
  //  Oblicza średnią arytmetyczną wektora wartości.
  // ----------------------------------------------------------
  static double average(const std::vector<double> &v) {
    if (v.empty()) return 0.0;
    double sum = 0.0;
    for (double x : v) sum += x;
    return sum / static_cast<double>(v.size());
  }

  // ----------------------------------------------------------
  //  Zwraca czas w nanosekundach między dwoma punktami.
  // ----------------------------------------------------------
  static double ns(std::chrono::high_resolution_clock::time_point t0,
                   std::chrono::high_resolution_clock::time_point t1) {
    return static_cast<double>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
  }

  // ----------------------------------------------------------
  //  Wczytuje pierwsze `n` par (klucz, wartość) z pliku danych.
  //  Format pliku: "klucz,wartość" (jeden rekord na linię).
  //  Potrzebne do deterministycznego wyboru elementu e
  //  w teście modify_key – wybieramy spośród wartości,
  //  które faktycznie istnieją w załadowanej strukturze.
  // ----------------------------------------------------------
  std::vector<std::pair<int,int>> load_pairs(const std::string &file,
                                             int n) const {
#ifndef PROJECT_ROOT_DIR
#define PROJECT_ROOT_DIR "."
#endif
    std::vector<std::pair<int,int>> pairs;
    pairs.reserve(n);

    std::filesystem::path file_path =
        std::filesystem::path(PROJECT_ROOT_DIR) / "data" /
        dataset_.get_name() / file;
    std::ifstream ifs(file_path);
    if (!ifs) return pairs;

    std::string line;
    int count = 0;
    while (count < n && std::getline(ifs, line)) {
      std::istringstream ss(line);
      int key, val;
      char comma;
      if (ss >> key >> comma >> val) {
        pairs.emplace_back(key, val);
        ++count;
      }
    }
    return pairs;
  }
};
