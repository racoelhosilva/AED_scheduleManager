#include <iostream>
#include <fstream>
#include "Turma.h"
#include <vector>

using namespace std;

void defineClasses() {
    ifstream in("../classes_per_uc.csv");
    if (in.fail()) return;
    string line;
    getline(in, line);
    vector<Turma> turmas;
    while (!in.eof()) {
        string codigoUC, codigoTurma;
        getline(in, codigoUC, ',');
        getline(in, codigoTurma, '\r');
        getline(in, line, '\n');
        turmas.push_back(Turma(codigoUC, codigoTurma));
    }
    ifstream in2("../classes.csv");
    if (in2.fail()) return;
    getline(in, line);
    while (!in2.eof()) {
        string codigoTurma, codigoUC, dia, horaInicio, duracao, tipo;
        getline(in2, codigoTurma, ',');
        getline(in2, codigoUC, ',');
        getline(in2, dia, ',');
        getline(in2, horaInicio, ',');
        getline(in2, duracao, ',');
        getline(in2, tipo, '\r');
        getline(in, line, '\n');
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


