#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

/*
1        начало (start)
2        константа, состояние 1, 0 (const1)
3        константа, состояние 2, x (const2)
4        идентификатор (id)
5        знаки операций (+, -, *, /)
6        знак присваивания (:=) (_equal)
7        комментарий (comment)
8        ключевое слово, состояние 1, f (for1)
9        ключевое слово, состояние 2, o (for2)
10       ключевое слово, состояние 3, r (for3)
11       разделитель лексем
12       логические знаки
13       открытая операторная скобка ("{")
14       закрытая операторная скобка ("}")
15       открытая круглая скобка ("(")
16       закрытая круглая скобка (")")
17       ошибка (error)
*/

enum states {
    start = 1,
    const1 = 2,
    const2 = 3,
    id = 4,
    _equal = 6,
    comment = 7,

    for1 = 8,
    for2 = 9,
    for3 = 10,

    error = 17,

    and1 = 18,
    and2 = 19,
    and3 = 20,

    or1 = 21,
    or2 = 22,

    not1 = 23,
    not2 = 24,
    not3 = 25,

    if1 = 26,

    else1 = 27
};

ofstream fout("out_LA.txt", ios_base::trunc);
ofstream id_out("out_id_LA.txt", ios_base::trunc);
vector <string> id_LA;

void lexem(string s, int y, int n) {
    bool f = true;
    fout << s;
    if (s != " ") {
        switch (y) {
        case 3:
            fout << " константа " << n << endl;
            break;
        case 4:
            fout << " идентификатор " << n << endl;
            for (int i = 0; i < id_LA.size(); i++) {
                if (s == id_LA[i]) {
                    f = false;
                }
            }
            if (f) {
                id_LA.push_back(s);
                id_out << s << endl;
            }
            break;
        case 5:
            fout << " арифметическая_операция " << n << endl;
            break;
        case 6:
            fout << " знак_присваивания " << n << endl;
            break;
        case 7:
            fout << " комментарий " << n << endl;
            break;
        case 10: case 20: case 22: case 25: case 26: case 27:
            fout << " ключевое_слово " << n << endl;
            break;
        case 11:
            fout << " разделитель_лексем " << n << endl;
            break;
        case 17:
            fout << " не_удалось_распознать " << n << endl;
            break;
        case 12:
            fout << " операция_сравнения " << n << endl;
            break;
        case 13:
            fout << " открытая_операторная_скобка " << n << endl;
            break;
        case 14:
            fout << " закрытая_операторная_скобка " << n << endl;
            break;
        case 15:
            fout << " открытая_круглая_скобка " << n << endl;
            break;
        case 16:
            fout << " закрытая_круглая_скобка " << n << endl;
            break;
        }
    }
}

int main() {
    ifstream code_in("input.txt");

    string now_line, res = "";
    int num = 0;
    states state = start;

    while (getline(code_in, now_line)) {
        if (res != "") {
            lexem(res, state, num);
            res = "";
        }

        if (state != comment) {
            state = start;
        }

        num++;

        for (int i = 0; i < now_line.size(); i++) {
            char n_sym = now_line[i];
            switch (state) {
            case (start):
                switch (n_sym) {
                case '0':
                    res += n_sym;
                    state = const1;
                    break;
                case '\\':
                    res += n_sym;
                    state = comment;
                    break;
                case '?':
                    res += n_sym;
                    state = if1;
                    break;
                case ':':
                    res += n_sym;
                    state = else1;
                    break;
                case '>': case '<': case '=':
                    res += n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    res += n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case 'f':
                    res += n_sym;
                    state = for1;
                    break;
                case 'a':
                    res += n_sym;
                    state = and1;
                    break;
                case 'o':
                    res += n_sym;
                    state = or1;
                    break;
                case 'n':
                    res += n_sym;
                    state = states::not1;
                    break;
                case 'b': case 'c': case 'd': case 'e':
                case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
                case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ' ': case '\t': case '\r':
                    break;
                case ';':
                    res += n_sym;
                    lexem(res, 11, num);
                    res = "";
                    break;
                case '{':
                    res += n_sym;
                    lexem(res, 13, num);
                    res = "";
                    break;
                case '}':
                    res += n_sym;
                    lexem(res, 14, num);
                    res = "";
                    break;
                case '(':
                    res += n_sym;
                    lexem(res, 15, num);
                    res = "";
                    break;
                case ')':
                    res += n_sym;
                    lexem(res, 16, num);
                    res = "";
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (const1):
                switch (n_sym) {
                case 'x':
                    res += n_sym;
                    state = const2;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (const2):
                switch (n_sym) {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'A': case 'B': case 'C': case 'D': case 'E': case'F':
                    res += n_sym;
                    state = const2;
                    break;
                case ':':
                    lexem(res, 3, num);
                    res = n_sym;
                    state = else1;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 3, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '\\':
                    lexem(res, 3, num);
                    res = n_sym;
                    state = comment;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (id):
                switch (n_sym) {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (comment):
                switch (n_sym) {
                case '\\':
                    res += n_sym;
                    lexem(res, 7, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = comment;
                    break;
                }
                break;
            case (for1):
                switch (n_sym) {
                case 'o':
                    res += n_sym;
                    state = for2;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
                case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (for2):
                switch (n_sym) {
                case 'r':
                    res += n_sym;
                    state = for3;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q':
                case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (for3):
                switch (n_sym) {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 10, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 10, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 10, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (and1):
                switch (n_sym) {
                case 'n':
                    res += n_sym;
                    state = and2;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
                case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    break;
                }
                break;
            case (and2):
                switch (n_sym) {
                case 'd':
                    res += n_sym;
                    state = and3;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c':
                case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    break;
                }
                break;
            case (and3):
                switch (n_sym) {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 20, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 20, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 20, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (or1):
                switch (n_sym) {
                case 'r':
                    res += n_sym;
                    state = or2;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q':
                case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (or2):
                switch (n_sym) {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 22, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 22, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 22, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (states::not1):
                switch (n_sym) {
                case 'o':
                    res += n_sym;
                    state = states::not2;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
                case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (states::not2):
                switch (n_sym) {
                case 't':
                    res += n_sym;
                    state = states::not3;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's':
                case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (states::not3):
                switch (n_sym) {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 25, num);
                    res = n_sym;
                    state = else1;
                    break;
                case '\\':
                    lexem(res, 25, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 25, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (if1):
                switch (n_sym) {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    lexem(res, 26, num);
                    res = n_sym;
                    state = id;
                    break;
                case '\\':
                    lexem(res, 26, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 26, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 26, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 26, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 26, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 26, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 26, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 26, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 26, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (else1):
                switch (n_sym) {
                case '=':
                    res += n_sym;
                    lexem(res, 6, num);
                    res = "";
                    state = start;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    lexem(res, 27, num);
                    res = n_sym;
                    state = id;
                    break;
                case '\\':
                    lexem(res, 27, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 27, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 27, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 27, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<':
                    lexem(res, 27, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 27, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 27, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 27, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 27, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (error):
                switch (n_sym) {
                case ' ': case '\t': case '\r':
                    lexem(res, 17, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 17, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    state = start;
                    break;

                default:
                    res += n_sym;
                    break;
                }
                break;
            default:
                break;
            }
        }
    }

    if (res != "") {
        lexem(res, state, num);
        res = "";
    }

    if (state != comment) {
        state = start;
    }

    code_in.close();
    fout.close();
    id_out.close();

    return 0;
}
