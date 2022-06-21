#ifndef BST_COURCE_BST_H

#include <vector>

using std::cout,
      std::vector;

struct Node {
    int data;
    Node *left;
    Node *right;
};

// Создание узла
Node *newNode(int item) {
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = item;
    temp->left = temp->right = nullptr;
    return temp;
}

// Симметричный Обход (Inorder Traversal)
void inorder(Node *root) {
    if (root != nullptr) {
        // Обход слева
        inorder(root->left);

        // Вывод информации из узла
        cout << root->data << " -> ";

        // Обход справа
        inorder(root->right);
    }
}

// Операция поиска
bool search(Node *root, int data) {
    bool result = true;
    if (root == nullptr) {
        return false;
    }

    if (data == root->data) {
        return true;
    }

    if (data < root->data) {
        result = search(root->left, data);
    } else if (data > root->data) {
        result = search(root->right, data);
    }
    return result;
}

// Операция вставки
Node *insert(Node *node, int data){
    if (node == nullptr) {
        return newNode(data);
    }

    if (data < node->data) {
        node->left = insert(node->left, data);
    } else {
        node->right = insert(node->right, data);
    }

    return node;
}

// Поиск следующего по порядку дочернего узла
Node *nextValueNode(Node *node) {
    Node *current = node;

    while (current && current->left != nullptr) {
        current = current->left;
    }

    return current;
}

// Операция удаления
Node *deleteNode(Node *root, int data) {
    if (root == nullptr) {
        return root;
    }

    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    } else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    } else {
        // Если узел с 0 или 1 дочерним узлом
        if (root->left == nullptr) {
            Node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == nullptr) {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        // Если у узла 2 дочерних узла
        Node *temp = nextValueNode(root->right);
        root->data = temp->data;
        // Удаляем "порядковый" узел из исходного положения
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

Node *clearAll(Node *root, vector<int> &arr) {
    for (int i : arr){
        root = deleteNode(root, i);
    }
    arr.clear();
    return root;
}

#endif //BST_COURCE_BST_H
