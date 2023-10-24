#include "Turma.h"

Turma::Turma(string codigoUC, string codigoTurma) {
    this->codigoUC = codigoUC;
    this->codigoTurma = codigoTurma;
    this->aulas = {};
}

string Turma::getcodigoUC() const {
    return codigoUC;
}

string Turma::getcodigoTurma() const {
    return codigoTurma;
}

list<Aula> Turma::getAulas() const {
    return aulas;
}

