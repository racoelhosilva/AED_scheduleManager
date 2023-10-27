#include "Interface.h"

int main() {
    Interface interface;
    if (interface.startMenu()) {
        interface.mainMenu();
        interface.closeMenu();
        return 0;
    }
    cout << "Error loading database files!!!\n";
    return 1;
}


