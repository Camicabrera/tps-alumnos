#include "tp1.h"
/* ---------------------------------------------------------------
 * Parte 1: funciones libres (no operan sobre un arreglo).
 * --------------------------------------------------------------- */

bool is_prime(int x) {
    // TODO: determinar si x es primo.
    if (x <= 1) {
        return false;
    }
    for (int i=2;i<x;i++){
        if (x%i==0){
            return false;
        }
    }
    return true;
}

int storage_capacity(float d, float v) {
    // TODO: calcular cuántos productos de volumen v entran en el depósito d.
    float productos=d/v;
    return (int) productos;
}

void swap(int *x, int *y) {
    // TODO: intercambiar los valores de x e y.
    int medio= *y;
    *y=*x;
    *x= medio;
}

/* ---------------------------------------------------------------
 * Parte 2: CustomVector. Administra su propia memoria con
 * new/delete. Recordá la regla de los tres: destructor,
 * constructor de copia y operator=.
 * --------------------------------------------------------------- */

CustomVector::CustomVector(int length) {
    // TODO: reservar memoria para 'length' enteros y setear capacity.
    this->length = length;
    this->capacity = length;
    this->data = new int[length];
}

CustomVector::CustomVector(const int *array, int length) {
    // TODO: reservar memoria, copiar 'length' enteros desde 'array' y
    // setear capacity.
    this->length = length;
    this->capacity = length;
    this->data = new int[length];
    for(int i=0; i< length;i++){
        this -> data[i]=array[i];
    }
}

CustomVector::CustomVector(const CustomVector &other) {
    // TODO: copia profunda del contenido de 'other' (incluida capacity).
    this->length = other.get_length();
    this->capacity = other.get_capacity();
    this->data = new int[this -> capacity];
    for (int i=0;i<this->length;i++){
        this -> data[i]=other[i];
    }
}

CustomVector &CustomVector::operator=(const CustomVector &other) {
    // TODO: liberar lo anterior y hacer una copia profunda de 'other'
    // (incluida capacity).
    if (this != &other){
        delete [] this -> data;
        this->length = other.get_length();
        this->capacity = other.get_capacity();
        this->data = new int[this -> capacity];
        for (int i=0;i<this->length;i++){
            this -> data[i]=other[i];
        }
    }

    return *this;
}

CustomVector::~CustomVector() {
    // TODO: liberar la memoria del vector.
    delete [] this ->data;
    this -> data=nullptr;
}

int CustomVector::get_length() const {
    return this->length;
}

int &CustomVector::operator[](int i) {
    // TODO: devolver el elemento en la posición i.
    return this->data[i];
}

const int &CustomVector::operator[](int i) const {
    // TODO: devolver el elemento en la posición i.
    return this->data[i];
}

int CustomVector::max() const {
    // TODO: devolver el máximo del vector.
    int max= *(this -> data);
    for (int i=1;i<this -> length;i++){
        if ((*this)[i] > max){
            max= (*this)[i];
        }
    }
    return max;
}

void CustomVector::map(int (*f)(int)) {
    // TODO: aplicar f a cada elemento. Si f es NULL, no hacer nada.
    if (f){
        for (int i=0;i<this -> length;i++){
            (*this)[i]=f((*this)[i]);
        }
    }
}

void CustomVector::bubble_sort() {
    // TODO: ordenar el vector ascendentemente con bubble sort.
    for (int i=1;i<this ->length;i++){
        for (int j=0;j<(this ->length - i);j++){
            int der= j+1;
            int izq=j;

            if ((*this)[izq]>(*this)[der]){
                int medio=(*this)[izq];
                (*this)[izq]=(*this)[der];
                (*this)[der]=medio;
            }
        }
    }
}

bool CustomVector::equal(const CustomVector &other) const {
    // TODO: determinar si los dos vectores son idénticamente iguales.
    if(this -> length == other.get_length()){
        for (int i=0;i<this ->length;i++){
            if ((*this)[i] != other[i]){
                return false;
            }
            
        }
        return true;
    }
    return false;
}

bool CustomVector::is_anagram(const CustomVector &other) const {
    // TODO: determinar si los dos vectores son anagramas.
    if(this -> length == other.get_length()){
        CustomVector copia(*this);
        CustomVector copia2(other);

        copia.bubble_sort();
        copia2.bubble_sort();

        if (copia.equal(copia2)){
            return true;
        }
    }
    return false;
}

int CustomVector::get_capacity() const {
    return this->capacity;
}

void CustomVector::push_back(int value) {
    // TODO: agregar 'value' al final del vector, creciendo la capacity
    // si hace falta.

    if (this -> length == this -> capacity){
        this-> capacity += 1;
        int *nuevo = new int[this -> capacity];

        for (int i=0;i<this-> length;i++){
            *(nuevo + i)= (*this)[i];
        } 
        *(nuevo + this -> length)= value;
        this -> length ++;

        delete [] this -> data;
        this -> data = nuevo;

    }
    else{

        (*this)[this -> length] = value;
        this -> length ++;
    }

}

void CustomVector::remove() {
    // TODO: quitar el último elemento del vector. Si está vacío, no
    // hacer nada.
    if (this -> length >0){
        this ->length --;
    }
}

/* ---------------------------------------------------------------
 * Parte 3: reserva de UN solo objeto (new / delete escalar).
 * --------------------------------------------------------------- */

CustomVector *war_winner(const CustomVector &a, const CustomVector &b) {
    // TODO: devolver un CustomVector nuevo con el ganador de cada
    // ronda (el máximo entre a[i] y b[i]).
    int ronda;
    if (a.get_length()> b.get_length()){
        ronda = b.get_length();
    }
    else{
        ronda = a.get_length();
    }
    
    CustomVector *puntaje= new CustomVector(ronda);

    for (int i=0;i<ronda;i++){
        if(a[i]>b[i]){
            (*puntaje)[i]=a[i];
        }
        else{
            (*puntaje)[i]=b[i];
        }
    }


    return puntaje;
}

void destroy_vector(CustomVector *vector) {
    // TODO: liberar vector con delete (versión escalar, sin []).
    delete vector;
}

CustomVector *tournament_champion(CustomVector **decks, int num_decks) {
    // TODO: enfrentar decks[0] contra cada mazo siguiente con
    // war_winner, liberando el campeón anterior y el mazo que
    // perdió en cada pelea, y devolver el campeón final.
    CustomVector *campeon= decks[0]; 
    for (int i=1;i<num_decks;i++){
        CustomVector *ganador = war_winner(*campeon,*(decks)[i]);
        destroy_vector (campeon);
        destroy_vector( decks[i]);
        campeon=ganador;

    }
    delete[] decks;

    return campeon ;
}
