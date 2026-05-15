#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "IIList.hpp"
#include "data_handler.hpp"

using BenchmarkResults = std::map<std::string, std::map<int, double>>;

class result_exporter {
public:
  static void export_to_csv(const BenchmarkResults &results,
                            const std::string &dataset_name,
                            const std::string &structure_name);
};

class benchmark {
public:
  explicit benchmark(const std::string &dataset_name);

  template <typename ListType>
  void run_structure_tests(const std::string &structure_name,
                           std::function<ListType *()> list_factory) {
    BenchmarkResults results;

    for (int n : dataset_.get_points()) {
      std::cout << "\n--- Testing " << structure_name << " for N=" << n
                << " ---\n";

      const auto &test_files = dataset_.get_test_files();
      size_t num_instances = test_files.size();

      // ---------------------------------------------------------------
      // Buduj instancje i zapamiętaj seed każdego pliku
      // ---------------------------------------------------------------
      std::vector<IIList<int> *> instances(num_instances);
      std::vector<unsigned int> file_seeds(num_instances);

      for (size_t i = 0; i < num_instances; ++i) {
        instances[i] = list_factory();
        dataset_.load_to_list(test_files[i], n, *instances[i]);
        file_seeds[i] = dataset_.get_file_seed(test_files[i]);
      }

      // 1. peek()
      std::cout << "Measuring peek()...\n";
      results["peek"][n] = measure_operation(instances, [](IIList<int> *list) {list->peek();});

      // 2. extract_max()
      std::cout << "Measuring extract_max() 1st...\n";
      results["extract_max"][n] = measure_operation(instances, [](IIList<int> *list) {list->extract_max();});


      for (size_t i = 0; i < num_instances; ++i) {
        dataset_.load_to_list(test_files[i], n, *instances[i]);
      }

      // 3. insert()
      std::cout << "Measuring insert() with random index...\n";
              // Wstaw unikalną wartość na losowy indeks (poza pomiarem)
        for (size_t i = 0; i < num_instances; ++i) {
        dataset_.load_to_list(test_files[i], n, *instances[i]);
        }

      results["insert_random"][n] = measure_operation_with_seeds(
          instances, file_seeds, n,
          [](IIList<int> *list, int idx, int /*n*/) {
            list->insert(1000000, idx);
          }, true);

      
      // 4. modify_key()
      std::cout << "Measuring modify_key() with random index...\n";
      results["modify_key"][n] = measure_operation_with_seeds(instances, file_seeds, n,
          [](IIList<int> *list, int idx, int /*n*/) {
            list->modify_key(1000000, idx);
          }, true);
      // ---------------------------------------------------------------
      // 1. push_front
      // ---------------------------------------------------------------
      // results["push_front"][n] = measure_operation(
      //     instances, [](IList<int> *list) { list->push_front(999); });

      // ---------------------------------------------------------------
      // 2. push_back
      // ---------------------------------------------------------------
      // results["push_back"][n] = measure_operation(
      //     instances, [](IList<int> *list) { list->push_back(999); });

      // ---------------------------------------------------------------
      // 3. pop_front
      // ---------------------------------------------------------------
      // results["pop_front"][n] = measure_operation(
      //     instances, [](IList<int> *list) { list->pop_front(); });

      // // ---------------------------------------------------------------
      // 4. pop_back
      // ---------------------------------------------------------------
      // results["pop_back"][n] = measure_operation(
      //     instances, [](IList<int> *list) { list->pop_back(); });

      // ---------------------------------------------------------------
      // 5. insert na losowym indeksie
      //    Używamy tego samego file_seed co dane — pełna odtwarzalność.
      //    Po pop_front/pop_back lista ma n-1 elementów, więc
      //    przeładowujemy świeże instancje przed każdą nową operacją.
      // ---------------------------------------------------------------

      // Przeładuj instancje (pop_front/pop_back zmodyfikowały listy)
      // for (size_t i = 0; i < num_instances; ++i) {
      //   dataset_.load_to_list(test_files[i], n, *instances[i]);
      // }

      // results["insert_random"][n] = measure_operation_with_seeds(
      //     instances, file_seeds, n,
      //     [](IList<int> *list, int idx, int /*n*/) {
      //       list->insert(999, idx);
      //     },
      //     /* index_range_is_n_plus_one = */ true); // indeks ∈ [0, n]

      // ---------------------------------------------------------------
      // 6. remove na losowym indeksie
      // ---------------------------------------------------------------
      // for (size_t i = 0; i < num_instances; ++i) {
      //   dataset_.load_to_list(test_files[i], n, *instances[i]);
      // }

      // results["remove_random"][n] = measure_operation_with_seeds(
      //     instances, file_seeds, n,
      //     [](IList<int> *list, int idx, int /*n*/) {
      //       list->remove(idx);
      //     },
       //   /* index_range_is_n_plus_one = */ false); // indeks ∈ [0, n-1]

      // ---------------------------------------------------------------
      // 7. find — szukamy wartości 1'000'000 wstawionej deterministycznie.
      //
      //    Przygotowanie per-instancja (NIE mierzymy tego czasu):
      //      a) wylosuj find_idx ∈ [0, n-1] z file_seed
      //      b) usuń element na find_idx
      //      c) wstaw 1'000'000 na find_idx
      //    Ponieważ dane są z [1, 999'999], wartość 1'000'000 jest unikalna.
      // ---------------------------------------------------------------
      // for (size_t i = 0; i < num_instances; ++i) {
      //   dataset_.load_to_list(test_files[i], n, *instances[i]);

      //   // Wstaw unikalną wartość na losowy indeks (poza pomiarem)
      //   std::mt19937 rng(file_seeds[i]);
      //   std::uniform_int_distribution<int> idx_dist(0, n - 1);
      //   int find_idx = idx_dist(rng);
      //   instances[i]->remove(find_idx);
      //   instances[i]->insert(1'000'000, find_idx);
      // }

      // results["find"][n] = measure_operation(
      //     instances, [](IList<int> *list) { list->find(1'000'000); });

      // ---------------------------------------------------------------
      // Zwolnij pamięć
      // ---------------------------------------------------------------
      for (auto ptr : instances) {
        delete ptr;
      }
    }

    result_exporter::export_to_csv(results, dataset_.get_name(),
                                   structure_name);
  }

private:
  data_set dataset_;

  // Podstawowy pomiar — ta sama operacja dla wszystkich instancji
  double measure_operation(const std::vector<IIList<int> *> &instances,
                           const std::function<void(IIList<int> *)> &operation) {
    long long total_nanoseconds = 0;

    for (auto list_instance : instances) {
      auto start = std::chrono::high_resolution_clock::now();
      operation(list_instance);
      auto end = std::chrono::high_resolution_clock::now();
      total_nanoseconds +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
    }

    if (instances.empty())
      return 0.0;
    return static_cast<double>(total_nanoseconds) / instances.size();
  }

  // Pomiar z losowym indeksem wyznaczonym z file_seed.
  // use_n_plus_one=true  → indeks ∈ [0, n]   (insert)
  // use_n_plus_one=false → indeks ∈ [0, n-1] (remove)
  double measure_operation_with_seeds(
      const std::vector<IIList<int> *> &instances,
      const std::vector<unsigned int> &file_seeds, int n,
      const std::function<void(IIList<int> *, int, int)> &operation,
      bool use_n_plus_one) {
    long long total_nanoseconds = 0;

    for (size_t i = 0; i < instances.size(); ++i) {
      std::mt19937 rng(file_seeds[i]);
      int upper = use_n_plus_one ? n : (n - 1);
      std::uniform_int_distribution<int> idx_dist(0, upper);
      int idx = idx_dist(rng);

      auto start = std::chrono::high_resolution_clock::now();
      operation(instances[i], idx, n);
      auto end = std::chrono::high_resolution_clock::now();
      total_nanoseconds +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
    }

    if (instances.empty())
      return 0.0;
    return static_cast<double>(total_nanoseconds) / instances.size();
  }
};
