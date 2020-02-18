#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <string>
#include "Semant.h"
#include "Scaner.h"
#include "defs.h"

Tree* Tree::Cur = (Tree*)NULL;
int cnt_empty=1;
bool Tree::flagInterpret = true;

// ����������� ������� ���� � ��������� ������� � �������
Tree::Tree(Tree * l, Tree * r, Tree * u, Node * d) {
	n = new Node();
	Up = u; Left = l; Right = r; // ���������� ������
	memcpy(n, d, sizeof(Node)); // ���������� ������
	cnt_empty = 1;
}

// ����������� ������� ����� ���� � ������� ������� � �������
Tree::Tree(void) {
	n = new Node();
	Up = NULL; Left = NULL; Right = NULL;
	memcpy(n, &("------"), sizeof(Node));
	cnt_empty = 1;
}

// ������� ������ ������� �� ������� ������� this
void Tree::SetLeft(Node * Data) {
	Tree * a = new Tree(NULL, NULL, this, Data); // ����� �������
	Left = a; // ������� this � ����� ��������
}

// ������� ������� ������� �� ������� ������� this
void Tree::SetRight(Node * Data) {
	Tree * a = new Tree(NULL, NULL, this, Data); // ����� �������
	Right = a; // ������� this � ����� ��������
}

// ����� �������� id ����� �� ������ �� ������� ������� From.
// ����� �������������� �� ����� ������ ����������� �� ����� ������
Tree * Tree::FindUpOneLevel(Tree * From, LEX id) {
	Tree * i = From; // ������� ������� ������
	while ((i != NULL))
	{
		if (i->Up == NULL)
			break;
		if (i->Up->Right == i)
			break;

		if (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) == 0)
			return i; // �a��� ����������� �������������
		i = i->Up; // ����������� ������ �� ������
	}
	return NULL;
}

// ���������� ��������� ������ ������
void Tree::Print()
{
	printf("������� � ������� %-10s ----->", n->id);

	if (Left != NULL) 
		printf(" �����  ������ %s", Left->n->id);

	if (Right != NULL) 
		printf(" ������ ������ %s", Right->n->id);

	printf("\n");
	if (Left != NULL) 
		Left->Print();
	if (Right != NULL)
		Right->Print();
		
}

// ���������� ������� ���� ������
void Tree::SetCur(Tree * a) {
	Cur = a;
}
// �������� �������� �������� ���� ������
Tree * Tree::GetCur(void) {
	return Cur;
}

// ����� ������ � ������, ������� �� ������� ������� this
// �� ��� ����� ����� �� ������
Tree * Tree::FindUp(LEX id) {
	return FindUp(this, id);
}

// ����� ������ � ������ �� �������� ������� From
// �� ��� ����� ����� �� ������
Tree * Tree::FindUp(Tree * From, LEX id) {
	Tree * i = From; // ������� ������� ������
	while ((i != NULL) &&
		(memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Up; // ����������� ������ �� ������
	return i;
}


// �������� �������������� � �� ��������� �������� ������ �����.
// ����� �������������� ����� �� ������� Addr.
int Tree::DupControl(Tree* Addr, LEX a) {
	if (FindUpOneLevel(Addr, a) == NULL) return 0;
	return 1;
}

//����������� ���� t ������� lex
TypeData Tree::DefDataType(LEX a) {

	if (!flagInterpret)
		return TYPE_UNKNOWN;

	if (strcmp(a, "int") == 0)
		return TYPE_INT;
	else if (strcmp(a, "double") == 0)
		return TYPE_DOUBLE;
}

//��������� ������� ������������ � ����� ������ data_type � �������.
Tree * Tree::IncludeVar(TScaner *scan, LEX id, TypeData data_type) {

	if (!flagInterpret)
		return NULL;

	if (DupControl(Cur, id))
		scan->PrintError("��������� �������� �������������� ", id);
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

//��������� ������� ������� � ����� ������ data_type � �������.
Tree * Tree::IncludeArr(TScaner *scan, LEX id, TypeData data_type) {

	if (!flagInterpret)
		return NULL;

	if (DupControl(Cur, id))
		scan->PrintError("��������� �������� �������������� ", id);
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

//��������� ���������� �� ��������� ��������� �������
void Tree::AddMeasure(int sz) {

	if (!flagInterpret)
		return;

	Cur->n->hg[Cur->n->N] = sz;
	Cur->n->N ++;
}

//��������� ������� id � ������, data_type - ��� ������������� ��������
Tree * Tree::IncludeFunc(TScaner *scan, LEX id, TypeData data_type) {

	if (!flagInterpret)
		return NULL;

	if (DupControl(Cur, id))
		scan->PrintError("��������� �������� �������������� ", id);
	Tree * v; Node b;
	memcpy(b.id, id, strlen(id) + 1);
	b.TypeObject = OBJ_FUNC;
	b.TypeData = data_type;
	Cur->SetLeft(&b); // ������� ������� - �������
	Cur = Cur->Left;
	v = Cur; // ����� �������� ����� ������ �� �������

	char str[15] = "EMPTY";
	char cnt[5];
	sprintf(cnt, "%d", cnt_empty++);
	strcat(str, cnt);

	memcpy(&b.id, &str, strlen(str) + 1); b.TypeData = TYPE_UNKNOWN; b.TypeObject = EMPTY;
	Cur->SetRight(&b); // ������� ������ �������
	Cur = Cur->Right;
	return v;
}

//��������� ���������� ��������� � �������
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
	v = Cur; // ����� �������� �� for � ���������� ���������
	
	strcpy(str,"EMPTY");
	sprintf(cnt, "%d", ++cnt_empty);
	strcat(str, cnt);
	memcpy(&b.id, &str, strlen(str) + 1);

	Cur->SetRight(&b);
	Cur = Cur->Right;
	return v;
}

//���������� ���� �������������
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
		scan->PrintError("������������� ������������", id);
		return tdata;
	}
	else {
		tdata.type = v->n->TypeData;
		tdata.value = v->n->value;
		return tdata;
	}
}

//���������� �������, � ������� ���������
Tree * Tree::RetNearestFunc( Tree* vert) {

	if (!flagInterpret)
		return NULL;

	Tree * i = vert; // ������� ������� ������
	while ((i != NULL) &&
		(i->n->TypeObject != OBJ_FUNC))
		i = i->Up; // ����������� ������ �� ������
	return i;
}

//�������� ��������� ������� � ������� node 
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
		scan->PrintError("������ ������ ��������� ������������� ��������", "");
}

void Tree::CheckCountIndex(TScaner *scan, int cnt, LEX id) {

	if (!flagInterpret)
		return;

	Tree* v = FindUp(Cur, id);
	if (v->n->N != cnt)
		scan->PrintError("�������� ���������� ��������", "");
}

void Tree::CheckReturnVal(TScaner *scan, TData typedata) {

	if (!flagInterpret)
		return;

	Tree *v = RetNearestFunc(Cur);
	if (typedata.type != v->n->TypeData)
		scan->PrintError("�������� ��� ������������� ��������", "");
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
		scan->PrintError("������� �������� �����������", "");
}

void Tree::CheckInit(TScaner *scan, Tree *vert, LEX id) { //�������� ������������� ����������

	if (!flagInterpret)
		return ;

	Tree *v = FindUp(vert, id);

	if(v->n->FlagInit == false)
		scan->PrintError("������������� ������������� ����������", id);
}

Tree * Tree::GetElementFromTree(Tree* vert, LEX id) {

	if (!flagInterpret)
		return NULL;

	return FindUp(vert,id);
}

