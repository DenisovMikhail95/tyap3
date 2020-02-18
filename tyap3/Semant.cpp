#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <string>
#include "Semant.h"
#include "Scaner.h"
#include "defs.h"

Tree* Tree::Cur = (Tree*)NULL;
int cnt_empty=1;
bool Tree::flagInterpret = true;

// конструктор создает узел с заданными связями и данными
Tree::Tree(Tree * l, Tree * r, Tree * u, Node * d) {
	n = new Node();
	Up = u; Left = l; Right = r; // установили ссылки
	memcpy(n, d, sizeof(Node)); // установили данные
	cnt_empty = 1;
}

// конструктор создает новый узел с пустыми связями и данными
Tree::Tree(void) {
	n = new Node();
	Up = NULL; Left = NULL; Right = NULL;
	memcpy(n, &("------"), sizeof(Node));
	cnt_empty = 1;
}

// создать левого потомка от текущей вершины this
void Tree::SetLeft(Node * Data) {
	Tree * a = new Tree(NULL, NULL, this, Data); // новая вершина
	Left = a; // связали this с новой вершиной
}

// создать правого потомка от текущей вершины this
void Tree::SetRight(Node * Data) {
	Tree * a = new Tree(NULL, NULL, this, Data); // новая вершина
	Right = a; // связали this с новой вершиной
}

// Поиск элемента id вверх по дереву от текущей вершины From.
// Поиск осуществляется на одном уровне вложенности по левым связям
Tree * Tree::FindUpOneLevel(Tree * From, LEX id) {
	Tree * i = From; // текущая вершина поиска
	while ((i != NULL))
	{
		if (i->Up == NULL)
			break;
		if (i->Up->Right == i)
			break;

		if (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) == 0)
			return i; // нaшли совпадающий идентификатор
		i = i->Up; // поднимаемся наверх по связям
	}
	return NULL;
}

// отладочная программа печати дерева
void Tree::Print()
{
	printf("Вершина с данными %-10s ----->", n->id);

	if (Left != NULL) 
		printf(" слева  данные %s", Left->n->id);

	if (Right != NULL) 
		printf(" справа данные %s", Right->n->id);

	printf("\n");
	if (Left != NULL) 
		Left->Print();
	if (Right != NULL)
		Right->Print();
		
}

// установить текущий узел дерева
void Tree::SetCur(Tree * a) {
	Cur = a;
}
// получить значение текущего узла дерева
Tree * Tree::GetCur(void) {
	return Cur;
}

// поиск данных в дереве, начиная от текущей вершины this
// до его корня вверх по связям
Tree * Tree::FindUp(LEX id) {
	return FindUp(this, id);
}

// поиск данных в дереве от заданной вершины From
// до его корня вверх по связям
Tree * Tree::FindUp(Tree * From, LEX id) {
	Tree * i = From; // текущая вершина поиска
	while ((i != NULL) &&
		(memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Up; // поднимаемся наверх по связям
	return i;
}


// Проверка идентификатора а на повторное описание внутри блока.
// Поиск осуществляется вверх от вершины Addr.
int Tree::DupControl(Tree* Addr, LEX a) {
	if (FindUpOneLevel(Addr, a) == NULL) return 0;
	return 1;
}

//определение типа t лексемы lex
TypeData Tree::DefDataType(LEX a) {

	if (!flagInterpret)
		return TYPE_UNKNOWN;

	if (strcmp(a, "int") == 0)
		return TYPE_INT;
	else if (strcmp(a, "double") == 0)
		return TYPE_DOUBLE;
}

//занесение объекта “переменная” с типом данных data_type в таблицу.
Tree * Tree::IncludeVar(TScaner *scan, LEX id, TypeData data_type) {

	if (!flagInterpret)
		return NULL;

	if (DupControl(Cur, id))
		scan->PrintError("Повторное описание идентификатора ", id);
	Node b;
	memcpy(b.id, id, strlen(id) + 1);
	b.TypeData = data_type;
	b.TypeObject = OBJ_VAR;
	b.FlagInit = 0;
	b.N = 0;
	Cur->SetLeft(&b);
	Cur = Cur->Left;
	return Cur;
}

//занесение объекта “массив” с типом данных data_type в таблицу.
Tree * Tree::IncludeArr(TScaner *scan, LEX id, TypeData data_type) {

	if (!flagInterpret)
		return NULL;

	if (DupControl(Cur, id))
		scan->PrintError("Повторное описание идентификатора ", id);
	Node b;
	memcpy(b.id, id, strlen(id) + 1);
	b.TypeData = data_type;
	b.TypeObject = OBJ_ARR;
	b.FlagInit = 0;
	b.N = 0;
	Cur->SetLeft(&b);
	Cur = Cur->Left;
	return Cur;
}

//занесение информации об очередном измерении массива
void Tree::AddMeasure(int sz) {

	if (!flagInterpret)
		return;

	Cur->n->hg[Cur->n->N] = sz;
	Cur->n->N ++;
}

//занесение функции id в дерево, data_type - тип возвращаемого значения
Tree * Tree::IncludeFunc(TScaner *scan, LEX id, TypeData data_type) {

	if (!flagInterpret)
		return NULL;

	if (DupControl(Cur, id))
		scan->PrintError("Повторное описание идентификатора ", id);
	Tree * v; Node b;
	memcpy(b.id, id, strlen(id) + 1);
	b.TypeObject = OBJ_FUNC;
	b.TypeData = data_type;
	Cur->SetLeft(&b); // сделали вершину - функцию
	Cur = Cur->Left;
	v = Cur; // точка возврата после выхода из функции

	char str[15] = "EMPTY";
	char cnt[5];
	sprintf(cnt, "%d", cnt_empty++);
	strcat(str, cnt);

	memcpy(&b.id, &str, strlen(str) + 1); b.TypeData = TYPE_UNKNOWN; b.TypeObject = EMPTY;
	Cur->SetRight(&b); // сделали пустую вершину
	Cur = Cur->Right;
	return v;
}

//занесение составного оператора в таблицу
Tree * Tree::IncludeOper() {

	if (!flagInterpret)
		return NULL;

	Tree * v; Node b;

	char str[15] = "EMPTY";
	char cnt[5];
	sprintf(cnt, "%d", cnt_empty++);
	strcat(str, cnt);

	memcpy(&b.id, &str, strlen(str) + 1); b.TypeData = TYPE_UNKNOWN; b.TypeObject = EMPTY;
	Cur->SetLeft(&b);
	Cur = Cur->Left;
	v = Cur; // точка возврата из for и составного оператора
	
	strcpy(str,"EMPTY");
	sprintf(cnt, "%d", ++cnt_empty);
	strcat(str, cnt);
	memcpy(&b.id, &str, strlen(str) + 1);

	Cur->SetRight(&b);
	Cur = Cur->Right;
	return v;
}

//установить флаг инициализации
void Tree::SetInit(Tree * Vert) {

	if (!flagInterpret)
		return;

	Vert->n->FlagInit = true;
}

TData Tree::CheckID(TScaner *scan,Tree *vert,LEX id) {
	
	TData tdata;
	tdata.type = TYPE_UNKNOWN;
	if (!flagInterpret)
		return tdata;

	Tree *v = FindUp(vert,id);
	if (v == NULL) {
		scan->PrintError("Необъявленный идентифкатор", id);
		return tdata;
	}
	else {
		tdata.type = v->n->TypeData;
		tdata.value = v->n->value;
		return tdata;
	}
}

//нахождение функции, в которой находимся
Tree * Tree::RetNearestFunc( Tree* vert) {

	if (!flagInterpret)
		return NULL;

	Tree * i = vert; // текущая вершина поиска
	while ((i != NULL) &&
		(i->n->TypeObject != OBJ_FUNC))
		i = i->Up; // поднимаемся наверх по связям
	return i;
}

//удаление поддерева начиная с вершины node 
void Tree::deleteSubTree(Tree *node) {

	if (!flagInterpret)
		return;

	if (node->Left != NULL)
		deleteSubTree(node->Left);
	delete node;
}

void Tree::CheckIndexInt(TScaner *scan, TData typedata) {

	if (!flagInterpret)
		return;

	if (typedata.type != TYPE_INT)
		scan->PrintError("Индекс должен принимать целочисленное занчение", "");
}

void Tree::CheckCountIndex(TScaner *scan, int cnt, LEX id) {

	if (!flagInterpret)
		return;

	Tree* v = FindUp(Cur, id);
	if (v->n->N != cnt)
		scan->PrintError("Неверное количество индексов", "");
}

void Tree::CheckReturnVal(TScaner *scan, TData typedata) {

	if (!flagInterpret)
		return;

	Tree *v = RetNearestFunc(Cur);
	if (typedata.type != v->n->TypeData)
		scan->PrintError("Неверный тип возвращаемого значения", "");
}

bool Tree::GetFlagInterpret() {
	return flagInterpret;
}
void Tree::SetFlagInterpret(bool flag) {
	flagInterpret = flag;
}

void Tree::CheckAllowUnnOper(TScaner *scan, bool flag) {
	if (!flagInterpret)
		return;

	if (flag == false)
		scan->PrintError("Унарная операция недопустима", "");
}

void Tree::CheckInit(TScaner *scan, Tree *vert, LEX id) { //проверка инициализации переменной

	if (!flagInterpret)
		return ;

	Tree *v = FindUp(vert, id);

	if(v->n->FlagInit == false)
		scan->PrintError("Использование необъявленной переменной", id);
}

Tree * Tree::GetElementFromTree(Tree* vert, LEX id) {

	if (!flagInterpret)
		return NULL;

	return FindUp(vert,id);
}

