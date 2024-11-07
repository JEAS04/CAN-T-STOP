#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <vector>

using namespace std;

class Jugador {
private:
    int numero;              // Número del jugador (1-4)
    int fichasDisponibles;   // Cantidad de fichas disponibles (10 al inicio)
    vector<int> columnasCoronadas;  // Columnas que ha completado

public:
    Jugador(int numJugador);
    int obtenerNumero() const;
    int obtenerFichasDisponibles() const;
    void usarFicha();
    void coronarColumna(int columna);
    bool haGanado() const;  // Verifica si tiene 3 columnas coronadas
    vector<int> obtenerColumnasCoronadas() const;
};

#endif