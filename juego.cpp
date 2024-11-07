#include "juego.h"
#include <iostream>
#include <algorithm>

Juego::Juego(int numJugadores) : 
    tablero(numJugadores), 
    jugadorActual(1) {
    for (int i = 1; i <= numJugadores; i++) {
        jugadores.emplace_back(i);
    }
}

void Juego::iniciar() {
    bool hayGanador = false;
    
    while (!hayGanador) {
        turnoJugador();
        
        // Verificar si hay un ganador revisando las columnas coronadas
        for (const auto& jugador : jugadores) {
            int columnasCoronadasPorJugador = 0;
            const auto& columnasCoronadas = tablero.obtenerColumnasCoronadas();
            
            for (const auto& [columna, jugadorId] : columnasCoronadas) {
                if (jugadorId == jugador.obtenerNumero()) {
                    columnasCoronadasPorJugador++;
                }
            }
            
            if (columnasCoronadasPorJugador >= 3) {
                cout << "\n¡¡¡FELICITACIONES!!!\n";
                cout << "¡El jugador " << jugador.obtenerNumero() 
                     << " ha ganado el juego al coronar 3 columnas!\n";
                cout << "\nColumnas coronadas por el jugador " 
                     << jugador.obtenerNumero() << ":\n";
                
                for (const auto& [columna, jugadorId] : columnasCoronadas) {
                    if (jugadorId == jugador.obtenerNumero()) {
                        cout << "- Columna " << columna << "\n";
                    }
                }
                
                hayGanador = true;
                tablero.imprimirTablero(); // Mostrar estado final del tablero
                break;
            }
        }
        
        if (!hayGanador) {
            cambiarJugador();
        }
    }
}

void Juego::turnoJugador() {
    cout << "\n=== Turno del Jugador " << jugadorActual << " ===\n";
    columnasUsadasEnTurno.clear();
    bool continuarTurno = true;
    bool primerTiro = true;
    
    while (continuarTurno) {
        dados.lanzar();
        dados.imprimirCombinaciones();
        
        // Verificar si puede continuar con las columnas actuales
        if (!primerTiro && !columnasUsadasEnTurno.empty()) {
            const auto& columnasCoronadas = tablero.obtenerColumnasCoronadas();
            if (!dados.puedeContinuar(columnasUsadasEnTurno, columnasCoronadas)) {
                cout << "\nNo puedes usar ninguna combinación con las columnas actuales. "
                     << "Pierdes el progreso de este turno.\n";
                tablero.cancelarMovimientos();
                return;
            }
        }
        
        tablero.imprimirTablero();
        
        // Procesar el turno
        procesarTurno();
        primerTiro = false;
        
        // Mostrar el estado actual después del movimiento
        tablero.imprimirTablero();
        
        // Preguntar si quiere continuar
        if (columnasUsadasEnTurno.size() > 0) {
            char respuesta;
            cout << "\n¿Deseas continuar lanzando? (s/n): ";
            cin >> respuesta;
            continuarTurno = (respuesta == 's' || respuesta == 'S');
            
            if (!continuarTurno) {
                tablero.confirmarMovimientos(jugadorActual);
            }
        }
    }
}

void Juego::procesarTurno() {
    auto combinaciones = dados.obtenerCombinaciones();
    const auto& columnasCoronadas = tablero.obtenerColumnasCoronadas();
    
    // Primero verificar si hay alguna combinación válida
    bool hayCombinacionValida = false;
    for (const auto& combinacion : combinaciones) {
        if (columnasCoronadas.find(combinacion.first) == columnasCoronadas.end() &&
            columnasCoronadas.find(combinacion.second) == columnasCoronadas.end()) {
            hayCombinacionValida = true;
            break;
        }
    }

    if (!hayCombinacionValida) {
        cout << "\nNo hay combinaciones válidas disponibles ya que todas incluyen al menos una columna coronada.\n";
        cout << "El turno pasa al siguiente jugador.\n";
        return;
    }

    // Mostrar solo las combinaciones válidas
    cout << "\nCombinaciones válidas disponibles:\n";
    vector<pair<int, int>> combinacionesValidas;
    for (const auto& combinacion : combinaciones) {
        if (columnasCoronadas.find(combinacion.first) == columnasCoronadas.end() &&
            columnasCoronadas.find(combinacion.second) == columnasCoronadas.end()) {
            combinacionesValidas.push_back(combinacion);
            cout << combinacionesValidas.size() << ") " 
                 << combinacion.first << " y " << combinacion.second << "\n";
        }
    }

    if (combinacionesValidas.empty()) {
        cout << "No hay combinaciones válidas disponibles.\n";
        return;
    }

    int seleccion;
    cout << "\nElige una combinación (1-" << combinacionesValidas.size() << "): ";
    cin >> seleccion;
    
    if (seleccion < 1 || seleccion > static_cast<int>(combinacionesValidas.size())) {
        cout << "Selección inválida.\n";
        return;
    }
    
    auto combinacion = combinacionesValidas[seleccion - 1];
    vector<int> numerosElegidos;
    
    // Verificar si podemos usar ambos números
    if (columnasUsadasEnTurno.size() < 3 || 
        (find(columnasUsadasEnTurno.begin(), columnasUsadasEnTurno.end(), combinacion.first) != columnasUsadasEnTurno.end() &&
         find(columnasUsadasEnTurno.begin(), columnasUsadasEnTurno.end(), combinacion.second) != columnasUsadasEnTurno.end())) {
        
        if (tablero.hayEspacioEnColumna(combinacion.first)) {
            numerosElegidos.push_back(combinacion.first);
        }
        if (tablero.hayEspacioEnColumna(combinacion.second)) {
            numerosElegidos.push_back(combinacion.second);
        }
    }

    // Si no podemos usar ambos números, no permitir el movimiento
    if (numerosElegidos.size() != 2) {
        cout << "No se pueden usar ambos números de la combinación.\n";
        return;
    }

    // Mover fichas en las columnas seleccionadas
    for (int columna : numerosElegidos) {
        if (tablero.moverFicha(columna, jugadorActual)) {
            if (find(columnasUsadasEnTurno.begin(), columnasUsadasEnTurno.end(), columna) == columnasUsadasEnTurno.end()) {
                columnasUsadasEnTurno.push_back(columna);
            }
        }
    }
}

bool Juego::juegoTerminado() const {
    for (const Jugador& jugador : jugadores) {
        if (jugador.haGanado()) return true;
    }
    return false;
}

void Juego::cambiarJugador() {
    jugadorActual = (jugadorActual % jugadores.size()) + 1;
}

