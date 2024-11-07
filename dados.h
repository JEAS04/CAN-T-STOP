#ifndef DADOS_H
#define DADOS_H

#include <vector>
#include <map>
#include <utility>
using namespace std;

class Dados {
private:
    vector<int> dados;
    vector<pair<int, int>> combinacionesPosibles;
    
public:
    Dados();
    void lanzar();
    vector<pair<int, int>> obtenerCombinaciones() const;
    // Modificada para incluir columnasCoronadas como parámetro
    bool puedeContinuar(const vector<int>& columnasUsadas, const map<int, int>& columnasCoronadas) const;
    void imprimirCombinaciones() const;
};

#endif