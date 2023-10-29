#include "Estudante.h"

Estudante::Estudante() {
    studentID = 0;
    studentName = "noName";
    schedule = {};
}

Estudante::Estudante(int id, string name, list<Turma> classes) {
    studentID = id;
    studentName = name;
    schedule = classes;
}

int const Estudante::getID() const {return studentID;}
string const Estudante::getName() const {return studentName;}
list<Turma> const Estudante::getSchedule() const {return schedule;}

void Estudante::setSchedule(list<Turma> newSchedule) {schedule = newSchedule;}

void Estudante::addToSchedule(Turma t) {
    schedule.push_back(t);
};

bool Estudante::operator<(const Estudante& r) const{
    return studentID < r.studentID;
}

bool Estudante::operator==(const Estudante& r) const{
    return studentID == r.studentID;
}
