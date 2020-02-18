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
	
	void S(); //���������
	void E(); //�������
	void C(); //������
	void G(); //��������� ��������
	void K(TypeData dt); //������
	void T(); //for
	void O(bool flag_for); //��������
	int R(LEX id, TData *tdata); //������ ��������
	void L(); //������������� �������
	void M(); //�������
	TData X(); //������� ���������
	TData J(); //���������
	TData U(); //���������
	TData Y(); //���������
};