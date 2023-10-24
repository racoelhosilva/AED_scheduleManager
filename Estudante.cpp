#include "Estudante.h"

Estudante::Estudante(int id, string name, list<pair<string,string>> classes) {
    for (pair<string, string> t : classes) {
        schedule.push_back(Turma(t.first, t.second));
    }
}

int const Estudante::getID() {return studentID;}
string const Estudante::getName() {return studentName;}
list<Turma> const Estudante::getSchedule() {return schedule;}

void setSchedule(list<Turma> newSchedule) const {schedule = newSchedule;}