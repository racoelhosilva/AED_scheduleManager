#ifndef AED2324_PRJ1_G15_GESTOR_H
#define AED2324_PRJ1_G15_GESTOR_H

#include "Turma.h"
#include "Estudante.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class Gestor {
private:
    vector<Turma> turmas;
    vector<Estudante> estudantes;

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

    bool outputOcupaçãoTurma(string codigoTurma);
    bool outputOcupaçãoUC(string codigoUC);
    bool outputOcupaçãoAno(int ano);

    void saveChanges(string fname);

    // Testing Functions for the extracts
    void outputAllTurmas();
    void outputAllAulas();
    void outputAllEstudantes();

};


#endif //AED2324_PRJ1_G15_GESTOR_H
