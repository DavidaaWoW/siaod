#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <fstream>

using namespace std;

//Длинна массива владельцев машин
const int ARR_LEN = 100;

ifstream iffile;
ofstream offile;
string filePath;

//Структура владельцев машин
struct carOwner
{
    int carNumber;
    string brand;
    string ownersName;
};

//Создаём "нулевого" владельца
const carOwner zero = { 0,"","" };

//Вектор владельцев машин, в котором реализуется цепочка
struct Node
{
    carOwner owner[100];
};

//Функция записи в файл
void addToFile(Node table[ARR_LEN])
{
    int len;
    offile.open(filePath, ios::binary | ios::out);
    for (int a = 0; a < ARR_LEN; a++)
    {
        for (int b = 0; table[a].owner[b].brand != ""; b++)
        {
            offile.write((char*)&a, 4);
            offile.write((char*)&b, 4);
            offile.write((char*)&table[a].owner[b].carNumber, 4);
            len = table[a].owner[b].brand.size() + 1;
            offile.write((char*)&len, 4);
            offile.write((char*)table[a].owner[b].brand.c_str(), len);
            len = table[a].owner[b].ownersName.size() + 1;
            offile.write((char*)&len, 4);
            offile.write((char*)table[a].owner[b].ownersName.c_str(), len);
        }
    }
    offile.close();
}

//Функция чтения из файла
void readFromFile(Node table[ARR_LEN])
{
    string lineText;
    int hashNumber;
    int chain;
    int carNumber;
    int len;
    char* buf;
    iffile.open(filePath, ios::binary | ios::in);
    for(int a = 0; a<ARR_LEN; a++)
    {
        iffile.read((char*)&hashNumber, 4);
        iffile.read((char*)&chain, 4);
        iffile.read((char*)&carNumber, 4);
        table[hashNumber].owner[chain].carNumber = carNumber;
        iffile.read((char*)&len, 4);
        buf = new char[len];
        iffile.read(buf, len);
        lineText = buf;
        table[hashNumber].owner[chain].brand = lineText;
        delete[]buf;
        iffile.read((char*)&len, 4);
        buf = new char[len];
        iffile.read(buf, len);
        lineText = buf;
        table[hashNumber].owner[chain].ownersName = lineText;
        delete[]buf;
    }
    iffile.close();
}

//Функция создания хеш-числа
int createHash(int number) {
    return number % ARR_LEN;
}

//Функция вывода структуры владельцев машин
void carOwnerOut(carOwner co)
{
    cout << "Номер машины: " << co.carNumber << ", марка: " << co.brand << ", имя владельца: " << co.ownersName << endl;
}

//Вывод хеш таблицы
void outTable(Node table[ARR_LEN])
{
    for (int a = 0; a < ARR_LEN; a++)
    {
        for (int b = 0;table[a].owner[b].brand!=""; b++)
        {
            if (table[a].owner[b].brand != "")
            {
                cout << "Хеш индекс: " << a << " номер звена: " << b+1 << endl << "Данные: ";
                carOwnerOut(table[a].owner[b]);
            }
        }
        cout << endl;
    }
}

//Функция поиска владельца по номеру машины
carOwner findOwnerByKey(Node table[ARR_LEN], int key)
{
    int index = createHash(key);
    for (int a = 0;table[index].owner[a].brand!=""; a++)
    {
        if (table[index].owner[a].carNumber == key)
        {
            return table[index].owner[a];
        }
    }
    return zero;
}

//Удаление владельца из таблицы
bool removeFromTable(Node table[ARR_LEN], carOwner owner) 
{
    int index = createHash(owner.carNumber);
    for (int a = 0;table[index].owner[a].brand!=""; a++)
    {
        if (table[index].owner[a].carNumber == owner.carNumber)
        {
            table[index].owner[a] = zero;
            for (int b = a; table[index].owner[b].brand != ""; b++)
            {
                table[index].owner[b] = table[index].owner[b + 1];
            }
            return true;
        }
    }
    return false;
}

//Функция дехеширования
carOwner deHash(Node table[ARR_LEN], int index, int chain)
{
    if (index >= ARR_LEN)
    {
        return zero;
    }
    return table[index].owner[chain-1];
}

//Функция добавления владельца в хеш-таблицу
void addToTable(Node table[ARR_LEN], carOwner owner)
{
    //Тут просчитываем тот самый хеш индекс
    int hashNumber = createHash(owner.carNumber);
    int temp = 0;
    //Итерируем до тех пор, пока не найдём нулевого владельца
    for( ; table[hashNumber].owner[temp].brand!="";temp++){}
    table[hashNumber].owner[temp] = owner;
    cout << "Присвоенный индекс: " << hashNumber << " | цепь " << temp+1 << endl;
}

//Функци случайной генерации владельцев
void randGen(carOwner arr[ARR_LEN], Node hashArr[ARR_LEN])
{
    int temp;
    // Генерируем структуру владельцев машин
    for (int a = 0; a < ARR_LEN; a++)
    {
        temp = rand() % 9999;
        while (temp < 1000)
            temp = rand() % 9999;
        arr[a].carNumber = temp;
        arr[a].brand = "SomeBrand" + to_string(a + 1);
        arr[a].ownersName = "SomeName" + to_string(a + 1);
        carOwnerOut(arr[a]);
    }

    //Заполняем односвязный хеш-список
    for (int a = 0; a < ARR_LEN; a++) {
        addToTable(hashArr, arr[a]);
    }
}

int main()
{
    setlocale(LC_ALL, "russian");
    srand(time(0));
    //Создаём первоначальный массив владельцев машин
    carOwner arr[ARR_LEN];
    //Хеш-таблица
    Node hashArr[ARR_LEN];
    int Interface;
    carOwner tempCarOwner;
    int carNumber;
    carOwner neededCar;
    int index, chain;
    cout << "1. Открыть файл. \n";
    cout << "2. Создать новый. \n";
    int filetag;
    cin >> filetag;
    string fileName;
    switch (filetag)
    {
    case 1:
        cout << "Введите полный путь до файла. \n";
        cin >> filePath;
        readFromFile(hashArr);
        break;
    case 2:
        cout << "Введите название файла" << endl;
        cin >> fileName;
        filePath = "D://"+fileName+".bin";
        ofstream outfile(filePath);
        break;
    }
    while (1) {
        cout << "1. Случайная генерация 100 элементов \n";
        cout << "2. Добавление одного владельца вручную \n";
        cout << "3. Удаление одного владельца вручную \n";
        cout << "4. Поиск владельца по номеру машины \n";
        cout << "5. Дехеширование элемента \n";
        cout << "6. Вывод всей хеш-таблицы \n";
        cout << "-1. Выход \n";
        cin >> Interface;
        

        switch (Interface)
        {

            case 1:
                randGen(arr, hashArr);
            break;

            case 2:
                //Добавление владельца вручную
                cout << "Введите номер машины, марку и данные владельца." << endl;
                cin >> tempCarOwner.carNumber >> tempCarOwner.brand >> tempCarOwner.ownersName;
                addToTable(hashArr, tempCarOwner);
            break;

            case 3:
                //Удаление владельца
                cout << "Введите номер машины для удаления" << endl;
                cin >> carNumber;
                if (removeFromTable(hashArr, findOwnerByKey(hashArr, carNumber)))
                {
                    cout << "Удалено" << endl;
                    carNumber = 0;
                }
                else
                {
                    cout << "Владелец не найден" << endl;
                }
            break;

            case 4:
                //Поиск владельца по номеру машины
                cout << "Введите номер машины для поиска" << endl;
                cin >> carNumber;
                neededCar = findOwnerByKey(hashArr, carNumber);
                if (neededCar.carNumber != 0)
                {
                    carOwnerOut(neededCar);
                    carNumber = 0;
                }
                else
                {
                    cout << "Владелец не найден!" << endl;
                }
            break;

            case 5:
                //Ручное дехеширование определённого элемента
                cout << "Введите хеш индекс и номер цепи" << endl;
                cin >> index >> chain;
                neededCar = deHash(hashArr, index, chain);
                if (neededCar.carNumber != 0)
                {
                    cout << "В хеш-таблице под индексом " << index << " под номером цепочки: " << chain << " находится: " << endl;
                    carOwnerOut(neededCar);
                    index = 0;
                    chain = 0;
                }
                else
                {
                    cout << "В хеш-таблице под индексом " << index << " под номером цепочки: " << chain << " ничего нет..." << endl;
                }
            break;

            case 6:
                outTable(hashArr);
            break;
            case -1:
                addToFile(hashArr);
                return 0;
            break;
        }
    }
}
