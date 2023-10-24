#ifndef AED2324_PRJ1_G15_ESTUDANTE_H
#define AED2324_PRJ1_G15_ESTUDANTE_H

#include <string>
#include <list>

using namespace std;

class Estudante {
private:
    int studentID;
    string studentName;
    list<Turma> schedule;
public:
    Estudante(int id, string name, list<pair<string,string>> classes) : studentID(id), studentName(name);

    int const getID();
    string const getName();
    list<Turma> const getSchedule();

    void setSchedule(list<Turma> newSchedule) const;
};


#endif //AED2324_PRJ1_G15_ESTUDANTE_H
