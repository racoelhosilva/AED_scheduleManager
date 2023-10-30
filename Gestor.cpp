#include <algorithm>
#include "Gestor.h"
#include <set>
#include <map>
#include <iomanip>
#include <cmath>
#include <algorithm>

void Gestor::setCap(const int newCap){
    this->cap = newCap;
}

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
    //sortTurmas(); // Kind of optional...
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
        int numWD = weekdayToNum[weekday];
        for (Turma &T : turmas) {
            if (T.getcodigoTurma() == classID && T.getcodigoUC() == ucID) {
                T.addAulas(Aula(numWD, start, duration, type));
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
bool Gestor::outputListaTurmas(string codigoUC){
    if(find_if(turmas.begin(), turmas.end(), [codigoUC](const Turma& t) {return t.getcodigoUC() == codigoUC;}) == turmas.end())
        return false;
    for (Turma t : turmas) {
        if(t.getcodigoUC() == codigoUC)
            cout << t.getcodigoTurma() << "\n";
    }
    return true;
}
void Gestor::outputListaUC(int ano){

    for (Turma t : turmas) {
        if (t.getcodigoTurma()[0] == (char)(ano + '0') && t.getcodigoTurma()[5] == '0' && t.getcodigoTurma()[6] == '1')
            cout << t.getcodigoUC() << "\n";
    }
}

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

bool Gestor::assessUCLimit(list<Turma> schedule) {
    return schedule.size() + 1 <= 7;
}

/*bool Gestor::assessTurmaVacancy(Turma t) {
    return t.attending <= cap;
}*/

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
bool Gestor::assessUCTurmaSingularity(list<Turma> schedule, Turma nt) {
    // assume-se que as turmas default do estudante não têm conflito.
    for (Turma t : schedule) {
        if (t.getcodigoUC() == nt.getcodigoUC()) {
            return false;
        }
    }
    return true;
}

bool Gestor::assessTurmaCap(Turma t) {
    return t.getOccupation() + 1 <= cap;
}

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

void Gestor::novoPedidoRemoção(int id, string codigoUC, string codigoTurma){
    pedidos.push({id, codigoUC, codigoTurma, "R"});
}
void Gestor::novoPedidoInserção(int id, string codigoUC, string codigoTurma){
    pedidos.push({id, codigoUC, codigoTurma, "I"});
}
void Gestor::novoPedidoTroca(int id, string codigoUCAtual, string codigoTurmaAtual, string codigoUCNova, string codigoTurmaNova){
    pedidos.push({id, codigoUCAtual, codigoTurmaAtual, codigoUCNova, codigoUCNova, "T"});
}

void Gestor::procPedido(){
    Pedido aProcessar = pedidos.front();
    cout << "A executar:\t" << aProcessar.getTipo() << ": " << aProcessar.getId();
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

void Gestor::procTodosPedidos(){
    while (!pedidos.empty()){
        procPedido();
    }
}

bool Gestor::existemMudanças() {
    return pedidosRealizados.empty();
}

bool Gestor::procPedidoRemoção(int id, string codigoUC, string codigoTurma){
    //assume-se que a remoção de UC e a remoção de turma consistem na mesma operação
    int idx = binarySearchEstudantes(id);
    if (idx == -1){
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
        return false;
    }

    //temporário
    int tIdx = -1;
    for (int i = 0; i < turmas.size(); i++){
        if (turmas[i].getcodigoUC() == codigoUC && turmas[i].getcodigoTurma() == codigoTurma){
            tIdx = i;
            break;
        }
    }

    if (tIdx == -1){return false;}
    if (!assessBalance(turmas[tIdx].getcodigoUC(), turmas[tIdx].getcodigoTurma())) {return false;}

    estudantes[idx].setSchedule(newSchedule);
    for (auto t : turmas){
        if (t.getcodigoUC() == codigoUC && t.getcodigoTurma() == codigoTurma){
            t.decreaseOccupation();
            break;
        }
    }
    return true;
}

bool Gestor::procPedidoInserção(int id, string codigoUC, string codigoTurma){
    int eIdx = binarySearchEstudantes(id);
    if (eIdx == -1){
        return false; //estudante não existe.
    }
    int tIdx = -1;
    for (int i = 0; i < turmas.size(); i++){
        if (turmas[i].getcodigoUC() == codigoUC && turmas[i].getcodigoTurma() == codigoTurma){
            tIdx = i;
            break;
        }
    }

    if (tIdx == -1){return false;}
    if (!assessUCLimit(estudantes[eIdx].getSchedule())) {return false;}
    if (!assessUCTurmaSingularity(estudantes[eIdx].getSchedule(), turmas[tIdx])) {return false;}
    if (!assessTurmaCap(turmas[tIdx])){return false;}
    if (!assessBalance(turmas[tIdx].getcodigoUC(), turmas[tIdx].getcodigoTurma())) {return false;}
    if (!assessScheduleConflict(estudantes[eIdx].getSchedule() ,turmas[tIdx])) {return false;}

    estudantes[eIdx].addToSchedule(turmas[tIdx]);
    turmas[tIdx].increaseOccupation();
    return true;
}

bool Gestor::procPedidoTroca(int id, string codigoUCAtual, string codigoTurmaAtual, string codigoUCNova, string codigoTurmaNova) {

    int eIdx = binarySearchEstudantes(id);
    if (eIdx == -1){

        cout << "Aluno não existe.\n";
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
        cout << "Turma não encontrada.\n";
        return false;
    }

    int tIdxNova = -1;
    for (int i = 0; i < turmas.size(); i++){
        if (turmas[i].getcodigoUC() == codigoUCNova && turmas[i].getcodigoTurma() == codigoTurmaNova){
            tIdxNova = i;
            break;
        }
    }
    if (tIdxNova == -1){cout << "Turma não existe.\n";return false;}
    if (!assessUCLimit(newSchedule)) {cout << "Limite de UCs excedido.\n";return false;}
    if (!assessScheduleConflict(newSchedule ,turmas[tIdxNova])) {cout << "Conflito de horários.\n";return false;}
    if (!assessUCTurmaSingularity(newSchedule ,turmas[tIdxNova])) {cout << "Singularidade :/.\n";return false;}
    if (!assessTurmaCap(turmas[tIdxNova])){cout << "Limite de alunos na turma excedido.\n";return false;}
    if (!assessBalance(turmas[tIdxNova].getcodigoUC(), turmas[tIdxNova].getcodigoTurma(), codigoTurmaAtual)) {cout << "Equilíbrio fodido.\n";return false;}

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

bool Gestor::desfazerÚltimoPedido(){

    return true;
}

// Testing Functions for the extract
void Gestor::outputAllTurmas() {
    for (Turma t : turmas){
        cout << t.getcodigoUC() << ' ' << t.getcodigoTurma() << ' ' << t.getOccupation() << '\n';
    }
}

void Gestor::outputAllAulas() {
    for (Turma t : turmas){
        cout << t.getcodigoUC() << ' ' << t.getcodigoTurma() << '\t';
            for (Aula a : t.getAulas())
                cout << '\t' << numToWeekday[a.getDia()] << ' ' << a.getDuracao() << "     \t|\t";
        cout << '\n';
    }
}

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
                /*for (Aula a: t.getAulas()) {
                    cout << "        " << numToWeekday[a.getDia()] << ", " << a.getHoraInicio() << ", "
                         << a.getDuracao() << ", " << a.getTipo() << endl;
                }*/
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
                for (Aula a: t.getAulas()) {
                    cout << "        " << numToWeekday[a.getDia()] << ", " << a.getHoraInicio() << ", "
                         << a.getDuracao() << ", " << a.getTipo() << endl;
                }
            }
        }
    }
    if (order == 3) {
        for (Estudante e : estudantes) {
            cout << e.getName() << ", " << e.getID() << endl;
            for (Turma t: e.getSchedule()) {
                cout << "\t" << t.getcodigoUC() << ", " << t.getcodigoTurma() << endl;
                for (Aula a: t.getAulas()) {
                    cout << "        " << numToWeekday[a.getDia()] << ", " << a.getHoraInicio() << ", "
                         << a.getDuracao() << ", " << a.getTipo() << endl;
                }
            }
        }
    }
    if (order == 4) {
        for (auto e = estudantes.rbegin(); e != estudantes.rend(); e++) {
            cout << e->getName() << ", " << e->getID() << endl;
            for (Turma t : e->getSchedule()) {
                cout << "\t" << t.getcodigoUC() << ", " << t.getcodigoTurma() << endl;
                for (Aula a: t.getAulas()) {
                    cout << "        " << numToWeekday[a.getDia()] << ", " << a.getHoraInicio() << ", "
                         << a.getDuracao() << ", " << a.getTipo() << endl;
                }
            }
        }
    }
}

void Gestor::saveChanges(string fname) {
    writeStudentClasses(fname);

    if (!pedidosRealizados.empty())
        writeDoneRequests("valid_requests.txt");
    if (!pedidosInválidos.empty())
        writeInvalidRequests("invalid_requests.txt");

    cout << "Alterações guardadas!\n";
}

void Gestor::writeDoneRequests(string fname){return;}
void Gestor::writeInvalidRequests(string fname){return;}

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

void Gestor::sortTurmas() {
    sort(turmas.begin(), turmas.end(), compareTurmas);
}

bool compareTurmas(const Turma &t1, const Turma &t2){
    bool compareUC = t1.getcodigoUC() < t2.getcodigoUC();
    bool compareTurma = t1.getcodigoUC() == t2.getcodigoUC() && t1.getcodigoTurma() < t2.getcodigoTurma();
    return  compareUC || compareTurma;
}

void Gestor::sortEstudantesByNumber() {
    sort(estudantes.begin(), estudantes.end(), compareEstudantes);
}

bool compareEstudantes(const Estudante &e1, const Estudante &e2){
    return e1.getID() < e2.getID();
}

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
