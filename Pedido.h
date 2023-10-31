#ifndef AED2324_PRJ1_G15_PEDIDO_H
#define AED2324_PRJ1_G15_PEDIDO_H

#include "Estudante.h"

/**
 * Classe de um pedido de alteração de turmas da parte de um estudante.
 */
class Pedido {
private:
    int studentId;
    string codigoUC;
    string codigoTurma;
    string codigoUCNova;
    string codigoTurmaNova;
    string tipo;

public:

    /**
     * Construtor dos pedidos de inserção e remoção.
     * @param id - Número do estudante.
     * @param codigoUCAtual - Código da turma sobre a qual o pedido será efetuado.
     * @param codigoTurmaAtual - Código da UC sobre a qual o pedido será efetuado.
     * @param type - Tipo de pedido.
     */
    Pedido(int id, string codigoUCAtual, string codigoTurmaAtual, string type):
        studentId(id), codigoUC(codigoUCAtual), codigoTurma(codigoTurmaAtual), tipo(type) {};

    /**
     * Construtor dos pedidos de troca.
     * @param id - Número do estudante
     * @param codigoUCAtual - Código da turma da qual o estudante quer ser removido.
     * @param codigoTurmaAtual - Código da UC da qual o estudante quer ser removido.
     * @param codigoUCNova - Código da turma na qual o estudante quer ser inserido.
     * @param codigoTurmaNova - Código da turma na qual o estudante quer ser inserido.
     * @param type - Tipo de pedido.
     */
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
