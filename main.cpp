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
        turmas.push_back(Turma(codigoUC, codigoTurma));
    }
    for (const auto& turma: turmas) {
        cout << "Turma" << turma.getcodigoTurma() << ", " << turma.getcodigoUC() << "\n";
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


