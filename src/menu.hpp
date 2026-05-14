#pragma once
#include "benchmark.hpp"
#include "data_handler.hpp"
#include "IIList.hpp"
#include "pairing_heap.hpp"
#include <iostream>

class menu{
private:
    IIList<int>* _type;
    std::string _dataset_name, _list_type;
    std::vector<int> _points;
    int _num_files;
    unsigned int _main_seed;
public:
    menu() : _type(nullptr), _dataset_name("benchmark_test_dataset"), _list_type("pairing_heap"), _points({1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, 512000}), _num_files(100), _main_seed(42) {};
    void display_menu(){
        std::cout << "1. Choose List Type:\n";
        std::cout << "2. Choose dataset.\n";
        std::cout << "3. Generate/Rebuild Dataset.\n";
        std::cout << "4. Choose Benchmark parameters.\n";
        std::cout << "5. Run Benchmarks\n";
        std::cout << "6. Manual Mode\n";
        std::cout << "7. Exit\n";
    }
    void choose_list_type(){
        int choice;
        std::cout << "Choose List Type:\n";
        std::cout << "1. Pairing heap\n";
        std::cin >> choice;
        switch (choice) {
            case 1:
                _type = new pairing_heap<int>();
                _list_type = "pairing_heap";
                break;
            default:
                std::cout << "Invalid choice. Defaulting to Singly Linked List.\n";
                _type = new pairing_heap<int>();
                _list_type = "pairing_heap";
        }
    }
    void generate_dataset(){
        std::cout << "Generating/Rebuilding dataset '" << _dataset_name
            << "' with " << _num_files << " files...\n";
        data_handler::delete_dataset(_dataset_name);
        data_handler::generate_dataset(_dataset_name, _points, _num_files, _main_seed);
    }
    void run_benchmarks(){
        std::cout << "\nInitializing benchmark suite for '" << _dataset_name
            << "'...\n";
        benchmark bench(_dataset_name);
        std::cout << "\nRunning Tests for " << _dataset_name <<" "<< _list_type <<"...\n";
        if(_list_type == "pairing_heap"){
            bench.run_structure_tests<pairing_heap<int>>(
                "pairing_heap", []() { return new pairing_heap<int>(); });
            }
        // else if(_list_type == "doubly_linked_list"){
        //     bench.run_structure_tests<doubly_linked_list<int>>(
        //         "doubly_linked_list", []() { return new doubly_linked_list<int>(); });
        std::cout<< "\nBenchmarks finished successfully. Results saved to results/"<< _dataset_name << "/\n";
    }
    void parameters(){
        std::cout << "Enter number of test files: ";
        std::cin >> _num_files;
        std::cout << "Enter main seed: ";
        std::cin >> _main_seed;
        std::cout << "Enter measurement points (comma separated): ";
        std::string points_input;
        std::cin >> points_input;
        _points.clear();
        size_t pos = 0;
        while ((pos = points_input.find(',')) != std::string::npos) {
            _points.push_back(std::stoi(points_input.substr(0, pos)));
            points_input.erase(0, pos + 1);
        }
        if (!points_input.empty()) {
            _points.push_back(std::stoi(points_input));
        }
    }
    void manual_mode(){
        int choice;
        while(true){
            std::cout << "Manual Mode:\n";
            std::cout << "1. Peek\n";
            std::cout << "2. Extract Max\n";
            std::cout << "3. Insert\n";
            std::cout << "4. Modify Key\n";
            std::cout << "5. Display List\n";
            std::cout << "6. Check Size\n";
            std::cout << "7. Clear List\n";
            std::cout << "0. Exit Manual Mode\n";
            std::cin >> choice;
            if(choice == 0) break;
            int value, key;
            switch (choice) {
                case 1:
                    std::cout << "Peek: " << _type->peek() << "\n";
                    break;
                case 2:
                    std::cout << "Extract max: " << _type->extract_max() << "\n";
                    break;
                case 3:
                    std::cout << "Enter value and key to insert: ";
                    std::cin >> value >> key;
                    _type->insert(value, key);
                    break;
                case 4:
                    std::cout << "Enter value to modify key: ";
                    std::cin >> value;
                    std::cout << "Enter new key: ";
                    std::cin >> key;
                    _type->modify_key(value, key);
                    break;
                case 5:
                    _type->display();
                    break;
                case 6:
                    std::cout << "List size: " << _type->return_size() << "\n";
                    break;
                case 7:
                    _type->clear();
                    std::cout << "List cleared.\n";
                    break;

                default:
                    std::cout << "Invalid choice.\n";
            }
        }
    }
    void run(){
        std::cout << "Welcome to the List Benchmarking Tool!\n";
        while(true){
            display_menu();
            int choice;
            std::cin >> choice;
            switch (choice) {
                case 1:
                    choose_list_type();
                    break;
                case 2:
                    std::cout << "Current dataset: " << _dataset_name << "\n";
                    std::cout << "Enter new dataset name: ";
                    std::cin >> _dataset_name;
                    break;
                case 3:
                    generate_dataset();
                    break;
                case 4:
                    parameters();
                    break;
                case 5:
                    run_benchmarks();
                    break;
                case 6:
                    manual_mode();
                    break;
                case 7:
                    std::cout << "Exiting...\n";
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};
