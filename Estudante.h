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
        Estudante();
        Estudante(int id, string name, list<Turma> classes = {});

        int const getID() const;
        string const getName() const;
        list<Turma> const getSchedule() const;

        void setSchedule(list<Turma> newSchedule);
        void addToSchedule(Turma t);

        bool operator<(const Estudante& r) const;
        bool operator==(const Estudante& r) const;
};


#endif //AED2324_PRJ1_G15_ESTUDANTE_H
