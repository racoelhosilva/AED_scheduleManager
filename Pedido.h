#ifndef AED2324_PRJ1_G15_PEDIDO_H
#define AED2324_PRJ1_G15_PEDIDO_H

#include "Estudante.h"

class Pedido {
private:
    int studentId;
    string codigoUC;
    string codigoTurma;
    string codigoUCNova;
    string codigoTurmaNova;
    string tipo;

public:
    Pedido(int id, string codigoUCAtual, string codigoTurmaAtual, string type) :
        studentId(id), codigoUC(codigoUCAtual), codigoTurma(codigoTurmaAtual), tipo(type) {};
    Pedido(int id, string codigoUCAtual, string codigoTurmaAtual, string codigoUCNova, string codigoTurmaNova, string type) :
        studentId(id), codigoUC(codigoUCAtual), codigoTurma(codigoTurmaAtual), codigoUCNova(codigoUCNova), codigoTurmaNova(codigoTurmaNova), tipo(type) {};

    int getId() {return studentId;}
    string getCodigoUC() {return codigoUC;};
    string getCodigoTurma() {return codigoTurma;}
    string getCodigoUCNova() {return codigoUCNova;}
    string getCodigoTurmaNova() {return codigoTurmaNova;}
    string getTipo() {return tipo;}

};


#endif //AED2324_PRJ1_G15_PEDIDO_H
