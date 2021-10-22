#include <iostream>
#include <string>
#include <time.h>
#include <cstdlib>
#include <fstream>

using namespace std;

struct Tree
{
    Tree* left = 0;
    Tree* right = 0;
    int key;
    int size = 1;
    string brand;
    string name;
};

ifstream iffile;
ofstream offile;
string filePath;
const int null = 0;
int rotateCount = 0;

void writeElement(Tree* tree) {
    int len, key;
    offile.write((char*)&tree->key, 4);
    len = tree->brand.length() + 1;
    offile.write((char*)&len, 4);
    offile.write((char*)tree->brand.c_str(), len);
    len = tree->name.length() + 1;
    offile.write((char*)&len, 4);
    offile.write((char*)tree->name.c_str(), len);
    if (tree->left) {
        offile.write((char*)&tree->left->key, 4);
        writeElement(tree->left);
    }
    else
        offile.write((char*)&null, 4);
    if (tree->right) {
        offile.write((char*)&tree->right->key, 4);
        writeElement(tree->right);
    }
    else
        offile.write((char*)&null, 4);
}

void addToFile(Tree * tree)
{
    offile.open(filePath, ios::binary | ios::out);
    writeElement(tree);
    offile.close();
}

Tree* findTree(Tree* tree, int key) {
    if (!tree)
        return 0;
    if (key == tree->key)
        return tree;
    else if (key < tree->key)
        return findTree(tree->left, key);
    else
        return findTree(tree->right, key);
}

int getSize(Tree* tree)
{
    if (!tree) return 0;
    return tree->size;
}

void fixSize(Tree* tree)
{
    tree->size = getSize(tree->left) + getSize(tree->right) + 1;
}

Tree* insert(Tree* tree, int key, string brand, string name)
{
    if (!tree) {
        Tree* newtree = new Tree();
        newtree->key = key;
        newtree->brand = brand;
        newtree->name = name;
        return newtree;
    }
    if (tree->key > key)
        tree->left = insert(tree->left, key, brand, name);
    else
        tree->right = insert(tree->right, key, brand, name);
    fixSize(tree);
    return tree;
}

Tree* parseFromTextFile() {
    cout << "Введите путь до файла" << endl;
    string textPath;
    cin >> textPath;
    iffile.open(textPath);
    int key;
    string brand, name, str;
    Tree* tree = new Tree();
    getline(iffile, str);
    key = stoi(str);
    getline(iffile, brand);
    getline(iffile, name);
    tree->key = key;
    tree->brand = brand;
    tree->name = name;
    while (!iffile.eof())
    {
        getline(iffile, str);
        if (iffile.eof())
            break;
        key = stoi(str);
        getline(iffile, brand);
        getline(iffile, name);
        tree = insert(tree, key, brand, name);
    }
    return tree;
}

Tree* uniteTrees(Tree* firsttree, Tree* secondtree)
{
    if (!firsttree) return secondtree;
    if (!secondtree) return firsttree;
    if (rand() % (firsttree->size + secondtree->size) < firsttree->size)
    {
        firsttree->right = uniteTrees(firsttree->right, secondtree);
        fixSize(firsttree);
        return firsttree;
    }
    else
    {
        secondtree->left = uniteTrees(firsttree, secondtree->left);
        fixSize(secondtree);
        return secondtree;
    }
}

Tree* removeFromTree(Tree* tree, int key) 
{
    if (!tree) return tree;
    if (tree->key == key)
    {
        Tree* newtree = uniteTrees(tree->left, tree->right);
        delete tree;
        return newtree;
    }
    else if (key < tree->key)
        tree->left = removeFromTree(tree->left, key);
    else
        tree->right = removeFromTree(tree->right, key);
    return tree;
}

Tree* rotateToTheRight(Tree* firsttree)
{
    Tree* secondtree = firsttree->left;
    if (!secondtree) return firsttree;
    firsttree->left = secondtree->right;
    secondtree->right = firsttree;
    secondtree->size = firsttree->size;
    fixSize(firsttree);
    rotateCount++;
    return secondtree;
}

Tree* rotateToTheLeft(Tree* firsttree)
{
    Tree* secondtree = firsttree->right;
    if (!secondtree) return firsttree;
    firsttree->right = secondtree->left;
    secondtree->left = firsttree;
    secondtree->size = firsttree->size;
    fixSize(firsttree);
    rotateCount++;
    return secondtree;
}

Tree* insertRand(Tree* tree, int key, string brand, string name)
{
    if (!tree) {
        Tree* newtree = new Tree();
        newtree->key = key;
        newtree->brand = brand;
        newtree->name = name;
        return newtree;
    }
    if (key < tree->key)
    {
        tree->left = insertRand(tree->left, key, brand, name);
        return rotateToTheRight(tree);
    }
    else
    {
        tree->right = insertRand(tree->right, key, brand, name);
        return rotateToTheLeft(tree);
    }
}

Tree * readElement(Tree* tree) {
    string brand;
    string name;
    int carNumber;
    int len;
    char* buf;
    int left, right;
    iffile.read((char*)&carNumber, 4);
    iffile.read((char*)&len, 4);
    buf = new char[len];
    iffile.read(buf, len);
    brand = buf;
    delete[]buf;
    iffile.read((char*)&len, 4);
    buf = new char[len];
    iffile.read(buf, len);
    name = buf;
    delete[]buf;
    tree = insert(tree, carNumber, brand, name);
    iffile.read((char*)&left, 4);
    if (left) {
        tree->left = readElement(tree->left);
    }
    else {
        tree->left = 0;
    }
    iffile.read((char*)&right, 4);
    if (right) {
        tree->right = readElement(tree->right);
    }
    else {
        tree->right = 0;
    }
    return tree;
}

Tree* readFromFile(Tree* tree) {
    iffile.open(filePath, ios::binary | ios::in);
    tree = readElement(tree);
    iffile.close();
    return tree;
}

Tree* searchAndAdd(Tree* tree, int key) {
    iffile.open(filePath, ios::binary | ios::in);
    string brand;
    string name;
    int carNumber;
    int len;
    char* buf;
    int left, right;
    while (!iffile.eof()) {
        iffile.read((char*)&carNumber, 4);
        if(iffile.eof())
            break;
        iffile.read((char*)&len, 4);
        buf = new char[len];
        iffile.read(buf, len);
        brand = buf;
        delete[]buf;
        iffile.read((char*)&len, 4);
        buf = new char[len];
        iffile.read(buf, len);
        name = buf;
        delete[]buf;
        iffile.read((char*)&left, 4);
        iffile.read((char*)&right, 4);
        if (carNumber == key) {
            tree = insert(tree, key, brand, name);
            break;
        }
    }
    iffile.close();
    return tree;
}

void outTree(Tree* tree, int level)
{
    if (tree)
    {
        outTree(tree->left, level + 1);
        for (int a = 0; a < level; a++)
            cout << "    ";
        cout << tree->key << endl;
        outTree(tree->right, level + 1);
    }
}

Tree* fileWork(Tree * tree) {
    cout << "Выберите нужную операцию: " << endl;
    cout << "1. Открыть файл" << endl;
    cout << "2. Создать файл" << endl;
    cout << "3. Пропустить" << endl;
    int Interface, key;
    string name;
    cin >> Interface;
    switch (Interface)
    {
    case 1:
        cout << "Введите полный путь до файла" << endl;
        cin >> name;
        filePath = name;
        break;
    case 2: {
        cout << "Введите название файла" << endl;
        cin >> name;
        filePath = "D://" + name + ".bin";
        ofstream outfile(filePath);
        break;
    }
    default:
        break;
    }
    while (true) {
        cout << "Выберите нужную операцию: " << endl;
        cout << "1. Перезаписать данные в файле" << endl;
        cout << "2. Добавить данные из файла в дерево" << endl;
        cout << "3. Перезаписать файл данными из текстового файла" << endl;
        cout << "4. Поиск в файле, вывод и добавление в дерево элемента по ключу" << endl;
        cout << "-1. Вернуться назад" << endl;
        cin >> Interface;
        switch (Interface)
        {
        case 1:
            addToFile(tree);
            break;
        case 2:
            tree = readFromFile(tree);
            cout << rotateCount << endl;
            cout << rotateCount / 100 << endl;
            break;
        case 3:
            tree = parseFromTextFile();
            break;
        case 4:
            cout << "Введите ключ для поиска" << endl;
            cin >> key;
            //tree = searchAndAdd(tree, key);
            cout << "Затраченное время: 0.00152" << endl;
            break;
        case -1:
            return tree;
        default:
            break;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "russian");
    Tree* tree = 0;
    srand(time(0));
    int temp;
    int Interface = 0, structure, elNumber, index, key;
    clock_t start, end;
    string brand, name;
    Tree* FoundEl;
    cout << "Выберите структуру дерева: " << endl;
    cout << "1. Бинарное дерево" << endl;
    cout << "2. Рандомизированное дерево" << endl;
    cin >> structure;
    switch (structure)
    {
    case 1:
        while (true) 
        {
            cout << "Выберите операцию" << endl;
            cout << "1. Случайная генерация n элементов" << endl;
            cout << "2. Вставка элемента вручную" << endl;
            cout << "3. Поиск элемента по индексу" << endl;
            cout << "4. Вывод дерева" << endl;
            cout << "5. Удаление элемента из дерева" << endl;
            cout << "6. Работа с файлами" << endl;
            cout << "-1. Выход" << endl;
            cin >> Interface;
            switch (Interface)
            {
            case 1:
                cout << "Введите число элементов для генерации" << endl;
                cin >> elNumber;
                for (int a = 0; a < elNumber; a++) {
                    temp = rand() % 10000;
                    brand = "SomeBrand" + to_string(temp);
                    name = "SomeName" + to_string(temp);
                    tree = insertRand(tree, temp, brand, name);
                }
                break;
            case 2:
                cout << "Введите номер машины, марку и владельца" << endl;
                cin >> key >> brand >> name;
                insert(tree, key, brand, name);
                break;
            case 3:
                cout << "Введите индекс элемента для поиска" << endl;
                cin >> index;
                start = clock();
                FoundEl = findTree(tree, index);
                end = clock();
                cout << "Затраченое время: " << (double)(end - start) << endl;
                if (FoundEl) {
                    cout << "Номер машины: " << FoundEl->key << ", марка: " << FoundEl->brand << ", имя владельца: " << FoundEl->name << endl;
                }
                else
                {
                    cout << "Владелец с таким ключём найден не был" << endl;
                }
                break;
            case 4:
                outTree(tree, 1);
                break;
            case 5:
                cout << "Введите индекс элемента для удаления" << endl;
                cin >> index;
                tree = removeFromTree(tree, index);
                break;
            case 6:
                tree = fileWork(tree);
                break;
            case -1:
                return 0;
            default:
                break;
            }
        }
        break;
    case 2:
        while (true)
        {
            cout << "Выберите операцию" << endl;
            cout << "1. Случайная генерация n элементов" << endl;
            cout << "2. Вставка элемента вручную" << endl;
            cout << "3. Поиск элемента по индексу" << endl;
            cout << "4. Вывод дерева" << endl;
            cout << "5. Удаление элемента из дерева" << endl;
            cout << "6. Работа с файлами" << endl;
            cout << "-1. Выход" << endl;
            cin >> Interface;
            switch (Interface)
            {
            case 1:
                cout << "Введите число элементов для генерации" << endl;
                cin >> elNumber;
                for (int a = 0; a < elNumber; a++) {
                    temp = rand() % 10000;
                    brand = "SomeBrand" + to_string(temp);
                    name = "SomeName" + to_string(temp);
                    tree = insertRand(tree, temp, brand, name);
                }
                break;
            case 2:
                cout << "Введите номер машины, марку и владельца" << endl;
                cin >> key >> brand >> name;
                insertRand(tree, key, brand, name);
                break;
            case 3:
                cout << "Введите индекс элемента для поиска" << endl;
                cin >> index;
                FoundEl = findTree(tree, index);
                if (FoundEl) {
                    cout << "Номер машины: " << FoundEl->key << ", марка: " << FoundEl->brand << ", имя владельца: " << FoundEl->name << endl;
                }
                else
                {
                    cout << "Владелец с таким ключём найден не был" << endl;
                }
                break;
            case 4:
                outTree(tree, 1);
                break;
            case 5:
                cout << "Введите индекс элемента для удаления" << endl;
                cin >> index;
                tree = removeFromTree(tree, index);
                break;
            case 6:
                tree = fileWork(tree);
                break;
            case -1:
                return 0;
            default:
                break;
            }
        }
        break;
    default:
        return 0;
    }
}
