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
	LEX id; // идентификатор объекта
	int TypeObject; // тип объекта (переменная, массив, функция)
	TypeData TypeData; //тип данных. для функции тип возвращаемого значения
	Value value;
	int N; // размерность массива
	int hg[MAX_N]; // верхние границы измерений массива
	int FlagInit; // Флаг начальной инициализации
};


class Tree { // элемент семантической таблицы	
	
public:
	static Tree * Cur; // текущий элемент дерева
	Node * n; // информация об объекте таблицы
	Tree * Up, *Left, *Right; // родитель, левый и правый потомок
	static bool flagInterpret;
	
	// ФУНКЦИИ ОБРАБОТКИ БИНАРНОГО ДЕРЕВА
	Tree(Tree * l, Tree * r, Tree * u, Node * Data); // конструктор создает узел с заданными связями и данными
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
	void Tree::deleteSubTree(Tree *node); //удаление поддерева начиная с вершины node 
	bool Tree::GetFlagInterpret();
	void Tree::SetFlagInterpret(bool flag);



	// СЕМАНТИЧЕСКИЕ ПОДПРОГРАММЫ
	int DupControl(Tree *Addr, LEX a); // проверка идентификатора а на повторное описание внутри блока
	TypeData Tree::DefDataType(LEX a); //определение типа t лексемы a
	Tree * Tree::IncludeVar(TScaner *scan, LEX id, TypeData data_type); //занесение объекта “переменная” с типом данных data_type в таблицу.
	Tree * Tree::IncludeArr(TScaner *scan, LEX id, TypeData data_type); //занесение объекта “массив” с типом данных data_type в таблицу.
	void Tree::AddMeasure(int sz); //занесение информации об очередном измерении массива
	Tree * Tree::IncludeFunc(TScaner *scan, LEX id, TypeData data_type); //занесение функции id в дерево, data_type - тип возвращаемого значения
	Tree * Tree::IncludeOper();//занесение составного оператора в таблицу
	void Tree::SetInit(Tree * Vert); //установить флаг инициализации
	void Tree::CheckInit(TScaner *scan, Tree *vert, LEX id); //проверка инициализации переменной
	TData  Tree::CheckID(TScaner *scan,Tree *vert, LEX id); //проверка наличия идентификатора в таблице и возврат его типа и значения(int,double)
	Tree * Tree::RetNearestFunc(Tree* vert); //нахождение функции, в которой находимся
	void Tree::CheckIndexInt(TScaner *scan,TData typedata); //проверка индекса на целочисленность
	void Tree::CheckCountIndex(TScaner *scan, int cnt, LEX id); //проверка коректности количества индексов
	void Tree::CheckReturnVal(TScaner *scan, TData typedata); //проверка соответствия возращаемого функцией значения 
	void Tree::CheckAllowUnnOper(TScaner *scan, bool flag); //проверка допустимости унарных операций
	Tree * Tree::GetElementFromTree(Tree* vert,LEX id);
};
