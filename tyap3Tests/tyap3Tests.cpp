#include "CppUnitTest.h"
#include "..\tyap3\defs.h"
#include "..\tyap3\Diagram.h"
#include "..\tyap3\Scaner.h"
#include "..\tyap3\Semant.h"
#include <fstream>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tyap3Tests
{		
	TEST_CLASS(ScanerTests)
	{
	public:
		TScaner *sc;
		TDiagram *dg;

		TEST_METHOD_INITIALIZE(sds) {
			sc = new TScaner("input2.txt");
			dg = new TDiagram(sc);
		}
		
		TEST_METHOD(CorrectScanAllLexs) {
			LEX l; int actual_code;
			char text[255] = "int double for return somevar42 \n\t 214 'a' , ; ( ) { } [ ] + - -- ++ * / % = < > <= >= == ! !=";	
			sc->SetText(text);
			
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_int, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_double, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_for, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_return, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_ident, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_const_int, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_const_char, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_comma, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_dot_com, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_LB, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_RB, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_FLB, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_FRB, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_SQLB, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_SQRB, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_plus, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_minus, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_un_minus, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_un_plus, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_mult, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_div, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_mod, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_save, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_less, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_more, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_eq_less, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_eq_more, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_eq, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_not, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_neq, actual_code);
			actual_code = sc->Scaner(l);
			Assert::AreEqual(T_end, actual_code);
		}
	};

	TEST_CLASS(SemantFuncsTests)
	{
	public:
		TScaner *sc;
		TDiagram *dg;
		Tree *tree;

		TEST_METHOD_INITIALIZE(sds) {
			sc = new TScaner("input2.txt");
			dg = new TDiagram(sc);
			tree = dg->getRoot();
		}

		TEST_METHOD(CheckCorrectAddVar) {
			Tree *cur_vert = tree->IncludeVar(sc, "var", TYPE_INT);		

			Assert::AreEqual("var", cur_vert->n->id);
			int expected_type = TYPE_INT;
			int actual_type = cur_vert->n->TypeData;
			Assert::AreEqual(expected_type,actual_type);
			Assert::AreEqual("EMPTY", cur_vert->Up->n->id);
		}

		TEST_METHOD(CheckCorrectAddArr) {
			Tree *cur_vert = tree->IncludeArr(sc, "arr", TYPE_DOUBLE);
			int col_measure = 2, measure1 = 2, measure2 = 3;
			tree->AddMeasure(measure1);
			tree->AddMeasure(measure2);

			Assert::AreEqual("arr", cur_vert->n->id);
			int expected_type = TYPE_DOUBLE;
			int actual_type = cur_vert->n->TypeData;
			Assert::AreEqual(expected_type, actual_type);
			Assert::AreEqual(cur_vert->n->N, col_measure);
			Assert::AreEqual(cur_vert->n->hg[0], measure1);
			Assert::AreEqual(cur_vert->n->hg[1], measure2);
			Assert::AreEqual("EMPTY", cur_vert->Up->n->id);	
		}

		TEST_METHOD(CheckCorrectAddFunc) {
			Tree *cur_vert = tree->IncludeFunc(sc, "var", TYPE_INT);

			Assert::AreEqual("var", cur_vert->n->id);
			int expected_type = TYPE_INT;
			int actual_type = cur_vert->n->TypeData;
			Assert::AreEqual(expected_type, actual_type);
			Assert::AreEqual("EMPTY1", cur_vert->Right->n->id);
			expected_type = TYPE_UNKNOWN;
			actual_type = cur_vert->Right->n->TypeData;
			Assert::AreEqual(expected_type, actual_type);
			Assert::AreEqual("EMPTY",cur_vert->Up->n->id);
		}

		TEST_METHOD(CheckCorrectAddOperator) {
			Tree *cur_vert = tree->IncludeOper();
			
			Assert::AreEqual("EMPTY1", cur_vert->n->id);
			int expected_type = TYPE_UNKNOWN;
			int actual_type = cur_vert->n->TypeData;
			Assert::AreEqual(expected_type, actual_type);	
			Assert::AreEqual("EMPTY2", cur_vert->Right->n->id);
			expected_type = TYPE_UNKNOWN;
			actual_type = cur_vert->Right->n->TypeData;
			Assert::AreEqual(expected_type, actual_type);
			Assert::AreEqual("EMPTY", cur_vert->Up->n->id);
		}

		TEST_METHOD(CheckDuplicateControl) {
			tree->IncludeVar(sc, "var", TYPE_INT);
			Tree *cur_vert = tree->IncludeVar(sc, "var2", TYPE_DOUBLE);
			bool expected = true;
			bool actual = tree->DupControl(cur_vert,"var");
			
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(IdSearchCheck) {
			int var_value = 10;
			tree->IncludeVar(sc, "var", TYPE_INT);
			tree->Cur->n->value.integer = var_value;
			tree->IncludeFunc(sc, "main", TYPE_INT);
			tree->IncludeVar(sc, "var2", TYPE_DOUBLE);

			TData var = tree->CheckID(sc, tree->Cur, "var");
			Assert::AreEqual(var_value, var.value.integer);
			int expected_type = TYPE_INT;
			int actual_type = var.type;
			Assert::AreEqual(expected_type, actual_type);
		}

		TEST_METHOD(CheckCorrectDefDataType) {
			int actual_type = TYPE_INT;
			int expected_type = tree->DefDataType("int");
			Assert::AreEqual(expected_type, actual_type);

			actual_type = TYPE_DOUBLE;
			expected_type = tree->DefDataType("double");
			Assert::AreEqual(expected_type, actual_type);
		}


	};
}