#include "defs.h"
#include "Scaner.h"
#include "Diagram.h"

void tuningconsole() {
	setlocale(LC_ALL, "Russian");
	HANDLE hWnd = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize = { 500, 500 };
	SetConsoleScreenBufferSize(hWnd, bufferSize);
}

int main() {

	tuningconsole();

	TScaner * sc = new TScaner("input.txt");
	TDiagram *dg = new TDiagram(sc);
	dg->S();

	printf("\n\n");
	system("pause");

	int a = 0;
	int b = a++ + 1;
	return 0;
}
	