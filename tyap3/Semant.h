#pragma once

#include "defs.h"
#include "Semant.h"
#include "Scaner.h"


enum TypeData {
	TYPE_UNKNOWN = -1,
	TYPE_INT = 1,
	TYPE_DOUBLE = 2
};

enum TypeObject {
	EMPTY = -1,
	OBJ_VAR = 1,
	OBJ_ARR = 2,
	OBJ_FUNC = 3
};

union Value
{
	int integer;
	double dbl;
	int *arrayInt;
	double *arrayDouble;
};

struct TData {
	TypeData type;
	Value value;
	bool flag_allow_unn = false;
};

struct Node
{
	LEX id; // ������������� �������
	int TypeObject; // ��� ������� (����������, ������, �������)
	TypeData TypeData; //��� ������. ��� ������� ��� ������������� ��������
	Value value;
	int N; // ����������� �������
	int hg[MAX_N]; // ������� ������� ��������� �������
	int FlagInit; // ���� ��������� �������������
};


class Tree { // ������� ������������� �������	
	
public:
	static Tree * Cur; // ������� ������� ������
	Node * n; // ���������� �� ������� �������
	Tree * Up, *Left, *Right; // ��������, ����� � ������ �������
	static bool flagInterpret;
	
	// ������� ��������� ��������� ������
	Tree(Tree * l, Tree * r, Tree * u, Node * Data); // ����������� ������� ���� � ��������� ������� � �������
	Tree();
	~Tree() {}
	void SetLeft(Node * Data);
	void SetRight(Node * Data);
	Tree * FindUpOneLevel(Tree * From, LEX id);
	void SetCur(Tree * a);
	Tree * GetCur();
	void Print();
	Tree * FindUp(LEX id);
	Tree * FindUp(Tree * From, LEX id);
	void Tree::deleteSubTree(Tree *node); //�������� ��������� ������� � ������� node 
	bool Tree::GetFlagInterpret();
	void Tree::SetFlagInterpret(bool flag);



	// ������������� ������������
	int DupControl(Tree *Addr, LEX a); // �������� �������������� � �� ��������� �������� ������ �����
	TypeData Tree::DefDataType(LEX a); //����������� ���� t ������� a
	Tree * Tree::IncludeVar(TScaner *scan, LEX id, TypeData data_type); //��������� ������� ������������ � ����� ������ data_type � �������.
	Tree * Tree::IncludeArr(TScaner *scan, LEX id, TypeData data_type); //��������� ������� ������� � ����� ������ data_type � �������.
	void Tree::AddMeasure(int sz); //��������� ���������� �� ��������� ��������� �������
	Tree * Tree::IncludeFunc(TScaner *scan, LEX id, TypeData data_type); //��������� ������� id � ������, data_type - ��� ������������� ��������
	Tree * Tree::IncludeOper();//��������� ���������� ��������� � �������
	void Tree::SetInit(Tree * Vert); //���������� ���� �������������
	void Tree::CheckInit(TScaner *scan, Tree *vert, LEX id); //�������� ������������� ����������
	TData  Tree::CheckID(TScaner *scan,Tree *vert, LEX id); //�������� ������� �������������� � ������� � ������� ��� ���� � ��������(int,double)
	Tree * Tree::RetNearestFunc(Tree* vert); //���������� �������, � ������� ���������
	void Tree::CheckIndexInt(TScaner *scan,TData typedata); //�������� ������� �� ���������������
	void Tree::CheckCountIndex(TScaner *scan, int cnt, LEX id); //�������� ����������� ���������� ��������
	void Tree::CheckReturnVal(TScaner *scan, TData typedata); //�������� ������������ ������������ �������� �������� 
	void Tree::CheckAllowUnnOper(TScaner *scan, bool flag); //�������� ������������ ������� ��������
	Tree * Tree::GetElementFromTree(Tree* vert,LEX id);
};
