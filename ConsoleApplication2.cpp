#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>     // для system()

// creators: Leaf and Jhon_Fine

static std::mt19937 gen(static_cast<unsigned int>(std::random_device{}()));
static std::vector<char> letters;
static std::vector<char> numbers;
static std::vector<char> symbols;
static std::vector<char> pool;

int promptType();
size_t promptLength();
bool promptYesNo(const char* msg);
void buildSets();
void buildPool(int choice);
std::string generatePassword(size_t length);
std::string evaluateStrength(const std::string& pwd);
void saveToFile(const std::string& pwd);
void showSaved();

int main() {
    // Встановлюємо CP1251 для консолі (Windows)
    system("chcp 1251 > nul");

    buildSets();
    while (true) {
        int choice = promptType();
        if (choice == 0) break;
        if (choice == 8) {
            showSaved();
            continue;
        }
        size_t length = promptLength();
        buildPool(choice);
        std::string pwd = generatePassword(length);
        std::cout << "Згенерований пароль: " << pwd << "\n";
        std::cout << "Стан пароля: " << evaluateStrength(pwd) << "\n";
        if (promptYesNo("Зберегти в файл? (y/n): ")) {
            saveToFile(pwd);
        }
        pool.clear();
    }
    return 0;
}

int promptType() {
    int t;
    while (true) {
        std::cout << "\nОберіть опцію:\n"
            << "1 - букви (a-z, A-Z)\n"
            << "2 - цифри (0-9)\n"
            << "4 - символи (!@#$...)\n"
            << "3 - букви & цифри\n"
            << "5 - букви & символи\n"
            << "6 - цифри & символи\n"
            << "7 - всі типи\n"
            << "8 - показати збережені паролі\n"
            << "0 - вихід\n> ";
        if (std::cin >> t && (t >= 0 && t <= 8)) return t;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Невірне значення. Спробуйте ще раз.\n";
    }
}

size_t promptLength() {
    size_t n;
    while (true) {
        std::cout << "Введіть довжину пароля (1–100): ";
        if (std::cin >> n && n >= 1 && n <= 100) return n;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Невірна довжина. Спробуйте ще раз.\n";
    }
}

bool promptYesNo(const char* msg) {
    char c;
    while (true) {
        std::cout << msg;
        if (std::cin >> c && (c == 'y' || c == 'n')) return c == 'y';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }
}

void buildSets() {
    letters = { 'q','w','e','r','t','y','u','i','o','p',
               'a','s','d','f','g','h','j','k','l',
               'z','x','c','v','b','n','m',
               'Q','W','E','R','T','Y','U','I','O','P',
               'A','S','D','F','G','H','J','K','L',
               'Z','X','C','V','B','N','M' };
    numbers = { '0','1','2','3','4','5','6','7','8','9' };
    symbols = { '!','@','#','$','%','^','&','*','(',')',
               '-','=','+','/','~','?','<','>' };
}

void buildPool(int choice) {
    if (choice & 1) pool.insert(pool.end(), letters.begin(), letters.end());
    if (choice & 2) pool.insert(pool.end(), numbers.begin(), numbers.end());
    if (choice & 4) pool.insert(pool.end(), symbols.begin(), symbols.end());
}

std::string generatePassword(size_t length) {
    std::string out;
    std::uniform_int_distribution<> dist(0, static_cast<int>(pool.size()) - 1);
    for (size_t i = 0; i < length; ++i) {
        out.push_back(pool[dist(gen)]);
    }
    return out;
}

std::string evaluateStrength(const std::string& pwd) {
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSymbol = false;
    for (char c : pwd) {
        if (std::islower(static_cast<unsigned char>(c))) hasLower = true;
        else if (std::isupper(static_cast<unsigned char>(c))) hasUpper = true;
        else if (std::isdigit(static_cast<unsigned char>(c))) hasDigit = true;
        else hasSymbol = true;
    }
    int score = static_cast<int>(pwd.length());
    score += (hasLower && hasUpper) ? 2 : 0;
    score += hasDigit ? 1 : 0;
    score += hasSymbol ? 2 : 0;

    if (score < 8) return "Слабкий";
    if (score < 12) return "Нормальний";
    return "Сильний";
}

void saveToFile(const std::string& pwd) {
    const char* filename = "passwords.txt";
    std::ifstream fin(filename);
    int count = 0;
    std::string line;
    while (std::getline(fin, line)) if (!line.empty()) ++count;
    fin.close();

    std::ofstream fout(filename, std::ios::app);
    if (fout) {
        fout << count + 1 << ". " << pwd << "\n";
        std::cout << "Збережено у файл як запис №" << count + 1 << ".\n";
    }
    else {
        std::cout << "Помилка відкриття файлу.\n";
    }
}

void showSaved() {
    std::ifstream fin("passwords.txt");
    if (!fin) {
        std::cout << "Не вдалося відкрити passwords.txt або файл не існує.\n";
        return;
    }
    std::cout << "\n-- Збережені паролі --\n";
    std::string line;
    while (std::getline(fin, line)) {
        std::cout << line << "\n";
    }
    std::cout << "-- кінець списку --\n";
}
