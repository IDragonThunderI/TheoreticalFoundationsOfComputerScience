#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

const int num_ID = 400;
int counter1 = 0, counter2 = 0;

class BinTree { // Класс "Бинарное дерево"
private:
    struct Node { // Структура "Узел дерева"
        string data; // Поле данных
        Node* left = NULL; // Левый узел
        Node* right = NULL; // Правый узел
        Node* parent = NULL; // Родительский узел
    };
    void RAA_Add(string id, Node* current, Node* parent); // Рекурсивный метод добавления
    void RAA_RemoveSubtree(Node* subRoot); // Удаление поддерева

public:
    Node* Root; // Корень дерева
    BinTree() : Root(NULL) {}; // Конструктор
    ~BinTree(); // Деструктор
    void RAA_AddNode(string id); // Добавление нового узла
    void RAA_FillFile(Node* node, vector<string> &buf); // Заполнение файла (от меньшего к большему)
};

struct SortedList { // Структура "Упорядоченный список"
    string data; // Поле данных
    SortedList* next = NULL; // Следующий элемент 
    void RAA_insert(SortedList** begin); // Вставка элемента в порядке возрастания
    void RAA_RemoveCell(SortedList* Cell); // Удаление ячеек списка
};

char RAA_FirstSymbol(); // Выбор первого символа ID
char RAA_OtherSymbol(); // Выбор остальных символов ID
int RAA_Hash(string id); // Хэш-функция "djb2"
void RAA_Method1(); // Заполнение таблицы ID методом 1
void RAA_Method2(); // Заполнение таблицы ID методом 2

int main() {
    setlocale(LC_ALL, "");
    ofstream ID;
    ID.open("ID.txt");

    srand(time(NULL));
    for (int i = 0; i < num_ID; i++) {
        int n = rand() % 6 + 3;
        char* id = new char[n + 1];

        for (int j = 0; j < n; j++) {
            if (j == 0) {
                id[j] = RAA_FirstSymbol();
            } else {
                id[j] = RAA_OtherSymbol();
            }
        }
        
        id[n] = '\0';
        ID << id << endl;
        delete[] id;
    }

    ID.close();

    RAA_Method1();
    wcout << L"Количество сравнений при заполнении таблицы ID методом бинарного дерева: " << counter1 << endl;
    
    RAA_Method2();
    wcout << L"Количество сравнений при заполнении таблицы ID методом хэш-функции с упорядоченным списком: " << counter2 << endl;

    return 0;
}

BinTree::~BinTree() {
    RAA_RemoveSubtree(Root);
}

void BinTree::RAA_RemoveSubtree(Node* subRoot) {
    if (subRoot != NULL) {
        RAA_RemoveSubtree(subRoot->left); // Удаляем левую часть дерева.
        RAA_RemoveSubtree(subRoot->right); // Удаляем правую часть дерева.

        delete subRoot;
    }
}

void BinTree::RAA_AddNode(string id) {
    if (Root == NULL) {
        Root = new Node; // Выделяем пямять под корень дерева
        Root->data = id; // Заполняем корень первым идентификатором
        Root->left = NULL;
        Root->right = NULL;
        Root->parent = NULL;
    } else {
        counter1++;
        if (id < Root->data) {
            RAA_Add(id, Root->left, Root);
        } else if (id > Root->data) {
            RAA_Add(id, Root->right, Root);
        }
    }
}

void BinTree::RAA_Add(string id, Node* current, Node* parent) {
    if (current == NULL) {
        Node* temp = new Node; // Выделяем память под узел
        temp->data = id; // Заполняем узел идентификатором
        temp->left = NULL;
        temp->right = NULL;
        temp->parent = parent; // Связываем узел с родительским

        if (temp->data < parent->data) {
            parent->left = temp;
        } else if (temp->data > parent->data) {
            parent->right = temp;
        }
    } else if (id < current->data) { // Условие добавления левого потомка
        counter1++;
        RAA_Add(id, current->left, current);
    } else if (id > current->data) { // Условие добавления правого потомка
        counter1++;
        RAA_Add(id, current->right, current);
    }
}

void BinTree::RAA_FillFile(Node* node, vector<string> &buf) {
    if (node == NULL) {
        return;
    } else {
        RAA_FillFile(node->left, buf);
        buf.push_back(node->data);
        RAA_FillFile(node->right, buf);
    }
}

void SortedList::RAA_insert(SortedList** begin) {
    SortedList* front = *begin; // Указатель на первый элемент списка

    counter2++;
    if (this->data < front->data) {
        this->next = front;
        *begin = this;
        return;
    }

    SortedList* back = front->next; // Указатель на элемент списка после вставляемого

    while (back) {
        counter2++;
        if (front->data < this->data && this->data < back->data) {
            front->next = this;
            this->next = back;
            return;
        }

        front = back;
        back = back->next;
    }

    counter2++;
    if (front->data < this->data) {
        front->next = this;
        this->next = NULL;
    }
}

void SortedList::RAA_RemoveCell(SortedList* Cell) {
    if (Cell != NULL) {
        RAA_RemoveCell(Cell->next);
        delete Cell;
    }
}

char RAA_FirstSymbol() {
    int num = rand() % 52 + 1;
    if (num <= 26) {
        return (64 + num); // Латинские буквы верхнего регистра
    } else {
        return (70 + num); // Латинские буквы нижнего регистра
    }
}

char RAA_OtherSymbol() {
    int num = rand() % 63 + 1;
    if (num <= 26) {
        return (64 + num); // Латинские буквы верхнего регистра
    } else if (num > 26 && num <= 52) {
        return (70 + num); // Латинские буквы нижнего регистра
    } else if (num > 52 && num <= 62) {
        return (num - 5); // Цифры 0-9
    } else {
        return (95); // Символ "_"
    }
}

int RAA_Hash(string id) {
    unsigned long long hash = 5381;
    for (int i = 0; i < id.size(); i++) {
        hash = hash * 33 + id[i];
    }
    return (hash % 1001);
}

void RAA_Method1() {
    ifstream ID;
    ofstream M_ID1;
    M_ID1.open("M_ID1.txt");
    ID.open("ID.txt");

    BinTree Tree;
    
    for (int i = 0; i < num_ID; i++) {
        string id;
        ID >> id;
        Tree.RAA_AddNode(id);
    }

    vector<string> buf;
    Tree.RAA_FillFile(Tree.Root, buf);

    for (int i = 0; i < buf.size(); i++) {
        M_ID1 << buf[i] << endl;
    }

    ID.close();
    M_ID1.close();
}

void RAA_Method2() {
    ifstream ID;
    ofstream M_ID2;
    M_ID2.open("M_ID2.txt");
    ID.open("ID.txt");

    vector<SortedList*> TableID(1001);
    
    for (int i = 0; i < num_ID; i++) {
        string id;
        ID >> id;
        int hash = RAA_Hash(id);

        SortedList* buf = new SortedList;
        buf->data = id;
        buf->next = NULL;

        if (TableID[hash] == NULL) {
            TableID[hash] = buf;
        } else {
            buf->RAA_insert(&TableID.at(hash));
        }
    }

    for (int i = 0; i < TableID.size(); i++) {
        if (TableID[i] != NULL) {
            M_ID2 << i << " " << TableID[i]->data;
            while (TableID[i]->next != NULL) {
                TableID[i] = TableID[i]->next;
                M_ID2 << " | " << TableID[i]->data;
            }
        } else {
            M_ID2 << i;
        }
        M_ID2 << endl;
    }

    for (int i = 0; i < TableID.size(); i++) {
        TableID[i]->RAA_RemoveCell(TableID[i]);
    }

    ID.close();
    M_ID2.close();
}
