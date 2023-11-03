#include "Interface.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <climits>

using namespace std;

void Interface::header() {
    system("clear");
    cout    << "┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐\n"
            << "├───────────────────────────────────────── Gestor de Horários (Grupo 15) ─────────────────────────────────────────┤\n\n";
}
void Interface::footer() {
    cout << "\n\n\n\n\n";
}
void Interface::inputWait() {
    string discard;
    cout << "\t\t\t\tEscreva algo e pressione <Enter> para continuar\t\t\t\t\t\t\t\t";
    cin >> discard;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
}

bool Interface::startMenu() {
    this->header();

    cout << "Deseja carregar as alterações realizadas na última sessão?\n";
    cout << "\t\t1. Sim\t\t\t\t2.Não\n";
    cout << "Opção: ";

    string fileOption;
    cin >> fileOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');

    while(fileOption != "1" && fileOption != "2"){
        cout << "Input inválido. Opção: ";
        cin >> fileOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    string classesPerUCFile = "../data/classes_per_uc.csv";
    string classesFile = "../data/classes.csv";
    string studentsFile = fileOption == "1" ? "../data/updated_students_classes.csv" : "../data/students_classes.csv";

    if (!this->gestor.extractTurmas(classesPerUCFile)){
        return false;
    }
    cout << "Turmas carregadas!\n";
    if (!this->gestor.extractAulas(classesFile)){
        return false;
    }
    cout << "Aulas carregadas!\n";
    if (!this->gestor.extractEstudantes(studentsFile)) {
        return false;
    }
    cout << "Estudantes carregados!\n";
    cout << "\n";
    int newCap;
    cout << "Qual o valor de \'cap\' que deseja? [default=30]: ";
    cin >> newCap;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while (newCap < 1 || newCap > 50){
        cout << "Input inválido. Tente novamente: ";
        cin >> newCap;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
    cout << "Nota:\teste valor só é usado para verificar se é possível realizar trocas,\n\t\tnão para carregar os dados iniciais da base de dados.\n";
    this->gestor.setCap(newCap);
    if (newCap != 30) {
        cout << "Valor \'cap\' atualizado!\n";
    }

    this->footer();
    return true;
}

void Interface::mainMenu() {
    this->header();

    cout << "O que deseja ver/fazer?\n";
    cout << "\t1. Horários\t\t2. Listagens\t\t3. Ocupações\t\t4. Alterações\t\t5. Listas Totais\t\t6. Sair\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while(actionOption != "1" && actionOption != "2" && actionOption != "3" && actionOption != "4" && actionOption != "5" && actionOption != "6"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    switch (stoi(actionOption)){
        case 1:
            cout << "\"Horários\" selecionado!\n\n";
            this->scheduleMenu();
            break;
        case 2:
            cout << "\"Listagens\" selecionado!\n\n";
            this->listingMenu();
            break;
        case 3:
            cout << "\"Ocupações\" selecionado!\n\n";
            this->occupationMenu();
            break;
        case 4:
            cout << "\"Alterações\" selecionado!\n\n";
            this->requestMenu();
            break;
        case 5:
            cout << "\"Listas Totais\" selecionado!\n\n";
            this->totalListingMenu();
            break;
        default:
            cout << "\"Sair\" selecionado!\n\n";
            this->footer();
            this->closeMenu();
    }
}

void Interface::scheduleMenu() {
    cout << "Que horário pretende ver?\n";
    cout << "\t1. Estudante\t\t2. Turma\t\t3. Unidade Curricular\t\t4. Voltar\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while(actionOption != "1" && actionOption != "2" && actionOption != "3" && actionOption != "4"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    switch (stoi(actionOption)){
        case 1:{
            int id;
            cout << "\"Horários por Estudante\" selecionado!\n";
            cout << "Número de Estudante (ex. 200000000): ";
            cin >> id;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (id <= 200000000 || id >= 209999999){
                cout << "Input inválido. Número de Estudante: ";
                cin >> id;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            if (!gestor.outputHorárioEstudante(id)){
                cout << "Estudante não encontrado!\n";
                this->scheduleMenu();
            }
            else {
                this->inputWait();
            }
            break;}
        case 2:{
            string codigoTurma;
            cout << "\"Horários por Turma\" selecionado!\n";
            cout << "Código da Turma (ex. 1LEIC00): ";
            cin >> codigoTurma;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (codigoTurma.length() != 7 || (codigoTurma[0] < '1' || codigoTurma[0] > '3')){
                cout << "Input inválido. Código da Turma: ";
                cin >> codigoTurma;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            if (!gestor.outputHorárioTurma(codigoTurma)){
                cout << "Turma não encontrada!\n";
                this->scheduleMenu();
            }
            this->inputWait();
            break;}
        case 3:{
            string codigoUC;
            cout << "\"Horários por Unidade Curricular\" selecionado!\n";
            cout << "Código da Unidade Curricular (ex. L.EIC000): ";
            cin >> codigoUC;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while ((codigoUC.length() != 8 || codigoUC[1] != '.' || codigoUC[0] != 'L') && codigoUC != "UP001"){
                cout << "Input inválido. Código da Unidade Curricular: ";
                cin >> codigoUC;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            if (!gestor.outputHorárioUC(codigoUC)){
                cout << "Unidade Curricular não encontrada!\n";
                this->scheduleMenu();
            }
            this->inputWait();
            break;}
        default:{
            cout << "\"Voltar\" selecionado!\n";
            break;}
    }

    this->footer();
    this->mainMenu();
}

void Interface::listingMenu() {
    cout << "Que tipo de listagem pretende?\n";
    cout << "\t1. Estudante\t\t2. Turma\t\t3. Unidade Curricular\t\t4. Estudante/Cadeira\t\t5. Voltar\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while(actionOption != "1" && actionOption != "2" && actionOption != "3" && actionOption != "4" && actionOption != "5"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    switch (stoi(actionOption)){
        case 1:{
            string codigoUC;
            cout << "\"Listagens de Estudantes\" selecionado!\n";
            this->listingStudentsMenu();
            break;}
        case 2:{
            string codigoUC;
            cout << "\"Listagens de Turmas\" selecionada!\n";
            cout << "Código da Unidade Curricular (ex. L.EIC000): ";
            cin >> codigoUC;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while ((codigoUC.length() != 8 || codigoUC[1] != '.' || codigoUC[0] != 'L') && codigoUC != "UP001"){
                cout << "Input inválido. Código da Unidade Curricular: ";
                cin >> codigoUC;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }

            if (!gestor.outputListaTurmas(codigoUC)){
                cout << "Unidade Curricular não encontrada!\n";
                this->listingMenu();
            }
            else {
                this->inputWait();
            }
            break;}
        case 3:{
            int ano;
            cout << "\"Listagens de Unidades Curriculares\" selecionada!\n";
            cout << "Ano (ex. 1): ";
            cin >> ano;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (ano < 1 || ano > 3){
                cout << "Input inválido. Ano: ";
                cin >> ano;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            gestor.outputListaUC(ano);
            this->inputWait();
            break;}
        case 4:
            cout << "\"Listagens de Estudantes/Cadeiras\" selecionado!\n\n";
            this->listingStudentsUCMenu();
            break;
        default:{
            cout << "\"Voltar\" selecionado!\n";
            break;}
    }

    this->footer();
    this->mainMenu();
}
void Interface::listingStudentsMenu() {
    cout << "Que tipo de listagem pretende?\n";
    cout << "\t1. Estudante numa Turma\t\t2. Estudante numa Unidade Curricular\t\t3. Estudante numa Turma e UC\t\t4. Estudante num Ano\t\t5. Voltar\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while(actionOption != "1" && actionOption != "2" && actionOption != "3" && actionOption != "4" && actionOption != "5"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    switch (stoi(actionOption)) {
        case 1: {
            string codigoTurma;
            cout << "Código da Turma (ex. 1LEIC00): ";
            cin >> codigoTurma;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (codigoTurma.length() != 7 || (codigoTurma[0] < '1' || codigoTurma[0] > '3')) {
                cout << "Input inválido. Código da Turma: ";
                cin >> codigoTurma;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            int order = this->orderMenu();
            if (!gestor.outputListaEstudanteTurma(codigoTurma, order)) {
                cout << "Turma não encontrada!\n";
                this->listingStudentsMenu();
            }
            this->inputWait();
            break;
        }
        case 2: {
            string codigoUC;
            cout << "Código da Unidade Curricular (ex. L.EIC000): ";
            cin >> codigoUC;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while ((codigoUC.length() != 8 || codigoUC[1] != '.' || codigoUC[0] != 'L') && codigoUC != "UP001") {
                cout << "Input inválido. Código da Unidade Curricular: ";
                cin >> codigoUC;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            int order = this->orderMenu();
            if (!gestor.outputListaEstudanteUC(codigoUC, order)) {
                cout << "Unidade Curricular não encontrada!\n";
                this->listingStudentsMenu();
            }
            this->inputWait();
            break;
        }
        case 3:{
            string codigoTurma;
            cout << "Código da Turma (ex. 1LEIC00): ";
            cin >> codigoTurma;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (codigoTurma.length() != 7 || (codigoTurma[0] < '1' || codigoTurma[0] > '3')) {
                cout << "Input inválido. Código da Turma: ";
                cin >> codigoTurma;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            string codigoUC;
            cout << "Código da Unidade Curricular (ex. L.EIC000): ";
            cin >> codigoUC;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while ((codigoUC.length() != 8 || codigoUC[1] != '.' || codigoUC[0] != 'L') && codigoUC != "UP001") {
                cout << "Input inválido. Código da Unidade Curricular: ";
                cin >> codigoUC;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            int order = this->orderMenu();
            if (!gestor.outputListaEstudanteTurmaUC(codigoTurma, codigoUC, order)) {
                cout << "Turma ou Unidade Curricular não encontrada!\n";
                this->listingStudentsMenu();
            }
            this->inputWait();
            break;
        }
        case 4:{
            int ano;
            cout << "Ano (ex. 1): ";
            cin >> ano;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (ano < 1 || ano > 3){
                cout << "Input inválido. Ano: ";
                cin >> ano;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            int order = this->orderMenu();
            gestor.outputListaEstudanteAno(ano, order);
            this->inputWait();
            break;}
        default:{
            cout << "\"Voltar\" selecionado!\n";
            break;}
    }

    this->listingMenu();
}
void Interface::listingStudentsUCMenu() {
    cout << "Que tipo de listagem pretende?\n";
    cout << "\t1. Estudante com n Unidades Curriculares\t\t2. Estudante com >n Unidades Curriculares\t\t3. Voltar\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while(actionOption != "1" && actionOption != "2" && actionOption != "3"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    switch (stoi(actionOption)) {
        case 1:{
            int n;
            cout << "n: ";
            cin >> n;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (n < 1 || n > 10){
                cout << "Input inválido. n: ";
                cin >> n;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            int order = this->orderMenu();
            gestor.outputListaEstudanteNUC(n, order);
            this->inputWait();
            break;}
        case 2:{
            int n;
            cout << "n: ";
            cin >> n;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (n < 1 || n > 10){
                cout << "Input inválido. n: ";
                cin >> n;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            int order = this->orderMenu();
            gestor.outputListaEstudanteMaisNUC(n, order);
            this->inputWait();
            break;}
        default:{
            cout << "\"Voltar\" selecionado!\n";
            break;}
    }

    this->listingMenu();
}
int Interface::orderMenu() {
    cout << "Que tipo de ordem pretende?\n";
    cout << "\t1. Alfabética\t\t2. Alfabética Inversa\t\t3. Numérica\t\t4. Numérica Inversa\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while(actionOption != "1" && actionOption != "2" && actionOption != "3" && actionOption != "4"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
    return stoi(actionOption);

}

void Interface::occupationMenu(){
    cout << "Que ocupações pretende ver?\n";
    cout << "\t1. Turma\t\t2. Unidade Curricular\t\t3. Ano\t\t4. Voltar\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while(actionOption != "1" && actionOption != "2" && actionOption != "3" && actionOption != "4"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    switch(stoi(actionOption)){
        case 1:{
            string codigoTurma;
            cout << "Código da Turma (ex. 1LEIC00): ";
            cin >> codigoTurma;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (codigoTurma.length() != 7 || (codigoTurma[0] < '1' || codigoTurma[0] > '3')){
                cout << "Input inválido. Código da Turma: ";
                cin >> codigoTurma;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            if (!gestor.outputOcupaçãoTurma(codigoTurma)){
                cout << "Turma não encontrada!\n";
                this->occupationMenu();
            }
            this->inputWait();
            break;}
        case 2:{
            string codigoUC;
            cout << "Código da Unidade Curricular (ex. L.EIC000): ";
            cin >> codigoUC;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while ((codigoUC.length() != 8 || codigoUC[1] != '.' || codigoUC[0] != 'L') && codigoUC != "UP001"){
                cout << "Input inválido. Código da Unidade Curricular: ";
                cin >> codigoUC;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            if (!gestor.outputOcupaçãoUC(codigoUC)){
                cout << "Unidade Curricular não encontrada!\n";
                this->occupationMenu();
            }
            this->inputWait();
            break;}
        case 3:{
            int ano;
            cout << "Ano (ex. 1): ";
            cin >> ano;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (ano < 1 || ano > 3){
                cout << "Input inválido. Ano: ";
                cin >> ano;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            gestor.outputOcupaçãoAno(ano);
            this->inputWait();
            break;}

        default:{
            cout << "\"Voltar\" selecionado!\n";
            break;}
    }

    this->footer();
    this->mainMenu();
}

void Interface::requestMenu(){
    cout << "Que alterações pretende fazer?\n";
    cout << "\t1. Remoção\t\t2. Inserção\t\t3. Troca\t\t4. Processar\t\t5. Anular Última Alteração\t\t6. Voltar\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while(actionOption != "1" && actionOption != "2" && actionOption != "3" && actionOption != "4" && actionOption != "5" && actionOption != "6"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    switch (stoi(actionOption)) {
        case 1:{
            int id;
            cout << "Número de Estudante (ex. 200000000): ";
            cin >> id;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (id <= 200000000 || id >= 209999999){
                cout << "Input inválido. Número de Estudante: ";
                cin >> id;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            string codigoUC;
            cout << "Código da Unidade Curricular (ex. L.EIC000): ";
            cin >> codigoUC;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while ((codigoUC.length() != 8 || codigoUC[1] != '.' || codigoUC[0] != 'L') && codigoUC != "UP001"){
                cout << "Input inválido. Código da Unidade Curricular: ";
                cin >> codigoUC;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            string codigoTurma;
            cout << "Código da Turma (ex. 1LEIC00): ";
            cin >> codigoTurma;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (codigoTurma.length() != 7 || (codigoTurma[0] < '1' || codigoTurma[0] > '3')){
                cout << "Input inválido. Código da Turma: ";
                cin >> codigoTurma;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            gestor.novoPedidoRemoção(id, codigoUC, codigoTurma);
            cout << "Pedido Realizado\n";
            this->inputWait();
            break;}
        case 2:{
            int id;
            cout << "Número de Estudante (ex. 200000000): ";
            cin >> id;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (id <= 200000000 || id >= 209999999){
                cout << "Input inválido. Número de Estudante: ";
                cin >> id;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            string codigoUC;
            cout << "Código da Unidade Curricular (ex. L.EIC000): ";
            cin >> codigoUC;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while ((codigoUC.length() != 8 || codigoUC[1] != '.' || codigoUC[0] != 'L') && codigoUC != "UP001"){
                cout << "Input inválido. Código da Unidade Curricular: ";
                cin >> codigoUC;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            string codigoTurma;
            cout << "Código da Turma (ex. 1LEIC00): ";
            cin >> codigoTurma;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (codigoTurma.length() != 7 || (codigoTurma[0] < '1' || codigoTurma[0] > '3')){
                cout << "Input inválido. Código da Turma: ";
                cin >> codigoTurma;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            gestor.novoPedidoInserção(id, codigoUC, codigoTurma);
            cout << "Pedido adicionado\n";
            this->inputWait();
            break;}
        case 3:{
            int id;
            cout << "Número de Estudante (ex. 200000000): ";
            cin >> id;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (id <= 200000000 || id >= 209999999){
                cout << "Input inválido. Número de Estudante: ";
                cin >> id;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            string codigoUCAtual;
            cout << "Código da Unidade Curricular (ex. L.EIC000): ";
            cin >> codigoUCAtual;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while ((codigoUCAtual.length() != 8 || codigoUCAtual[1] != '.' || codigoUCAtual[0] != 'L') && codigoUCAtual != "UP001"){
                cout << "Input inválido. Código da Unidade Curricular: ";
                cin >> codigoUCAtual;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            string codigoTurmaAtual;
            cout << "Código da Turma Atual (ex. 1LEIC00): ";
            cin >> codigoTurmaAtual;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (codigoTurmaAtual.length() != 7 || (codigoTurmaAtual[0] < '1' || codigoTurmaAtual[0] > '3')){
                cout << "Input inválido. Código da Turma: ";
                cin >> codigoTurmaAtual;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            string codigoUCNova;
            cout << "Código da Unidade Curricular (ex. L.EIC000): ";
            cin >> codigoUCNova;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while ((codigoUCNova.length() != 8 || codigoUCNova[1] != '.' || codigoUCNova[0] != 'L') && codigoUCNova != "UP001"){
                cout << "Input inválido. Código da Unidade Curricular: ";
                cin >> codigoUCNova;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            string codigoTurmaNova;
            cout << "Código da Nova Turma (ex. 1LEIC00): ";
            cin >> codigoTurmaNova;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            while (codigoTurmaNova.length() != 7 || (codigoTurmaNova[0] < '1' || codigoTurmaNova[0] > '3')){
                cout << "Input inválido. Código da Turma: ";
                cin >> codigoTurmaNova;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
            }
            gestor.novoPedidoTroca(id, codigoUCAtual, codigoTurmaAtual,codigoUCNova, codigoTurmaNova);
            cout << "Pedido adicionado\n";
            this->inputWait();
            break;}
        case 4:{
            this->processingMenu();
            break;}
        case 5:{
            if (!gestor.existemMudanças()){
                cout << "Operação não permitida! Nenhuma alteração realizada.\n";
                this->requestMenu();
            }
            if (!gestor.desfazerÚltimoPedido()){
                cout << "Operação não permitida!\n";
                this->requestMenu();
            }
            cout << "Anulamento realizado com sucesso.\n";
            this->inputWait();
            break;}
        default:{
            cout << "\"Voltar\" selecionado!\n";
            break;}
    }

    this->footer();
    this->mainMenu();
}

void Interface::processingMenu() {
    cout << "Como deseja processar os pedidos?\n";
    cout << "\t1. Processar 1 pedido\t\t2. Processar todos\t\t3. Voltar\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while(actionOption != "1" && actionOption != "2" && actionOption != "3"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
    switch (stoi(actionOption)){
        case 1:{
            gestor.procPedido();
            cout << "Pedido processado\n";
            this->inputWait();
            break;}
        case 2:{
            gestor.procTodosPedidos();
            cout << "Pedidos processados\n";
            this->inputWait();
            break;}
        default:{
            cout << "\"Voltar\" selecionado!\n";
            break;}
    }

    this->footer();
    this->requestMenu();
}

void Interface::totalListingMenu() {
    cout << "Que listagem pretende ver?\n";
    cout << "\t1. Estudantes\t\t2. Aulas\t\t3. Turmas\t\t4. Voltar\n";
    cout << "Opção: ";
    string actionOption;
    cin >> actionOption;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    while(actionOption != "1" && actionOption != "2" && actionOption != "3" && actionOption != "4"){
        cout << "Input inválido. Opção: ";
        cin >> actionOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    switch (stoi(actionOption)) {
        case 1:{
            int order = this->orderMenu();
            gestor.outputAllEstudantes(order);
            this->inputWait();
            break;}
        case 2:{
            gestor.outputAllAulas();
            this->inputWait();
            break;}
        case 3:{
            gestor.outputAllTurmas();
            this->inputWait();
            break;}
        default:{
            cout << "\"Voltar\" selecionado!\n";
            break;}
    }

    this->footer();
    this->mainMenu();
}

void Interface::closeMenu() {
    this->header();

    if (gestor.existemMudanças()){
        cout << "Deseja guardar as alterações realizadas?\n";
        cout << "\t\t1. Sim\t\t\t\t2.Não\n";
        cout << "Opção: ";

        string savingOption;
        cin >> savingOption;
        cin.clear();
        cin.ignore(INT_MAX, '\n');


        while(savingOption != "1" && savingOption != "2") {
            cout << "Input inválido. Opção: ";
            cin >> savingOption;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        if (savingOption == "1"){
            if (gestor.faltamProcPedidos()){
                cout << "Faltam realizar pedidos, deseja realizá-los?\n";
                cout << "\t\t1. Sim\t\t\t\t2.Não\n";
                cout << "Opção: ";

                string procOption;
                cin >> procOption;
                cin.clear();
                cin.ignore(INT_MAX, '\n');

                while(procOption != "1" && procOption != "2") {
                    cout << "Input inválido. Opção: ";
                    cin >> procOption;
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                }
                if (procOption == "1"){
                    gestor.procTodosPedidos();
                }
            }
            string fname = "../data/updated_students_classes.csv";
            gestor.saveChanges(fname);
        }


    }

    cout << "Obrigado por usar o Gestor de Horários.\n";
    exit(0);
}