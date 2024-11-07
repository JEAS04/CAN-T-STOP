#include "tablero.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

const string BLUE = "\033[34m";
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string YELLOW = "\033[33m";
const string RESET = "\033[0m";

string getColorForPlayer(int player) {
    switch(abs(player)) {  // abs para manejar tanto fichas temporales como permanentes
        case 1: return BLUE;
        case 2: return GREEN;
        case 3: return RED;
        case 4: return YELLOW;
        default: return "";
    }
}

Tablero::Tablero(int cantidadJugadores) : numJugadores(cantidadJugadores) {
    inicializarAlturas();
    // Inicializar el tablero con vectores vacíos para cada celda
    tablero.resize(13); // 0-12, ignoramos 0 y 1
    for (int i = 2; i <= 12; i++) {
        tablero[i].resize(alturaColumnas.at(i));
        for (int j = 0; j < alturaColumnas.at(i); j++) {
            tablero[i][j] = vector<int>();
        }
    }
    
    // Inicializar el registro de posiciones permanentes
    posicionesPermanentes.resize(13);
    for (int i = 2; i <= 12; i++) {
        posicionesPermanentes[i].resize(numJugadores + 1, -1); // +1 porque los jugadores empiezan en 1
    }
}

void Tablero::inicializarAlturas() {
    alturaColumnas[2] = 3;
    alturaColumnas[3] = 5;
    alturaColumnas[4] = 7;
    alturaColumnas[5] = 9;
    alturaColumnas[6] = 11;
    alturaColumnas[7] = 13;
    alturaColumnas[8] = 11;
    alturaColumnas[9] = 9;
    alturaColumnas[10] = 7;
    alturaColumnas[11] = 5;
    alturaColumnas[12] = 3;
}

bool Tablero::moverFicha(int columna, int jugadorActual) {
    if (columna < 2 || columna > 12) return false;
    
    // Si la columna ya está coronada, no permitir movimiento
    if (columnasCoronadas.find(columna) != columnasCoronadas.end()) {
        cout << "La columna " << columna << " ya está coronada por el jugador " 
             << columnasCoronadas[columna] << "\n";
        return false;
    }
    
    // Determinar el nivel de partida para la ficha temporal
    int nivelDeInicio = posicionesPermanentes[columna][jugadorActual] != -1 ? 
                        posicionesPermanentes[columna][jugadorActual] + 1 : 0;
    
    // Buscar si ya hay una ficha temporal del jugador en esta columna
    int nivelTemporal = nivelDeInicio;
    for (auto& pos : posicionesTemporales) {
        if (pos.first == columna && tablero[columna][pos.second].back() == -jugadorActual) {
            // Si ya existe una ficha temporal, avanzarla un nivel
            nivelTemporal = pos.second + 1;
            // Eliminar la ficha temporal del nivel anterior
            tablero[columna][pos.second].pop_back();
            break;
        }
    }

    // Verificar si el nivel está dentro de los límites de la columna
    if (nivelTemporal >= alturaColumnas.at(columna)) {
        return false; // No se puede avanzar más en esta columna
    }

    // Agregar o actualizar la ficha temporal en el nuevo nivel
    tablero[columna][nivelTemporal].push_back(-jugadorActual);
    
    // Actualizar o agregar la posición de la ficha temporal en la lista
    bool encontrada = false;
    for (auto& pos : posicionesTemporales) {
        if (pos.first == columna) {
            pos.second = nivelTemporal;
            encontrada = true;
            break;
        }
    }
    if (!encontrada) {
        posicionesTemporales.push_back({columna, nivelTemporal});
    }

    if (nivelTemporal == alturaColumnas.at(columna) - 1) {
        cout << "¡El jugador " << jugadorActual << " está en posición de coronar la columna " 
             << columna << "!\n";
    }

    return true;
}



void Tablero::confirmarMovimientos(int jugadorActual) {
    // Crear un mapa para guardar el nivel más alto alcanzado en cada columna
    map<int, int> nivelesMaximos;
    
    // Encontrar el nivel más alto alcanzado en cada columna
    for (const auto& pos : posicionesTemporales) {
        for (auto ficha : tablero[pos.first][pos.second]) {
            if (ficha == -jugadorActual) {
                if (nivelesMaximos.find(pos.first) == nivelesMaximos.end() ||
                    nivelesMaximos[pos.first] < pos.second) {
                    nivelesMaximos[pos.first] = pos.second;
                }
            }
        }
    }
    
    // Eliminar fichas permanentes anteriores y colocar las nuevas
    for (const auto& [columna, nivelMaximo] : nivelesMaximos) {
        // Eliminar ficha permanente anterior si existe
        int nivelAnterior = posicionesPermanentes[columna][jugadorActual];
        if (nivelAnterior != -1) {
            auto& fichasEnNivel = tablero[columna][nivelAnterior];
            fichasEnNivel.erase(
                remove(fichasEnNivel.begin(), fichasEnNivel.end(), jugadorActual),
                fichasEnNivel.end()
            );
        }
        
        // Colocar nueva ficha permanente en el nivel máximo alcanzado
        tablero[columna][nivelMaximo].push_back(jugadorActual);
        posicionesPermanentes[columna][jugadorActual] = nivelMaximo;
        
        // Verificar coronación después de colocar la ficha permanente
        if (nivelMaximo == alturaColumnas.at(columna) - 1) {
            verificarCoronacion(columna, jugadorActual);
        }
    }
    
    // Limpiar todas las fichas temporales del jugador
    for (const auto& pos : posicionesTemporales) {
        auto& fichasEnNivel = tablero[pos.first][pos.second];
        fichasEnNivel.erase(
            remove(fichasEnNivel.begin(), fichasEnNivel.end(), -jugadorActual),
            fichasEnNivel.end()
        );
    }
    
    posicionesTemporales.clear();
}

void Tablero::cancelarMovimientos() {
    // Eliminar todas las fichas temporales
    for (const auto& pos : posicionesTemporales) {
        auto& fichasEnNivel = tablero[pos.first][pos.second];
        fichasEnNivel.erase(
            remove_if(fichasEnNivel.begin(), fichasEnNivel.end(),
                     [](int ficha) { return ficha < 0; }),
            fichasEnNivel.end()
        );
    }
    posicionesTemporales.clear();
}

bool Tablero::puedeMoverse(int columna) const {
    if (columna < 2 || columna > 12) return false;
    return tablero[columna][alturaColumnas.at(columna) - 1].empty();
}

bool Tablero::columnaCoronada(int columna, int jugador) const {
    if (columna < 2 || columna > 12) return false;
    int altura = alturaColumnas.at(columna);
    const auto& fichasEnCima = tablero[columna][altura - 1];
    return find(fichasEnCima.begin(), fichasEnCima.end(), jugador) != fichasEnCima.end();
}

bool Tablero::hayEspacioEnColumna(int columna) const {
    if (columna < 2 || columna > 12) return false;
    return puedeMoverse(columna);
}

void Tablero::imprimirTablero() const {
    cout << "\n=== TABLERO CANT STOP ===\n\n";
    
    // Imprimir información de columnas coronadas
    cout << "Columnas coronadas:\n";
    for (const auto& [columna, jugador] : columnasCoronadas) {
        cout << "Columna " << columna << " - Coronada por jugador " 
             << getColorForPlayer(jugador) << jugador << RESET << "\n";
    }
    cout << "\n";
    
    // Calcular el ancho de cada celda basado en el número de jugadores
    int anchoCelda = 3 + numJugadores * 2 - 1;  // Aumentado en 1 para el espacio extra
    
    // Imprimir el tablero de arriba a abajo
    for (int nivel = 12; nivel >= 0; nivel--) {
        cout << setw(2) << nivel + 1 << " ";
        
        for (int col = 2; col <= 12; col++) {
            if (nivel < alturaColumnas.at(col)) {
                const auto& fichasEnNivel = tablero[col][nivel];
                cout << "[";
                
                if (fichasEnNivel.empty()) {
                    cout << setw(anchoCelda - 2) << " ";
                } else {
                    string contenido;
                    for (size_t i = 0; i < fichasEnNivel.size(); ++i) {
                        if (i > 0) contenido += ",";
                        
                        // Agregar color según el jugador
                        string color = getColorForPlayer(fichasEnNivel[i]);
                        if (fichasEnNivel[i] < 0) {
                            contenido += color + "T" + to_string(-fichasEnNivel[i]) + RESET;
                        } else {
                            contenido += color + to_string(fichasEnNivel[i]) + RESET;
                        }
                    }
                    
                    // Centrar el contenido en el espacio disponible
                    int longitudSinColores = 0;
                    for (const auto& ficha : fichasEnNivel) {
                        longitudSinColores += (abs(ficha) > 9 ? 2 : 1);  // Número de dígitos
                        if (ficha < 0) longitudSinColores += 1;  // 'T' para temporales
                    }
                    if (fichasEnNivel.size() > 1) {
                        longitudSinColores += fichasEnNivel.size() - 1;  // Comas
                    }
                    
                    if (longitudSinColores <= anchoCelda - 2) {
                        int espaciosRestantes = anchoCelda - 2 - longitudSinColores;
                        int espaciosIzquierda = espaciosRestantes / 2;
                        cout << string(espaciosIzquierda, ' ') << contenido
                             << string(espaciosRestantes - espaciosIzquierda, ' ');
                    } else {
                        cout << contenido;
                    }
                }
                cout << "]";
            } else {
                cout << string(anchoCelda, ' ');
            }
            cout << " ";
        }
        cout << "\n";
    }
    
    // Imprimir números de columnas
    cout << "    ";
    for (int col = 2; col <= 12; col++) {
        string numCol = to_string(col);
        int espaciosTotal = anchoCelda;
        int espaciosIzquierda = (espaciosTotal - numCol.length()) / 2;
        
        cout << string(espaciosIzquierda, ' ') << numCol 
             << string(espaciosTotal - espaciosIzquierda - numCol.length(), ' ') << " ";
    }
    cout << "\n\n";
}
void Tablero::verificarCoronacion(int columna, int jugador) {
    if (columnasCoronadas.find(columna) == columnasCoronadas.end()) {
        columnasCoronadas[columna] = jugador;
        cout << "\n¡El jugador " << jugador << " ha coronado la columna " << columna << "!\n";
        
        // Contar cuántas columnas ha coronado este jugador
        int columnasCoronadasPorJugador = 0;
        for (const auto& [col, jug] : columnasCoronadas) {
            if (jug == jugador) columnasCoronadasPorJugador++;
        }
        
        cout << "El jugador " << jugador << " ha coronado " << columnasCoronadasPorJugador 
             << " columna(s)\n";
        
        if (columnasCoronadasPorJugador >= 3) {
            cout << "\n¡¡¡FELICITACIONES!!! ¡El jugador " << jugador 
                 << " ha coronado 3 columnas y ha ganado el juego!\n";
        }
    }
}
const map<int, int>& Tablero::obtenerColumnasCoronadas() const {
    return columnasCoronadas;
}


