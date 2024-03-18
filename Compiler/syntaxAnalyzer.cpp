#include <iostream>
#include <fstream>
#include <vector>
#include "syntaxAnalyzer.hpp"
using namespace std;

int buf_i, num_tokens = 0;
vector<int> rules;
vector<string> tokensAC;
vector<string> buf_lexem;
vector<string> derevo_obrat;

char rule_matrix[21][21] = {
    //       ;  while  (    )   do    {    }    a    c   :=   or   and   <    >    =   not   -    +    *    /    #
    /* ; */ '>', ' ', ' ', ' ', '>', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>',
  /*while*/ ' ', ' ', '<', ' ', '<', ' ', ' ', ' ', ' ', ' ', '<', '<', ' ', ' ', ' ', '<', ' ', ' ', ' ', ' ', '>',
    /* ( */ ' ', ' ', '<', '=', ' ', ' ', ' ', '<', '<', ' ', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' ',
    /* ) */ '>', '<', ' ', '>', ' ', '<', '>', '<', ' ', '>', '>', '>', '>', '>', '>', ' ', '>', '>', '>', '>', '>',
    /*do */ '<', '=', ' ', ' ', ' ', '<', '>', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>',
    /* { */ '<', ' ', ' ', '>', '<', ' ', '=', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /* } */ '>', '>', ' ', ' ', ' ', ' ', '>', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '>',
    /* a */ '>', ' ', ' ', '>', '>', ' ', '>', ' ', ' ', '=', '>', '>', '>', '>', '>', ' ', '>', '>', '>', '>', ' ',
    /* c */ '>', ' ', ' ', '>', '>', ' ', '>', ' ', ' ', ' ', '>', '>', '>', '>', '>', ' ', '>', '>', '>', '>', ' ',
    /*:= */ '>', ' ', '<', ' ', '>', '>', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '<', '<', ' ',
    /*or */ '>', ' ', '<', '>', '>', ' ', ' ', '<', '<', ' ', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' ',
    /*and*/ '>', ' ', '<', '>', '>', ' ', ' ', '<', '<', ' ', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' ',
    /* < */ ' ', ' ', '<', '>', ' ', ' ', ' ', '<', '<', ' ', '>', '>', ' ', ' ', ' ', ' ', '<', '<', '<', '<', ' ',
    /* > */ ' ', ' ', '<', '>', ' ', ' ', ' ', '<', '<', ' ', '>', '>', ' ', ' ', ' ', ' ', '<', '<', '<', '<', ' ',
    /* = */ ' ', ' ', '<', '>', '>', ' ', ' ', '<', '<', ' ', '>', '>', ' ', ' ', ' ', ' ', '<', '<', '<', '<', ' ',
    /*not*/ '<', ' ', '<', '>', '>', ' ', ' ', '<', ' ', ' ', '>', '>', '<', '<', '<', ' ', '>', '>', '>', '>', ' ',
    /* - */ '>', ' ', '<', '>', ' ', ' ', '>', '<', '<', ' ', '>', '>', '>', '>', '>', ' ', '>', '>', '<', '<', ' ',
    /* + */ '>', ' ', '<', '>', ' ', ' ', '>', '<', '<', ' ', '>', '>', '>', '>', '>', ' ', '>', '>', '<', '<', ' ',
    /* * */ '>', ' ', '<', '>', ' ', ' ', '>', '<', '<', ' ', '>', '>', '>', '>', '>', ' ', '>', '>', '>', '>', ' ',
    /* / */ '>', ' ', '<', '>', ' ', ' ', '>', '<', '<', ' ', '>', '>', '>', '>', '>', ' ', '>', '>', '>', '>', ' ',
    /* $ */ '<', '<', '<', ' ', '<', '<', ' ', '<', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '<', '<', ' '
};

char Get_Relations(string left, string right) {
    int l = -1, r = -1;
    
    if (left == ";")          l = 0;
    else if (left == "while") l = 1;
    else if (left == "(")     l = 2;
    else if (left == ")")     l = 3;
    else if (left == "do")    l = 4;
    else if (left == "{")     l = 5;
    else if (left == "}")     l = 6;
    else if (left == "a")     l = 7;
    else if (left == "c")     l = 8;
    else if (left == ":=")    l = 9;
    else if (left == "or")    l = 10;
    else if (left == "and")   l = 11;
    else if (left == "<")     l = 12;
    else if (left == ">")     l = 13;
    else if (left == "=")     l = 14;
    else if (left == "not")   l = 15;
    else if (left == "-")     l = 16;
    else if (left == "+")     l = 17;
    else if (left == "*")     l = 18;
    else if (left == "/")     l = 19;
    else if (left == "$")     l = 20;

    if (right == ";")          r = 0;
    else if (right == "while") r = 1;
    else if (right == "(")     r = 2;
    else if (right == ")")     r = 3;
    else if (right == "do")    r = 4;
    else if (right == "{")     r = 5;
    else if (right == "}")     r = 6;
    else if (right == "a")     r = 7;
    else if (right == "c")     r = 8;
    else if (right == ":=")    r = 9;
    else if (right == "or")    r = 10;
    else if (right == "and")   r = 11;
    else if (right == "<")     r = 12;
    else if (right == ">")     r = 13;
    else if (right == "=")     r = 14;
    else if (right == "not")   r = 15;
    else if (right == "-")     r = 16;
    else if (right == "+")     r = 17;
    else if (right == "*")     r = 18;
    else if (right == "/")     r = 19;
    else if (right == "#")     r = 20;

    if (l == -1 || r == -1) {
        cout << "Left is " << left << " right is " << right << "\nERROR with l=" << l << " and r=" << r << endl;
        return '~';
    }
    
    return rule_matrix[l][r];
}
                                       
int Get_Rule(string search_rule) {
    if (search_rule == "E;E")               return 2;
    else if (search_rule == "E;")           return 3;
    else if (search_rule == "doEwhileB")    return 4;
    else if (search_rule == "{E}")          return 5;
    else if (search_rule == "a:=E")         return 6;
    else if (search_rule == "BandB")        return 7;
    else if (search_rule == "BorB")         return 8;
    else if (search_rule == "notB")         return 9;
    else if (search_rule == "(B)")          return 10;
    else if (search_rule == "E<E")          return 11;
    else if (search_rule == "E=E")          return 12;
    else if (search_rule == "E>E")          return 13;
    else if (search_rule == "E-E")          return 14;
    else if (search_rule == "E+E")          return 15;
    else if (search_rule == "E*E")          return 16;
    else if (search_rule == "E/E")          return 17;
    else if (search_rule == "(E)")          return 18;
    else if (search_rule == "a")            return 19;
    else if (search_rule == "c")            return 20;
    else                                    return -1;
}

void derevo(int g) {
    vector<string> bufer;
    bufer.clear();
    int buf_now = 0;
    if (rules[buf_i] == 2) {
        bufer.push_back("E"); bufer.push_back(";"); bufer.push_back("E");
    } else if (rules[buf_i] == 3) {
        bufer.push_back("E"); bufer.push_back(";");
    } else if (rules[buf_i] == 4) {
        bufer.push_back("do"); bufer.push_back("E"); bufer.push_back("while"); bufer.push_back("B");
    } else if (rules[buf_i] == 5) {
        bufer.push_back("{"); bufer.push_back("E"); bufer.push_back("}");
    } else if (rules[buf_i] == 6) {
        bufer.push_back("a"); bufer.push_back(":="); bufer.push_back("E");
    } else if (rules[buf_i] == 7) {
        bufer.push_back("B"); bufer.push_back("and"); bufer.push_back("B");
    } else if (rules[buf_i] == 8) {
        bufer.push_back("B"); bufer.push_back("or"); bufer.push_back("B");
    } else if (rules[buf_i] == 9) {
        bufer.push_back("not"); bufer.push_back("B");
    } else if (rules[buf_i] == 10) {
        bufer.push_back("("); bufer.push_back("B"); bufer.push_back(")");
    } else if (rules[buf_i] == 11) {
        bufer.push_back("E"); bufer.push_back("<"); bufer.push_back("E");
    } else if (rules[buf_i] == 12) {
        bufer.push_back("E"); bufer.push_back("="); bufer.push_back("E");
    } else if (rules[buf_i] == 13) {
        bufer.push_back("E"); bufer.push_back(">"); bufer.push_back("E");
    } else if (rules[buf_i] == 14) {
        bufer.push_back("E"); bufer.push_back("-"); bufer.push_back("E");
    } else if (rules[buf_i] == 15) {
        bufer.push_back("E"); bufer.push_back("+"); bufer.push_back("E");
    } else if (rules[buf_i] == 16) {
        bufer.push_back("E"); bufer.push_back("*"); bufer.push_back("E");
    } else if (rules[buf_i] == 17) {
        bufer.push_back("E"); bufer.push_back("/"); bufer.push_back("E");
    } else if (rules[buf_i] == 18) {
        bufer.push_back("("); bufer.push_back("E"); bufer.push_back(")");
    } else if (rules[buf_i] == 19) {
        bufer.push_back("a");
    } else if (rules[buf_i] == 20) {
        bufer.push_back("c");
    }

    while (bufer.size() - buf_now > 0) {
        buf_now++;
        if (bufer[bufer.size() - buf_now] == "E" || bufer[bufer.size() - buf_now] == "B") {
            buf_i--;
            derevo(g + 1);
        }
        if (bufer[bufer.size() - buf_now] == "a" || bufer[bufer.size() - buf_now] == "c") {
            bufer[bufer.size() - buf_now] = tokensAC[num_tokens];
            num_tokens--;
        }
        derevo_obrat.push_back("");
        for (int j = 0; j < g; j++) {
            derevo_obrat[derevo_obrat.size() - 1] += " ";
        }
        derevo_obrat[derevo_obrat.size() - 1] += bufer[bufer.size() - buf_now];
    }
    bufer.clear();
}

void fill_buf_lexem() {
    ifstream lex_tabl;
    buf_lexem.push_back("$");
    string buf1, buf2;
    int buf3;
    lex_tabl.open("out_LA.txt");

    vector<int> hashRepository;
    while (!lex_tabl.eof()) {
        lex_tabl >> buf1;
        if (buf1 == "\\") {
            lex_tabl >> buf1;
            while (buf1 != "\\") {
                lex_tabl >> buf1;
            }
        }
        lex_tabl >> buf2;
        lex_tabl >> buf3;
        if (buf2 != "") {
            if (buf2 != "не_удалось_распознать") {
                if (buf2 != "комментарий") {
                    if (buf2 == "идентификатор") {
                        tokensAC.push_back(buf1);
                        buf_lexem.push_back("a");
                    } else if (buf2 == "константа") {
                        tokensAC.push_back(buf1);
                        buf_lexem.push_back("c");
                    } else {
                        buf_lexem.push_back(buf1);
                    }
                }
            } else {
                cout << "Не удалось распознать лексему!\nСтрока: " << buf3 << endl;
            }
        }
        buf1 = "";
        buf2 = "";
        buf3 = 0;
    }
    buf_lexem.push_back("#");
    lex_tabl.close();
}

void s_analyz(ofstream& file_out_SR) {
    vector <char> vec_buf;
    char relations;
    bool error_check = false, succes_check = false;
    int ruleCount = 0, counter = 0, count_for_buf = 1;
    string gamma;

    while (error_check == false && succes_check == false) {
        file_out_SR << ++counter << ":  | ";
        for (int i = count_for_buf; i < buf_lexem.size(); i++) {
            file_out_SR << buf_lexem[i];
        }
        file_out_SR << " | ";
        for (int i = 0; i < count_for_buf; i++) {
            file_out_SR << buf_lexem[i];
        }

        int i = count_for_buf - 1;
        while (buf_lexem[i] == "B" || buf_lexem[i] == "E" || buf_lexem[i] == "") {
            i--;
        }

        if (buf_lexem[i] == "$" && buf_lexem[count_for_buf] == "#") {
            succes_check = true;
        } else {
            int j = count_for_buf - 1;
            relations = Get_Relations(buf_lexem[i], buf_lexem[count_for_buf]);
            if (relations == '=' && buf_lexem[j] != "B" && buf_lexem[j + 1] == ";") {
                relations = '<';
            }
            switch (relations) {
            case '<':
            case '=':
                file_out_SR << " | Сдвиг | ";
                for (int i = 0; i < ruleCount; i++) {
                    file_out_SR << rules[i] << " ";
                }
                file_out_SR << " | " << endl;

                count_for_buf++;
                vec_buf.push_back(relations);
                break;
            case '>': {
                int c = 1, bb = 0;
                file_out_SR << " | Свертка";
                while (vec_buf[vec_buf.size() - c] == '=' || vec_buf[vec_buf.size() - c] == '.') {
                    c++;
                }
                while (vec_buf.size() != c && vec_buf[vec_buf.size() - c - 1] == '.') {
                    c++;
                }
                while (c > 0) {
                    gamma.insert(0, buf_lexem[j - bb]);
                    vec_buf[vec_buf.size() - 1 - bb] = '.';
                    buf_lexem[j - bb] = "";
                    c--;
                    bb++;
                }

                int num_rule = Get_Rule(gamma);
                rules.push_back(num_rule);
                file_out_SR << " \"" << gamma << "\" по правилу " << rules[ruleCount] << " | ";
                if (rules[ruleCount] != -1) {
                    ruleCount++;
                    buf_lexem[j] = (rules[ruleCount - 1] >= 7 && rules[ruleCount - 1] <= 13) ? "B" : "E";

                    for (int i = 0; i < ruleCount; i++) {
                        file_out_SR << rules[i] << " ";
                    }
                    file_out_SR << " | ";
                } else {
                    error_check = true;
                }
                file_out_SR << endl;
                gamma = "";
                break;
            }
            case ' ':
                cout << "Найден пробел в правилах \n";
                error_check = true;
                break;
            default:
                cout << "\n Ошибка в правилах \n" << endl;
                error_check = true;
                break;
            }
        }

        ofstream Rules("Rules.txt", ios_base::trunc);

        if (succes_check == true) {
            file_out_SR << " | Разбор закончен \n\n Цепочка правил: ";
            for (int i = 0; i < ruleCount; i++) {
                Rules << rules[i] << " ";
                file_out_SR << rules[i] << " ";
            }
            file_out_SR << endl;
            Rules.close();
        }
    }
}

void syntaxAnalysis() {
setlocale(LC_ALL, "");
    ofstream file_out_SR;
    file_out_SR.open("out_SR.txt");

    fill_buf_lexem();

    for (int i = 0; i < buf_lexem.size(); i++) {
        file_out_SR << buf_lexem[i];
    }
    file_out_SR << "\n\n";

    s_analyz(file_out_SR);
    num_tokens = tokensAC.size() - 1;
    buf_i = rules.size() - 1;
    while (buf_i > 0) {
        derevo(1);
    }

    ofstream file_derevo("derevo_v.txt", ios_base::trunc);
    file_derevo << "E" << endl;
    for (int i = derevo_obrat.size() - 1; i >= 0; i--) {
        file_derevo << derevo_obrat[i] << endl;
    }

    file_out_SR.close();
    file_derevo.close();
}
