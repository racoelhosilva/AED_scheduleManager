#include <iostream>
#include <fstream>
#include <sstream>
#include "Turma.h"
#include <vector>

using namespace std;

void defineClasses() {
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
}

int main() {
    defineClasses();
    return 0;
}


