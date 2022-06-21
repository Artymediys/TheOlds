#ifndef BINARYTREE_BINTREE_H

class IntBinTree{
private:
    struct Node{
        int Data;
        Node* LeftBranch;
        Node* RightBranch;

        Node(int data, Node* lb, Node* rb){
            this->Data = data;
            this->LeftBranch = lb;
            this->RightBranch = rb;
        }

        ~Node(){
            delete LeftBranch;
            delete RightBranch;
        }
    };
    unsigned int Size;
    Node* root;

    void show(IntBinTree::Node *node) {
        if (node->LeftBranch == nullptr &&
            node->RightBranch == nullptr){
            std::cout << node->Data << "\n";
        } else if (node->LeftBranch != nullptr &&
                   node->RightBranch != nullptr){
            show(node->LeftBranch);
            show(node->RightBranch);
            std::cout << node->Data << "\n";
        } else if (node->RightBranch == nullptr){
            show(node->LeftBranch);
            std::cout << node->Data << "\n";
        } else if (node->LeftBranch == nullptr){
            show(node->RightBranch);
            std::cout << node->Data << "\n";
        }
    }
public:
    IntBinTree();
    ~IntBinTree();

    bool operator[](int data);
    unsigned int GetSize() {return Size;}

    void add(int data);
    void showAll();
};

IntBinTree::IntBinTree() {
    this->Size = 0;
    this->root = nullptr;
}

IntBinTree::~IntBinTree() {
    delete this->root;
}

bool IntBinTree::operator[](int data) {
    if (this->Size == 0){
        std::cerr << "Empty array!\n";
        return false;
    } else {
        Node* tmp = this->root;
        bool flag = true;
        while (flag){
            if (tmp != nullptr && data == tmp->Data){
                return true;
            } else if (tmp != nullptr && data < tmp->Data){
                tmp = tmp->LeftBranch;
            } else if (tmp != nullptr && data > tmp->Data){
                tmp = tmp->RightBranch;
            } else {
                flag = false;
            }
        }
        return flag;
    }
}

void IntBinTree::add(int data) {
    if (this->root == nullptr){
        this->root = new Node(data, nullptr, nullptr);
        std::cout << "Data added!\n";
        this->Size++;
        return;
    }
    Node* tmp = this->root;
    while (true){
        if (data == tmp->Data){
            std::cout << "Such data already exists!\n";
            return;
        } else if (data < tmp->Data){
            if (tmp->LeftBranch == nullptr){
                tmp->LeftBranch = new Node(data, nullptr, nullptr);
                this->Size++;
                std::cout << "Data added!\n";
                return;
            } else {
                tmp = tmp->LeftBranch;
            }
        } else if (data > tmp->Data){
            if (tmp->RightBranch == nullptr){
                tmp->RightBranch = new Node(data, nullptr, nullptr);
                this->Size++;
                std::cout << "Data added!\n";
                return;
            } else {
                tmp = tmp->RightBranch;
            }
        }
    }
}

void IntBinTree::showAll() {
    if (this->root != nullptr) {
        this->show(this->root);
    } else {
        std::cerr << "Tree is empty!\n";
    }
}


#endif //BINARYTREE_BINTREE_H
