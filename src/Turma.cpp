#include "Turma.h"

Turma::Turma() {
    codigoUC = "noIdUC",
    codigoTurma = "noIdTurma";
    aulas = {};
}


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

void Turma::addAulas(const Aula& aula) {
    aulas.push_back(aula);
}

const int Turma::getOccupation() const{
    return occupation;
}

void Turma::increaseOccupation(){
    occupation++;
}
void Turma::decreaseOccupation(){
    if (occupation > 0){
        occupation--;
    }
}
