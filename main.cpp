#include <iostream>
#include <fstream>
#include <sstream>
#include "Estudante.h"
#include <vector>

using namespace std;

/*void defineClasses() {
    ifstream in("../classes_per_uc.csv");
    if (in.fail()) return;
    string line;
    getline(in, line);
    vector<Turma> turmas;
    while (getline(in, line)) {
        istringstream iss(line);
        string codigoUC, codigoTurma;
        getline(iss, codigoUC, ',');
        getline(iss, codigoTurma, '\r');
        turmas.push_back(Turma(codigoUC, codigoTurma));
    }
    ifstream in2("../classes.csv");
    if (in2.fail()) return;
    getline(in2, line);
    while (getline(in2, line)) {
        istringstream iss(line);
        string codigoTurma, codigoUC, dia, horaInicio, duracao, tipo;
        getline(iss, codigoTurma, ',');
        getline(iss, codigoUC, ',');
        getline(iss, dia, ',');
        getline(iss, horaInicio, ',');
        getline(iss, duracao, ',');
        getline(iss, tipo, '\r');
        for(auto& turma : turmas) {
            if (turma.getcodigoUC() == codigoUC && turma.getcodigoTurma() == codigoTurma) {
                turma.addAulas(Aula(dia, stof(horaInicio), stof(duracao), tipo));
                break;
            }
        }

    }

    for (const auto& turma: turmas) {
        cout << "Turma " << turma.getcodigoTurma() << ", " << turma.getcodigoUC() << "\n";
        for (const auto& aula : turma.getAulas()) {
            cout << "--" << aula.getDia() << ", " << aula.getHoraInicio() << ", " << aula.getDuracao()
                 << ", " << aula.getTipo() << "\n";
        }
    }
}*/

void extractTurmas(vector<Turma> &t) {
    ifstream in("../classes_per_uc.csv");
    string l;
    getline(in, l);
    while (getline(in, l)) {
        istringstream iss(l);
        string ucID, classID;
        getline(iss, ucID, ',');
        getline(iss, classID, '\r');
        t.push_back(Turma(ucID, classID));
    }
}

void extractAulas(vector<Turma> &t) {
    ifstream in("../classes.csv");
    string l;
    getline(in, l);
    vector<Turma> classes;
    while (getline(in, l)) {
        istringstream iss(l);
        string classID, ucID, weekday, type;
        float start, duration;
        getline(iss, classID, ',');
        getline(iss, ucID, ',');
        getline(iss, weekday, ',');
        char x;
        iss >> start >> x >> duration >> x;
        getline(iss, type, '\r');
        for (Turma &T : t) {
            if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
                T.addAulas(Aula(weekday, start, duration, type));
            }
        }
    }

}

void extractEstudantes(vector<Turma> &t, vector<Estudante> &e) {
    ifstream in("../students_classes.csv");
    string l;
    getline(in, l);
    int id, idOLD; string name, ucID, classID;

    getline(in, l);
    istringstream iss(l);
    char x;
    iss >> id >> x;
    getline(iss, name, ',');
    getline(iss, ucID, ',');
    getline(iss, classID, '\r');
    Estudante st = Estudante(id, name);
    idOLD = id;
    list<Turma> c = {};
    for (Turma &T : t) {
        if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
            c.push_back(T);
        }
    }

    while (getline(in, l)) {
        istringstream iss(l);
        iss >> id >> x;
        getline(iss, name, ',');
        getline(iss, ucID, ',');
        getline(iss, classID, '\r');
        if (id != idOLD) {
            st.setSchedule(c);
            e.push_back(st);
            st = Estudante(id, name);
            c = {};
            idOLD = id;
        }
        for (Turma &T : t) {
            if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
                c.push_back(T);
            }
        }
    }
    st.setSchedule(c);
    e.push_back(st);
}

int main() {
    vector<Turma> turmas;
    vector<Estudante> estudantes;
    extractTurmas(turmas);
    extractAulas(turmas);
    /*for(Turma t : turmas) {
        cout << t.getcodigoUC() << ", " << t.getcodigoTurma() << endl;
        for (Aula a : t.getAulas()) {
            cout << "    " << a.getDia() << ", " << a.getHoraInicio() << ", " << a.getDuracao() << ", " << a.getTipo() << endl;
        }
    }
    cout << '\n';*/
    extractEstudantes(turmas, estudantes);
    for (Estudante e : estudantes) {
        cout << e.getName() << ", " << e.getID() << endl;
        for (Turma t : e.getSchedule()) {
            cout << "    " << t.getcodigoUC() << ", " << t.getcodigoTurma() << endl;
            for (Aula a : t.getAulas()) {
                cout << "        " << a.getDia() << ", " << a.getHoraInicio() << ", " << a.getDuracao() << ", " << a.getTipo() << endl;
            }
        }
    }
    return 0;
}


