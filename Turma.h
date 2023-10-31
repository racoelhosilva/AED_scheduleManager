#ifndef AED2324_PRJ1_G15_TURMA_H
#define AED2324_PRJ1_G15_TURMA_H
#include "Aula.h"
#include <list>

/**
 * Classe que define uma turma.
 */
class Turma {
private:
    string codigoUC;
    string codigoTurma;
    list<Aula> aulas;
    int occupation = 0;
public:

    /**
     * Construtor sem parâmetros de uma turma.
     */
    Turma();

    /**
     * Construtor parametrizado de uma turma.
     * @param codigoUC - Código da UC.
     * @param codigoTurma - Código da turma.
     */
    Turma(string codigoUC, string codigoTurma);
    string getcodigoUC() const;
    string getcodigoTurma() const;
    list<Aula> getAulas() const;
    void addAulas(const Aula& aula);
    const int getOccupation() const;
    void increaseOccupation();
    void decreaseOccupation();
};


#endif //AED2324_PRJ1_G15_TURMA_H
