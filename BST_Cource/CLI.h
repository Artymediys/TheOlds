#ifndef BST_COURCE_CLI_H

#include "BST.h"
#include <string>

using std::cin,
      std::cerr,
      std::string;

void greetings() {
    cout << "Курсовая работа по теме \"Древовидная структура данных. Бинарное дерево поиска.\"\n"
         << "Выполнил: Плетнёв Артём Александрович\n"
         << "Задание: Написать реализацию бинарного дерева поиска.\n\n";
}

void program() {
    char ans = 'y';
    cout << "Хотите создать бинарное дерево поиска?\n"
            "Да - 'y'; Нет - 'n'\n";
    cin >> ans;
    if (cin.fail() || ((ans != 'y') && (ans != 'n'))) {
        cerr << "Неверный формат ввода!\n\n";
        program();
        return;
    } else if (ans == 'y') {
        Node *tree = nullptr;
        vector<int> arr;
        cout << "Бинарное дерево поиска успешно создано!\n\n";

        bool flag = true;
        unsigned short int act = 42;
        while (flag) {
            cout << "Меню выбора действий\n"
                    "1. Добавить элемент в дерево\n"
                    "2. Проверить элемент на наличие в дереве\n"
                    "3. Произвести симметричный обход дерева\n"
                    "4. Удалить элемент из дерева\n"
                    "5. Очистить всё дерево\n\n"
                    "0. Выйти из программы\n\n";
            cout << "Выберите действие: ";
            cin >> act;
            if (cin.fail() || act > 5) {
                cerr << "Неверный формат ввода!\n\n";
                continue;
            } else {
                int num = 0;
                switch (act) {
                    case 1:
                        cout << "Введите число, котортое хотите добавить в дерево: \n";
                        cin >> num;
                        if (cin.fail()) {
                            cerr << "Неверный формат ввода!\n\n";
                        } else if (search(tree, num)) {
                            cerr << "Число '" << num << "' уже существует в дереве!\n\n";
                        } else {
                            tree = insert(tree, num);
                            arr.push_back(num);
                            cout << "Число '" << num << "' успешно добавлено в дерево!\n\n";
                        }
                        break;
                    case 2:
                        cout << "Введите число, котортое хотите проверить: \n";
                        cin >> num;
                        if (cin.fail()){
                            cerr << "Неверный формат ввода!\n\n";
                        } else if (search(tree, num)) {
                            cout << "Число '" << num << "' найдено в дереве!\n\n";
                        } else {
                            cout << "Числа '" << num << "' не существует в дереве!\n\n";
                        }
                        break;
                    case 3:
                        cout << "Симметричный обход дерева: ";
                        inorder(tree);
                        cout << "\n\n";
                        break;
                    case 4:
                        cout << "Введите число, котортое хотите удалить из дерева: \n";
                        cin >> num;
                        if (cin.fail()) {
                            cerr << "Неверный формат ввода!\n\n";
                        } else if (search(tree, num)) {
                            cout << "Вы действительно хотите удалить число '" << num << "'?\n"
                                    "Да - y; Нет - n\n";
                            cin >> ans;
                            if (cin.fail() || ((ans != 'y') && (ans != 'n'))) {
                                cerr << "Неверный формат ввода!\n\n";
                            } else if (ans == 'y') {
                                tree = deleteNode(tree, num);
                                cout << "Число '" << num << "' успешно удалено из дерева!\n\n";
                            } else if (ans == 'n') {
                                cout << "Число '" << num << "' не было удалено из дерева!\n\n";
                            }
                        } else {
                            cerr << "Числа '" << num << "' не существует в дереве!\n\n";
                        }
                        break;
                    case 5:
                        cout << "Вы действительно хотите очистить дерево?\n"
                                "Да - y; Нет - n\n";
                        cin >> ans;
                        if (cin.fail() || ((ans != 'y') && (ans != 'n'))) {
                            cerr << "Неверный формат ввода!\n\n";
                        } else if (ans == 'y') {
                            tree = clearAll(tree, arr);
                            cout << "Дерево полностью очищено!\n\n";
                        } else if (ans == 'n') {
                            cout << "Дерево не было очишено!\n\n";
                        }
                        break;
                    case 0:
                        cout << "Вы действительно хотите завершить программу?\n"
                                "Да - y; Нет - n\n";
                        cin >> ans;
                        if (cin.fail() || ((ans != 'y') && (ans != 'n'))) {
                            cerr << "Неверный формат ввода!\n\n";
                        } else if (ans == 'y') {
                            tree = clearAll(tree, arr);
                            cout << "Завершение программы...\n";
                            flag = false;
                        } else if (ans == 'n') {
                            cout << "Тогда работаем дальше!\n\n";
                        }
                        break;
                    default:
                        cout << "Сюда невозможно попасть, но если это случилось, "
                                "то отдельное поздравление от меня!\n";
                        flag = false;
                        break;
                }
            }
        }
    } else if (ans == 'n') {
        cout << "Больше мне нечего Вам предложить. Завершение программы...\n";
    }
}

#endif //BST_COURCE_CLI_H
