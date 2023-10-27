#ifndef AED2324_PRJ1_G15_INTERFACE_H
#define AED2324_PRJ1_G15_INTERFACE_H

#include "Gestor.h"

class Interface {
private:
    Gestor gestor;
    bool alteraçõesFeitas = false;

    void header();
    void footer();
public:
    // Reading Files
    void startMenu();
    // Operations
    void mainMenu();
    // Saving and Exit
    void closeMenu();
};


#endif //AED2324_PRJ1_G15_INTERFACE_H
