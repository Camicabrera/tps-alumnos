#include <iostream>
#include <string>

#include "goleadores.h"

// Cambiar a true si hicieron la parte optativa (los dos listados).
const bool CON_LISTADOS = true;

// Lee de la entrada estandar la lista de goles del grupo (un gol por linea:
// el nombre del jugador que lo hizo) e imprime el resumen.
//
// Uso: ./goleadores < datos/ejemplo.txt
int main() {
    // COMPLETAR: crear la estructura donde se van a cargar los goles.
    Goleadores goleadores;
    std::string jugador;

    while (std::getline(std::cin, jugador)) {
        if (jugador != "") {
            // En este punto, jugador tiene el nombre de quien hizo el gol.
            goleadores.anotar_gol(jugador);
        }
    }

   
    std::cout << "Jugadores con goles: "
          << goleadores.cantidad_de_jugadores() << std::endl;   // Imprime cuántos jugadores distintos anotaron 

    if (goleadores.cantidad_de_jugadores() > 0) {               // Si hay al menos uno, obtiene el máximo goleador y muestra sus goles
        std::string maximo = goleadores.maximo_goleador();

        std::cout << "Maximo goleador: "
                << maximo << " ("
                << goleadores.goles(maximo)
                << " goles)" << std::endl;
    }

    if (CON_LISTADOS) {
        std::cout << std::endl;
        std::cout << "Goleadores por nombre:" << std::endl;
        goleadores.imprimir_por_nombre();

        std::cout << std::endl;
        std::cout << "Goleadores por goles:" << std::endl;
        goleadores.imprimir_por_goles();
    }

    return 0;
}
