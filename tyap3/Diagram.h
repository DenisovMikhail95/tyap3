#pragma once

#include "Scaner.h"
#include "Semant.h"
#include "defs.h"

class TDiagram
{
private:
	TScaner *sc;
	Tree * Root;

public:
	TDiagram(TScaner *s) { 
		sc = s;
		Node *b = new Node();
		memcpy(&b->id, &"EMPTY", strlen("EMPTY")+1); b->TypeData = TYPE_UNKNOWN; b->TypeObject = EMPTY;
		Root = new Tree(NULL,NULL,NULL,b);
		Root->SetCur(Root);
	}
	TDiagram(){}
	Tree* getRoot();
	
	void S(); //программа
	void E(); //функция
	void C(); //данные
	void G(); //составной оператор
	void K(TypeData dt); //массив
	void T(); //for
	void O(bool flag_for); //оператор
	int R(LEX id, TData *tdata); //список индексов
	void L(); //инициализация массива
	void M(); //элемент
	TData X(); //простое выражение
	TData J(); //выражение
	TData U(); //слагаемое
	TData Y(); //множитель
};