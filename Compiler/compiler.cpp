#include <iostream>
#include "lexAnalyzer.hpp"
#include "syntaxAnalyzer.hpp"
#include "codeGen.hpp"
using namespace std;

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");

	lexAnalysis(argv[1]);
	syntaxAnalysis();
	genTriads();

	wcout << L"Компиляция успешно завершена!" << endl;
	return 0;
}
