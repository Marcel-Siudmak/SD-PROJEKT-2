#include "benchmark.hpp"
#include "data_handler.hpp"
#include "menu.hpp"
#include <iostream>

int main() {
  std::cout << "Starting SD-PROJEKT-1 Benchmarks...\n\n";
  menu main_menu;
    main_menu.run();
//   std::string dataset_name = "benchmark_test_dataset";
//   std::vector<int> measurement_points = {1000, 5000};
//   int num_test_files = 100;
//   unsigned int main_seed = 42;

//   std::cout << "[1] Generating/Rebuilding dataset '" << dataset_name
//             << "' with " << num_test_files << " files...\n";
//   data_handler::delete_dataset(dataset_name);
//   data_handler::generate_dataset(dataset_name, measurement_points,
//                                  num_test_files, main_seed);

//   std::cout << "\n[2] Initializing benchmark suite for '" << dataset_name
//             << "'...\n";
//   benchmark bench(dataset_name);

//   std::cout << "\n[3] Running Tests for SinglyLinkedList...\n";
//   bench.run_structure_tests<singly_linked_list<int>>(
//       "singly_linked_list", []() { return new singly_linked_list<int>(); });

//   std::cout << "\n[3] Running Tests for DoublyLinkedList...\n";
//   bench.run_structure_tests<doubly_linked_list<int>>(
//       "doubly_linked_list", []() { return new doubly_linked_list<int>(); });


//   std::cout<< "\n[3] Running Tests for ArrayList...\n";
//   bench.run_structure_tests<array_list<int>>(
//         "array_list", []() { return new array_list<int>(); });
//   std::cout
//       << "\n[4] Benchmarks finished successfully. Results saved to results/"
//       << dataset_name << "/\n";

  return 0;
}
