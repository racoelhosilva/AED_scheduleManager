#include "Aula.h"

Aula::Aula(string dia, float horaInicio, float duracao, string tipo) {
    this->dia = dia;
    this->horaInicio = horaInicio;
    this->duracao = duracao;
    this->tipo = tipo;
}

string Aula::getDia() const {
    return this->dia;
}

float Aula::getHoraInicio() const {
    return this->horaInicio;
}

float Aula::getDuracao() const {
    return this->duracao;
}

string Aula::getTipo() const {
    return this->tipo;
}




