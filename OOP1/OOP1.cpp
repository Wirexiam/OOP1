#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <locale>
#include <algorithm>
#include <Windows.h>

using namespace std;

class Date {
public:
    int day;
    int month;
    int year;

    Date(int _day, int _month, int _year) : day(_day), month(_month), year(_year) {}

    bool operator>(const Date& other) const {
        if (year > other.year) {
            return true;
        }
        else if (year < other.year) {
            return false;
        }
        if (month > other.month) {
            return true;
        }
        else if (month < other.month) {
            return false;
        }
        return day > other.day;
    }
};

class Food {
private:
    char* name;
    int quant;
    double cost;
    Date shelf_life;
    static int count;

public:
    Food() : name(nullptr), quant(0), cost(0.0), shelf_life(0, 0, 0) {
        count++;
    }

    Food(const char* _name, int _quant, double _cost, const Date& _shelf_life)
        : quant(_quant), cost(_cost), shelf_life(_shelf_life) {
        name = new char[strlen(_name) + 1];
        strcpy(name, _name);
        count++;
    }

    Food(const Food& other)
        : quant(other.quant), cost(other.cost), shelf_life(other.shelf_life) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        count++;
    }

    ~Food() {
        delete[] name;
        count--;
    }

    static int GetCount() {
        return count;
    }

    Date GetShelfLife() const {
        return shelf_life;
    }

    void FindProductByName(const char* searchName) const {
        if (strcmp(name, searchName) == 0) {
            cout << name << "\t" << quant << "\t" << cost << "\t" << shelf_life.day << "-" << shelf_life.month << "-" << shelf_life.year << endl;
        }
    }

    static void FindProductsByQuantity(const vector<Food>& database, int maxQuantity) {
        cout << "Изделия с количеством меньше " << maxQuantity << ":" << endl;
        cout << "Название\tКоличество\tСтоимость\tСрок годности" << endl;
        for (const auto& food : database) {
            if (food.quant < maxQuantity) {
                cout << food.name << "\t" << food.quant << "\t" << food.cost << "\t" << food.shelf_life.day << "-" << food.shelf_life.month << "-" << food.shelf_life.year << endl;
            }
        }
    }

    bool operator>(const Date& currentDate) const {
        return shelf_life > currentDate;
    }

    friend ostream& operator<<(ostream& os, const Food& food) {
        os << food.name << "\t" << food.quant << "\t" << food.cost << "\t" << food.shelf_life.day << "-" << food.shelf_life.month << "-" << food.shelf_life.year;
        return os;
    }
};

int Food::count = 0;

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<Food> database;

    ifstream file("database.txt");
    if (file.is_open()) {
        while (!file.eof()) {
            char name[100];
            int quant;
            double cost;
            int day, month, year;
            file >> name >> quant >> cost >> day >> month >> year;
            Date shelfLife(day, month, year);
            Food food(name, quant, cost, shelfLife);
            database.push_back(food);
        }
        file.close();
    }

    int choice = 0;
    do {
        cout << "Меню:" << endl;
        cout << "1. Добавить новое изделие" << endl;
        cout << "2. Печать базы данных" << endl;
        cout << "3. Поиск по названию" << endl;
        cout << "4. Поиск по количеству" << endl;
        cout << "5. Найти изделия с истекшим сроком годности" << endl;
        cout << "6. Выход из программы" << endl;
        cin >> choice;

        switch (choice) {
        case 1: {
            char name[100];
            int quant;
            double cost;
            int day, month, year;
            cout << "Введите название изделия: ";
            cin >> name;
            cout << "Введите количество: ";
            cin >> quant;
            cout << "Введите стоимость: ";
            cin >> cost;
            cout << "Введите срок годности (день месяц год): ";
            cin >> day >> month >> year;
            Date shelfLife(day, month, year);
            Food newFood(name, quant, cost, shelfLife);
            database.push_back(newFood);
            break;
        }
        case 2:
            cout << "Название\tКоличество\tСтоимость\tСрок годности" << endl;
            for (const auto& food : database) {
                cout << food << endl;
            }
            break;
        case 3: {
            char searchName[100];
            cout << "Введите название для поиска: ";
            cin >> searchName;
            cout << "Результаты поиска по названию \"" << searchName << "\":" << endl;
            for (const auto& food : database) {
                food.FindProductByName(searchName);
            }
            break;
        }
        case 4: {
            int maxQuantity;
            cout << "Введите максимальное количество для поиска: ";
            cin >> maxQuantity;
            Food::FindProductsByQuantity(database, maxQuantity);
            break;
        }
        case 5: {
            int currentDay, currentMonth, currentYear;
            cout << "Введите текущую дату (день месяц год): ";
            cin >> currentDay >> currentMonth >> currentYear;
            Date currentDate(currentDay, currentMonth, currentYear);

            vector<Food> Rez;
            for (const auto& food : database) {
                if (food > currentDate) {
                    Rez.push_back(food);
                }
            }

            cout << "Изделия с истекшим сроком годности:" << endl;
            cout << "Название\tКоличество\tСтоимость\tСрок годности" << endl;
            for (const auto& food : Rez) {
                cout << food << endl;
            }
            break;
        }
        case 6: {
            ofstream saveFile("database.txt");
            for (const auto& food : database) {
                saveFile << food << "\t" << food.GetShelfLife().day << "-" << food.GetShelfLife().month << "-" << food.GetShelfLife().year << endl;
            }
            saveFile.close();
            break;
        }
        default:
            cout << "Неверный выбор. Пожалуйста, выберите опцию из меню." << endl;
            break;
        }
    } while (choice != 6);

    return 0;
}
