#pragma once

#include "defs.h"

class TScaner {

private:
	
	TEXT t; //�������� �����
	int uk; // ��������� ������� ������� � �������� ������
	int line;
	int pos;
public:
	
	TScaner(char *); 
	~TScaner() {}

	void SetUK(int i); //���������� ������� ��������� � ������
	int GetUK(void); //�������� ������� ��������� � ������
	void SetLine(int l);
	int GetLine(void);
	void SetPos(int p);
	int GetPos(void);
	void PrintError(char *, char *); //������� ������ ������
	void PrintWarning(char *, char *); //������� ������ ��������������
	int Scaner(LEX l); //��������� �������
	void GetData(char *); // ����� ����� � �������� �������
	void SetText(char *);
};



