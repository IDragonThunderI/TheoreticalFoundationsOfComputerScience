#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>
using namespace std;

struct Node {
    string key;
    vector <Node> leaf;
} main_Node;

vector <string> der_operation;
vector <string> str_all;
vector <int> str_all_num;
vector <string> triads_gen;
vector <int> tr_left;
int _count = 1;
int count_for(Node& _node) {
    int c = 0;
    for (int i = 0; i < _node.leaf.size(); i++) {
        if (_node.leaf[i].key != "." && _node.leaf[i].leaf.size() > 0) {
            c++;
        }
    }
    for (int i = 0; i < _node.leaf.size(); i++) {
        c += count_for(_node.leaf[i]);
    }
    return c;
}

vector <int> _bufer_for_jmp;
vector <int> _bufer_for_jmp2;
void gen_triads(ofstream& fout, Node& _node) {
    int all = 0, all_buf = 0;
    bool pred_left = false, pred_right = false, flag_not = false;
    
    for (int i = 0; i < _node.leaf.size(); i++) {
        if (_node.leaf[i].key == "for") {
            all = count_for(_node.leaf[i + 2]);
            all_buf = count_for(_node.leaf[i + 1].leaf[2]) + 1;
            gen_triads(fout, _node.leaf[i + 1].leaf[0]);
            gen_triads(fout, _node.leaf[i + 1].leaf[1]);
            fout << _count << " for(^" << _count - 1 << ",^" << _count + all + all_buf + 2 << ")" << endl;
            _count++;
            gen_triads(fout, _node.leaf[i + 1].leaf[2]);
            _bufer_for_jmp.push_back(_count + all);
            _bufer_for_jmp2.push_back(_count - all_buf - 1);
        } else if (_node.leaf[i].key == "if") {
            all = count_for(_node.leaf[i + 2]) + 1;
            all_buf = count_for(_node.leaf[i + 4]) + 1;
            gen_triads(fout, _node.leaf[i + 1].leaf[0]);
            fout << _count << " if(^" << _count - 1 << ",^" << _count + all + 2 << ")" << endl;
            _count++;
            _bufer_for_jmp.push_back(_count + all);
            _bufer_for_jmp2.push_back(_count + all + all_buf + 1);
        } else if (_node.leaf[i].key == ":=") {
            gen_triads(fout, _node.leaf[i]);
        } else if (_node.leaf[i].key == "-") {
            if (_node.key == "not") {
                flag_not = true;
                pred_left = false;
                pred_right = false;
            }
            gen_triads(fout, _node.leaf[i]);
            if (i == 0) {
                pred_left = true;
            }
            if (i == 1) {
                pred_right = true;
            }
        } else if (_node.leaf[i].key == "+") {
            if (i == 0) {
                pred_left = true;
            }
            if (i == 1) {
                pred_right = true;
            }
            if (_node.key == "not") {
                flag_not = true;
                pred_left = false;
                pred_right = false;
            }
            gen_triads(fout, _node.leaf[i]);
        } else if (_node.leaf[i].key == "*") {
            if (i == 0) {
                pred_left = true;
            }
            if (i == 1) {
                pred_right = true;
            }
            if (_node.key == "not") {
                flag_not = true;
                pred_left = false;
                pred_right = false;
            }
            gen_triads(fout, _node.leaf[i]);
        } else if (_node.leaf[i].key == "/") {
            if (i == 0) {
                pred_left = true;
            }
            if (i == 1) {
                pred_right = true;
            }
            if (_node.key == "not") {
                flag_not = true;
                pred_left = false;
                pred_right = false;
            }
            gen_triads(fout, _node.leaf[i]);
        } else if (_node.leaf[i].key == "<") {
            if (i == 0) {
                pred_left = true;
            }
            if (i == 1) {
                pred_right = true;
            }
            if (_node.key == "not") {
                flag_not = true;
                pred_left = false;
                pred_right = false;
            }
            gen_triads(fout, _node.leaf[i]);
        } else if (_node.leaf[i].key == ">") {
            if (i == 0) {
                pred_left = true;
            }
            if (i == 1) {
                pred_right = true;
            }
            if (_node.key == "not") {
                flag_not = true;
                pred_left = false;
                pred_right = false;
            }
            gen_triads(fout, _node.leaf[i]);
        } else if (_node.leaf[i].key == "=") {
            if (i == 0) {
                pred_left = true;
            }
            if (i == 1) {
                pred_right = true;
            }
            if (_node.key == "not")
            {
                flag_not = true;
                pred_left = false;
                pred_right = false;
            }
            gen_triads(fout, _node.leaf[i]);
        } else if (_node.leaf[i].key == "and") {
            if (i == 0) {
                pred_left = true;
            }
            if (i == 1) {
                pred_right = true;
            }
            if (_node.key == "not") {
                flag_not = true;
                pred_left = false;
                pred_right = false;
            }
            gen_triads(fout, _node.leaf[i]);
        } else if (_node.leaf[i].key == "or") {   
            if (i == 0) {
                pred_left = true;
            }
            if (i == 1) {
                pred_right = true;
            }
            if (_node.key == "not") {
                flag_not = true;
                pred_left = false;
                pred_right = false;
            }
            gen_triads(fout, _node.leaf[i]);
        } else if (_node.leaf[i].key == "not") {
            if (i == 0) {
                pred_left = true;
            }
            if (i == 1) {
                pred_right = true;
            }
            if (_node.key == "not") {
                flag_not = true;
                pred_left = false;
                pred_right = false;
            }
            gen_triads(fout, _node.leaf[i]);
        } else if (_node.leaf[i].key == ".") {
            gen_triads(fout, _node.leaf[i]);
        }
    }

    if (_node.key != "." && _node.key != "$" && _node.key != "for" && _node.key != "") {
        if (pred_left && pred_right) {
            all = count_for(_node.leaf[1]) + 1;
            fout << (_count) << " " << _node.key << "(^" << (_count - 1 - all) << ",^" << (_count - 1) << ")" << endl;
        } else if (pred_left) {
            fout << (_count) << " " << _node.key << "(^" << (_count - 1) << "," << _node.leaf[1].key << ")" << endl;
        } else if (pred_right) {
            fout << (_count) << " " << _node.key << "(" << _node.leaf[0].key << ",^" << (_count - 1) << ")" << endl;
        } else if (flag_not) {
            fout << (_count) << " " << _node.key << "(1,^" << (_count - 1) << ")" << endl;
        } else {
            fout << (_count) << " " << _node.key << "(" << _node.leaf[0].key << "," << _node.leaf[1].key << ")" << endl;
        }
        _count++;
    }

    for (int j = 0; j < _bufer_for_jmp.size(); j++) {
        if (_bufer_for_jmp[j] == _count) {
            fout << _count << " jmp(1,^" << _bufer_for_jmp2[j] << ")" << endl;
            _count++;
        }
    }
}
void create_tree(int n, Node& _node, int i) {
    vector <string> b;
    vector <int> b_help;
    Node _node_buf;

    while (i < str_all_num.size() && str_all_num[i] >= n && str_all_num[i] != -1) {
        if (str_all_num[i] == n) {
            if (str_all[i] != "(" && str_all[i] != ")" && str_all[i] != "{" && str_all[i] != "}" && str_all[i] != ";") {
                b.push_back(str_all[i]);
                b_help.push_back(i);
            }
            str_all_num[i] = -1;
        }
        i++;
    }

    if (_node.key != "E" && _node.key != "B") {
        Node buf;
        if (b.size() == 3) {
            if (b[0] == "for") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[0].key = "for";
                _node.leaf.push_back(_node_buf);
                _node.leaf[1].key = "$";
                _node.leaf.push_back(_node_buf);
                _node.leaf[2].key = ".";
                create_tree(n + 1, _node.leaf[1], b_help[1] + 1);
                create_tree(n + 1, _node.leaf[2], b_help[2] + 1);
            } else if (b[1] == "?") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[0].key = "?";
                _node.leaf.push_back(_node_buf);
                _node.leaf[1].key = "$";
                _node.leaf.push_back(_node_buf);
                _node.leaf[2].key = ".";
                create_tree(n + 1, _node.leaf[0], b_help[0] + 1);
                create_tree(n + 1, _node.leaf[2], b_help[2] + 1);
            } else if (b[1] == ":=") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[_node.leaf.size() - 1].key = ":=";
                _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[0];

                if (b[2] != "E") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "<") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[_node.leaf.size() - 1].key = "<";

                if (b[0] != "E" && b[0] != "B") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E" && b[0] != "B") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == ">") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[_node.leaf.size() - 1].key = ">";

                if (b[0] != "E" && b[0] != "B") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E" && b[0] != "B") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "=") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[_node.leaf.size() - 1].key = "=";

                if (b[0] != "E" && b[0] != "B") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E" && b[0] != "B") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "-") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[_node.leaf.size() - 1].key = "-";

                if (b[0] != "E") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "+") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[_node.leaf.size() - 1].key = "+";

                if (b[0] != "E") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "*") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[_node.leaf.size() - 1].key = "*";

                if (b[0] != "E") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "/") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[_node.leaf.size() - 1].key = "/";

                if (b[0] != "E") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E") {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf[_node.leaf.size() - 1].leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].leaf[_node.leaf[_node.leaf.size() - 1].leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "B") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[0].key = "E";
                _node.leaf.push_back(_node_buf);
                _node.leaf[1].key = "B";
                _node.leaf.push_back(_node_buf);
                _node.leaf[2].key = "E";
                create_tree(n + 1, _node.leaf[0], b_help[0] + 1);
                create_tree(n + 1, _node.leaf[1], b_help[1] + 1);
                create_tree(n + 1, _node.leaf[2], b_help[2] + 1);
            }
        } else if (b.size() == 5) {
            if (b[1] == "?") {
                _node.leaf.push_back(_node_buf);
                _node.leaf[0].key = "if";
                _node.leaf.push_back(_node_buf);
                _node.leaf[1].key = "$";
                _node.leaf.push_back(_node_buf);
                _node.leaf[2].key = "E";
                _node.leaf.push_back(_node_buf);
                _node.leaf[3].key = ":";
                _node.leaf.push_back(_node_buf);
                _node.leaf[4].key = "E";
                create_tree(n + 1, _node.leaf[1], b_help[0] + 1);
                create_tree(n + 1, _node.leaf[2], b_help[2] + 1);
                create_tree(n + 1, _node.leaf[4], b_help[4] + 1);
            }
        } else if (b.size() == 1) {
            _node.leaf.push_back(_node_buf);
            _node.leaf[_node.leaf.size() - 1].key = "E";
            create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
        }
    } else if (_node.key == "E" || _node.key == "B") {
        if (b.size() == 1) {
            if (b[0] == "E" || b[0] == "B") {
                create_tree(n + 1, _node, b_help[0] + 1);
            } else {
                _node.key = b[0];
            }
        } else if (b.size() == 3) {
            if (b[1] == ":=") {
                _node.leaf.push_back(_node_buf);
                _node.key = ":=";
                _node.leaf[_node.leaf.size() - 1].key = b[0];

                if (b[2] != "E") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "and") {
                _node.key = "and";

                if (b[0] != "E" && b[0] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E" && b[0] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "or") {
                _node.key = "or";

                if (b[0] != "E" && b[0] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E" && b[0] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "<") {
                _node.key = "<";

                if (b[0] != "E" && b[0] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E" && b[0] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == ">") {
                _node.key = ">";

                if (b[0] != "E" && b[0] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E" && b[0] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "=") {
                _node.key = "=";

                if (b[0] != "E" && b[0] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E" && b[0] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "-") {
                _node.key = "-";

                if (b[0] != "E") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "+") {
                _node.key = "+";

                if (b[0] != "E") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "*") {
                _node.key = "*";

                if (b[0] != "E") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            } else if (b[1] == "/") {
                _node.key = "/";

                if (b[0] != "E") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[0];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[0] + 1);
                }

                if (b[2] != "E") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[2];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[2] + 1);
                }
            }
        } else if (b.size() == 2) {
            if (b[0] == "not") {
                _node.key = "not";

                if (b[1] != "E" && b[1] != "B") {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = b[1];
                } else {
                    _node.leaf.push_back(_node_buf);
                    _node.leaf[_node.leaf.size() - 1].key = "E";
                    create_tree(n + 1, _node.leaf[_node.leaf.size() - 1], b_help[1] + 1);
                }
            } else {
                _node.key = ".";
                _node.leaf.push_back(_node_buf);
                _node.leaf.push_back(_node_buf);
                _node.leaf[0].key = "E";
                _node.leaf[1].key = "E";
                create_tree(n + 1, _node.leaf[0], b_help[0] + 1);
                create_tree(n + 1, _node.leaf[1], b_help[1] + 1);
            }
        }
    }
}

void vivod(ofstream& fout, Node& _node, int space) {
    if (_node.key != "" && _node.key != "$" && _node.key != ".") {
        for (int j = 0; j < space; j++) {
            fout << " ";
        }
        fout << _node.key << endl;
    }
    if (_node.leaf.size() == 0) {
        return;
    }
    for (int i = 0; i < _node.leaf.size(); i++) {
        if (_node.key == "." || _node.key == "" || _node.key == "$" || _node.key == ":=") {
            vivod(fout, _node.leaf[i], space);
        }
        else {
            vivod(fout, _node.leaf[i], space + 1);
        }
    }
}

void derevo_oper() {
    ifstream file("derevo_v.txt");

    string b;
    while (getline(file, b)) {
        str_all.push_back(b);
    }

    int num_space;
    for (int i = 0; i < str_all.size(); i++) {
        num_space = 0;
        while (str_all[i][0] == ' ') {
            num_space++;
            str_all[i].erase(str_all[i].begin());
        }
        str_all_num.push_back(num_space);
    }

    create_tree(2, main_Node, 2);
    ofstream fout("derevo_operations.txt", ios_base::trunc);
    vivod(fout, main_Node, 0);
    ofstream file_out("triads.txt", ios_base::trunc);
    gen_triads(file_out, main_Node);

    file.close();
    fout.close();
    file_out.close();
}

void triads() {
    derevo_oper();
}

int main() {
    triads();
}