#include <iostream>

using namespace std;

struct Tree
{
    Tree* left;
    Tree* right;
    int data;
};

int countEven(Tree* tree, int count = 0)
{
    if (tree)
    {
        if (tree->data % 2 == 0)
            count++;
        countEven(tree->left, count);
        countEven(tree->right, count);
    }
    return count;
}

void findMoreEven(Tree* tree)
{
    int left = countEven(tree->left);
    int right = countEven(tree->right);
    if (left > right)
        cout << "В правом поддереве больше чётных чисел" << endl;
    else if(left<right)
        cout << "В левом поддереве больше чётных чисел" << endl;
    else
        cout << "В обоих поддеревьях равное количество чётных чисел" << endl;
    return;
}

Tree* createTree(int n, Tree* tree)
{
    Tree* temp;
    int left, right;
    if (!n)
    {
        return NULL;
    }

    left = n / 2;
    right = n - left - 1;
    temp = new Tree();
    cin >> temp->data;
    temp->left = createTree(left, temp->left);
    temp->right = createTree(right, temp->right);
    tree = temp;
    return tree;
}

void outTree(Tree* tree, int level)
{
    if (tree)
    {
        outTree(tree->left, level + 1);
        for (int a = 0; a < level; a++)
            cout << "    ";
        cout << tree->data << endl;
        outTree(tree->right, level + 1);
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    Tree* tree = 0;
    int len = 0;
    Tree* copy;
    int Interface = 0;
    while (Interface != -1) {
        cout << "Интерфейс программы: " << endl;
        cout << "1. Создать дерево из n элементов" << endl;
        cout << "2. Вывести дерево" << endl;
        cout << "3. Создать копию дерева" << endl;
        cout << "4. Определить в каком поддереве исходного дерева больше чётных чисел" << endl;
        cout << "-1. Выход" << endl;
        cin >> Interface;
        switch (Interface)
        {
        case 1:
            cout << "Введите высоту дерева" << endl;
            cin >> len;
            tree = createTree(len, tree);
            break;
        case 2:
            outTree(tree, len);
            break;
        case 3:
            copy = tree;
            cout << "Скопировано" << endl;
            break;
        case 4:
            findMoreEven(tree);
            break;
        case -1:
            return 0;
            break;
        default:
            cout << "Введено неверное значение!" << endl;
            break;
        }
    }
    return 0;
}


