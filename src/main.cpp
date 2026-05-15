#include "benchmark.hpp"
#include "data_handler.hpp"
#include "menu.hpp"
#include "priority_q.hpp"
#include <iostream>

int main() {
    std::cout << "Uruchamianie SD-PROJEKT-2 – Kolejki priorytetowe\n\n";
    menu main_menu;
    main_menu.run();
    return 0;
}
