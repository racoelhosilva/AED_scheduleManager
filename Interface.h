#ifndef AED2324_PRJ1_G15_INTERFACE_H
#define AED2324_PRJ1_G15_INTERFACE_H

#include "Gestor.h"

class Interface {
private:
    Gestor gestor;
    int alteraçõesFeitas = 0;

    void header();
    void footer();
    void inputWait();
public:
    // Reading Files
    bool startMenu();

    // Operations
    void mainMenu();

    // Schedules
    void scheduleMenu();

    // Listings
    void listingMenu();
    void listingStudentsMenu();
    void listingStudentsUCMenu();

    //Order
    int orderMenu();

    /* Orders:
     * 1 = Alfabética
     * 2 = Alfabética Inversa
     * 3 = Numérica
     * 4 = Numérica Inversa
     */


    // Occupations
    void occupationMenu();

    // Requests
    void requestMenu();

    // Saving and Exit
    void closeMenu();
};


#endif //AED2324_PRJ1_G15_INTERFACE_H
