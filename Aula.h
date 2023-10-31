#ifndef AED2324_PRJ1_G15_AULA_H
#define AED2324_PRJ1_G15_AULA_H
#include <string>

using namespace std;

/**
 * Define uma aula de uma turma.
 */
class Aula {
private:
    int dia;
    float horaInicio;
    float duracao;
    string tipo;

public:
    
    /**
     * Construtor parametrizado de uma aula.
     * @param dia - Dia da aula.
     * @param horaInicio - Hora de início da aula.
     * @param duracao - Duração da aula.
     * @param tipo - Tipo de aula.
     */
    Aula(int dia, float horaInicio, float duracao, string tipo);
    int getDia() const;
    float getHoraInicio() const;
    float getDuracao() const;
    string getTipo() const;
};


#endif //AED2324_PRJ1_G15_AULA_H
