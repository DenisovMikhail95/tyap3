#include "defs.h"
#include "Scaner.h"

LEX Keyword[MAX_KEYW] = { "int", "double", "for", "return" };
int IndexKeyword[MAX_KEYW] = { T_int, T_double, T_for, T_return };

TScaner::TScaner(char * FileName) { //�����������
	GetData(FileName);
	SetUK(0);
	line = 1;
	pos = 1;
}

void TScaner::SetUK(int i) { uk = i;} //������������ ���������

int TScaner::GetUK(void) { return uk;} // ����� ���������

void TScaner::SetLine(int l) { line = l; } 
int TScaner::GetLine(void) { return line; } 
void TScaner::SetPos(int p) { pos = p; }
int TScaner::GetPos(void) { return pos; }

void TScaner::PrintError(char * err, char * a) {
	printf("������(������ %d, ������� %d) : %s       %s\n", line, pos, a, err);
	printf("\n\n");
	system("pause");
	exit(0);
}
void TScaner::PrintWarning(char *err, char *a) {
	printf("������(������ %d, ������� %d) : %s       %s\n", line, pos, a, err);
	printf("\n");
}

void TScaner::GetData(char * FileName) // ���� ����� � �������� �������
{
	char aa;
	FILE * in = fopen(FileName, "r");
	if (in == NULL) {
		printf("\n����������� ������� ����\n");
		system("pause");
		exit(1);
	}
	int i = 0;
	while (!feof(in))
	{
		fscanf(in, "%c", &aa);
		if (!feof(in)) t[i++] = aa;
		if (i >= MAX_TEXT - 1)
		{
			printf("\n������� ������� ������ ��������� ������\n");
			break;
		}
	}
	t[i] = '\0'; // ����������� ���� �\0� � ����� ������
	fclose(in);
}

int TScaner::Scaner(LEX l) {

	int i; // ������� ����� �������
	for (i = 0; i < MAX_LEX; i++) 
		l[i] = 0; //�������� ���� �������
	i = 0; // ������� ����������� � ������� i

	start:
	while ((t[uk] == ' ') || (t[uk] == '\n') || (t[uk] == '\t')) { //������, ������� ������, ���������
		if (t[uk] == ' ')
			pos++;
		else if (t[uk] == '\n') {
			pos = 0;
			line++;
		}
		else if (t[uk] == '\t')
			pos += 8;
		
		uk++;	
	}

	if ((t[uk] ==  '/') && (t[uk + 1] == '/')) {  	// ������� ���������� ���������
		// ������� �����������, ���� ���������� ����� �� �\n�
		uk = uk + 2; pos = pos + 2;

		while ((t[uk] != '\n')) {
			if (t[uk] == '\0') {
				return T_end;
			}

			uk++; pos++;
		}
		goto start;
	}

	if (t[uk] == '\0'){
		strcpy(l,"����� ������"); 
		return T_end;
	}

	if (t[uk] <= '9' && t[uk] >= '0') { //��������� �����

		while (((t[uk] <= '9') && (t[uk] >= '0'))) {
			if (i < MAX_LEX - 1) {
				l[i++] = t[uk++]; pos++;
			}
			else {
				uk++; pos++;
			}
		}

		if (((t[uk] >= 'a') && (t[uk] <= 'z')) || ((t[uk] >= 'A') && (t[uk] <= 'Z'))) {
			while (t[uk] != ' ' && t[uk] != '\n') {
				l[i++] = t[uk++]; pos++;
			}
			PrintError("������������ ���������", l);
			return T_error;
		}
		
		
		return T_const_int;
		
	}
	else if (t[uk] == '\'') { //��������� ����������
		
		uk++; pos++;

		if (t[uk] == '\\') {
			l[i++] = t[uk++]; pos++;
			l[i++] = t[uk++]; pos++;
		}
		else {
			l[i++] = t[uk++]; pos++;
		}

		if (t[uk] != '\'') {
			if (l[i] != '\'') {
				l[i - 1] = '\0';
				i--;
			}
			PrintError("�������� ���������� ���������", l);
			return T_error;
		}
		uk++; pos++;
		return T_const_char;
	}
	else if (((t[uk] >= 'a') && (t[uk] <= 'z')) || ((t[uk] >= 'A') && (t[uk] <= 'Z'))) { //�������������

		l[i++] = t[uk++]; pos++;
		while (((t[uk] <= '9') && (t[uk] >= '0')) ||
			((t[uk] >= 'a') && (t[uk] <= 'z')) ||
			((t[uk] >= 'A') && (t[uk] <= 'Z')) || t[uk] == '_') {
			
			if (i < MAX_LEX - 1) {
				l[i++] = t[uk++]; pos++;
			}
			else {
				uk++; pos++;
			}
		}
		//�������� �� �������� �����:
		for (int j = 0; j < MAX_KEYW; j++) 
			if (strcmp(l, Keyword[j]) == 0) 
				return IndexKeyword[j];
		return T_ident;
	}


	else if (t[uk] == '>') {
		l[i++] = t[uk++]; pos++;
		if (t[uk] == '=') {
			l[i++] = t[uk++]; pos++;
			return T_eq_more;
		}
		return T_more;
	}
	else if (t[uk] == '<') {
		l[i++] = t[uk++]; pos++;
		if (t[uk] == '=') {
			l[i++] = t[uk++]; pos++;
			return T_eq_less;
		}
		return T_less;
	}
	else if (t[uk] == '=') {
		l[i++] = t[uk++]; pos++;
		if (t[uk] == '=') {
			l[i++] = t[uk++]; pos++;
			return T_eq;
		}
		return T_save;
	}
	else if (t[uk] == '+') {
		l[i++] = t[uk++]; pos++;
		if (t[uk] == '+') {
			l[i++] = t[uk++]; pos++;
			return T_un_plus;
		}
		return T_plus;
	}
	else if (t[uk] == '-') {
		l[i++] = t[uk++]; pos++;
		if (t[uk] == '-') {
			l[i++] = t[uk++]; pos++;
			return T_un_minus;
		}
		return T_minus;
	}
	else if (t[uk] == '!') {
		l[i++] = t[uk++]; pos++;
		if (t[uk] == '=') {
			l[i++] = t[uk++]; pos++;
			return T_neq;
		}
		return T_not;
	}

	
	else if (t[uk] == ',')
	{
		l[i++] = t[uk++]; pos++; return T_comma;
	}
	else if (t[uk] == ';')
	{
		l[i++] = t[uk++]; pos++; return T_dot_com;
	}
	else if (t[uk] == '(')
	{
		l[i++] = t[uk++]; pos++; return T_LB;
	}
	else if (t[uk] == ')')
	{
		l[i++] = t[uk++]; pos++; return T_RB;
	}
	else if (t[uk] == '{')
	{
		l[i++] = t[uk++]; pos++; return T_FLB;
	}
	else if (t[uk] == '}')
	{
		l[i++] = t[uk++]; pos++; return T_FRB;
	}
	else if (t[uk] == '[')
	{
		l[i++] = t[uk++]; pos++; return T_SQLB;
	}
	else if (t[uk] == ']')
	{
		l[i++] = t[uk++]; pos++; return T_SQRB;
	}
	else if (t[uk] == '*')
	{
		l[i++] = t[uk++]; pos++; return T_mult;
	}
	else if (t[uk] == '/')
	{
		l[i++] = t[uk++]; pos++; return T_div;
	}
	else if (t[uk] == '%')
	{
		l[i++] = t[uk++]; pos++; return T_mod;
	}


	else {
		l[i++] = t[uk++]; pos++;
		PrintError("�������� ������", l); // ������
		return T_error;
	}
	
}

void TScaner::SetText(char *text) {
	strcpy(t,text);
}