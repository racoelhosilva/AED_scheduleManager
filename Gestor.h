#ifndef AED2324_PRJ1_G15_GESTOR_H
#define AED2324_PRJ1_G15_GESTOR_H

#include "Turma.h"
#include "Estudante.h"
#include "Pedido.h"
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
#include <sstream>
#include <iostream>

class Gestor {
private:
    vector<Turma> turmas;
    vector<Estudante> estudantes;
    queue<Pedido> pedidos;
    stack<Pedido> pedidosRealizados;
    list<Pedido> pedidosInválidos;

public:

    bool extractTurmas(string fname);
    bool extractAulas(string fname);
    bool extractEstudantes(string fname);

    bool outputHorárioEstudante(int id);
    bool outputHorárioTurma(string codigoTurma);
    bool outputHorárioUC(string codigoUC);

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
    bool pedidoRemoção(int id, string codigoTurma);
    bool pedidoInserção(int id, string codigoTurma);
    bool pedidoTroca(int id, string codigoTurmaAtual, string codigoTurmaNova);
    bool desfazerÚltimoPedido();

    void saveChanges(string fname);

    // Testing Functions for the extracts and output
    void outputAllTurmas();
    void outputAllAulas();
    void outputAllEstudantes();

};


#endif //AED2324_PRJ1_G15_GESTOR_H
