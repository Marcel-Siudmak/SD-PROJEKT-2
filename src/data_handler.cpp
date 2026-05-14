#include "data_handler.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#ifndef PROJECT_ROOT_DIR
#define PROJECT_ROOT_DIR "."
#endif

namespace fs = std::filesystem;

inline fs::path get_data_dir() { return fs::path(PROJECT_ROOT_DIR) / "data"; }

data_set::data_set(const std::string &dataset_name) : name_(dataset_name) {
  fs::path dataset_dir = get_data_dir() / dataset_name;

  if (!fs::exists(dataset_dir)) {
    std::cerr << "Dataset directory does not exist: " << dataset_dir << "\n";
    return;
  }

  fs::path points_file = dataset_dir / "points.txt";
  if (fs::exists(points_file)) {
    std::ifstream file(points_file);
    int point;
    while (file >> point) {
      points_.push_back(point);
    }
  } else {
    std::cerr << "points.txt not found in dataset " << dataset_name << "\n";
  }

  for (const auto &entry : fs::directory_iterator(dataset_dir)) {
    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
      std::string filename = entry.path().filename().string();
      if (filename != "points.txt") {
        test_files_.push_back(filename);
      }
    }
  }
}

std::string data_set::get_name() const { return name_; }

std::vector<int> data_set::get_points() const { return points_; }

std::vector<std::string> data_set::get_test_files() const {
  return test_files_;
}

std::vector<int> data_set::get_data(const std::string &test_file,
                                    int num_elements) const {
  std::vector<int> data;
  data.reserve(num_elements);

  fs::path file_path = get_data_dir() / name_ / test_file;
  std::ifstream file(file_path);

  if (!file) {
    std::cerr << "Failed to open " << file_path << "\n";
    return data;
  }

  int val;
  int count = 0;
  while (count < num_elements && file >> val) {
    data.push_back(val);
    count++;
  }

  if (count < num_elements) {
    std::cerr << "Warning: " << file_path
              << " does not contain enough data (requested " << num_elements
              << ", found " << count << ").\n";
  }

  return data;
}

unsigned int data_set::get_file_seed(const std::string &test_file) const {
  // Nazwa pliku to "<seed>.txt", np. "3748291234.txt"
  fs::path p(test_file);
  return static_cast<unsigned int>(std::stoul(p.stem().string()));
}

void data_handler::generate_dataset(const std::string &dataset_name,
                                    const std::vector<int> &points,
                                    int num_files, unsigned int main_seed) {
  if (points.empty()) {
    std::cerr << "Measurement points vector cannot be empty.\n";
    return;
  }

  int max_points = *std::max_element(points.begin(), points.end());

  fs::path base_dir = get_data_dir();
  fs::path dataset_dir = base_dir / dataset_name;

  if (!fs::exists(dataset_dir)) {
    fs::create_directories(dataset_dir);
  }

  fs::path points_path = dataset_dir / "points.txt";
  std::ofstream points_file(points_path);
  if (!points_file) {
    std::cerr << "Failed to create " << points_path << "\n";
    return;
  }
  for (int point : points) {
    points_file << point << "\n";
  }
  points_file.close();

  std::mt19937 main_rng(main_seed);
  std::uniform_int_distribution<unsigned int> seed_dist;

  for (int i = 0; i < num_files; ++i) {
    unsigned int file_seed = seed_dist(main_rng);
    fs::path data_file_path =
        dataset_dir / (std::to_string(file_seed) + ".txt");

    std::ofstream data_file(data_file_path);
    if (!data_file) {
      std::cerr << "Failed to create " << data_file_path << "\n";
      continue;
    }

    std::mt19937 file_rng(file_seed);
    // Zakres [1, 999'999] — wartość 1'000'000 jest zarezerwowana jako unikalna
    // wartość szukana przez operację find w benchmarku.
    std::uniform_int_distribution<int> num_dist(1, 999'999);
    std::uniform_int_distribution<int> key_dist(1, 2000000);
    for (int j = 0; j < max_points; ++j) {
      data_file << key_dist(file_rng) << ',' << num_dist(file_rng) << "\n";
    }
    data_file.close();
  }
}

void data_handler::delete_dataset(const std::string &dataset_name) {
  fs::path base_dir = get_data_dir();
  fs::path dataset_dir = base_dir / dataset_name;
  if (fs::exists(dataset_dir)) {
    fs::remove_all(dataset_dir);
  }
}

std::vector<std::string> data_handler::list_datasets() {
  std::vector<std::string> datasets;
  fs::path base_dir = get_data_dir();

  if (!fs::exists(base_dir)) {
    return datasets;
  }

  for (const auto &entry : fs::directory_iterator(base_dir)) {
    if (entry.is_directory()) {
      datasets.push_back(entry.path().filename().string());
    }
  }

  return datasets;
}