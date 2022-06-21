#ifndef DCL_COURCE_DCLIST_H
#include <iostream>

using std::cin,
      std::cout,
      std::cerr;

template<typename T>
class DCList{
private:
    template<typename TN>
    struct Node{
        Node *pNext;
        Node *pPrev;
        TN data;

        explicit Node(TN data = TN(), Node *pNext = nullptr, Node *pPrev = nullptr){
            this->data = data;
            this->pNext = pNext;
            this->pPrev = pPrev;
        }
    };
    unsigned int Size;
    Node<T> *head;
    Node<T> *tail;

public:
    DCList();
    ~DCList();

    T &operator[](unsigned int index);
    int GetSize() {return Size;}
    void cOut();
    void rangeOut();

    void pop_front();
    void pop_back();
    void push_front(T data);
    void push_back(T data);
    void insert(T data, unsigned int index);
    void removeAt(unsigned int index);
    void clear();
};


template<typename T>
DCList<T>::DCList(){
    Size = 0;
    head = nullptr;
    tail = nullptr;
}

template<typename T>
DCList<T>::~DCList(){
    clear();
}

template<typename T>
T &DCList<T>::operator[](const unsigned int index) {
    if ((Size == 0) || (index > Size - 1)){
        cerr << "Incorrect index!";
    } else {
        if (index <= Size / 2) {
            unsigned int counter = 0;
            Node<T> *current = this->head;
            do {
                if (counter == index) {
                    return current->data;
                }
                current = current->pNext;
                counter++;
            } while ((current != nullptr) || (current == this->head));
        } else if (index > Size / 2){
            unsigned int counter = Size - 1;
            Node<T> *current = this->tail;
            do {
                if (counter == index) {
                    return current->data;
                }
                current = current->pPrev;
                counter--;
            } while ((current != nullptr) || (current == this->tail));
        }
    }
}

template<typename T>
void DCList<T>::cOut() {
    for (unsigned int i = 0; i < this->GetSize(); i++){
        cout << "ID - " << i << ": " << this->operator[](i) << "\n";
    }
}

template<typename T>
void DCList<T>::rangeOut() {
    unsigned int i = 0, j = 0;
    cout << "Введите нижнюю границу диапазона 'i': ";
    cin >> i;
    if ((cin.fail()) || (i >= this->GetSize())){
        cerr << "Incorrect input of data!\n";
        return;
    }
    cout << "Введите верхнюю границу диапазона 'j': ";
    cin >> j;
    if ((cin.fail()) || (j >= this->GetSize())){
        cerr << "Incorrect input of data!\n";
        return;
    }
    cout << "Элементы в заданном диапазоне:\n";
    for (; i <= j; i++){
        cout << "ID - " << i << ": " << this->operator[](i) << "\n";
    }
}

template<typename T>
void DCList<T>::pop_front(){
    if (Size > 1 ) {
        Node<T> *toDelete = head;
        head = head->pNext;
        head->pPrev = tail;
        delete toDelete;
    } else if (Size == 1){
        Node<T> *toDelete = head;
        head = tail = nullptr;
        delete toDelete;
    } else if (Size == 0){
        cerr << "List is empty!";
        return;
    }
    Size--;
}

template<typename T>
void DCList<T>::pop_back(){
    if (Size > 1) {
        Node<T> *toDelete = tail;
        tail = tail->pPrev;
        tail->pNext = head;
        delete toDelete;
    } else if (Size == 1){
        Node<T> *toDelete = tail;
        head = tail = nullptr;
        delete toDelete;
    } else if (Size == 0){
        cerr << "List is empty!";
        return;
    }
    Size--;
}

template<typename T>
void DCList<T>::push_front(T data){
    if (Size == 0) {
        head = tail = new Node<T>(data);
    } else if (Size == 1){
        head = new Node<T>(data, head, head);
        tail->pPrev = tail->pNext = head;
    } else if (Size > 1){
        Node<T> *temp = head;
        head = new Node<T>(data, head, tail);
        temp->pPrev = head;
    }
    Size++;
}

template<typename T>
void DCList<T>::push_back(T data){
    if (Size == 0){
        head = tail = new Node<T>(data);
    } else if (Size == 1){
        tail = new Node<T>(data, head, head);
        head->pNext = head->pPrev = tail;
    } else if (Size > 1){
        Node<T> *temp = tail;
        tail = new Node<T>(data, head, tail);
        temp->pNext = tail;
    }
    Size++;
}

template<typename T>
void DCList<T>::insert(T data, unsigned int index){
    if (index <= Size / 2){
        if (index == 0){
            push_front(data);
        } else {
            Node<T> *previous = this->head;
            for (int i = 0; i < index - 1; i++) {
                previous = previous->pNext;
            }
            Node<T> *temp = previous->pNext;
            temp->pPrev = previous->pNext = new Node<T>(data, previous->pNext, previous);
            Size++;
        }
    } else if (index > Size / 2){
        if (index > Size){
            cerr << "Incorrect input!\n";
            return;
        } else if (index == Size - 1){
            Node<T> *temp = tail->pPrev;
            tail->pPrev = temp->pNext = new Node<T>(data, tail, tail->pPrev);
            Size++;
        } else if (index == Size){
            push_back(data);
        } else {
            Node<T> *next = this->tail;
            for (int i = Size - 1; index < i; i--) {
                next = next->pPrev;
            }
            Node<T> *temp = next->pPrev;
            temp->pNext = next->pPrev = new Node<T>(data, next, next->pPrev);
            Size++;
        }
    }
}

template<typename T>
void DCList<T>::removeAt(unsigned int index) {
    if (index == 0) {
        pop_front();
    } else if (index <= Size / 2) {
        Node<T> *previous = this->head;
        for (int i = 0; i < index - 1; i++) {
            previous = previous->pNext;
        }
        Node<T> *toDelete = previous->pNext;
        previous->pNext = toDelete->pNext;
        Node<T> *temp = toDelete->pNext;
        temp->pPrev = previous;
        delete toDelete;
        Size--;
    } else if (index > Size / 2) {
        if (index >= Size){
            cerr << "Incorrect input!\n";
            return;
        } else if (index == Size - 1) {
            pop_back();
        } else {
            Node<T> *next = this->tail;
            for (int i = Size - 2; index < i; i--) {
                next = next->pPrev;
            }
            Node<T> *toDelete = next->pPrev;
            next->pPrev = toDelete->pPrev;
            Node<T> *previous = toDelete->pPrev;
            previous->pNext = next;
            delete toDelete;
            Size--;
        }
    }
}

template<typename T>
void DCList<T>::clear(){
    while (Size){
        pop_front();
    }
}

#endif //DCL_COURCE_DCLIST_H
