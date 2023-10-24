#ifndef AED2324_PRJ1_G15_AULA_H
#define AED2324_PRJ1_G15_AULA_H
#include <string>

using namespace std;

class Aula {
private:
    string dia;
    float horaInicio;
    float duracao;
    string tipo;

public:
    Aula(string dia, float horaInicio, float duracao, string tipo);
    string getDia() const;
    float getHoraInicio() const;
    float getDuracao() const;
    string getTipo() const;
};


#endif //AED2324_PRJ1_G15_AULA_H
