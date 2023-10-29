#ifndef AED2324_PRJ1_G15_GESTOR_H
#define AED2324_PRJ1_G15_GESTOR_H

#include "Turma.h"
#include "Estudante.h"
#include "Pedido.h"
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

// Comparison operators
struct EstudanteAlphaAscending {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getName() < b.getName();
    }
};

struct EstudanteAlphaDescending {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getName() > b.getName();
    }
};

struct EstudanteNumAscending {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getID() < b.getID();
    }
};

struct EstudanteNumDescending {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getID() > b.getID();
    }
};

struct compareHorario{
    bool operator()(const pair<Aula,string> &a1, const pair<Aula,string> &a2) const {
        bool compareDia = a1.first.getDia() < a2.first.getDia();
        bool compareHora = a1.first.getDia() == a2.first.getDia() &&  a1.first.getHoraInicio() < a2.first.getHoraInicio();
        return compareDia || compareHora;
    }
};

class Gestor {
private:
    vector<Turma> turmas;
    vector<Estudante> estudantes;
    queue<Pedido> pedidos;
    stack<Pedido> pedidosRealizados;
    list<Pedido> pedidosInválidos;
    int cap = 30;

    map<int,string> numToWeekday = {{0, "Monday"},{1, "Tuesday"},{2, "Wednesday"},{3, "Thursday"},{4, "Friday"}};
    map<string,int> weekdayToNum = {{"Monday", 0},{"Tuesday", 1},{"Wednesday", 2},{"Thursday", 3},{"Friday", 4}};

public:

    void setCap(const int newCap);

    bool extractTurmas(string fname);
    bool extractAulas(string fname);
    bool extractEstudantes(string fname);

    bool outputHorárioEstudante(int id);
    bool outputHorárioTurma(string codigoTurma);
    bool outputHorárioUC(string codigoUC);
    void printHorarios(set<pair<Aula,string>, compareHorario> horario);

    bool outputListaEstudanteTurma(string codigoTurma, int order);
    bool outputListaEstudanteUC(string codigoUC, int order);
    void outputListaEstudanteAno(int ano, int order);
    bool outputListaTurmas(string codigoUC);
    void outputListaUC(int ano);
    void outputListaEstudanteNUC(int n, int order);
    void outputListaEstudanteMaisNUC(int n, int order);

    /* Orders:
     * 1 = Alfabética
     * 2 = Alfabética Inversa
     * 3 = Numérica
     * 4 = Numérica Inversa
     */


    bool outputOcupaçãoTurma(string codigoTurma);
    bool outputOcupaçãoUC(string codigoUC);
    void outputOcupaçãoAno(int ano);

    // Falta função para verificar se pode trocar
    bool makePedido(int id, string codigoUC1, string codigoTurma1, string codigoUC2 = "", string codigoTurma2 = "");
    bool desfazerÚltimoPedido();

    void saveChanges(string fname);

    void sortTurmas();
    void sortEstudantesByNumber();
    int binarySearchEstudantes(int id);

    // Testing Functions for the extracts and output
    void outputAllTurmas();
    void outputAllAulas();
    void outputAllEstudantes(int order);

};

bool compareTurmas(const Turma &t1, const Turma &t2);
bool compareEstudantes(const Estudante &e1, const Estudante &e2);


#endif //AED2324_PRJ1_G15_GESTOR_H
