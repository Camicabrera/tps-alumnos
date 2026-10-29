#include "funcion_hash.h"

size_t funcion_hash(const std::string& clave) {
    /*FUNCION ORIGINAL*/
    size_t hash = 0;                                        // static_cast<unsigned char> transforma a los carácteres en un número
                                                            
    for (char c : clave) {                  
        hash = hash * 31 + static_cast<unsigned char>(c);   // Multiplica por 31 para que el orden de 
    }                                                       // las letras influya en el hash y así reducir 
    return hash;                                            // las colisiones entre palabras distintas
}


    //MALAS FUNCIONES DE HASH
    //EJEMPLO CON SUMA DE CARACTERES (PALABRAS QUE TENGAN LAS MISMAS LETRAS HAY COLISION)
    /*size_t suma=0; 
    for (char c: clave){
        suma += size_t (c); 
    }
    return suma;
    */

    /*EJEMPLO TOMANDO LA PRIMERA LETRA
    return clave[0]; //solo toma la primera letra de la palabra (palabras != pero inicial=, colisionan)
    */

    /*EJEMPLO CON LARGO
    return clave.size();*/

    /*PRUEBA CON NUMERO QUE ES POTENCIA DE 2
    size_t h=0; //size_t porque es un unsigned long  (sin signo de 64 bits)
    for (char c: clave){
        h= h*32 + size_t (c); //no da numero grande porque no hay ñ ni letras con acentos 
    }
    return h;
    */


}

size_t funcion_hash(int clave) {
    return static_cast<size_t>(clave);
}
