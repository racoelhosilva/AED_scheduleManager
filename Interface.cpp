#include "Interface.h"
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

void Interface::header() {
    system("clear");
    cout    << "┌───────────────────────────────────────────────────────────────────────────────────────────┐\n"
            << "├────────────────────────────── Gestor de Horários (Grupo 15) ──────────────────────────────┤\n\n";
}

void Interface::footer() {
    cout << "\n\n\n\n\n";
}

void Interface::startMenu() {
    this->header();

    cout << "Deseja carregar as alterações realizadas na última sessão?\n";
    cout << "\t\t1. Sim\t\t\t\t2.Não\n";
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

    gestor.extractTurmas(classesPerUCFile);
    cout << "Turmas carregadas!\n";
    gestor.extractAulas(classesFile);
    cout << "Aulas carregadas!\n";
    gestor.extractEstudantes(studentsFile);
    cout << "Estudantes carregados!\n";

    this->footer();
}

void Interface::mainMenu() {
    this->header();
    cout << "O que deseja ver/fazer?\n";
    cout << "\t1. Horários\t\t2. Listagens\t\t3. Ocupações\t\t4. Alterações\t\t5. Sair\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    while(actionOption != "1" && actionOption != "2" && actionOption != "3" && actionOption != "4" && actionOption != "5"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
    }
    switch(stoi(actionOption)) {
        case 1:
            gestor.outputAllTurmas();
            break;
        case 2:
            this->listingsMenu();
            break;
    }

    this->footer();
}

void Interface::listingsMenu() {
    this->header();
    cout << "O que deseja listar?\n";
    cout << "\t1. Estudantes por turma\t\t2. Estudantes por UC\t\t3. Voltar\n";
    cout << "Opcão: ";
    string actionOption;
    cin >> actionOption;
    while(actionOption != "1" && actionOption != "2" && actionOption != "3"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
    }
    switch(stoi(actionOption)) {
        case 1:
            this->listEstudantesPerTurma();
            break;
        case 2:
            this->listEstudantesPerUC();
            break;
        case 3:
            this->mainMenu();
            break;
    }

    this->footer();
}

void Interface::listEstudantesPerUC() {
    cout << "Qual o código da UC? (ex: L.EIC001)\n";
    cout << "Opção: ";
    string ucCode;
    cin >> ucCode;
    while (!gestor.listEstudantesPerUC(ucCode)) {
        cout << "Input Inválido. Opção: ";
        cin >> ucCode;
    }
}

void Interface::listEstudantesPerTurma() {
    cout << "Qual o código da turma? (ex: 1LEIC12)\n";
    cout << "Opção: ";
    string classCode;
    cin >> classCode;
    while (!gestor.listEstudantesPerTurma(classCode)) {
        cout << "Input Inválido. Opção: ";
        cin >> classCode;
    }
}

void Interface::closeMenu() {
    this->header();

    if (alteraçõesFeitas){
        cout << "Deseja guardar as alterações realizadas?\n";
        cout << "\t\t1. Sim\t\t\t\t2.Não\n";
        cout << "Opção: ";

        string savingOption;
        cin >> savingOption;


        while(savingOption != "1" && savingOption != "2") {
            cout << "Input inválido. Opção: ";
            cin >> savingOption;
        }
        if (savingOption == "1"){
            string fname = "../updated_students_classes.csv";
            gestor.saveChanges(fname);
        }
    }

    cout << "Obrigado por usar o Gestor de Horários.";
}