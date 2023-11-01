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

/**
 * Operador de comparação de organizar estudantes alfabeticamente de forma ascendente.
 */
struct EstudanteAlphaAscending {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getName() < b.getName();
    }
};

/**
 * Operador de comparação de organizar estudantes alfabeticamente de forma descendente.
 */
struct EstudanteAlphaDescending {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getName() > b.getName();
    }
};


/**
 * Operador de comparação para organizar aulas de forma cronológica.
 */
struct compareHorario{
    bool operator()(const pair<Aula,Turma> &a1, const pair<Aula,Turma> &a2) const {
        bool compareDia = a1.first.getDia() < a2.first.getDia();
        bool compareHora = a1.first.getDia() == a2.first.getDia() &&  a1.first.getHoraInicio() < a2.first.getHoraInicio();
        return compareDia || compareHora;
    }
};

/**
 * Classe que define o gestor de horários.
 */
class Gestor {
private:
    vector<Turma> turmas;
    vector<Estudante> estudantes;
    queue<Pedido> pedidos;
    stack<Pedido> pedidosRealizados;
    queue<Pedido> pedidosInválidos;
    int cap = 30;

    string numToWeekday[5] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    map<string,int> weekdayToNum = {{"Monday", 0},{"Tuesday", 1},{"Wednesday", 2},{"Thursday", 3},{"Friday", 4}};

public:

    void setCap(int newCap);

    bool extractTurmas(string fname);
    bool extractAulas(string fname);
    bool extractEstudantes(string fname);

    bool outputHorárioEstudante(int id);
    bool outputHorárioTurma(string codigoTurma);
    bool outputHorárioUC(string codigoUC);
    void printHorarios(set<pair<Aula,Turma>, compareHorario> horario);

    bool outputListaEstudanteTurma(string codigoTurma, int order);
    bool outputListaEstudanteUC(string codigoUC, int order);
    bool outputListaEstudanteTurmaUC(string codigoTurma, string codigoUC, int order);
    void outputListaEstudanteAno(int ano, int order);
    bool outputListaTurmas(string codigoUC);
    void outputListaUC(int ano);
    void outputListaEstudanteNUC(int n, int order);
    void outputListaEstudanteMaisNUC(int n, int order);

    bool outputOcupaçãoTurma(string codigoTurma);
    bool outputOcupaçãoUC(string codigoUC);
    void outputOcupaçãoAno(int ano);

    bool assessUCLimit(list<Turma> schedule);
    bool assessScheduleConflict(list<Turma> schedule, Turma nt);
    bool assessUCTurmaSingularity(list<Turma> schedule, Turma nt);
    bool assessTurmaCap(Turma t);
    bool assessBalance(string idUC, string idTurma, string idTurmaAnterior);

    void novoPedidoRemoção(int id, string codigoUC, string codigoTurma);
    void novoPedidoInserção(int id, string codigoUC, string codigoTurma);
    void novoPedidoTroca(int id, string codigoUCAtual, string codigoTurmaAtual, string codigoUCNova, string codigoTurmaNova);

    void procPedido();
    void procTodosPedidos();
    bool existemMudanças();
    bool faltamProcPedidos();

    bool procPedidoRemoção(int id, string codigoUC, string codigoTurma);
    bool procPedidoInserção(int id, string codigoUC, string codigoTurma);
    bool procPedidoTroca(int id, string codigoUCAtual, string codigoTurmaAtual, string codigoUCNova, string codigoTurmaNova);
    bool desfazerÚltimoPedido();

    void saveChanges(string fname);
    void writeStudentClasses(string fname);
    void writeDoneRequests(string fname);
    void writeInvalidRequests(string fname);

    void sortTurmas();
    void sortEstudantesByNumber();
    int binarySearchEstudantes(int id);

    void outputAllTurmas();
    void outputAllAulas();
    void outputAllEstudantes(int order);

};

bool compareTurmas(const Turma &t1, const Turma &t2);
bool compareEstudantes(const Estudante &e1, const Estudante &e2);


#endif //AED2324_PRJ1_G15_GESTOR_H
