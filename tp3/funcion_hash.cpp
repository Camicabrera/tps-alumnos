#include "funcion_hash.h"

size_t funcion_hash(const std::string& clave) {
  
    size_t h=0; //size_t porque es un unsigned long  (sin signo de 64 bits)
    for (char c: clave){
        h= h*31 + size_t (c); //no da numero grande porque no hay ñ ni letras con acentos 
    }
    return h;
}

size_t funcion_hash(int clave) {

    return size_t(clave);
}
