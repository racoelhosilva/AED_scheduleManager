#ifndef AED2324_PRJ1_G15_TURMA_H
#define AED2324_PRJ1_G15_TURMA_H
#include "Aula.h"
#include <list>

class Turma {
private:
    string codigoUC;
    string codigoTurma;
    list<Aula> aulas;

public:
    Turma(string codigoUC, string codigoTurma);
    string getcodigoUC() const;
    string getcodigoTurma() const;
    list<Aula> getAulas() const;
};


#endif //AED2324_PRJ1_G15_TURMA_H
