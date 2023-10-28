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

    void extractTurmas(string fname);
    void extractAulas(string fname);
    void extractEstudantes(string fname);

    bool listEstudantesPerUC(string ucCode);
    bool listEstudantesPerTurma(string classCode);

    void saveChanges(string fname);

    // Testing Functions for the extracts
    void outputAllTurmas();
    void outputAllAulas();
    void outputAllEstudantes();

};


#endif //AED2324_PRJ1_G15_GESTOR_H
