#include "Gestor.h"

void Gestor::extractTurmas(string fname) {
    ifstream fileReader(fname);
    string line;
    getline(fileReader, line);
    while (getline(fileReader, line)) {
        istringstream fieldReader(line);
        string ucID, classID;
        getline(fieldReader, ucID, ',');
        getline(fieldReader, classID, '\r');
        turmas.push_back(Turma(ucID, classID));
    }
}

void Gestor::extractAulas(string fname) {
    ifstream fileReader(fname);
    string line;
    getline(fileReader, line);
    while (getline(fileReader, line)) {
        istringstream fieldReader(line);
        string classID, ucID, weekday, type;
        float start, duration;
        getline(fieldReader, classID, ',');
        getline(fieldReader, ucID, ',');
        getline(fieldReader, weekday, ',');
        char x;
        fieldReader >> start >> x >> duration >> x;
        getline(fieldReader, type, '\r');
        for (Turma &T : turmas) {
            if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
                T.addAulas(Aula(weekday, start, duration, type));
            }
        }
    }
}

void Gestor::extractEstudantes(string fname) {
    ifstream fileReader(fname);
    string line;
    getline(fileReader, line);

    int id, idOLD; string name, ucID, classID;

    getline(fileReader, line);
    istringstream fieldReader(line);
    char x = ',';
    fieldReader >> id >> x;
    getline(fieldReader, name, ',');
    getline(fieldReader, ucID, ',');
    getline(fieldReader, classID, '\r');
    Estudante currentEstudante = Estudante(id, name);
    idOLD = id;
    list<Turma> turmasEstudante = {};
    for (Turma &T : turmas) {
        if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
            turmasEstudante.push_back(T);
        }
    }

    while (getline(fileReader, line)) {
        istringstream fieldReader(line);
        fieldReader >> id >> x;
        getline(fieldReader, name, ',');
        getline(fieldReader, ucID, ',');
        getline(fieldReader, classID, '\r');
        if (id != idOLD) {
            currentEstudante.setSchedule(turmasEstudante);
            estudantes.push_back(currentEstudante);
            currentEstudante = Estudante(id, name);
            turmasEstudante = {};
            idOLD = id;
        }
        for (Turma &T : turmas) {
            if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
                turmasEstudante.push_back(T);
            }
        }
    }
    currentEstudante.setSchedule(turmasEstudante);
    estudantes.push_back(currentEstudante);
}




// Testing Functions for the extract
void Gestor::outputAllTurmas() {
    for (Turma t : turmas){
        cout << t.getcodigoUC() << ' ' << t.getcodigoTurma() << '\n';
    }
}

void Gestor::outputAllAulas() {
    for (Turma t : turmas){
        cout << t.getcodigoUC() << ' ' << t.getcodigoTurma() << '\t';
            for (Aula a : t.getAulas())
                cout << '\t' << a.getDia() << ' ' << a.getDuracao() << "     \t|\t";
        cout << '\n';
    }
}

void Gestor::outputAllEstudantes() {

    for (Estudante e : estudantes) {
        cout << e.getName() << ", " << e.getID() << endl;
        for (Turma t : e.getSchedule()) {
            cout << "\t" << t.getcodigoUC() << ", " << t.getcodigoTurma() << endl;
            for (Aula a : t.getAulas()) {
                cout << "        " << a.getDia() << ", " << a.getHoraInicio() << ", " << a.getDuracao() << ", " << a.getTipo() << endl;
            }
        }
    }

}