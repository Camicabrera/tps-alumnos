#include "goleadores.h"
#include <vector>
#include <iostream>

struct GolesJugador{
    std::string jugador;
    int goles;
};
std::vector<GolesJugador> volcar (Diccionario<std::string, int>& dicc){
    //funcion que manda todo lo que esta en el diccionario a un vector usando iterador
    std::vector<GolesJugador> lista;
    Diccionario<std::string,int> :: Iterador it= dicc.crear_iterador();

    while (it.hay_mas()){
        lista.push_back({it.clave(),it.valor()});
        it.avanzar();
    }
    return lista;
}

bool va_despues(const GolesJugador& a, const GolesJugador& b, bool por_goles){
    /* pre:-
       post: si se ordena por goles, si a tiene menos goles que b es true,
        si tiene mas es false, y si son iguales o no se ordena por goles se ordena alfabeticamente 
    */
    if (por_goles){
        if (a.goles< b.goles){
            return true;
        }
        else if ( a.goles>b.goles){
            return false;
        }
    }
    return a.jugador>b.jugador; 
}

size_t IndiceDelMayor(std::vector<GolesJugador>& A, size_t n, size_t i, bool por_goles){
    size_t m=i;
    size_t izq= 2*i+1; //se multiplica por 2 para bajar un nivel
    size_t der= 2*i+2;

    if (izq< n && va_despues(A[izq], A[m],por_goles)){
        m=izq;
    }
    if (der< n && va_despues(A[der], A[m],por_goles)){
        m=der;
    }
    return m;

}

void AjustarHaciaAbajo(std::vector<GolesJugador>& A, size_t n, size_t i, bool por_goles){
    size_t j= IndiceDelMayor(A,n,i,por_goles);
    while (j!=i){
        GolesJugador medio= A[i];
        A[i]= A[j];
        A[j]= medio;

        i=j;
        j=IndiceDelMayor(A,n,i,por_goles);
    }
}
void Heapify(std::vector<GolesJugador>& A, bool por_goles){
    size_t n = A.size();
    for (size_t i = n; i > 0; i--) {
        AjustarHaciaAbajo(A, n, i - 1, por_goles);
    }
}

void Heapsort(std::vector<GolesJugador>& A, bool por_goles){
    size_t n = A.size();
    Heapify(A,por_goles);
    if (n>0){
        for (size_t k=n-1;k>=1;k--){
            GolesJugador medio= A[0];
            A[0]= A[k];
            A[k]= medio;

            AjustarHaciaAbajo(A,k,0,por_goles);
        }
    }

}

Goleadores::Goleadores() {

}

void Goleadores::anotar_gol(const std::string& jugador) {
    int goles_act= goles(jugador);
    goles_por_jugador.insertar(jugador,goles_act+1);
    
    if (maximo!= jugador){
        int max_goles= goles(maximo);
        if (goles_act+1 > max_goles){
            maximo= jugador;
        }
    }
    
}

int Goleadores::goles(const std::string& jugador) {
    if (goles_por_jugador.pertenece(jugador)){
        return goles_por_jugador.obtener(jugador);
    }
    return 0;
}

size_t Goleadores::cantidad_de_jugadores() {
    return goles_por_jugador.cantidad();
}

std::string Goleadores::maximo_goleador() {
    return maximo;
}

void Goleadores::imprimir_por_nombre() {
    std::vector<GolesJugador> lista= volcar(goles_por_jugador);
    Heapsort(lista,false);

    for (size_t i=0; i<lista.size();i++){
        std::cout<<lista[i].jugador<<": "<<lista[i].goles<<std::endl;
    }
}

void Goleadores::imprimir_por_goles() {
    std::vector<GolesJugador> lista= volcar(goles_por_jugador);
    Heapsort(lista,true);

    for (size_t i=0; i<lista.size();i++){
        std::cout<<lista[i].jugador<<": "<<lista[i].goles<<std::endl;
    }
}
