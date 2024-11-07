#ifndef TABLERO_H
#define TABLERO_H

#include <vector>
#include <map>
using namespace std;

class Tablero {
private:
    int numJugadores;
    map<int, int> alturaColumnas;
    vector<vector<vector<int>>> tablero; // [columna][nivel][jugadores]
    vector<pair<int, int>> posicionesTemporales; // Par de (columna, nivel)
    vector<vector<int>> posicionesPermanentes; // [columna][jugador] = nivel
    void verificarCoronacion(int columna, int jugador);
    
    
    
    void inicializarAlturas();

public:
    Tablero(int cantidadJugadores);
    bool moverFicha(int columna, int jugadorActual);
    void confirmarMovimientos(int jugadorActual);
    void cancelarMovimientos();
    bool puedeMoverse(int columna) const;
    bool columnaCoronada(int columna, int jugador) const;
    bool hayEspacioEnColumna(int columna) const;
    void imprimirTablero() const;
    vector<pair<int, int>> obtenerPosicionesTemporales() const;
    map<int, int> columnasCoronadas; // Columna como clave, valor es el ID del jugador que la coronó
    
    const map<int, int>& obtenerColumnasCoronadas() const;
    int contarColumnasCoronadasPorJugador(int jugador) const {
        int count = 0;
        for (const auto& [columna, jugadorId] : columnasCoronadas) {
            if (jugadorId == jugador) count++;
        }
        return count;
    }
    
};


#endif