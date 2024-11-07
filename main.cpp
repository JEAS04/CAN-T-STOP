#include "juego.h"
#include <iostream>
#include <limits>

using namespace std;

int main() {
    cout << "¡Bienvenido a Can't Stop!\n\n";
    
    int numJugadores;
    do {
        cout << "Ingrese el número de jugadores (2-4): ";
        cin >> numJugadores;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            numJugadores = 0;
        }
        
        if (numJugadores < 2 || numJugadores > 4) {
            cout << "Número de jugadores inválido. Por favor, ingrese un número entre 2 y 4.\n";
        }
    } while (numJugadores < 2 || numJugadores > 4);
    
    Juego juego(numJugadores);

    juego.iniciar();
    
    return 0;
}