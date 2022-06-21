#ifndef DCL_COURCE_CLI_H
#include "DCList.h"
#include <string>

using std::string;

void greetings(){
    cout << "Курсовая работа по теме \"Двунаправленный (симметричный) циклический линейный список\"\n"
         << "Выполнил: Плетнёв Артём Александрович, 15.11Д-МО12/19б\n"
         << "Задание: Написать реализацию двусвязного циклического списка +\n         дополнительно "
            "написать метод, который выводит в консоль элементы в заданном диапазоне.\n\n";
}

template <typename T>
void action(DCList<T> &list, T &data, string &t){
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n";
    cout << "Пустой список типа \"" << t << "\" успешно создан!\n\n";

    unsigned short int act = 42;
    bool end = false;
    while (!end) {
        cout << "Меню выбора действий\n"
                "1.  Добавить элемент в начало списка\n"
                "2.  Добавить элемент в конец списка\n"
                "3.  Вставить элемент в список(по индексу)\n"
                "4.  Удалить элемент из начала списка\n"
                "5.  Удалить элемент из конца списка\n"
                "6.  Удалить элемент из списка(по индексу)\n"
                "7.  Узнать размер списка\n"
                "8.  Вывести весь список\n"
                "9.  Вывести элементы списка в определённом диапазоне(по индексам)\n"
                "10. Удалить все элементы списка\n\n"
                "0.  Завершение программы\n\n";
        cout << "Выберите действие: ";
        cin >> act;
        if ((cin.fail()) || (act > 10)) {
            cerr << "Incorrect input of data!\n\n";
            continue;
        } else {
            unsigned int index = 0;
            bool ans = true;
            switch (act) {
                case 1:
                    cout << "Введите данные, которые хотите добавить:\n";
                    cin >> data;
                    if (cin.fail()){
                        cerr << "Incorrect input of data!\n";
                        break;
                    }
                    list.push_front(data);
                    cout << "Элемент добавлен!\n\n";
                    break;
                case 2:
                    cout << "Введите данные, которые хотите добавить:\n";
                    cin >> data;
                    if (cin.fail()){
                        cerr << "Incorrect input of data!\n";
                        break;
                    }
                    list.push_back(data);
                    cout << "Элемент добавлен!\n\n";
                    break;
                case 3:
                    index = 0;
                    cout << "Введите данные, которые хотите добавить, "
                            "а также индекс:\n";
                    cout << "Data: ";
                    cin >> data;
                    if (cin.fail()){
                        cerr << "Incorrect input of data!\n";
                        break;
                    }
                    cout << "Index: ";
                    cin >> index;
                    if (cin.fail()){
                        cerr << "Incorrect input of data!\n";
                        break;
                    }
                    list.insert(data, index);
                    cout << "Элемент вставлен!\n\n";
                    break;
                case 4:
                    ans = true;
                    cout << "Вы точно хотите удалить элемент?\n"
                            "1. Да\n0. Нет\n";
                    cin >> ans;
                    if (cin.fail()) {
                        cerr << "Incorrect input of data!\n";
                        break;
                    } else if (ans == 1) {
                        list.pop_front();
                        cout << "Элемент удалён!\n\n";
                    } else if (ans == 0) {
                        break;
                    }
                    break;
                case 5:
                    ans = true;
                    cout << "Вы точно хотите удалить элемент?\n"
                            "1. Да\n0. Нет\n";
                    cin >> ans;
                    if (cin.fail()) {
                        cerr << "Incorrect input of data!\n";
                        break;
                    } else if (ans == 1) {
                        list.pop_back();
                        cout << "Элемент удалён!\n\n";
                    } else if (ans == 0) {
                        break;
                    }
                    break;
                case 6:
                    ans = true;
                    index = 0;
                    cout << "Вы точно хотите удалить какой-нибудь элемент?\n"
                            "1. Да\n0. Нет\n";
                    cin >> ans;
                    if (cin.fail()) {
                        cerr << "Incorrect input of data!\n";
                        break;
                    } else if (ans == 1) {
                        cout << "Введите индекс:\n";
                        cin >> index;
                        if (cin.fail()){
                            cerr << "Incorrect input of data!\n";
                            break;
                        }
                        list.removeAt(index);
                        cout << "Элемент удалён!\n\n";
                    } else if (ans == 0) {
                        break;
                    }
                    break;
                case 7:
                    cout << "Размер списка: " << list.GetSize() << "\n";
                    break;
                case 8:
                    cout << "Список:\n";
                    list.cOut();
                    cout << "\n";
                    break;
                case 9:
                    list.rangeOut();
                    cout << "\n";
                    break;
                case 10:
                    ans = true;
                    cout << "Вы точно хотите удалить все элементы?\n"
                            "1. Да\n0. Нет\n";
                    cin >> ans;
                    if (cin.fail()) {
                        cerr << "Incorrect input of data!\n";
                        break;
                    } else if (ans == 1) {
                        list.clear();
                        cout << "Список очищен!\n\n";
                    } else if (ans == 0) {
                        break;
                    }
                    break;
                case 0:
                    ans = true;
                    cout << "Вы точно хотите завершить программу?\n"
                            "1. Да\n0. Нет\n";
                    cin >> ans;
                    if (cin.fail()) {
                        cerr << "Incorrect input of data!\n";
                        break;
                    } else if (ans == 1) {
                        cout << "Завершение программы...\n";
                        end = true;
                        return;
                    } else if (ans == 0) {
                        break;
                    }
                default:
                    cout << "Что-то пошло не так...\n\n";
                    break;
            }
        }
    }
}

void program() {
    char ans = 'y';
    cout << "Хотите создать список?\n"
            "Да - 'y'; Нет - 'n'\n";
    cin >> ans;
    if ((cin.fail()) || ((ans != 'y') && (ans != 'n'))) {
        cerr << "Incorrect input of data!\n";
        return;
    } else if (ans == 'y') {
        string t;
        cout << "Данная версия программы рассчитана лишь на демонстрацию работы "
                "двусвязного циклического списка.\n"
                "Для демонстрации разрешено использовать только \"примитивные\" типы данных,\n"
                "использование пользовательских типов данных доступно только для разработчиков!\n\n";
        cout << "Укажите какого типа данных создать список: {bool, char, int, float, double}:\n";
        cin >> t;
        if ((t != "bool") && (t != "char") && (t != "int") &&
            (t != "float") && (t != "double")) {
            cerr << "Incorrect input of data!\n";
            return;
        } else if (t == "bool") {
            bool data = true;
            DCList<bool> list;
            action(list, data, t);
        } else if (t == "char") {
            char data = 'a';
            DCList<char> list;
            action(list, data, t);
        } else if (t == "int") {
            int data = 0;
            DCList<int> list;
            action(list, data, t);
        } else if (t == "float") {
            float data = 1.0;
            DCList<float> list;
            action(list, data, t);
        } else if (t == "double") {
            double data = 1.0;
            DCList<double> list;
            action(list, data, t);
        }
    } else if (ans == 'n') {
        cout << "Завершение программы...\n";
        return;
    }
}

#endif //DCL_COURCE_CLI_H
