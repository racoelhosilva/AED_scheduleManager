#include <algorithm>
#include "Gestor.h"
#include <set>
#include <map>
#include <iomanip>
#include <cmath>
#include <algorithm>

/**
 * Altera a número máximo de alunos permitido numa turma.
 * Complexidade: O(1).
 * @param newCap - Novo limite de alunos por turma.
 */
void Gestor::setCap(const int newCap){
    this->cap = newCap;
}

/**
 * Extrai as turmas para um vetor ordenado.
 * Complexidade: O(n), sendo n o número de linhas do ficheiro a ler.
 * @param fname - Nome do ficheiro que contém as turmas a extrair.
 * @return Verdadeiro se o ficheiro existe.
 */
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
    sortTurmas();
    return true;
}

/**
 * Extrai as aulas para uma lista pertencente à turma correspondente.
 * Complexidade: O(n * m), sendo n o número de linhas do ficheiro a ler e m o número de turmas.
 * @param fname - Nome do ficheiro que contém as aulas a extrair.
 * @return Verdadeiro se o ficheiro existe.
 */
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
        int numWD = weekdayToNum[weekday];
        for (Turma &T : turmas) {
            if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
                T.addAulas(Aula(numWD, start, duration, type));
            }
        }
    }
    return true;
}

/**
 * Extrai os estudantes para um vetor ordenado por ordem numérica.
 * Complexidade: O (n * m), sendo n o número de linhas do ficheiro a ler e m o número de turmas.
 * @param fname - Nome do ficheiro que contém os estudantes a extrair.
 * @return Verdadeiro se o ficheiro existe.
 */
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
            T.increaseOccupation();
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
                T.increaseOccupation();
            }
        }
    }
    currentEstudante.setSchedule(turmasEstudante);
    estudantes.push_back(currentEstudante);
    sortEstudantesByNumber();
    return true;
}

/**
 * Imprime o horário de um estudante.
 * Complexidade: O(log n), sendo n o número de estudantes.
 * @param id - Número do estudante cujo horário deverá ser impresso.
 * @return Verdadeiro se o estudante existe.
 */
bool Gestor::outputHorárioEstudante(int id){
    Estudante target(id, "", {});
    int idx = binarySearchEstudantes(id);
    if (idx == -1){
        return false;
    }
    set<pair<Aula,Turma>, compareHorario> horario;
    for (Turma turma : estudantes[idx].getSchedule()) {
        for (auto aula : turma.getAulas()){
            horario.insert({aula, {turma.getcodigoUC(), turma.getcodigoTurma()}});
        }
    }cout << "Horário do estudante: " << estudantes[idx].getName() << " (" << id << ")\n";
    printHorarios(horario);
    return true;
}

/**
 * Imprime o horário de uma turma.
 * Complexidade: O(n), sendo n o número de turmas.
 * @param codigoTurma - Código da turma cujo horário deverá ser impresso.
 * @return Verdadeiro se a turma existe.
 */
bool Gestor::outputHorárioTurma(string codigoTurma){
    set<pair<Aula,Turma>, compareHorario> horario;
    for (Turma turma : turmas) {
        if (turma.getcodigoTurma() == codigoTurma) {
            for (auto aula: turma.getAulas()) {
                horario.insert({aula, {turma.getcodigoUC(), turma.getcodigoTurma()}});
            }
        }
    }
    if (horario.empty()){
        return false;
    }
    cout << "Horário da Turma: " << codigoTurma << '\n';
    printHorarios(horario);
    return true;
}

/**
 * Imprime o horário de uma UC.
 * Complexidade: 0(n), sendo n o número de turmas.
 * @param codigoUC - Código da UC cujo horário deverá ser impresso.
 * @return Verdadeiro se a UC existe.
 */
bool Gestor::outputHorárioUC(string codigoUC){
    set<pair<Aula,Turma>, compareHorario> horario;
    for (Turma turma : turmas) {
        if (turma.getcodigoUC() == codigoUC) {
            for (auto aula: turma.getAulas()) {
                horario.insert({aula, {turma.getcodigoUC(), turma.getcodigoTurma()}});
            }
        }
    }
    if (horario.empty()){
        return false;
    }
    cout << "Horário da Unidade Curricular: " << codigoUC << '\n';
    printHorarios(horario);
    return true;
}

/**
 * Formata a impressão dos horários.
 * Complexidade: O(n), sendo n o número de aulas.
 * @param horario - Set de aulas que constituem o horário.
 */
void Gestor::printHorarios(set<pair<Aula,Turma>, compareHorario> horario){
    int currentWeekday = -1;
    for (auto x : horario){
        if (x.first.getDia() > currentWeekday){
            currentWeekday = x.first.getDia();
            cout << numToWeekday[currentWeekday] << '\n';
        }
        int h1, m1, h2, m2;
        h1 = int(x.first.getHoraInicio());
        m1 = (x.first.getHoraInicio() - h1) == 0.0 ? 0 : 30;
        h2 = int(x.first.getHoraInicio() + x.first.getDuracao());
        m2 = (x.first.getHoraInicio() + x.first.getDuracao() - h2) == 0.0 ? 0 : 30;
        cout << '\t' <<
             setw(2) << setfill('0') << h1 << ':' << setw(2) << setfill('0') << m1 << " - " <<
             setw(2) << setfill('0') << h2 << ':' << setw(2) << setfill('0') << m2 << '\t' <<
             x.second.getcodigoUC() << "  (" << x.first.getTipo() << ")\t" << x.second.getcodigoTurma() << '\n';
    }
}

/**
 * Imprime a lista de estudantes pertencentes a uma turma, em determinada ordem.
 * Complexidade: O(n * m), sendo n o número de estudantes e m o número de turmas a que cada estudante pertence.
 * @param codigoTurma - Código da turma à qual os estudantes pertencem.
 * @param order - Ordem em qual deverão ser impressos os estudantes.
 * @return Verdadeiro se a turma existe.
 */
bool Gestor::outputListaEstudanteTurma(string codigoTurma, int order){
    if(find_if(turmas.begin(), turmas.end(), [codigoTurma](const Turma& t) {return t.getcodigoTurma() == codigoTurma;}) == turmas.end())
        return false;
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma() == codigoTurma) {
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
                if (t.getcodigoTurma() == codigoTurma) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 3) {
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma() == codigoTurma) {
                    cout << e.getID() << " " << e.getName() << "\n";
                    break;
                }
            }
        }
    }
    if (order == 4) {
        for (auto e = estudantes.rbegin(); e != estudantes.rend(); e++) {
            for (Turma t : e->getSchedule()) {
                if (t.getcodigoTurma() == codigoTurma) {
                    cout << e->getID() << " " << e->getName() << "\n";
                    break;
                }
            }
        }
    }
    return true;
}

/**
 * Imprime a lista de estudantes pertencentes a uma UC, em determinada ordem.
 * Complexidade: O(n * m * log n), sendo n o número de estudantes e m o número de turmas a que cada estudante pertence.
 * @param codigoUC - Código da UC à qual os estudantes pertencem.
 * @param order - Ordem em qual deverão ser impressos os estudantes.
 * @return Verdadeiro se a UC existe.
 */
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
    if (order == 3) {
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoUC() == codigoUC) {
                    cout << e.getID() << " " << e.getName() << "\n";
                    break;
                }
            }
        }
    }
    if (order == 4) {
        for (auto e = estudantes.rbegin(); e != estudantes.rend(); e++) {
            for (Turma t : e->getSchedule()) {
                if (t.getcodigoUC() == codigoUC) {
                    cout << e->getID() << " " << e->getName() << "\n";
                    break;
                }
            }
        }
    }
    return true;
}

/**
 * Imprime a lista de estudantes pertencentes a um ano curricular, em determinada ordem.
 * Complexidade: O(n * m * log n), sendo n o número de estudantes e m o número de turmas a que cada estudante pertence.
 * @param ano - Ano curricular a qual os estudantes pertencem.
 * @param order - Ordem em qual deverão ser impressos os estudantes.
 */
void Gestor::outputListaEstudanteAno(int ano, int order){
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma()[0] == (char)(ano + '0')) {
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
                if (t.getcodigoTurma()[0] == (char)(ano + '0')) {
                    lista.insert(e);
                    break;
                }
            }
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 3) {
        for (Estudante e: estudantes) {
            for (Turma t: e.getSchedule()) {
                if (t.getcodigoTurma()[0] == (char)(ano + '0')) {
                    cout << e.getID() << " " << e.getName() << "\n";
                    break;
                }
            }
        }
    }
    if (order == 4) {
        for (auto e = estudantes.rbegin(); e != estudantes.rend(); e++) {
            for (Turma t : e->getSchedule()) {
                if (t.getcodigoTurma()[0] == (char)(ano + '0')) {
                    cout << e->getID() << " " << e->getName() << "\n";
                    break;
                }
            }
        }
    }
}

/**
 * Imprime a lista de turmas pertencentes a uma UC.
 * Complexidade: O(n), sendo n o número de turmas.
 * @param codigoUC - Código da UC à qual as turmas pertencem.
 * @return Verdadeiro se a UC existe.
 */
bool Gestor::outputListaTurmas(string codigoUC){
    if(find_if(turmas.begin(), turmas.end(), [codigoUC](const Turma& t) {return t.getcodigoUC() == codigoUC;}) == turmas.end())
        return false;
    for (Turma t : turmas) {
        if(t.getcodigoUC() == codigoUC)
            cout << t.getcodigoTurma() << "\n";
    }
    return true;
}

/**
 * Imprime a lista de UCs pertencentes a um ano curricular.
 * Complexidade: O(n), sendo n o número de turmas.
 * @param ano - Ano curricular a qual as UCs pertencem.
 */
void Gestor::outputListaUC(int ano){

    for (Turma t : turmas) {
        if (t.getcodigoTurma()[0] == (char)(ano + '0') && t.getcodigoTurma()[5] == '0' && t.getcodigoTurma()[6] == '1')
            cout << t.getcodigoUC() << "\n";
    }
}

/**
 * Imprime a lista de estudantes pertencentes a n UCs.
 * Complexidade: O(n log n), sendo n o número de estudantes.
 * @param n - Número de UCs
 * @param order - Ordem em qual deverão ser impressos os estudantes.
 */
void Gestor::outputListaEstudanteNUC(int n, int order) {
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for (Estudante e: estudantes) {
            if (e.getSchedule().size() == n)
                lista.insert(e);
        }
        for (Estudante e: lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 2) {
        set<Estudante, EstudanteAlphaDescending> lista;
        for (Estudante e: estudantes) {
            if (e.getSchedule().size() == n)
                lista.insert(e);
        }
        for (Estudante e: lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 3) {
        for (Estudante e: estudantes) {
            if (e.getSchedule().size() == n) {
                cout << e.getID() << " " << e.getName() << "\n";
            }
        }
    }

    if (order == 4) {
        for (auto e = estudantes.rbegin(); e != estudantes.rend(); e++) {
            if (e->getSchedule().size() == n) {
                cout << e->getID() << " " << e->getName() << "\n";
            }
        }
    }
}

/**
 * Imprime a lista de estudantes pertencentes a mais do que n UCs.
 * Complexidade: O(n log n), sendo n o número de estudantes.
 * @param n - Número de UCs.
 * @param order - Ordem em qual deverão ser impressos os estudantes.
 */
void Gestor::outputListaEstudanteMaisNUC(int n, int order){
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for(Estudante e : estudantes) {
            if (e.getSchedule().size() > n)
                lista.insert(e);
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 2) {
        set<Estudante, EstudanteAlphaDescending> lista;
        for(Estudante e : estudantes) {
            if (e.getSchedule().size() > n)
                lista.insert(e);
        }
        for (Estudante e : lista) {
            cout << e.getID() << " " << e.getName() << "\n";
        }
    }
    if (order == 3) {
        for (Estudante e: estudantes) {
            if (e.getSchedule().size() > n) {
                cout << e.getID() << " " << e.getName() << "\n";
            }
        }
    }
    if (order == 4) {
        for (auto e = estudantes.rbegin(); e != estudantes.rend(); e++) {
            if (e->getSchedule().size() > n) {
                cout << e->getID() << " " << e->getName() << "\n";
            }
        }
    }
}

/**
 * Imprime a ocupação de uma turma.
 * Complexidade: O(n * m), sendo n o número de estudantes e m o número de turmas a que cada estudante pertence.
 * @param codigoTurma - Código da turma cuja ocupação deverá ser impressa.
 * @return Verdadeiro se a turma existe.
 */
bool Gestor::outputOcupaçãoTurma(string codigoTurma){
    if(find_if(turmas.begin(), turmas.end(), [codigoTurma](const Turma& t) {return t.getcodigoTurma() == codigoTurma;}) == turmas.end())
        return false;
    map<string, int> occupations;
    for (Turma t : turmas) {
        if (t.getcodigoTurma() == codigoTurma)
            occupations.insert({t.getcodigoUC(), 0});
    }
    for (Estudante e : estudantes) {
        for (Turma t : e.getSchedule()) {
            if (t.getcodigoTurma() == codigoTurma)
                occupations[t.getcodigoUC()] += 1;
        }
    }

    for (auto occupation : occupations) {
        cout << occupation.first << " " << occupation.second << " estudantes" << "\n";
    }
    return true;
}

/**
 * Imprime a ocupação de uma UC.
 * Complexidade: O(n * m), sendo n o número de estudantes e m o número de turmas a que cada estudante pertence.
 * @param codigoUC - Código da UC cuja ocupação deverá ser impressa.
 * @return Verdadeiro se a UC existe.
 */
bool Gestor::outputOcupaçãoUC(string codigoUC){
    if(find_if(turmas.begin(), turmas.end(), [codigoUC](const Turma& t) {return t.getcodigoUC() == codigoUC;}) == turmas.end())
        return false;
    map<string, int> occupations;
    for (Turma t : turmas) {
        if (t.getcodigoUC() == codigoUC)
            occupations.insert({t.getcodigoTurma(), 0});
    }
    for (Estudante e : estudantes) {
        for (Turma t : e.getSchedule()) {
            if (t.getcodigoUC() == codigoUC)
                occupations[t.getcodigoTurma()] += 1;
        }
    }

    for (auto occupation : occupations) {
        cout << occupation.first << " " << occupation.second << " estudantes" << "\n";
    }
    return true;
}

/**
 * Imprime a ocupação de um ano curricular.
 * Complexidade: O(n * m), sendo n o número de estudantes e m o número de turmas a que cada estudante pertence.
 * @param ano - Ano curricular cuja ocupação deverá ser impressa.
 */
void Gestor::outputOcupaçãoAno(int ano) {
    map<string, int> occupations;
    for (Turma t : turmas) {
        if (t.getcodigoTurma()[0] == (char)(ano + '0'))
            occupations.insert({t.getcodigoUC(), 0});
    }
    for (Estudante e : estudantes) {
        for (Turma t : e.getSchedule()) {
            if (t.getcodigoTurma()[0] == (char)(ano + '0'))
                occupations[t.getcodigoUC()] += 1;
        }
    }

    for (auto occupation : occupations) {
        cout << occupation.first << " " << occupation.second << " estudantes" << "\n";
    }

}

/**
 * Verifica se um estudante atingiu o limite de 7 UCs.
 * Complexidade: O(1).
 * @param schedule - Horário do estudante.
 * @return Verdadeiro se o estudante não atingiu o limite.
 */
bool Gestor::assessUCLimit(list<Turma> schedule) {
    return schedule.size() + 1 <= 7;
}

/*bool Gestor::assessTurmaVacancy(Turma t) {
    return t.attending <= cap;
}*/

/**
 * Verifica se entrar numa turma irá gerar um conflito de aulas no horário de um estudante.
 * Complexidade: O(n * m * p), sendo n o número de aulas da nova turma, m o número de turmas do estudante e p o número de aulas de cada turma.
 * @param schedule - Horário do estudante.
 * @param nt - Turma na qual o estudante quer entrar.
 * @return Verdadeiro se não existirem conflitos.
 */
bool Gestor::assessScheduleConflict(list<Turma> schedule, Turma nt) {
    // assume-se que as turmas default do estudante não têm conflito.
    for (Aula na : nt.getAulas()) {
        if (na.getTipo() == "T") { continue; }
        for (Turma t: schedule) {
            for (Aula a: t.getAulas()) {
                if (a.getTipo() == "T") { continue; }
                if (a.getDia() != na.getDia()) { continue; }
                if (na.getHoraInicio() <= (a.getHoraInicio() + a.getDuracao()) &&
                    na.getHoraInicio() >= a.getHoraInicio()) { return false; }
                if ((na.getHoraInicio() + na.getDuracao()) <= (a.getHoraInicio() + a.getDuracao()) &&
                    (na.getHoraInicio() + na.getDuracao()) >= a.getHoraInicio()) { return false; }
            }
        }
    }
    return true;
}

/**
 * Verifica se entrar numa turma fará com que um estudante esteja em duas turmas da mesma UC.
 * Complexidade: O(n), sendo n o número de turmas do estudante.
 * @param schedule - Horário do estudante.
 * @param nt - Turma na qual o estudante quer entrar.
 * @return Verdadeiro se não existirem conflitos.
 */
bool Gestor::assessUCTurmaSingularity(list<Turma> schedule, Turma nt) {
    for (Turma t : schedule) {
        if (t.getcodigoUC() == nt.getcodigoUC()) {
            return false;
        }
    }
    return true;
}

/**
 * Verifica se entrar numa turma fará com que a turma exceda a capacidade máxima.
 * Complexidade: O(1).
 * @param t - Turma na qual o estudante quer entrar.
 * @return Verdadeiro se adicionar o estudante não exceder a capacidade máxima.
 */
bool Gestor::assessTurmaCap(Turma t) {
    return t.getOccupation() + 1 <= cap;
}

/**
 * Verifica se mudar um estudante de turma irá perturbar o equilíbrio da ocupação das turmas.
 * Complexidade: O(n), sendo n o número de turmas.
 * @param idUC - Código da UC cujo equilíbrio será verificado.
 * @param idTurma - Turma na qual o estudante quer entrar.
 * @param idTurmaAnterior - Turma da qual o estudante irá sair.
 * @return Verdadeiro se o equilíbrio for mantido.
 */
bool Gestor::assessBalance(string idUC, string idTurma, string idTurmaAnterior = "") {
    //assumindo que o último 'class' remete para 'aula' e não para 'turma' -> procurar apenas as turmas com aquele idTurma
    int maxOccupation = 0, minOccupation = cap, newOccupation, oldOccupation;
    int sum = 0, count = 0;
    for (Turma tu : turmas) {
        if (tu.getcodigoUC() == idUC){
            int current = tu.getOccupation();
            if (tu.getcodigoTurma() == idTurma){
                newOccupation = current;
            }
            if (tu.getcodigoTurma() == idTurmaAnterior){
                oldOccupation = current;
            }
            if (current > maxOccupation) maxOccupation = current;
            if (current < minOccupation) minOccupation = current;

        }
    }
    int amplitude = maxOccupation - minOccupation;
    if (amplitude < 4){ return true; }
    if (amplitude == 4 && newOccupation != maxOccupation){ return true; }
    if (idTurmaAnterior != ""){
        if ((maxOccupation - newOccupation) > 4) {return true;}
        // Se houver uma falha de balanço, dá prioridade a inserir nas turmas que não estão balançadas
    }
    else {
        if ((oldOccupation - newOccupation) > 4){ return true;}
        // Se houver uma falha de balanço, dá prioridade a trocar de uma turma com pelo menos mais 4 pessoas do que a nova
    }
    return false;
}

/**
 * Cria um novo pedido de remoção de um estudante de uma turma.
 * Complexidade: O(1)
 * @param id - Número de identificação do estudante.
 * @param codigoUC - Código da UC da qual o estudante quer ser removido.
 * @param codigoTurma - Código da turma da qual o estudante quer ser removido.
 */
void Gestor::novoPedidoRemoção(int id, string codigoUC, string codigoTurma){
    pedidos.push({id, codigoUC, codigoTurma, "R"});
}

/**
 * Cria um novo pedido de inserção de um estudante de uma turma.
 * Complexidade: O(1)
 * @param id - Número de identificação do estudante.
 * @param codigoUC - Código da UC na qual o estudante quer ser inserido.
 * @param codigoTurma - Código da turma na qual o estudante quer ser inserido.
 */
void Gestor::novoPedidoInserção(int id, string codigoUC, string codigoTurma){
    pedidos.push({id, codigoUC, codigoTurma, "I"});
}

/**
 * Cria um novo pedido de troca de um estudante entre turmas.
 * Complexidade: O(1).
 * @param id - Número de identificação do estudante.
 * @param codigoUCAtual - Código da UC à qual o estudante pertence.
 * @param codigoTurmaAtual - Código da turma à qual o estudante pertence.
 * @param codigoUCNova - Código da UC na qual o estudante quer ser inserido.
 * @param codigoTurmaNova - Código da turma na qual o estudante quer ser inserido.
 */
void Gestor::novoPedidoTroca(int id, string codigoUCAtual, string codigoTurmaAtual, string codigoUCNova, string codigoTurmaNova){
    pedidos.push({id, codigoUCAtual, codigoTurmaAtual, codigoUCNova, codigoTurmaNova, "T"});
}

/**
 * Gere o processamento de um pedido.
 * Complexidade: O(1).
 */
void Gestor::procPedido(){
    Pedido aProcessar = pedidos.front();
    cout << "A executar:\t" << aProcessar.getTipo() << ": " << aProcessar.getId() << '\n';
    bool done = false;
    if (aProcessar.getTipo() == "R"){
        done = (procPedidoRemoção(aProcessar.getId(), aProcessar.getCodigoUC(), aProcessar.getCodigoTurma()));
    }
    if (aProcessar.getTipo() == "I"){
        done = procPedidoInserção(aProcessar.getId(), aProcessar.getCodigoUC(), aProcessar.getCodigoTurma());
    }
    if (aProcessar.getTipo() == "T"){
        done = procPedidoTroca(aProcessar.getId(), aProcessar.getCodigoUC(), aProcessar.getCodigoTurma(), aProcessar.getCodigoUCNova(), aProcessar.getCodigoTurmaNova());
    }
    pedidos.pop();
    if (done){
        pedidosRealizados.push(aProcessar);
    }
    else {
        pedidosInválidos.push(aProcessar);
    }
}

/**
 * Gere o processamento de todos os pedidos em fila de espera de uma só vez.
 * Complexidade: O(n * m), sendo n o número de pedidos e m o número de turmas..
 */
void Gestor::procTodosPedidos(){
    while (!pedidos.empty()){
        procPedido();
    }
}

/**
 * Verifica se mudanças foram feitas aos horários dos estudantes.
 * Complexidade: O(1).
 * @return Verdadeiro se não foram feitas mudanças.
 */
bool Gestor::existemMudanças() {
    return pedidosRealizados.empty();
}

/**
 * Verifica se ainda existem pedidos por processar.
 * Complexidade: O(1).
 * @return Verdadeiro se não existirem pedidos por processar.
 */
bool Gestor::faltamProcPedidos(){
    return pedidos.empty();
}

/**
 * Processa um pedido de remoção.
 * Complexidade: O(n), sendo n o número de turmas.
 * @param id - Número do estudante a ser removido.
 * @param codigoUC - Código da UC da qual o estudante quer ser removido.
 * @param codigoTurma - Código da turma da qual o estudante quer ser removido.
 * @return Verdadeiro se o pedido foi processado com sucesso.
 */
bool Gestor::procPedidoRemoção(int id, string codigoUC, string codigoTurma){
    //assume-se que a remoção de UC e a remoção de turma consistem na mesma operação
    int idx = binarySearchEstudantes(id);
    if (idx == -1){
        cout << "ERRO: Aluno não existe.\n";
        return false;
    }

    list<Turma> newSchedule = {};
    bool turmaFound = false;
    for(Turma &t : estudantes[idx].getSchedule()) {
        if (t.getcodigoTurma() == codigoTurma && t.getcodigoUC() == codigoUC) {
            turmaFound = true;
        } else {
            newSchedule.push_back(t);
        }
    }
    if (!turmaFound) {
        cout << "ERRO: Turma atual não foi encontrada.\n";
        return false;
    }

    //temporário - só para assessBalance()
    int tIdx = -1;
    for (int i = 0; i < turmas.size(); i++){
        if (turmas[i].getcodigoUC() == codigoUC && turmas[i].getcodigoTurma() == codigoTurma){
            tIdx = i;
            break;
        }
    }
    if (!assessBalance(turmas[tIdx].getcodigoUC(), turmas[tIdx].getcodigoTurma())) {cout << "ERRO: Pedido aumenta desequilíbrio entre as turmas.\n";return false;}

    estudantes[idx].setSchedule(newSchedule);
    for (auto t : turmas){
        if (t.getcodigoUC() == codigoUC && t.getcodigoTurma() == codigoTurma){
            t.decreaseOccupation();
            break;
        }
    }
    return true;
}

/**
 * Processa um pedido de inserção.
 * Complexidade: O(n), sendo n o número de turmas.
 * @param id - Número do estudante a ser inserido.
 * @param codigoUC - Código da UC na qual o estudante quer ser inserido.
 * @param codigoTurma - Código da turma na qual o estudante quer ser inserido.
 * @return Verdadeiro se o pedido foi processado com sucesso.
 */
bool Gestor::procPedidoInserção(int id, string codigoUC, string codigoTurma){
    int eIdx = binarySearchEstudantes(id);
    if (eIdx == -1){
        cout << "ERRO: Aluno não existe.\n";
        return false; //estudante não existe.
    }
    int tIdx = -1;
    for (int i = 0; i < turmas.size(); i++){
        if (turmas[i].getcodigoUC() == codigoUC && turmas[i].getcodigoTurma() == codigoTurma){
            tIdx = i;
            break;
        }
    }

    if (tIdx == -1){cout << "ERRO: Turma nova não existe.\n"; return false;}
    if (!assessUCLimit(estudantes[eIdx].getSchedule())) {cout << "ERRO: Limite de UCs excedido.\n";return false;}
    if (!assessUCTurmaSingularity(estudantes[eIdx].getSchedule(), turmas[tIdx])) {cout << "ERRO: Estudante já se encontra inscrito nesta UC, mas em outra turma.\n";return false;}
    if (!assessTurmaCap(turmas[tIdx])){cout << "ERRO: Limite de alunos na turma excedido.\n";return false;}
    if (!assessBalance(turmas[tIdx].getcodigoUC(), turmas[tIdx].getcodigoTurma())) {cout << "ERRO: Pedido aumenta desequilíbrio entre as turmas.\n";return false;}
    if (!assessScheduleConflict(estudantes[eIdx].getSchedule() ,turmas[tIdx])) {cout << "ERRO: Conflito de horários.\n";return false;}

    estudantes[eIdx].addToSchedule(turmas[tIdx]);
    turmas[tIdx].increaseOccupation();
    return true;
}

/**
 * Processa um pedido de troca.
 * Complexidade: O(n), sendo n o número de turmas.
 * @param id - Número do estudante a ser inserido.
 * @param codigoUCAtual - Código da UC na qual o estudante se encontra.
 * @param codigoTurmaAtual - Código da turma na qual o estudante se encontra.
 * @param codigoUCNova - Código da UC na qual o estudante quer ser inserido.
 * @param codigoTurmaNova - Código da turma na qual o estudante quer ser inserido.
 * @return Verdadeiro se o pedido foi processado com sucesso.
 */
bool Gestor::procPedidoTroca(int id, string codigoUCAtual, string codigoTurmaAtual, string codigoUCNova, string codigoTurmaNova) {

    int eIdx = binarySearchEstudantes(id);
    if (eIdx == -1){

        cout << "ERRO: Aluno não existe.\n";
        return false; //estudante não existe.
    }
    list<Turma> newSchedule = {};
    bool turmaFound = false;
    for(Turma &t : estudantes[eIdx].getSchedule()) {
        if (t.getcodigoTurma() == codigoTurmaAtual && t.getcodigoUC() == codigoUCAtual) {
            turmaFound = true;
        } else {
            newSchedule.push_back(t);
        }
    }
    if (!turmaFound) {
        cout << "ERRO: Turma atual não foi encontrada.\n";
        return false;
    }

    int tIdxNova = -1;
    for (int i = 0; i < turmas.size(); i++){
        if (turmas[i].getcodigoUC() == codigoUCNova && turmas[i].getcodigoTurma() == codigoTurmaNova){
            tIdxNova = i;
            break;
        }
    }
    if (tIdxNova == -1){cout << "ERRO: Turma nova não existe.\n";return false;}
    if (!assessUCLimit(newSchedule)) {cout << "ERRO: Limite de UCs excedido.\n";return false;}
    if (!assessUCTurmaSingularity(newSchedule ,turmas[tIdxNova])) {cout << "ERRO: Estudante já se encontra inscrito nesta UC, mas em outra turma.\n";return false;}
    if (!assessTurmaCap(turmas[tIdxNova])){cout << "ERRO: Limite de alunos na turma excedido.\n";return false;}
    if (!assessBalance(turmas[tIdxNova].getcodigoUC(), turmas[tIdxNova].getcodigoTurma(), codigoTurmaAtual)) {cout << "ERRO: Pedido aumenta desequilíbrio entre as turmas.\n";return false;}
    if (!assessScheduleConflict(newSchedule ,turmas[tIdxNova])) {cout << "ERRO: Conflito de horários.\n";return false;}

    estudantes[eIdx].setSchedule(newSchedule);
    for (auto t : turmas){
        if (t.getcodigoUC() == codigoUCAtual && t.getcodigoTurma() == codigoTurmaAtual){
            t.decreaseOccupation();
            break;
        }
    }

    estudantes[eIdx].addToSchedule(turmas[tIdxNova]);
    turmas[tIdxNova].increaseOccupation();
    return true;
}

/**
 * Anula as alterações feitas pelo último pedido realizado.
 * Complexidade: O(n), sendo n o número de turmas.
 * @return Verdadeiro se o pedido foi anulado com sucesso.
 */
bool Gestor::desfazerÚltimoPedido(){
    bool result;
    if (pedidosRealizados.top().getTipo() == "R") {
        result = procPedidoInserção(pedidosRealizados.top().getId(), pedidosRealizados.top().getCodigoUC(), pedidosRealizados.top().getCodigoTurma());
    } else if (pedidosRealizados.top().getTipo() == "I") {
        result = procPedidoRemoção(pedidosRealizados.top().getId(), pedidosRealizados.top().getCodigoUC(), pedidosRealizados.top().getCodigoTurma());
    } else if (pedidosRealizados.top().getTipo() == "T") {
        result = procPedidoTroca(pedidosRealizados.top().getId(), pedidosRealizados.top().getCodigoUCNova(), pedidosRealizados.top().getCodigoTurmaNova(), pedidosRealizados.top().getCodigoUC(), pedidosRealizados.top().getCodigoTurma());
    }
    if (result) pedidosRealizados.pop();
    return result;
}

/**
 * Imprime a lista total de turmas.
 * Complexidade: O(n), sendo n o número de turmas.
 */
void Gestor::outputAllTurmas() {
    for (Turma t : turmas){
        cout << t.getcodigoUC() << ' ' << t.getcodigoTurma() << ' ' << t.getOccupation() << '\n';
    }
}

/**
 * Imprime a lista total de aulas de cada turma.
 * Complexidade: O(n * m), sendo n o número de turmas e m o número de aulas de cada turma.
 */
void Gestor::outputAllAulas() {
    for (Turma t : turmas){
        cout << t.getcodigoUC() << ' ' << t.getcodigoTurma() << '\t';
        for (Aula a : t.getAulas())
            cout << '\t' << numToWeekday[a.getDia()] << ' ' << a.getDuracao() << "     \t|\t";
        cout << '\n';
    }
}

/**
 * Imprime a lista total de estudantes em determinada ordem.
 * Complexidade: O(n * m * log n), sendo n o número de estudantes e m o número de turmas de cada estudante.
 * @param order - Ordem na qual os estudantes deverão ser impressos.
 */
void Gestor::outputAllEstudantes(int order) {
    if (order == 1) {
        set<Estudante, EstudanteAlphaAscending> lista;
        for(Estudante e : estudantes) {
            lista.insert(e);
        }
        for (Estudante e : lista) {
            cout << e.getName() << ", " << e.getID() << endl;
            for (Turma t: e.getSchedule()) {
                cout << "\t" << t.getcodigoUC() << ", " << t.getcodigoTurma() << endl;
            }
        }
    }
    if (order == 2) {
        set<Estudante, EstudanteAlphaDescending> lista;
        for(Estudante e : estudantes) {
            lista.insert(e);
        }
        for (Estudante e : lista) {
            cout << e.getName() << ", " << e.getID() << endl;
            for (Turma t : e.getSchedule()) {
                cout << "\t" << t.getcodigoUC() << ", " << t.getcodigoTurma() << endl;
            }
        }
    }
    if (order == 3) {
        for (Estudante e : estudantes) {
            cout << e.getName() << ", " << e.getID() << endl;
            for (Turma t: e.getSchedule()) {
                cout << "\t" << t.getcodigoUC() << ", " << t.getcodigoTurma() << endl;
            }
        }
    }
    if (order == 4) {
        for (auto e = estudantes.rbegin(); e != estudantes.rend(); e++) {
            cout << e->getName() << ", " << e->getID() << endl;
            for (Turma t : e->getSchedule()) {
                cout << "\t" << t.getcodigoUC() << ", " << t.getcodigoTurma() << endl;
            }
        }
    }
}

/**
 * Gere o armazenamento das alteracões feitas.
 * Complexidade: O(n), sendo n o número de linhas a ser escritas no ficheiro.
 * @param fname - Nome do ficheiro onde serão guardadas os horários dos estudantes.
 */
void Gestor::saveChanges(string fname) {
    writeStudentClasses(fname);

    if (!pedidosRealizados.empty())
        writeDoneRequests("valid_requests.txt");
    if (!pedidosInválidos.empty())
        writeInvalidRequests("invalid_requests.txt");

    cout << "Alterações guardadas!\n";
}

/**
 * Guarda em ficheiro os pedidos que foram realizados com sucesso.
 * Complexidade: O(n), sendo n o número de pedidos.
 * @param fname - Nome do ficheiro onde serão guardadas as alterações.
 */
void Gestor::writeDoneRequests(string fname){
    vector<Pedido> toWrite;
    while (!pedidosRealizados.empty()){
        toWrite.push_back(pedidosRealizados.top());
        pedidosRealizados.pop();
    }

    ofstream fileWriter(fname);
    for (auto p = toWrite.rbegin(); p != toWrite.rend(); p++){
        Pedido current = *p;
        if (current.getTipo() == "T")
            fileWriter << current.getTipo() << '\t' << current.getId() <<
                       "\t(" << current.getCodigoUC() << ',' << current.getCodigoTurma() << ") -> (" <<
                       current.getCodigoTurmaNova() << ',' << current.getCodigoTurmaNova() << ")\n";
        if (current.getTipo() == "R")
            fileWriter << current.getTipo() << '\t' << current.getId() <<
                       "\t(" << current.getCodigoUC() << ',' << current.getCodigoTurma() << ") ->\n";
        if (current.getTipo() == "I")
            fileWriter << current.getTipo() << '\t' << current.getId() <<
                       "\t-> (" << current.getCodigoUC() << ',' << current.getCodigoTurma() << ")\n";
    }
    fileWriter.close();
}

/**
 * Guarda em ficheiro os pedidos inválidos.
 * Complexidade: O(n), sendo n o número de pedidos.
 * @param fname - Nome do ficheiro onde serão guardadas as alterações.
 */
void Gestor::writeInvalidRequests(string fname){
    ofstream fileWriter(fname);
    while (!pedidosInválidos.empty()){
        Pedido current = pedidosInválidos.front();
        if (current.getTipo() == "T")
            fileWriter << current.getTipo() << '\t' << current.getId() <<
                        "\t(" << current.getCodigoUC() << ',' << current.getCodigoTurma() << ") -> (" <<
                        current.getCodigoTurmaNova() << ',' << current.getCodigoTurmaNova() << ")\n";
        if (current.getTipo() == "R")
            fileWriter << current.getTipo() << '\t' << current.getId() <<
                       "\t(" << current.getCodigoUC() << ',' << current.getCodigoTurma() << ") ->\n";
        if (current.getTipo() == "I")
            fileWriter << current.getTipo() << '\t' << current.getId() <<
                       "\t-> (" << current.getCodigoUC() << ',' << current.getCodigoTurma() << ")\n";
        pedidosInválidos.pop();
    }
    fileWriter.close();
}

/**
 * Guarda em ficheiro as alterações realizadas aos horários dos estudantes.
 * Complexidade: O(n), sendo n o número de linhas a ser escritas no ficheiro.
 * @param fname - Nome do ficheiro onde serão guardadas as alterações.
 */
void Gestor::writeStudentClasses(string fname) {
    ofstream fileWriter(fname);
    fileWriter << "StudentCode,StudentName,UcCode,ClassCode\r";
    for (auto e : estudantes){
        for (auto t : e.getSchedule()){
            fileWriter << e.getID() << ',' << e.getName() << ',' << t.getcodigoUC() << ',' << t.getcodigoTurma() << '\r';
        }
    }
    fileWriter.close();
}

/**
 * Organiza as turmas pela função compareTurmas.
 * Complexidade: O(n log n), sendo n o número de turmas.
 */
void Gestor::sortTurmas() {
    sort(turmas.begin(), turmas.end(), compareTurmas);
}

/**
 * Define a comparação entre turmas.
 * @param t1 - Turma 1 a ser comparada.
 * @param t2 - Turma 2 a ser comparada.
 * @return Verdadeiro se o código da UC for menor ou, em caso de equivalência, se o código da turma for menor.
 */
bool compareTurmas(const Turma &t1, const Turma &t2){
    bool compareUC = t1.getcodigoUC() < t2.getcodigoUC();
    bool compareTurma = t1.getcodigoUC() == t2.getcodigoUC() && t1.getcodigoTurma() < t2.getcodigoTurma();
    return  compareUC || compareTurma;
}

/**
 * Organiza os estudantes por número de identificação.
 * Complexidade: O(n log n), sendo n o número de estudantes.
 */
void Gestor::sortEstudantesByNumber() {
    sort(estudantes.begin(), estudantes.end(), compareEstudantes);
}

/**
 * Define a comparação entre estudantes.
 * @param e1 - Estudante 1 a ser comparado.
 * @param e2 - Estudante 2 a ser comparado.
 * @return Verdadeiro se o número de identificação for menor.
 */
bool compareEstudantes(const Estudante &e1, const Estudante &e2){
    return e1.getID() < e2.getID();
}

/**
 * Implementa Binary Search para encontrar um estudante.
 * Complexidade: O(log n), sendo n o número de estudantes.
 * @param id - Número de identificação do estudante.
 * @return Índice do estudante no vetor de estudantes.
 */
int Gestor::binarySearchEstudantes(int id) {
    int lowerBound = 0; int upperBound = estudantes.size()-1;
    while (lowerBound <= upperBound){
        int middleValue = (lowerBound + upperBound)/2;
        if (estudantes[middleValue].getID() < id){
            lowerBound = middleValue + 1;
        }
        else if (estudantes[middleValue].getID() > id){
            upperBound = middleValue - 1;
        }
        else {
            return middleValue;
        }
    }
    return -1;
}