#include "dados.h"
#include <random>
#include <algorithm>
#include <iostream>

Dados::Dados() : dados(4) {}

void Dados::lanzar() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 6);
    
    // Lanzar los 4 dados
    for (int i = 0; i < 4; i++) {
        dados[i] = dis(gen);
    }
    
    // Calcular todas las combinaciones posibles
    combinacionesPosibles.clear();
    
    // Primera combinación: dados[0] + dados[1], dados[2] + dados[3]
    combinacionesPosibles.push_back({dados[0] + dados[1], dados[2] + dados[3]});
    
    // Segunda combinación: dados[0] + dados[2], dados[1] + dados[3]
    combinacionesPosibles.push_back({dados[0] + dados[2], dados[1] + dados[3]});
    
    // Tercera combinación: dados[0] + dados[3], dados[1] + dados[2]
    combinacionesPosibles.push_back({dados[0] + dados[3], dados[1] + dados[2]});
    
    // Eliminar duplicados
    sort(combinacionesPosibles.begin(), combinacionesPosibles.end());
    combinacionesPosibles.erase(
        unique(combinacionesPosibles.begin(), combinacionesPosibles.end()),
        combinacionesPosibles.end()
    );
}

vector<pair<int, int>> Dados::obtenerCombinaciones() const {
    return combinacionesPosibles;
}

bool Dados::puedeContinuar(const vector<int>& columnasUsadas, const map<int, int>& columnasCoronadas) const {
    for (const auto& combinacion : combinacionesPosibles) {
        // Si cualquiera de los números está en una columna coronada, 
        // esta combinación no es válida
        if (columnasCoronadas.find(combinacion.first) != columnasCoronadas.end() ||
            columnasCoronadas.find(combinacion.second) != columnasCoronadas.end()) {
            continue;  // Pasar a la siguiente combinación
        }

        // Si llegamos aquí, ninguno de los números está coronado
        bool ambosNumerosValidos = true;

        // Verificar el primer número
        if (columnasUsadas.size() >= 3) {
            // Si ya usamos 3 columnas, el número debe estar entre ellas
            bool encontrado = false;
            for (int columna : columnasUsadas) {
                if (combinacion.first == columna) {
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) ambosNumerosValidos = false;
        }

        // Verificar el segundo número
        if (columnasUsadas.size() >= 3) {
            // Si ya usamos 3 columnas, el número debe estar entre ellas
            bool encontrado = false;
            for (int columna : columnasUsadas) {
                if (combinacion.second == columna) {
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) ambosNumerosValidos = false;
        }

        if (ambosNumerosValidos) return true;
    }
    return false;
}

void Dados::imprimirCombinaciones() const {
    cout << "\nDados lanzados: ";
    for (int dado : dados) {
        cout << dado << " ";
    }
    cout << "\nCombinaciones posibles:\n";
    
    for (size_t i = 0; i < combinacionesPosibles.size(); i++) {
        cout << i + 1 << ") " << combinacionesPosibles[i].first 
             << " y " << combinacionesPosibles[i].second << "\n";
    }
}

