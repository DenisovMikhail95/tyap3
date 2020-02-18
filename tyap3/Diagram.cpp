#include "defs.h"
#include "Scaner.h"
#include "Diagram.h"

void TDiagram::S() {

	LEX l; int type, uk1; int ps, ln;

	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
	while (type == T_int || type == T_double) {
		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
		type = sc->Scaner(l);
		type = sc->Scaner(l);
		sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
		if (type == T_LB) {
			E();
		}
		else {
			C();
		}
		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
		sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
	}
	if (type != T_end) {
		sc->PrintError("Ожидалось дальнейшее определение данных или функций, либо конец файла", l);
	}

	printf("\n\n");
	Root->Print();
}

void TDiagram::E() {
	
	LEX l; int type, uk1; 
	type = sc->Scaner(l);
	if (type != T_int && type != T_double)
		sc->PrintError("Ожидалось определение типа (int,double)",l);

	TypeData data_type = Root->DefDataType(l);

	type = sc->Scaner(l);
	if (type != T_ident)
		sc->PrintError("Ожидалcя идентификатор", l);

	Tree *pnt_ret = Root->IncludeFunc(sc,l,data_type);

	type = sc->Scaner(l);
	if (type != T_LB)
		sc->PrintError("Ожидался символ (", l);
	type = sc->Scaner(l);
	if (type != T_RB)
		sc->PrintError("Ожидался символ )", l);
	
	G();
	Root->Cur = pnt_ret;

	//удаление блока
	if (Root->Cur != NULL) {
		Root->deleteSubTree(Root->Cur->Right);
		Root->Cur->Right = NULL;
	}
}

void TDiagram::C() {

	LEX l; int type, uk1; int ps, ln; TData tdJ;
	type = sc->Scaner(l);
	if (type != T_int && type != T_double)
		sc->PrintError("Ожидалось определение типа (int,double)", l);
	
	TypeData data_type = Root->DefDataType(l);

	do {
		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
		type = sc->Scaner(l);
		sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);

		if (type == T_SQLB) {
			K(data_type);
			uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
			type = sc->Scaner(l);
		}
		else {
			uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
			type = sc->Scaner(l);
			if (type != T_ident)
				sc->PrintError("Ожидался идентификатор", l);

			Tree* CreatedVar = Root->IncludeVar(sc,l,data_type); //адрес переменной для присваивания

			uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
			type = sc->Scaner(l);
			if (type == T_save) {
				tdJ = J(); // получаем тип и значение выражения
				//запомнить вычесленное значение 
				if (Root->flagInterpret) {
					switch (CreatedVar->n->TypeData)
					{
					case TYPE_INT:
						switch (tdJ.type)
						{
						case TYPE_INT:
							CreatedVar->n->value.integer = tdJ.value.integer;
							break;
						case TYPE_DOUBLE:
							CreatedVar->n->value.integer = tdJ.value.dbl;
							break;
						default:
							break;
						}
						break;
					case TYPE_DOUBLE:
						switch (tdJ.type)
						{
						case TYPE_INT:
							CreatedVar->n->value.dbl = tdJ.value.integer;
							break;
						case TYPE_DOUBLE:
							CreatedVar->n->value.dbl = tdJ.value.dbl;
							break;
						default:
							break;
						}
						break;
					default:
						break;
					}
					if (CreatedVar->n->TypeData == TYPE_INT)
						printf("\nИдентификатор: %s. Новое значение: %d\n", CreatedVar->n->id, CreatedVar->n->value.integer);
					else
						printf("\nИдентификатор: %s. Новое значение: %f\n", CreatedVar->n->id, CreatedVar->n->value.dbl);
				}
				Root->SetInit(CreatedVar);
				uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
				type = sc->Scaner(l);
			}
		}

	} while (type == T_comma);
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);


	type = sc->Scaner(l);
	if (type != T_dot_com)
		sc->PrintError("Ожидался символ ;", l);
}

void TDiagram::G() {

	LEX l; int type, uk1; int ps, ln;

	type = sc->Scaner(l);
	if(type != T_FLB)
		sc->PrintError("Ожидался символ {", l);

	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
	while (type == T_int || type == T_double || type == T_ident ||
		type == T_for || type == T_FLB || type == T_return || type == T_dot_com) {
		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
		sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
		if (type == T_int || type == T_double) {
			C();
		}
		else {
			O(false);
		}
		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
		sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
	}

	type = sc->Scaner(l);
	if (type != T_FRB)
		sc->PrintError("Ожидался символ }", l);
	
}


void TDiagram::K(TypeData dt) {
	
	LEX l; int type, uk1; int ps, ln;
	type = sc->Scaner(l);
	if (type != T_ident)
		sc->PrintError("Ожидался идентификатор", l);

	Tree* CreatedArr = Root->IncludeArr(sc,l, dt);

	do{
		type = sc->Scaner(l);
		if (type != T_SQLB)
			sc->PrintError("Ожидался символ [", l);
		type = sc->Scaner(l);

		int info_mesuare = atoi(l);

		if (type != T_const_int)
			sc->PrintError("Ожидалась целая константа", l);
		type = sc->Scaner(l);
		if (type != T_SQRB)
			sc->PrintError("Ожидался символ ]", l);

		Root->AddMeasure(info_mesuare);

		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
		sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
	} while (type == T_SQLB);

	if (Root->flagInterpret == true) {
		if (CreatedArr->n->TypeData == TYPE_INT) {
			int size = 1;
			for (int i = 0; i < CreatedArr->n->N; i++)
				size *= CreatedArr->n->hg[i];
			int *arr = new int[size];
			CreatedArr->n->value.arrayInt = arr;
		}
		else {
			int size = 1;
			for (int i = 0; i < CreatedArr->n->N; i++)
				size *= CreatedArr->n->hg[i];
			double *arr = new double[size];
			CreatedArr->n->value.arrayDouble = arr;
		}
	}

	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l); 
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
	if (type == T_save) {
		type = sc->Scaner(l);
		type = sc->Scaner(l);
		if(type != T_FLB)
			sc->PrintError("Ожидался символ {", l);
		L();
		type = sc->Scaner(l);
		if (type != T_FRB)
			sc->PrintError("Ожидался символ }", l);
		
		Root->SetInit(CreatedArr);
	}

}


void TDiagram::T() {
	LEX l, lx; int type, uk1; int ps, ln;
	int flag_not_decl = true; TypeData tip; TData tdJ;
	Tree *pnt_ret = Root->IncludeOper();
	Tree *CreatedVar;

	type = sc->Scaner(l);
	if (type != T_for)
		sc->PrintError("Ожидался for", l);
	type = sc->Scaner(l);
	if (type != T_LB)
		sc->PrintError("Ожидался символ (", l);
	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	if (type != T_int && type != T_double) {
		sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps); flag_not_decl = false;
	}
	if (flag_not_decl) {
		tip = Root->DefDataType(l);
	}
	type = sc->Scaner(l);
	strcpy(lx, l);
	if (type != T_ident)
		sc->PrintError("Ожидался идентификатор", l);
	if(flag_not_decl)
		CreatedVar = Root->IncludeVar(sc,l, tip);
	else{
		Root->CheckID(sc, Root->Cur, l);
	}
	Tree *vertElem = Root->GetElementFromTree(Root->Cur, l);
	type = sc->Scaner(l);
	if (type != T_save)
		sc->PrintError("Ожидался символ =", l);
	tdJ = J();//получаем тип и значение выражения
			  //запомнить вычесленное значение 
	if (Root->flagInterpret) {
		switch (vertElem->n->TypeData)
		{
		case TYPE_INT:
			switch (tdJ.type)
			{
			case TYPE_INT:
				vertElem->n->value.integer = tdJ.value.integer;
				break;
			case TYPE_DOUBLE:
				vertElem->n->value.integer = tdJ.value.dbl;
				break;
			default:
				break;
			}
			break;
		case TYPE_DOUBLE:
			switch (tdJ.type)
			{
			case TYPE_INT:
				vertElem->n->value.dbl = tdJ.value.integer;
				break;
			case TYPE_DOUBLE:
				vertElem->n->value.dbl = tdJ.value.dbl;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		if (vertElem->n->TypeData == TYPE_INT)
			printf("\nИдентификатор: %s. Новое значение: %d\n", vertElem->n->id, vertElem->n->value.integer);
		else
			printf("\nИдентификатор: %s. Новое значение: %f\n", vertElem->n->id, vertElem->n->value.dbl);
	}


	type = sc->Scaner(l);
	if (type != T_dot_com)
		sc->PrintError("Ожидался символ ;", l);
	J();
	type = sc->Scaner(l);
	if (type != T_dot_com)
		sc->PrintError("Ожидался символ ;", l);
	J();
	type = sc->Scaner(l);
	if (type != T_RB)
		sc->PrintError("Ожидался символ )", l);
	O(true);

	Root->Cur = pnt_ret;

	//удаление поддерева for
	if (Root->Cur != NULL) {
		Root->deleteSubTree(Root->Cur->Right);
		Root->Cur->Right = NULL;
	}
}

int TDiagram::R(LEX id, TData *tdata) {
	LEX l; int type, uk1; int ps, ln;
	int count = 0; TData tip;
	int offset = 0; //смещение

	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);

	//получим вершину с массивом, чтобы узнать информацию о измерениях
	Tree *vertArr = Root->GetElementFromTree(Root->Cur,id);
	
	while (type == T_SQLB) {
		type = sc->Scaner(l);
		tip = J();
		Root->CheckIndexInt(sc,tip); //проверка на целочисленность индекса
		
		if (Root->flagInterpret)
			if((vertArr->n->hg[count] - 1 ) < tip.value.integer)
				sc->PrintError("Выход за пределы массива","");

		type = sc->Scaner(l);
		if (type != T_SQRB)
			sc->PrintError("Ожидался символ ]", l);
		
		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
		sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);

		if (vertArr != NULL)
			offset = offset*vertArr->n->hg[count] + tip.value.integer; //вычисление смещения

		count++;
	}
	
	Root->CheckCountIndex(sc,count,id); //проверка коректности количества индексов

	//выборка значения из массива
	if(Root->flagInterpret)
	{
		if (tdata->type == TYPE_INT)
			tdata->value.integer = vertArr->n->value.arrayInt[offset];
		if (tdata->type == TYPE_DOUBLE)
			tdata->value.dbl = vertArr->n->value.arrayDouble[offset];
	}

	return offset;
	
}

void TDiagram::L() {
	LEX l; int type, uk1; int ps, ln;

	do {
		M();
		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
	} while (type == T_comma);
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
}


void TDiagram::M() {
	LEX l; int type, uk1; int ps, ln;

	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);

	if (type == T_FLB) {
		type = sc->Scaner(l);
		L();
		type = sc->Scaner(l);
		if (type != T_FRB)
			sc->PrintError("Ожидался символ }", l);
	}
	else {
		J();
	}
}


void TDiagram::O(bool flag_for) {
	LEX l; int type, uk1;  int ps, ln; Tree *pnt_ret, *vertElem; TData td, tdJ;
	LEX lx_param; int offset;

	uk1 = sc->GetUK();ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
	if (type == T_ident) {
		type = sc->Scaner(l);
		td = Root->CheckID(sc, Root->Cur, l); //проверка на наличие в таблице
		vertElem = Root->GetElementFromTree(Root->Cur,l); //получить адрес переменной для присваивания
		strcpy(lx_param,l);
		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
		sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
		if (type != T_save) {
			offset = R(lx_param, &td);
		}
		type = sc->Scaner(l);
		if (type != T_save)
			sc->PrintError("Ожидался символ =", l);
		tdJ = J(); //получили тип и значение выражения
		//запомнить вычесленное значение 
		if (Root->flagInterpret) {
			switch (vertElem->n->TypeData)
			{
			case TYPE_INT:
				switch (tdJ.type)
				{
				case TYPE_INT:
					if (vertElem->n->TypeObject == OBJ_ARR)
						vertElem->n->value.arrayInt[offset] = tdJ.value.integer;
					else
						vertElem->n->value.integer = tdJ.value.integer;
					break;
				case TYPE_DOUBLE:
					if (vertElem->n->TypeObject == OBJ_ARR)
						vertElem->n->value.arrayInt[offset] = tdJ.value.dbl;
					else
						vertElem->n->value.integer = tdJ.value.dbl;
					break;
				default:
					break;
				}
				break;
			case TYPE_DOUBLE:
				switch (tdJ.type)
				{
				case TYPE_INT:
					if (vertElem->n->TypeObject == OBJ_ARR)
						vertElem->n->value.arrayDouble[offset] = tdJ.value.integer;
					else
						vertElem->n->value.dbl = tdJ.value.integer;
					break;
				case TYPE_DOUBLE:
					if (vertElem->n->TypeObject == OBJ_ARR)
						vertElem->n->value.arrayDouble[offset] = tdJ.value.dbl;
					else
						vertElem->n->value.dbl = tdJ.value.dbl;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}

			if (vertElem->n->TypeData == TYPE_INT)
				if(tdJ.type == TYPE_INT)
					printf("\nИдентификатор: %s. Новое значение: %d\n", vertElem->n->id, tdJ.value.integer);
				else
					printf("\nИдентификатор: %s. Новое значение: %d\n", vertElem->n->id, tdJ.value.dbl);
			else
				if (tdJ.type == TYPE_INT)
					printf("\nИдентификатор: %s. Новое значение: %f\n", vertElem->n->id, (double)tdJ.value.integer);
				else
					printf("\nИдентификатор: %s. Новое значение: %f\n", vertElem->n->id, tdJ.value.dbl);


		}

		type = sc->Scaner(l);
		if (type != T_dot_com)
			sc->PrintError("Ожидался символ ;", l);	
	}
	else if (type == T_for) {
		T();
	}
	else if (type == T_FLB) {
		if(flag_for == false)
			pnt_ret = Root->IncludeOper();
		G();
		if (flag_for == false)
			Root->Cur = pnt_ret;
		
		//удаление блока
		if (flag_for == false) {
			if (Root->Cur != NULL) {
				Root->deleteSubTree(Root->Cur->Right);
				Root->Cur->Right = NULL;
			}
		}
	}
	else if (type == T_return) {
		type = sc->Scaner(l);
		TData tp = J();

		Root->CheckReturnVal(sc, tp);//проверка соответствия возращаемого функцией значения 


		type = sc->Scaner(l);
		if (type != T_dot_com)
			sc->PrintError("Ожидался символ ;", l);
	}
	else if (type == T_dot_com) {
		type = sc->Scaner(l);
		
	}
	else if (type == T_int || type == T_double) {
		C();
	}
	else
		sc->PrintError("Неправильная конструкция for","");
}

TData TDiagram::X() {
	LEX l; int type, uk1; int ps, ln;
	LEX lx_param;  TData td;

	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);

	if (type == T_ident) {
		type = sc->Scaner(l);
		td = Root->CheckID(sc,Root->Cur,l); //проверка на наличие в таблице и получение типа и значения
		strcpy(lx_param,l);
		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
		sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
		if (type == T_SQLB) {
			R(lx_param, &td);
		}
		td.flag_allow_unn = true;
	}
	else if (type == T_const_int || type == T_const_char) {
		type = sc->Scaner(l);
		if (type == T_const_char) {
			char ch = l[0];
			td.value.integer = ch;
		}
		else
			td.value.integer = atoi(l);
		td.type = TYPE_INT;
		return td;
	}
	else if (type == T_LB) {
		type = sc->Scaner(l);
		td = J();
		type = sc->Scaner(l);
		if (type != T_RB)
			sc->PrintError("Ожидался символ )", l);
	}
	else if (type == T_RB) {
		sc->PrintError("Пустое выражение", l);
	}
	else {
		sc->PrintError("Некоректное выражение","");
	}
	return td;
}

TData TDiagram::J() {
	LEX l; int type, uk1; int ps, ln; TData td;
	
	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);

	if (type == T_dot_com) {
		sc->PrintError("Не указано выражение","");
	}
	if (type == T_un_minus || type == T_un_plus) {
		type = sc->Scaner(l);
		td = U();
		Root->CheckAllowUnnOper(sc,td.flag_allow_unn);//проверка допустимости унарных операций
		if (type == T_un_minus) {
			switch (td.type)
			{
			case TYPE_INT:
				td.value.integer--;
				break;
			case TYPE_DOUBLE:
				td.value.dbl--;
				break;
			default:
				break;
			}
		}
		else {
			switch (td.type)
			{
			case TYPE_INT:
				td.value.integer++;
				break;
			case TYPE_DOUBLE:
				td.value.dbl++;
				break;
			default:
				break;
			}
		}
	}
	else {
		td = U();
		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
		if(type != T_un_minus && type != T_un_plus){
			sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
		}
		else {
			Root->CheckAllowUnnOper(sc, td.flag_allow_unn); //проверка допустимости унарных операций
			if (type == T_un_minus) {
				switch (td.type)
				{
				case TYPE_INT:
					td.value.integer--;
					break;
				case TYPE_DOUBLE:
					td.value.dbl--;
					break;
				default:
					break;
				}
			}
			else {
				switch (td.type)
				{
				case TYPE_INT:
					td.value.integer++;
					break;
				case TYPE_DOUBLE:
					td.value.dbl++;
					break;
				default:
					break;
				}
			}
		}
	}
	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	while ((type >= T_less && type <= T_eq) || type == T_neq)
	{
		TData td0 = td;
		td = U();
		td.flag_allow_unn = false;
		
		switch (td0.type)//тип первого операнда
		{
		case TYPE_INT: //первый операнд int
			switch (td.type) // тип второго операнда
			{
			case TYPE_INT: //второй операнд int
				if (type == T_less)
					if (td0.value.integer < td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				else if (type == T_more)
					if (td0.value.integer > td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				else if (type == T_eq_less)
					if (td0.value.integer <= td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				else if (type == T_eq_more)
					if (td0.value.integer >= td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				else if (type == T_eq)
					if (td0.value.integer == td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				else if (type == T_neq)
					if (td0.value.integer != td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				break;
			case TYPE_DOUBLE: //второй операнд double
				if (type == T_less)
					if (td0.value.integer < td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				else if (type == T_more)
					if (td0.value.integer > td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				else if (type == T_eq_less)
					if (td0.value.integer <= td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				else if (type == T_eq_more)
					if (td0.value.integer >= td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				else if (type == T_eq)
					if (td0.value.integer == td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				else if (type == T_neq)
					if (td0.value.integer != td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				td.type = TYPE_INT;
				td.value.integer = td.value.dbl;
				break;
			default:
				break;
			}
			break;
		case TYPE_DOUBLE: //первый операнд double
			switch (td.type) // тип второго операнда
			{
			case TYPE_INT: //второй операнд int
				if (type == T_less)
					if (td0.value.dbl < td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				else if (type == T_more)
					if (td0.value.dbl > td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				else if (type == T_eq_less)
					if (td0.value.dbl <= td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				else if (type == T_eq_more)
					if (td0.value.dbl >= td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				else if (type == T_eq)
					if (td0.value.dbl == td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				else if (type == T_neq)
					if (td0.value.dbl != td.value.integer)
						td.value.integer = 1;
					else
						td.value.integer = 0;
				break;
			case TYPE_DOUBLE: //второй операнд double
				if (type == T_less)
					if (td0.value.dbl < td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				else if (type == T_more)
					if (td0.value.dbl > td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				else if (type == T_eq_less)
					if (td0.value.dbl <= td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				else if (type == T_eq_more)
					if (td0.value.dbl >= td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				else if (type == T_eq)
					if (td0.value.dbl == td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				else if (type == T_neq)
					if (td0.value.dbl != td.value.dbl)
						td.value.dbl = 1;
					else
						td.value.dbl = 0;
				td.type = TYPE_INT;
				td.value.integer = td.value.dbl;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);
	}
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
	return td;
}


TData TDiagram::U() {
	LEX l; int type, uk1; int ps, ln; TData td1, td2;
	td1 = Y();
	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	while ((type == T_plus) || (type == T_minus))
	{
		td2 = Y();

		switch (td1.type)//тип первого операнда
		{
		case TYPE_INT: //первый операнд int
			switch (td2.type) // тип второго операнда
			{
			case TYPE_INT: //второй операнд int
				if (type == T_plus)
					td1.value.integer += td2.value.integer;
				else if (type == T_minus)
					td1.value.integer -= td2.value.integer;
				break;
			case TYPE_DOUBLE: //второй операнд double
				td1.type = td2.type;
				td1.value.dbl = td1.value.integer;
				if (type == T_plus)
					td1.value.dbl += td2.value.dbl;
				else if (type == T_minus)
					td1.value.dbl -= td2.value.dbl;
				break;
			default:
				break;
			}
			break;
		case TYPE_DOUBLE: //первый операнд double
			switch (td2.type) // тип второго операнда
			{
			case TYPE_INT: //второй операнд int
				if (type == T_plus)
					td1.value.dbl += td2.value.integer;
				else if (type == T_minus)
					td1.value.dbl -= td2.value.integer;
				break;
			case TYPE_DOUBLE: //второй операнд double
				if (type == T_plus)
					td1.value.dbl += td2.value.dbl;
				else if (type == T_minus)
					td1.value.dbl -= td2.value.dbl;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);

		td1.flag_allow_unn = false; //запретитть унарные операции 
	}
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
	
	return td1;
}

TData TDiagram::Y() {
	LEX l; int type, uk1; int ps, ln; TData td1, td2;
	td1 = X();
	uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
	type = sc->Scaner(l);
	while (type == T_mult || type == T_div)
	{
		td2 = X();


		switch (td1.type)//тип первого операнда
		{
		case TYPE_INT: //первый операнд int
			switch (td2.type) // тип второго операнда
			{
			case TYPE_INT: //второй операнд int
				if (type == T_mult)
					td1.value.integer *= td2.value.integer;
				else if (type == T_div)
					td1.value.integer /= td2.value.integer;
				break;
			case TYPE_DOUBLE: //второй операнд double
				td1.type = td2.type;
				td1.value.dbl = td1.value.integer;
				if (type == T_mult)
					td1.value.dbl *= td2.value.dbl;
				else if (type == T_div)
					td1.value.dbl /= td2.value.dbl;
			default:
				break;
			}
			break;
		case TYPE_DOUBLE: //первый операнд double
			switch (td2.type) // тип второго операнда
			{
			case TYPE_INT: //второй операнд int
				if (type == T_mult)
					td1.value.dbl *= td2.value.integer;
				else if (type == T_div)
					td1.value.dbl /= td2.value.integer;
				break;
			case TYPE_DOUBLE: //второй операнд double
				if (type == T_mult)
					td1.value.dbl *= td2.value.dbl;
				else if (type == T_div)
					td1.value.dbl /= td2.value.dbl;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		uk1 = sc->GetUK(); ln = sc->GetLine(); ps = sc->GetPos();
		type = sc->Scaner(l);

		td1.flag_allow_unn = false; //запретитть унарные операции 
	}
	sc->SetUK(uk1); sc->SetLine(ln); sc->SetPos(ps);
	
	return td1;
}

Tree* TDiagram::getRoot() {
	return Root;
}