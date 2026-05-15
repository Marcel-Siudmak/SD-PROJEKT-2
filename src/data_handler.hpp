#pragma once

#include "IIList.hpp"
#include "pairing_heap.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <random>

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
    if (IIList<T>* pairingHeap = dynamic_cast<IIList<T>*>(&list)) {
      // std::cout << "Loading to pairing heap\n";
      int seed = get_file_seed(test_file);
      std::mt19937 rng(seed);
      std::uniform_real_distribution<double> dist(0.0, 1.0);
      double random_value = dist(rng);
      double please_work;

      // std::cout << "Loading to pairing heap\n";

      T val;
      int key;
      std::string line;
      int count = 0;
      while (count < num_elements && std::getline(file, line)) {
        line = line.replace(line.find(','), 1, " "); // Zamień przecinek na spację
        std::stringstream ss(line);
        ss >> key;
        ss >> val;
        // std::cout<< "Inserting: " << val << " with key: " << key << std::endl;
        please_work = dist(rng);
        if(please_work < random_value && list.return_size() > 0) { // Random extract_max to create a more organic heap structure
          Node<T> root(list.extract_max(), key);
          list.insert(root._value, root._key);
        }
        list.insert(val,key);
        
        count++;
      }
    } else {
      // std::cout << "Loading to list\n";
      T val;
      int key;
      std::string line;
      int count = 0;
      while (count < num_elements && std::getline(file, line)) {
        line = line.replace(line.find(','), 1, " "); // Zamień przecinek na spację
        std::stringstream ss(line);
        ss >> key;
        ss >> val;
        // std::cout<< "Inserting: " << val << " with key: " << key << std::endl;
        list.insert(val,key);
        count++;
      }

      if (count < num_elements) {
        std::cerr << "Warning: " << file_path
                  << " does not contain enough data (requested " << num_elements
                  << ", found " << count << ").\n";
      }
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

