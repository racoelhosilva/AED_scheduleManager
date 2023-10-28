#include "Estudante.h"

Estudante::Estudante(int id, string name, list<Turma> classes) {
    studentID = id;
    studentName = name;
    schedule = classes;
}

int const Estudante::getID() {return studentID;}
string const Estudante::getName() {return studentName;}
list<Turma> const Estudante::getSchedule() {return schedule;}

void Estudante::setSchedule(list<Turma> newSchedule) {schedule = newSchedule;}

void Estudante::addToSchedule(Turma t) {
    schedule.push_back(t);
};

bool Estudante::operator<(const Estudante& r) const{
    return studentName < r.studentName;
}
