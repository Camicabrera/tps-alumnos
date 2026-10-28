#ifndef TABLA_HASH_H
#define TABLA_HASH_H

#include <cstddef>

#include "funcion_hash.h"

// TAD TablaHash<K, V>: tabla de hash con direccionamiento cerrado.
//
// Guarda pares <clave, valor> repartidos en buckets: el par de una clave esta
// en el bucket numero funcion_hash(clave) % capacidad().
//
// Requisitos sobre los tipos:
//   - K: se puede copiar, comparar con == y existe funcion_hash(K).
//   - V: se puede copiar.
//
// Invariante de representacion:
    //   - c_buckets >= 1.
    //   - buckets es un arreglo dinamico de c_buckets posiciones.
    //   - cada buckets[i] es nullptr (bucket vacio) o apunta al primer nodo
    //     de una lista enlazada, cuyo ultimo nodo tiene siguiente == nullptr.
    //   - cada nodo del bucket i cumple funcion_hash(clave) % c_buckets == i.
    //   - ninguna clave aparece mas de una vez en toda la tabla.
    //   - c_claves es la cantidad total de nodos, sumando todos los buckets.
    //   - c_claves <= FACTOR_DE_CARGA_MAXIMO * c_buckets.
//
// Complejidades: n es la cantidad de claves guardadas y c la capacidad.
// "Promedio" supone que la funcion de hash reparte las claves de manera
// uniforme y que calcularla (y comparar dos claves) cuesta O(1).
template <typename K, typename V>
class TablaHash {
  private:
    // COMPLETAR: la representacion la eligen ustedes (que guardan, y que hay
    // en cada bucket). Pueden agregar tambien funciones auxiliares privadas.
    // Lo que el Iterador necesite de aca tiene que estar declarado antes que el.


    struct Nodo {
        K clave;
        V valor;
        Nodo* siguiente;
    };
    size_t c_claves; //cantidad de claves
    size_t c_buckets; //cantidad de buckets 
    Nodo** buckets; //al ser un doble puntero puede ser nullptr, si es uno solo es un arreglo de nodos 

    void redimensionar(); //funcion agregada para redimensionar en caso de ser necesario antes de insertar

  public:
    static const size_t FACTOR_DE_CARGA_MAXIMO = 2;

    // Pre:  capacidad_inicial >= 1.
    // Post: crea una tabla vacia, con capacidad_inicial buckets.
    // Costo: O(capacidad_inicial).
    TablaHash(size_t capacidad_inicial);

    // Post: libera toda la memoria pedida por la tabla.
    // Costo: O(n + c) en peor caso.
    ~TablaHash();

    // Copiar una tabla copiaria los punteros y no su contenido: lo prohibimos.
    TablaHash(const TablaHash&) = delete;
    TablaHash& operator=(const TablaHash&) = delete;

    // Pre:  la clave NO esta en la tabla.
    // Post: la tabla tiene ademas el par <clave, valor>, y
    //       cantidad() <= FACTOR_DE_CARGA_MAXIMO * capacidad().
    // Costo: O(1) amortizado (en peor caso O(n), cuando toca redimensionar).
    void insertar(const K& clave, const V& valor);

    // Pre:  la clave esta en la tabla.
    // Post: la tabla ya no tiene la clave (ni su valor).
    // Costo: O(1) en promedio, O(n) en peor caso.
    void eliminar(const K& clave);

    // Pre:  -
    // Post: si la clave esta, devuelve un puntero a su valor (que se puede
    //       usar para leerlo o modificarlo); si no, devuelve nullptr.
    //       El puntero deja de ser valido al insertar o eliminar.
    // Costo: O(1) en promedio, O(n) en peor caso.
    V* buscar(const K& clave);

    // Post: devuelve la cantidad de claves guardadas. Costo: O(1).
    size_t cantidad();

    // Post: devuelve la cantidad de buckets. Costo: O(1).
    size_t capacidad();

    // Post: devuelve la cantidad de claves que comparten bucket con alguna
    //       clave que llego antes, es decir:
    //       cantidad() - (cantidad de buckets que no estan vacios).
    // Costo: O(c) en peor caso.
    size_t colisiones();

    // Iterador de la tabla: recorre todos los pares <clave, valor>, cada uno
    // exactamente una vez, en algun orden (ver TAD Iterador en el enunciado).
    // Deja de ser valido si se inserta o se elimina en la tabla.
    //
    // Invariante de representacion:
    //  p_tabla es un puntero a la tabla hash
    //  actual es nullptr, o apunta a un nodo de la lista de buckets[indice]
    //  0 <= indice <= p_tabla->c_buckets
    // si actual!= nullptr, indice<p_tabla->buckets y actual es un nodo de la lista de p_tabla->buckets[indice]
    // si actual== nullptr, idnice== p_tab;a->buckets (ya se recorrio todo)
    class Iterador {
      private:
        
        TablaHash* p_tabla;
        size_t indice;
        Nodo* actual;
        // Para que crear_iterador() pueda armar el iterador.
        friend class TablaHash;
        
        void ir_desde(size_t inicio) {
            indice= inicio;
            while (indice < p_tabla->c_buckets && p_tabla->buckets[indice]==nullptr){
                indice++;
            }
            if (indice == p_tabla->c_buckets){
                actual=nullptr;
                return;
            }
            actual=  p_tabla->buckets[indice];
        }

      public:
        // Pre:  -
        // Post: indica si quedan pares por recorrer.
        // Costo: O(1).
        bool hay_mas() {
            return actual != nullptr;
        }

        // Pre:  hay_mas().
        // Post: devuelve la clave del par actual, sin modificar el iterador.
        // Costo: O(1).
        K clave() {
            // COMPLETAR (reemplazar la linea de abajo)
            return actual->clave;
        }

        // Pre:  hay_mas().
        // Post: devuelve el valor del par actual, sin modificar el iterador.
        // Costo: O(1).
        V valor() {
            // COMPLETAR (reemplazar la linea de abajo)
            return actual->valor;
        }

        // Pre:  hay_mas().
        // Post: el iterador pasa al par siguiente (o al final, si no hay mas).
        // Costo: O(c) en peor caso. Recorrer la tabla entera (crear el
        //        iterador y avanzar hasta el final) cuesta O(n + c).
        void avanzar() {
            // COMPLETAR
            if (actual->siguiente != nullptr){
                actual= actual->siguiente;
                return;
            }
            ir_desde(indice + 1);
        }
    };

    // Pre:  -
    // Post: devuelve un iterador al primer par de la tabla (si la tabla esta
    //       vacia, el iterador ya esta al final).
    // Costo: O(c) en peor caso.
    Iterador crear_iterador();
};

// ---------------------------------------------------------------------------
// Implementacion. Al ser una clase template, va en el mismo .h.
// ---------------------------------------------------------------------------

template <typename K, typename V>
TablaHash<K, V>::TablaHash(size_t capacidad_inicial) {
    // COMPLETAR
    buckets= new Nodo* [capacidad_inicial];
    c_buckets= capacidad_inicial;
    c_claves=0;

    for (size_t i=0; i< capacidad_inicial; i++){
        buckets[i]= nullptr;
    }
}

template <typename K, typename V>
TablaHash<K, V>::~TablaHash() {
    // COMPLETAR
    for (size_t i=0; i< c_buckets; i++){
       Nodo* actual= buckets[i];
       while (actual!= nullptr){
            Nodo* viejo= actual;
            actual= actual->siguiente;
            delete viejo;
       }
    }
    delete[] buckets;
}
template <typename K, typename V>
void TablaHash<K, V>::redimensionar() {
    Nodo** viejos= buckets;
    size_t c= c_buckets;
    c_buckets= c*2;
    buckets= new Nodo*[c_buckets];
    //se ponen todos en null
    for (size_t i=0; i< c_buckets; i++){
        buckets[i]= nullptr;
    }

    //copia a la nueva tabla
    for (size_t i=0;i<c;i++){
        Nodo* actual=viejos[i];
        while (actual!= nullptr){
            Nodo* proximo=actual->siguiente;
            size_t j= funcion_hash(actual->clave)%c_buckets;
            actual->siguiente= buckets[j];
            buckets[j]= actual;
            actual= proximo;
        }
    }
    delete[] viejos;
}

template <typename K, typename V>
void TablaHash<K, V>::insertar(const K& clave, const V& valor) {
    if (cantidad()>= FACTOR_DE_CARGA_MAXIMO* capacidad()){
        redimensionar();
    }
    size_t indice= funcion_hash(clave)%c_buckets;
    Nodo* nuevo = new Nodo{clave, valor, buckets[indice]};
    buckets[indice]= nuevo;
    c_claves ++; 
}

template <typename K, typename V>
void TablaHash<K, V>::eliminar(const K& clave) {
    size_t i= funcion_hash(clave)% c_buckets;
    Nodo* actual= buckets[i];
    if (actual->clave == clave){
        buckets[i]= actual -> siguiente;
    }

    else{
        while (!(actual-> siguiente -> clave == clave)){
            actual= actual -> siguiente;
        }
        Nodo* viejo= actual ->siguiente;
        actual->siguiente= viejo->siguiente;
        actual= viejo;
    }
    delete actual;
    c_claves--;
}

template <typename K, typename V>
V* TablaHash<K, V>::buscar(const K& clave) {
    size_t i=  funcion_hash(clave)%c_buckets;
    Nodo* actual= buckets[i];

    while (actual!=nullptr && !(actual->clave== clave)){
        actual= actual-> siguiente;
    }
    if (actual!= nullptr){
        return &actual->valor;
    }
    return nullptr;
}

template <typename K, typename V>
size_t TablaHash<K, V>::cantidad() {
   
    return c_claves;
}

template <typename K, typename V>
size_t TablaHash<K, V>::capacidad() {
  
    return c_buckets;
}

template <typename K, typename V>
size_t TablaHash<K, V>::colisiones() {
    size_t ocupados=0;
    for (size_t i=0; i<c_buckets; i++){
        if (!(buckets[i]==nullptr)){
            ocupados++;
        }
    }
    return c_claves-ocupados;
}

// El "typename" le avisa al compilador que TablaHash<K, V>::Iterador es un tipo.
template <typename K, typename V>
typename TablaHash<K, V>::Iterador TablaHash<K, V>::crear_iterador() {
    Iterador it;
    it.p_tabla = this;
    it.ir_desde(0);
    return it;
}

#endif
