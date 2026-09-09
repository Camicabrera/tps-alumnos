#ifndef TP2_H
#define TP2_H

#include <cstddef> // size_t

/*
 * ============================================================
 *  Lista doblemente enlazada -> clase List.
 *  List es un tipo paramétrico: para cada T específico se tiene
 *  un tipo de datos List<T>.
 *
 *  La lista es dueña de sus nodos (RAII): los reserva con
 *  new y los libera en el destructor con delete. Cada nodo contiene
 *  un valor de tipo genérico T. Del contrato del tipo T,
 *  solamente se puede asumir que el tipo T tiene definidos un
 *  constructor por copia, un operador de asignación,
 *  y un destructor adecuados, pero ninguna otra operación.
 * 
 *  Los tipos predefinidos estandar de C++ como int, bool, char,
 *  punteros, string, vector, etc cumplen esto así que se
 *  debe poder crear List<int>, List<bool>, List<string>,
 *  List<vector<int>>, etc.
 *  Como la propia lista tiene definidos en este archivo
 *  esas operaciones, se puede crear listas de listas
 *  como por ejemplo List<List<int>> o List<List<List<string>>>.
 * 
 *  C++ se encarga de llamar automáticamente al destructor de T tras
 *  ejecutar el destructor de un Node<T> desencadenado por
 *  un delete de un nodo (para poder liberar el value del nodo).
 * ============================================================
 */
template<typename T>
class List {
private:
    /*
     * Nodo interno de la lista. Es un detalle de implementación,
     * por eso queda privado. El tipo completo es List<T>::Node
     */
    struct Node {
        // Unico constructor de Node: utiliza el constructor por copia de T
        Node(const T& v) : value(v) {}
        T value;
        Node *next;
        Node *prev;
    };

    Node *head;
    Node *tail;
    size_t size;

public:
    /*
     * Crea una lista vacía.
     * P (Pre):  -
     * Q (Post): is_empty() == true && length() == 0.
     */
    List();

    /* Constructor por copia copia.
     * P (Pre):  other es una List válida.
     * Q (Post): *this ahora contiene una copia independiente de other,
     *            con sus propios nodos diferentes de los de other (deep copy)
     */
    List(const List<T> &other);

    /* Operador de asignacion.
     * P (Pre):  other es una List válida.
     * Q (Post): *this pasa a contener una copia independiente de other,
     *            con sus propios nodos diferentes de los de other (deep copy)
     *           No se debe perder memoria!!!

     */
    List &operator=(const List<T> &other);

    /*
     * Destructor: libera todos los nodos que queden con delete.
     * No necesita encargarse de liberar por separado
     * los valores T de los value explicitamente:
     * C++ llama al destructor del tipo T automaticamente al hacer delete de un nodo
     * P (Pre):  '*this' es una List válida.
     * Q (Post): la memoria de todos los nodos quedó liberada.
     */
    ~List();

    /*
     * Devuelve true si la lista está vacía.
     * P (Pre):  '*this' es una List válida.
     * Q (Post): devuelve length() == 0; no modifica la lista (const).
     */
    bool is_empty() const;

    /*
     * Cantidad de elementos de la lista.
     * P (Pre):  '*this' es una List válida.
     * Q (Post): devuelve la cantidad de elementos (>= 0);
     *           no modifica la lista (const).
     */
    size_t length() const;

    /*
     * Inserta un elemento al principio de la lista.
     * P (Pre):  '*this' es una List válida. Sea n0 = length().
     * Q (Post): length() == n0 + 1, peek_head() == value.
     */
    void insert_head(const T& value);

    /*
     * Inserta un elemento al final de la lista.
     * P (Pre):  '*this' es una List válida. Sea n0 = length().
     * Q (Post): length() == n0 + 1, peek_tail() == value.
     */
    void insert_tail(const T& value);

    /*
     * Elimina el elemento del principio y devuelve el valor que guardaba.
     * P (Pre):  '*this' es una List no vacia. Sea n0 = length().
     * Q (Post): saca el primer nodo, length() == n0 - 1 y
     *           devuelve el valor que tenía.
     */
    T pop_head();

    /*
     * Elimina el elemento del final y devuelve el valor que guardaba.
     * P (Pre):  '*this' es una List no vacia. Sea n0 = length().
     * Q (Post): saca el último nodo, length() == n0 - 1 y
     *           devuelve el valor que tenía.
     */
    T pop_tail();

    /*
     * Devuelve el valor del principio de la lista sin sacarlo.
     * P (Pre):  '*this' es una List no vacia.
     * Q (Post): devuelve el primer valor;
     *           no modifica la lista (const).
     */
    const T &peek_head() const;

    /*
     * Devuelve el valor del final de la lista sin sacarlo.
     * P (Pre):  '*this' es una List<T> no vacia.
     * Q (Post): devuelve el último valor;
     *           no modifica la lista (const).
     */
    const T &peek_tail() const;
    
    /*
     * ============================================================
     *  Iterador de lista. El tipo completo es List<T>::ListIter
     *
     *  Puede acceder a los nodos y miembros privados de List,
     *  al ser una clase anidada dentro del scope List<T>
     *  Se crean con los metodos List::create_head / List::create_tail
     * ============================================================
     */
    class ListIter {
    private:
        List *list;
        List::Node *curr;

        /*
         * Constructor privado: se usa desde las fábricas List::create_head /
         * List::create_tail. Deja el iterador parado en 'start'.
         */
        ListIter(List<T> *list, List<T>::Node *start);

        friend List<T>;

    public:
        /*
         * Avanza el iterador una posición hacia el final.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): si hay un elemento siguiente, avanza y devuelve true;
         *           si no, no se mueve y devuelve false.
         */
        bool forward();

        /*
         * Retrocede el iterador una posición hacia el principio.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): si hay un elemento anterior, retrocede y devuelve true;
         *           si no, no se mueve y devuelve false.
         */
        bool backward();

        /*
         * Devuelve el valor de la posición actual.
         * P (Pre):  '*this' es un ListIter válido a un elemento de una lista no vacia.
         * Q (Post): devuelve el valor actual; no modifica nada (const).
         */
        const T& peek_current() const;

        /*
         * Indica si el iterador está en el último elemento.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): devuelve true si está en el último (o la lista está vacía),
         *           false en otro caso; no modifica nada (const).
         */
        bool at_last() const;

        /*
         * Indica si el iterador está en el primer elemento.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): devuelve true si está en el primero (o la lista está vacía),
         *           false en otro caso; no modifica nada (const).
         */
        bool at_first() const;

        /*
         * Inserta un valor justo después del elemento actual.
         * Si la lista está vacía, inserta el primer elemento y queda parado en él.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): agrega el valor detrás del actual sin mover la posición del
         *           iterador y devuelve true.
         */
        bool insert_after(const T&value);

        /*
         * Inserta un valor justo antes del elemento actual.
         * Si la lista está vacía, inserta el primer elemento y queda parado en él.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): agrega el valor delante del actual sin mover la posición del
         *           iterador y devuelve true.
         */
        bool insert_before(const T&value);

        /*
         * Elimina el elemento actual y devuelve el valor que guardaba.
         * P (Pre):  '*this' es un ListIter que apunta a un elemento válido en una lista no vacia.
         * Q (Post): saca el nodo actual, se para en el siguiente (o en el
         *           anterior si no había siguiente) y devuelve el valor que tenía.
         */
        T remove();
    };

    /*
     * Crea un iterador parado en el principio de la lista.
     * P (Pre):  '*this' es una List<T> válida.
     * Q (Post): devuelve un iterador nuevo parado en el primer elemento.
     */
    ListIter create_head();

    /*
     * Crea un iterador parado en el final de la lista.
     * P (Pre):  '*this' es una List<T> válida.
     * Q (Post): devuelve un iterador nuevo parado en el último elemento.
     */
    ListIter create_tail();


};


// ****************************************************************
//  SECCION DE IMPLEMENTACION!!!
//    Por tratarse de una clase template, la implementacion
//    debe ir en el mismo .h, debido a como funcionan
//    los templates en C++
// ****************************************************************



/* ---------------------------------------------------------------
 * Clase List: lista doblemente enlazada, dueña de sus nodos.
 * Reservá y liberá los nodos con new / delete (nunca malloc / free).
 * --------------------------------------------------------------- */

template<typename T>
List<T>::List() {
    // TODO: dejar la lista vacía (head, tail en nullptr y size en 0).
    this->head = nullptr;
    this->tail = nullptr;
    this->size = 0;
}

template<typename T>
List<T>::List(const List<T> &other) {
    // TODO: crear la nueva lista, como una copia independiente de other
    head = nullptr;
    tail = nullptr;
    size = 0;

    Node* actual = other.head; 
    while (actual != nullptr) {
        insert_tail(actual->value);
        actual = actual->next;
    }
}

template<typename T>
List<T> &List<T>::operator=(const List<T> &other) {
    // TODO: reemplazar la lista actual con una copia independiente de other
    // La memoria anterior no debe perderse, sino liberarse correctamente
    if (this == &other) {
        return *this;
    }

    while(head!=nullptr){
        Node* eliminar=head;
        head=head->next;
        delete eliminar;
    }
    tail=nullptr;
    size=0;
    //libero nodo por nodo asi no pierdo memoria y se libera correctamente
    

    Node* actual = other.head; 
    while (actual != nullptr) {
        insert_tail(actual->value);
        actual = actual->next;
    }
    //hago la copia 

    return *this;
}

template<typename T>
List<T>::~List() {
    // TODO: liberar todos los nodos que queden con delete.
    while(head!=nullptr){
        Node* eliminar=head;
        head=head->next;
        delete eliminar;
    }
    tail=nullptr;
}

template<typename T>
bool List<T>::is_empty() const {
    // TODO: devolver si la lista está vacía.
    return head==nullptr;
}

template<typename T>
size_t List<T>::length() const {
    // TODO: devolver la cantidad de elementos.
    return size;
}

template<typename T>
void List<T>::insert_head(const T& value) {
    // TODO: reservar un nodo con new, enlazarlo al principio y
    // actualizar head/tail/size.
    Node* nuevo= new Node(value);
    if (head!=nullptr){
        nuevo->next=head;
        head->prev=nuevo;
    }
    else{
        nuevo->next=nullptr;
        this->tail=nuevo;
    }
    nuevo->prev=nullptr;
    head=nuevo;
    size ++;
}

template<typename T>
void List<T>::insert_tail(const T& value) {
    // TODO: reservar un nodo con new, enlazarlo al final y
    // actualizar head/tail/size.

    Node* nuevo= new Node(value);
    if (head!=nullptr){
        nuevo->prev= tail;
        tail->next=nuevo;
    }
    else{
        nuevo->prev=nullptr;
        this->head=nuevo;
    }
    

    nuevo->next=nullptr;
    this-> tail= nuevo;
    this-> size ++;
}

template<typename T>
T List<T>::pop_head() {
    // TODO: sacar el primer nodo (con delete), devolver su valor.
    Node* viejo= head;
    T dato= head->value;
    if (head->next!=nullptr){
        head->next-> prev= nullptr;
        head=viejo->next;
    }
    else{
        head=nullptr;
        tail=nullptr;
    }
    delete viejo;
    size --;

    return dato;
}

template<typename T>
T List<T>::pop_tail() {
    // TODO: sacar el último nodo (con delete), devolver su valor.
    Node* viejo= tail;
    T dato= tail->value;
    if( tail->prev != nullptr){
        tail->prev->next=nullptr;
        tail= viejo->prev;
    }
    else{
        head=nullptr;
        tail=nullptr;
    }
    delete viejo;
    size --;

    return dato;
}

template<typename T>
const T& List<T>::peek_head() const {
    // TODO: devolver el valor del principio.
    return head->value;
}

template<typename T>
const T& List<T>::peek_tail() const {
    // TODO: devolver el valor del final.
    return tail->value;
}

template <typename T>
typename List<T>::ListIter List<T>::create_head() {
    // TODO: retornar un iterador parado en el principio de la lista.
    return ListIter(this,head);

}

template <typename T>
typename List<T>::ListIter List<T>::create_tail() {
    // TODO: retornar un iterador parado en el final de la lista.
    return ListIter(this,tail);
}

/* ---------------------------------------------------------------
 * Clase ListIter: iterador sobre una List.
 * Se obtienen mediante los métodos fábrica List::create_head / List::create_tail
 * --------------------------------------------------------------- */

// Constructor privado, utilizado por los métodos fábrica create_head y create_tail
template <typename T>
List<T>::ListIter::ListIter(List *list, List::Node *start) {
    this->list = list;
    this->curr = start;
}

template <typename T>
bool List<T>::ListIter::forward() {
    // TODO: avanzar una posición si se puede.
    if (curr!=nullptr && curr->next != nullptr){
        curr=curr->next;
        return true;
    }
    return false;
}

template <typename T>
bool List<T>::ListIter::backward() {
    // TODO: retroceder una posición si se puede.
    if(curr!= nullptr && curr->prev != nullptr){
        curr=curr->prev;
        return true;
    }
    return false;
}

template <typename T>
const T&  List<T>::ListIter::peek_current() const {
    // TODO: devolver el valor actual
    return curr->value;
}

template <typename T>
bool List<T>::ListIter::at_last() const {
    // TODO: devolver si el iterador está en el último elemento.
    if(list->tail==curr){
        return true;
    }
    return false;
}

template <typename T>
bool List<T>::ListIter::at_first() const {
    // TODO: devolver si el iterador está en el primer elemento.
    if (list->head==curr){
        return true;
    }
    return false;
}

template <typename T>
bool List<T>::ListIter::insert_after(const T&value) {
    // TODO: insertar un valor detrás del actual con new.
    Node* nuevo= new Node(value);
    if (curr == nullptr){
        nuevo->prev=nullptr;
        nuevo->next=nullptr;

        list->head= nuevo;
        list->tail=nuevo;
        curr=nuevo;
        
    }
    else if(at_last()){
        nuevo->prev=curr;
        curr->next=nuevo;

        nuevo->next=nullptr;
        list->tail=nuevo;
    }
    else{
        nuevo->next=curr->next;
        nuevo->prev=curr;

        curr->next->prev= nuevo;
        curr->next=nuevo;
    }
    list->size++;

    return true;
}

template <typename T>
bool List<T>::ListIter::insert_before(const T&value) {
    // TODO: insertar un valor delante del actual con new.
    Node* nuevo= new Node(value);
    if (curr == nullptr){
        nuevo->prev=nullptr;
        nuevo->next=nullptr;

        list->head= nuevo;
        list->tail=nuevo;
        curr=nuevo;
    }
    else if(at_first()){
        nuevo->next=curr;
        curr->prev=nuevo;

        nuevo->prev=nullptr;
        list->head= nuevo;
    }
    else{
        nuevo->prev=curr->prev;
        nuevo->next=curr;

        curr->prev->next=nuevo;
        curr->prev=nuevo;

    }
    list->size ++;
    return true;

}

template <typename T>
T List<T>::ListIter::remove() {
    // TODO: sacar el nodo actual (con delete), reposicionar el iterador
    // y devolver el valor que tenía.

    Node* viejo= curr;
    T dato=viejo->value;
    if(list->size== (size_t)1){
        curr=nullptr;
        list->head=nullptr;
        list->tail=nullptr;

    }
    else if(at_first()){
        curr->next->prev=nullptr;
        curr=curr->next;

        list->head=curr;
        
    }
    else if(at_last()){
        curr->prev->next=nullptr;
        curr=curr->prev;

        list->tail=curr;
    }
    else{
        curr->prev->next= curr->next;
        curr->next->prev=curr->prev;

        curr=curr->next;
       
        
    }
    delete viejo;
    list->size --;
    return dato;
}

#endif // TP2_H

