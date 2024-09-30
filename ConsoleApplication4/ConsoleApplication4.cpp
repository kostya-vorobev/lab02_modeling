#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <numeric>
#include <iomanip> // Для работы с выводом
#include <map> // Если M представлен как std::map
#include <fstream>
#include <cmath>
#include <string>

const int N = 7;
using namespace std;

vector<vector<int>> M = {
    {0}, // Для удобства индексации, 0-й элемент не используется.
    {12, 9, 17, 6, 13, 1, 15},
    {8, 0, 1, 12, 2, 3, 14},
    {10, 9, 15, 2, 15, 9, 10},
    {4, 11, 0, 12, 5, 8, 3},
    {8, 12, 14, 2, 0, 15, 4},
    {16, 7, 2, 11, 0, 10, 15},
    {9, 6, 4, 8, 8, 11, 0}
};
/*
vector<vector<int>> M = {
    {0}, // Для удобства индексации, 0-й элемент не используется.
    {10, 8, 5, 12, 11, 0, 6},
    {3, 0, 6, 10, 13, 2, 7},
    {4, 5, 15, 0, 20, 3, 1},
    {7, 10, 0, 8, 11, 9, 12},
    {9, 11, 3, 7, 0, 5, 4},
    {5, 7, 11, 3, 13, 10, 0},
    {3, 6, 7, 5, 10, 9, 4}
};*/

vector<int> PI1(N, 0);
vector<int> PI2(N, 0);
vector<int> LI(N, 0);

vector<int> Tpr(N);
vector<int> Toj(N);
vector<vector<int>> Posl(5); // Для хранения разных правил

// Функция для расчёта PI1 и PI2
void calcPI() {
    int ot, to;

    if (N % 2 == 0) {
        ot = N / 2;
        to = N / 2;
    }
    else {
        ot = (N + 1) / 2 - 1;
        to = (N + 1) / 2;
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < to; ++j) {
            PI1[i] += M[i + 1][j];
        }
        for (int j = ot; j < N; ++j) {
            PI2[i] += M[i + 1][j];
        }
    }
}

// Функция для расчёта LI
void calcLI() {
    for (int i = 0; i < N; ++i) {
        LI[i] = PI2[i] - PI1[i];
    }
}

// Правила Петрова
vector<int> Rule1() {
    vector<int> posled;
    vector<array<int, 3>> D01, D2;

    for (int i = 0; i < N; ++i) {
        if (LI[i] >= 0) {
            D01.push_back({ i, PI1[i], LI[i] });
        }
        else {
            D2.push_back({ i, PI2[i], LI[i] });
        }
    }

    sort(D01.begin(), D01.end(), [](const auto& a, const auto& b) {
        return a[1] < b[1] || (a[1] == b[1] && a[2] > b[2]);
        });
    sort(D2.begin(), D2.end(), [](const auto& a, const auto& b) {
        return a[1] > b[1] || (a[1] == b[1] && a[2] < b[2]);
        });

    for (const auto& i : D01) {
        posled.push_back(i[0] + 1);
    }
    for (const auto& i : D2) {
        posled.push_back(i[0] + 1);
    }

    return posled;
}

vector<int> Rule2() {
    vector<int> posled;
    vector<array<int, 3>> copyLI;

    for (int i = 0; i < N; ++i) {
        copyLI.push_back({ i, LI[i], PI1[i] });
    }

    sort(copyLI.begin(), copyLI.end(), [](const auto& a, const auto& b) {
        return a[1] > b[1] || (a[1] == b[1] && a[2] > b[2]);
        });

    for (const auto& i : copyLI) {
        posled.push_back(i[0] + 1);
    }

    return posled;
}

vector<int> Rule3() {
    vector<int> posled;
    vector<array<int, 2>> D0, D1, D2;

    for (int i = 0; i < N; ++i) {
        if (LI[i] == 0) {
            D0.push_back({ i, PI1[i] });
        }
        else if (LI[i] > 0) {
            D1.push_back({ i, PI1[i] });
        }
        else {
            D2.push_back({ i, PI2[i] });
        }
    }

    sort(D0.begin(), D0.end(), [](const auto& a, const auto& b) {
        return a[1] < b[1];
        });
    sort(D1.begin(), D1.end(), [](const auto& a, const auto& b) {
        return a[1] < b[1];
        });
    sort(D2.begin(), D2.end(), [](const auto& a, const auto& b) {
        return a[1] > b[1];
        });

    for (const auto& i : D1) {
        posled.push_back(i[0] + 1);
    }
    for (const auto& i : D0) {
        posled.push_back(i[0] + 1);
    }
    for (const auto& i : D2) {
        posled.push_back(i[0] + 1);
    }

    return posled;
}

vector<int> Rule4() {
    vector<int> posled;
    vector<array<int, 4>> masPair; // Изменяем размерность на 4, чтобы хранить все необходимые значения.
    vector<array<int, 3>> D0, D1, D2;

    for (int i = 0; i < N; ++i) {
        if (LI[i] == 0) {
            D0.push_back({ i, PI1[i], PI2[i] });
        }
        else if (LI[i] > 0) {
            D1.push_back({ i, PI1[i], PI2[i] });
        }
        else {
            D2.push_back({ i, PI1[i], PI2[i] });
        }
    }

    // Обработка D1
    while (!D1.empty()) {
        if (D1.size() == 1) break;
        auto pair0 = *max_element(D1.begin(), D1.end(), [](const auto& a, const auto& b) { return a[2] < b[2]; });
        D1.erase(remove(D1.begin(), D1.end(), pair0), D1.end());
        auto pair1 = *min_element(D1.begin(), D1.end(), [](const auto& a, const auto& b) { return a[1] < b[1]; });
        D1.erase(remove(D1.begin(), D1.end(), pair1), D1.end());
        masPair.push_back({ pair0[0], pair1[0], pair0[2] - pair1[1], max(pair0[2] - pair0[1], pair1[2] - pair1[1]) });

    }

    // Обработка D0
    while (!D0.empty()) {
        if (D1.size() == 1 && D0.size() == 1) {
            masPair.push_back({ D1[0][0], D0[0][0], D1[0][2] - D0[0][1],
                               max(D1[0][2] - D1[0][1], D0[0][2] - D0[0][1]) });
            break;
        }
        else if (D0.size() == 1) {
            break;
        }
        else if (D1.size() == 1) {
            auto pair0 = D1[0];
            D1.erase(D1.begin());
            auto pair1 = *min_element(D0.begin(), D0.end(), [](const auto& a, const auto& b) { return a[1] < b[1]; });
            D0.erase(remove(D0.begin(), D0.end(), pair1), D0.end());
            masPair.push_back({ pair0[0], pair1[0], pair0[2] - pair1[1],
                               max(pair0[2] - pair0[1], pair1[2] - pair1[1]) });
            break;
        }

        auto pair0 = *max_element(D0.begin(), D0.end(), [](const auto& a, const auto& b) { return a[2] < b[2]; });
        D0.erase(remove(D0.begin(), D0.end(), pair0), D0.end());
        auto pair1 = *min_element(D0.begin(), D0.end(), [](const auto& a, const auto& b) { return a[1] < b[1]; });
        D0.erase(remove(D0.begin(), D0.end(), pair1), D0.end());
        masPair.push_back({ pair0[0], pair1[0], pair0[2] - pair1[1],
                           max(pair0[2] - pair0[1], pair1[2] - pair1[1]) });
    }

    while (!D2.empty()) {
        if ((D1.size() == 1 || D0.size() == 1) && D2.size() == 1) {
            auto pair0 = (D1.size() == 1) ? D1[0] : D0[0];
            if (D1.size() == 1) D1.clear(); // Удаляем элемент
            else D0.clear(); // Удаляем элемент

            auto pair1 = *D2.begin(); // Здесь гарантированно выбираем единственный элемент
            D2.clear(); // Удаляем элемент из D2
            masPair.push_back({ pair0[0], pair1[0], pair0[2] - pair1[1],
                               max(pair0[2] - pair0[1], pair1[2] - pair1[1]) });
            break;
        }
        else if (D2.size() == 1) {
            break;
        }


        auto pair0 = *max_element(D2.begin(), D2.end(), [](const auto& a, const auto& b) { return a[2] < b[2]; });
        D2.erase(remove(D2.begin(), D2.end(), pair0), D2.end());
        auto pair1 = *min_element(D2.begin(), D2.end(), [](const auto& a, const auto& b) { return a[1] < b[1]; });
        D2.erase(remove(D2.begin(), D2.end(), pair1), D2.end());

        masPair.push_back({ pair0[0], pair1[0], pair0[2] - pair1[1],
                           max(pair0[2] - pair0[1], pair1[2] - pair1[1]) });
    }


    int ldx = 0;
    array<int, 3> dx{};

    if (N % 2 != 0) {
        if (!D0.empty()) {
            dx = D0[0];
        }
        else if (!D1.empty()) {
            dx = D1[0];
        }
        else {
            dx = D2[0];
        }
        ldx = LI[dx[0]];
    }
    // Отладочные данные перед финальной обработкой
    cout << "dx = {" << dx[0] << ", " << dx[1] << ", " << dx[2] << "}, ldx = " << ldx << "\n";

    int pos = -1;
    for (size_t i = 0; i < masPair.size() - 1; ++i) {
        if (masPair[i][2] > ldx && masPair[i + 1][3] < ldx) {
            pos = i + 1;
            break;
        }
    }

    for (size_t i = 0; i < masPair.size(); ++i) {
        if (i == pos) {
            posled.push_back(dx[0] + 1);
        }
        for (int j = 0; j < 2; j++) {
            posled.push_back(masPair[i][j] + 1);
        }
    }

    if (pos == -1) {
        posled.push_back(dx[0] + 1);
    }

    return posled;
}

// Функция для вычисления массива времени
vector<vector<int>> calcT(const vector<int>& posled) {
    vector<vector<int>> T(N, vector<int>(N, 0)); // Инициализация массива времени

    T[0][0] = M[posled[0]][0];
    for (int i = 1; i < N; ++i) {
        T[0][i] = T[0][i - 1] + M[posled[0]][i];
        T[i][0] = T[i - 1][0] + M[posled[i]][0];
    }

    for (int i = 1; i < N; ++i) {
        for (int j = 1; j < N; ++j) {
                T[i][j] = max(T[i - 1][j], T[i][j - 1]) + M[posled[i]][j];
        }
    }

    return T;
}

// Функция для вычисления времени простоя станка
void calcTpr(const vector<vector<int>>& T) {
    for (int i = 0; i < N; ++i) {
        int sumSt = 0;
        for (int j = 0; j < N; ++j) {
            sumSt += M[j + 1][i]; // Определите, что здесь корректно обращаетесь к всем станкам
        }
        Tpr[i] = T[N - 1][i] - sumSt;
    }
}

// Функция для вычисления времени ожидания детали
void calcToj(const vector<vector<int>>& T, const vector<int>& posled) {
    for (int i = 0; i < N; ++i) {

            Toj[i] = T[i][N - 1] - accumulate(M[posled[i]].begin(), M[posled[i]].end(), 0);
        
    }
}

// Функция для печати таблицы
void printTable() {
    cout << left << setw(5) << "№"; // Выравнивание влево с шириной 5 символов
    for (int i = 1; i <= N; ++i) {
        cout << left << setw(5) << i; // Печать заголовков
    }
    cout << endl;

    // Предположим, что M была составлена как 2D-вектор
    for (int i = 0; i < M.size(); ++i) {
        cout << left << setw(5) << i; // Индекс строки
        for (const auto& value : M[i]) {
            cout << left << setw(5) << value; // Печать значений
        }
        cout << endl;
    }
}

// Функция для печати таблицы по последовательности
int printTable2(const vector<int>& posled, const vector<vector<int>>& T) {
    int toReturn = 0;
    cout << left << setw(5) << "№";
    for (int i = 1; i <= N; ++i) {
        cout << left << setw(10) << i; // Печать заголовков
    }
    cout << left << setw(10) << "Т ож" << endl;

    for (int i = 0; i < 7; ++i) {
        cout << left << setw(5) << posled[i];
        for (int j = 0; j < N; ++j) {
            cout << left << setw(10) << to_string(M[posled[i]][j]) + " / " + to_string(T[i][j]);
            toReturn = T[i][j];
        }
        cout << left << setw(10) << Toj[i] << endl;
    }

    // Вывод T пр на отдельной строке
    cout << left << setw(7) << "Tпр";
    for (const auto& value : Tpr) {
        cout << left << setw(10) << value; // Печать значений T пр
    }

    // Подсчет и вывод итогов
    int totalTpr = accumulate(Tpr.begin(), Tpr.end(), 0);
    int totalToj = accumulate(Toj.begin(), Toj.end(), 0);
    cout << left << totalTpr << " / " << totalToj << endl; // Итоговый вывод

    return toReturn;
}

// Функция для перестановки
vector<int> swap(vector<int>& MAS, int i, int j) {
    int t = MAS[i];
    MAS[i] = MAS[j];
    MAS[j] = t;
    return MAS;
}

// Функция для нахождения следующей перестановки в лексикографическом порядке
bool NextSet(vector<int>& a, int n) {
    int j = n - 2;
    while (j != -1 && a[j] >= a[j + 1]) {
        j -= 1;
    }

    if (j == -1) {
        return false; // Невозможно получить следующую перестановку
    }

    int k = n - 1;
    while (a[j] >= a[k]) {
        k -= 1;
    }

    swap(a, j, k);
    int l = j + 1;
    int r = n - 1;

    // Сортируем оставшуюся часть последовательности
    while (l < r) {
        swap(a, l, r);
        l += 1;
        r -= 1;
    }

    return true;
}

// Функция для получения всех перестановок в лексикографическом порядке
vector<vector<int>> allPosled() {
    vector<vector<int>> permutations;
    vector<int> Nt(N);
    for (int i = 0; i < N; ++i) {
        Nt[i] = i + 1; // Заполнение массива от 1 до N
    }

    permutations.push_back(Nt); // Первая перестановка
    while (NextSet(Nt, N)) {
        permutations.push_back(Nt);
    }

    return permutations;
}

// Функция для записи данных в файл
void printToFile(const string& name, const vector<vector<int>>& data) {
    ofstream file(name);
    if (!file.is_open()) {
        cerr << "Ошибка при открытии файла!" << endl;
        return;
    }

    for (const auto& row : data) {
        for (const auto& val : row) {
            file << val << " "; // Записываем значения
        }
        file << endl; // Переход на новую строку после каждой строки вектора
    }
}

void printVector(const vector<int>& vec) {
    for (const auto& el : vec) {
        cout << el << " ";
    }
}

int main() {
    setlocale(LC_ALL, "russian");
    // Примеры использования
    auto permutations = allPosled();
    int minT = 9999999;
    int minToj = 9999999;
    vector<int> minPerm;

    for (const auto& perm : permutations) {
        auto T = calcT(perm);
        calcToj(T, perm);
        if (T[N - 1][N - 1] < minT) {
            minT = T[N - 1][N - 1];
            minPerm = perm;
            minToj = accumulate(Toj.begin(), Toj.end(), 0);
        }
        if (T[N - 1][N - 1] == minT) {
            if (accumulate(Toj.begin(), Toj.end(), 0) < minToj) {
                minPerm = perm;
                minToj = accumulate(Toj.begin(), Toj.end(), 0);
            }
        }
    }

    calcPI();
    calcLI();
    vector<vector<int>> Posl(5);
    Posl[0] = Rule1();
    Posl[1] = Rule2();
    Posl[2] = Rule3();
    Posl[3] = Rule4();
    Posl[4] = { 2, 4, 3, 6, 7, 1, 5 };

    cout << "Initial data:" << endl;
    printTable();
    printToFile("initial.txt", M);

    vector<int> findMin(5, 0);
    for (int i = 0; i < 5; ++i) {
        cout << "Rule " << (i + 1) << " {";
        printVector(Posl[i]); // Используем функцию для вывода вектора
        cout << "} \n";
        auto T = calcT(Posl[i]);
        calcTpr(T);
        calcToj(T, Posl[i]);
        findMin[i] = printTable2(Posl[i], T);
    }

    int minIndex = min_element(findMin.begin(), findMin.end()) - findMin.begin();
    vector<vector<int>> petrov;
    for (int i : Posl[minIndex]) {
        petrov.push_back(M[i]);
    }
    printToFile("petrov.txt", petrov);

    cout << "Iterating through the elements" << endl;
    auto T = calcT(minPerm);
    calcTpr(T);
    calcToj(T, minPerm);
    printTable2(minPerm, T);

    vector<vector<int>> iterating;
    for (int i : minPerm) {
        iterating.push_back(M[i]);
    }
    printToFile("iterating.txt", iterating);

    cout << "Open 'ЛР2.xlsx' to see more" << endl;

    return 0;
}
