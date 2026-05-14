#pragma once

#include "IIList.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class data_set {
public:
  explicit data_set(const std::string &dataset_name);

  std::string get_name() const;
  std::vector<int> get_points() const;
  std::vector<std::string> get_test_files() const;

  std::vector<int> get_data(const std::string &test_file,
                            int num_elements) const;

  // Parsuje seed z nazwy pliku (np. "3748291234.txt" → 3748291234)
  unsigned int get_file_seed(const std::string &test_file) const;

  template <typename T>
  void load_to_list(const std::string &test_file, int num_elements,
                    IIList<T> &list) const {
#ifndef PROJECT_ROOT_DIR
#define PROJECT_ROOT_DIR "."
#endif
    std::filesystem::path file_path =
        std::filesystem::path(PROJECT_ROOT_DIR) / "data" / name_ / test_file;
    std::ifstream file(file_path);

    if (!file) {
      std::cerr << "Failed to open " << file_path << "\n";
      return;
    }

    list.clear();

    T val;
    int key;
    std::string line;
    int count = 0;
    while (count < num_elements && std::getline(file, line)) {
      // Format pliku: "klucz,wartość" (generowany przez data_handler)
      std::stringstream ss(line);
      char comma;
      ss >> key >> comma >> val;
      if (ss) {
        list.insert(val, key);
        count++;
      }
    }

    if (count < num_elements) {
      std::cerr << "Warning: " << file_path
                << " does not contain enough data (requested " << num_elements
                << ", found " << count << ").\n";
    }
  }

private:
  std::string name_;
  std::vector<int> points_;
  std::vector<std::string> test_files_;
};

class data_handler {
public:
  static void generate_dataset(const std::string &dataset_name,
                               const std::vector<int> &points, int num_files,
                               unsigned int main_seed);

  static void delete_dataset(const std::string &dataset_name);

  static std::vector<std::string> list_datasets();
};

