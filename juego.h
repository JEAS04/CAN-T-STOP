#ifndef JUEGO_H
#define JUEGO_H

#include "tablero.h"
#include "jugador.h"
#include "dados.h"
#include <vector>

using namespace std;

class Juego {
private:
    Tablero tablero;
    vector<Jugador> jugadores;
    Dados dados;
    int jugadorActual;
    vector<int> columnasUsadasEnTurno;

public:
    Juego(int numJugadores);
    void iniciar();
    void turnoJugador();
    bool juegoTerminado() const;
    void cambiarJugador();
    void procesarTurno();
};

#endif