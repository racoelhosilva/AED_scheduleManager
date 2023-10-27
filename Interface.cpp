#include "Interface.h"
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

void Interface::startMenu() {

    system("clear");
    cout << "Bem-vindo ao Gestor de Horários de LEIC\n\n";

    cout << "Deseja carregar as alterações realizadas na última sessão?\n";
    cout << "\t1. Sim\t\t\t2.Não\n";
    cout << "Opção: ";

    string fileOption;
    cin >> fileOption;


    while(fileOption != "1" && fileOption != "2"){
        cout << "Input inválido. Opção: ";
        cin >> fileOption;
    }

    string classesPerUCFile = "../classes_per_uc.csv";
    string classesFile = "../classes.csv";
    string studentsFile = fileOption == "1" ? "../updated_students_classes.csv" : "../students_classes.csv";

    Gestor gestor;
    gestor.extractTurmas(classesPerUCFile);
    cout << "Turmas carregadas\n";
    gestor.extractAulas(classesFile);
    cout << "Aulas carregadas\n";
    gestor.extractEstudantes(studentsFile);
    cout << "Estudantes carregados\n";
    gestor.outputEstudantes();
}