#pragma once

#include "defs.h"

class TScaner {

private:
	
	TEXT t; //исходный текст
	int uk; // указатель текущей позиции в исходном тексте
	int line;
	int pos;
public:
	
	TScaner(char *); 
	~TScaner() {}

	void SetUK(int i); //установить позицию указателя в тексте
	int GetUK(void); //получить позицию указателя в тексте
	void SetLine(int l);
	int GetLine(void);
	void SetPos(int p);
	int GetPos(void);
	void PrintError(char *, char *); //функция печати ошибок
	void PrintWarning(char *, char *); //функция печати предупреждений
	int Scaner(LEX l); //программа сканера
	void GetData(char *); // вывод файла с исходным модулем
	void SetText(char *);
};



