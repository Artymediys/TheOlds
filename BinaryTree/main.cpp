#include <iostream>
#include "BinTree.h"

using std::cout;

int main() {
    IntBinTree tree;
    cout << "Size of Tree: " << tree.GetSize() << "\n";
    tree.add(50);
    tree.add(55);
    tree.add(42);
    tree.add(45);
    tree.add(40);
    cout << "Size of Tree: " << tree.GetSize() << "\n";
    if (tree[45]){
        cout << "45 is exist!\n";
    } else {
        cout << "45 is NOT exist.\n";
    }
    if (tree[20]){
        cout << "20 is exist!\n";
    } else {
        cout << "20 is NOT exist.\n";
    }
    cout << "\n";
    tree.showAll();
    return 0;
}