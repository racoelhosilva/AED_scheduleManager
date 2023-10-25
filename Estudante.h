#ifndef AED2324_PRJ1_G15_ESTUDANTE_H
#define AED2324_PRJ1_G15_ESTUDANTE_H

#include <string>
#include <list>
#include "Turma.h"

using namespace std;

class Estudante {
    private:
        int studentID;
        string studentName;
        list<Turma> schedule;
    public:
        Estudante(int id, string name, list<Turma> classes);

        int const getID();
        string const getName();
        list<Turma> const getSchedule();

        void setSchedule(list<Turma> newSchedule);
};


#endif //AED2324_PRJ1_G15_ESTUDANTE_H
