#ifndef AED2324_PRJ1_G15_PEDIDO_H
#define AED2324_PRJ1_G15_PEDIDO_H

#include "Estudante.h"

class Pedido {
private:
    int studentId;
    int codigoUC;
    int codigoTurma;
    int codigoUCNova;
    int codigoTurmaNova;
    string tipo;

public:
    Pedido(int id, string codigoUCAtual, string codigoTurmaAtual, string type);
    Pedido(int id, string codigoUCAtual, string codigoTurmaAtual, string codigoUCNova, string codigoTurmaNova, string type);

    int getId() {return studentId;}
    int getCodigoUC() {return codigoUC};
    int getCodigoTurma() {return codigoTurma;}
    int getCodigoUCNova() {return codigoUCNova;}
    int getCodigoTurmaNova() {return codigoTurmaNova;}
    string getTipo() {return tipo;}


};


#endif //AED2324_PRJ1_G15_PEDIDO_H
