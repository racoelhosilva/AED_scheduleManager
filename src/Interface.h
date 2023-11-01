#ifndef AED2324_PRJ1_G15_INTERFACE_H
#define AED2324_PRJ1_G15_INTERFACE_H

#include "Gestor.h"

/**
 * Classe que gere a interface da aplicação com o gestor de horários.
 */
class Interface {
private:
    Gestor gestor;

    void header();
    void footer();
    void inputWait();
public:

    bool startMenu();

    void mainMenu();

    void scheduleMenu();

    void listingMenu();
    void listingStudentsMenu();
    void listingStudentsUCMenu();

    int orderMenu();

    void occupationMenu();

    void requestMenu();
    void processingMenu();

    void totalListingMenu();

    void closeMenu();
};


#endif //AED2324_PRJ1_G15_INTERFACE_H
