#include "jugador.h"

Jugador::Jugador(int numJugador) : numero(numJugador), fichasDisponibles(10) {}

int Jugador::obtenerNumero() const {
    return numero;
}

int Jugador::obtenerFichasDisponibles() const {
    return fichasDisponibles;
}

void Jugador::usarFicha() {
    if (fichasDisponibles > 0) {
        fichasDisponibles--;
    }
}

void Jugador::coronarColumna(int columna) {
    columnasCoronadas.push_back(columna);
}

bool Jugador::haGanado() const {
    return columnasCoronadas.size() >= 3;
}

vector<int> Jugador::obtenerColumnasCoronadas() const {
    return columnasCoronadas;
}