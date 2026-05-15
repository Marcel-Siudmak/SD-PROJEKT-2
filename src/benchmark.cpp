#include "benchmark.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace fs = std::filesystem;

benchmark::benchmark(const std::string &dataset_name)
    : dataset_(dataset_name) {}

// ============================================================
//  Otwiera (lub nadpisuje) plik CSV i zapisuje nagłówek.
//  Format wiersza: operacja;n;czas_ns
// ============================================================
std::ofstream result_exporter::open_csv(const std::string &dataset_name,
                                        const std::string &structure_name) {
#ifndef PROJECT_ROOT_DIR
#define PROJECT_ROOT_DIR "."
#endif
  fs::path results_dir =
      fs::path(PROJECT_ROOT_DIR) / "results" / dataset_name;
  if (!fs::exists(results_dir)) {
    fs::create_directories(results_dir);
  }

  fs::path csv_path = results_dir / (structure_name + ".csv");
  std::ofstream file(csv_path, std::ios::out | std::ios::trunc);

  if (!file) {
    std::cerr << "Failed to create result file: " << csv_path << "\n";
    return file;
  }

  // Nagłówek
  file << "operacja;n;czas_ns\n";
  std::cout << "Plik wynikowy: " << csv_path << "\n";
  return file;
}

// ============================================================
//  Dopisuje jeden wiersz wyników do otwartego strumienia.
// ============================================================
void result_exporter::append_row(std::ofstream &file,
                                 const std::string &operation, int n,
                                 double time_ns) {
  file << operation << ";" << n << ";" << std::fixed << std::setprecision(2)
       << time_ns << "\n";
}

// ============================================================
//  Zachowana dla zgodności wstecznej – zapisuje całą mapę.
// ============================================================
void result_exporter::export_to_csv(const BenchmarkResults &results,
                                    const std::string &dataset_name,
                                    const std::string &structure_name) {
  auto file = open_csv(dataset_name, structure_name);
  if (!file) return;

  for (const auto &[op, n_times] : results) {
    for (const auto &[n, t] : n_times) {
      append_row(file, op, n, t);
    }
  }
  file.close();
}
