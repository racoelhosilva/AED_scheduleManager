#include <algorithm>
#include "Gestor.h"
#include <set>

// Comparison operators
struct EstudanteAlphaAscending {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getName() < b.getName();
    }
};

struct EstudanteAlphaDescending {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getName() > b.getName();
    }
};

struct EstudanteNumAscending {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getID() < b.getID();
    }
};

struct EstudanteNumDescending {
    bool operator()(const Estudante& a, const Estudante& b) const {
        return a.getID() > b.getID();
    }
};

bool Gestor::extractTurmas(string fname) {
    ifstream fileReader(fname);
    string line;

    if (!getline(fileReader, line)){
        return false;
    }

    while (getline(fileReader, line)) {
        istringstream fieldReader(line);
        string ucID, classID;
        getline(fieldReader, ucID, ',');
        getline(fieldReader, classID, '\r');
        turmas.emplace_back(ucID, classID);
    }
    return true;
}

bool Gestor::extractAulas(string fname) {
    ifstream fileReader(fname);
    string line;

    if (!getline(fileReader, line)){
        return false;
    }

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
    return true;
}

bool Gestor::extractEstudantes(string fname) {
    ifstream fileReader(fname);
    string line;

    if (!getline(fileReader, line)){
        return false;
    }

    int id, previousId;
    string name, ucID, classID;

    getline(fileReader, line);
    istringstream fieldReader(line);
    char x = ',';
    fieldReader >> id >> x;
    getline(fieldReader, name, ',');
    getline(fieldReader, ucID, ',');
    getline(fieldReader, classID, '\r');
    Estudante currentEstudante = Estudante(id, name);
    previousId = id;
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
        if (id != previousId) {
            currentEstudante.setSchedule(turmasEstudante);
            estudantes.push_back(currentEstudante);
            currentEstudante = Estudante(id, name);
            turmasEstudante = {};
            previousId = id;
        }
        for (Turma &T : turmas) {
            if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
                turmasEstudante.push_back(T);
            }
        }
    }
    currentEstudante.setSchedule(turmasEstudante);
    estudantes.push_back(currentEstudante);

    return true;
}

bool Gestor::outputHorárioEstudante(int id){
    return true;
}

bool Gestor::outputHorárioTurma(string codigoTurma){
    return true;
}

bool Gestor::outputHorárioUC(string codigoUC){
    return true;
}

bool Gestor::outputListaEstudanteTurma(string codigoTurma, int order){
}

bool Gestor::outputListaEstudanteUC(string codigoUC, int order){
    if(find_if(turmas.begin(), turmas.end(), [codigoUC](const Turma& t) {return t.getcodigoUC() == codigoUC;}) == turmas.end())
        return false;
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoUC() == codigoUC) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 2) {
        set<Estudante, EstudanteAlphaDescending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoUC() == codigoUC) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    return true;
}
void Gestor::outputListaEstudanteAno(int ano, int order){return;}
bool Gestor::outputListaTurmas(string codigoUC){return true;}
void Gestor::outputListaUC(int ano){return;}

void Gestor::outputListaEstudanteNUC(int n, int order){return;}
void Gestor::outputListaEstudanteMaisNUC(int n, int order){return;}

bool Gestor::outputOcupaçãoTurma(string codigoTurma){return true;}
bool Gestor::outputOcupaçãoUC(string codigoUC){return true;}
void Gestor::outputOcupaçãoAno(int ano) {return;}

bool Gestor::pedidoRemoção(int id, string codigoTurma){return true;}
bool Gestor::pedidoInserção(int id, string codigoTurma){return true;}
bool Gestor::pedidoTroca(int id, string codigoTurmaAtual, string codigoTurmaNova) {return true;}
bool Gestor::desfazerÚltimoPedido(){return true;}

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

void Gestor::saveChanges(string fname) {
    cout << "Alterações guardadas!\n";
}
