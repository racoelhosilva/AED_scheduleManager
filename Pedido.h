#ifndef AED2324_PRJ1_G15_PEDIDO_H
#define AED2324_PRJ1_G15_PEDIDO_H

#include "Estudante.h"

class Pedido {
    public:
        Pedido(Estudante &e, Turma& t1, Turma& t2, string type);

        void addUC(Estudante& e, Turma& t1);
        void removeUC(Estudante& e, Turma& t1);
        void switchUC(Estudante& e, Turma& t1, Turma& t2);
        void addTurma(Estudante& e, Turma& t1);
        void removeTurma(Estudante& e, Turma& t1);
        void switchTurma(Estudante& e, Turma& t1, Turma& t2);

        //funções de verificação
        bool assessUCLimit();
        bool assessTurmaVacancy();
        bool assessScheduleConflict();
        bool assessUCTurmaSingularity();
        bool assessTurmaCap();
        bool assessBalance();

        bool assessValidity();
};


#endif //AED2324_PRJ1_G15_PEDIDO_H
