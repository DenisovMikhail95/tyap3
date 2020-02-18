#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <vector>


#define MAX_TEXT 10000	//максимальная длина текста
#define MAX_LEX 100		// максимальная длина лексемы
#define MAX_KEYW 4		// число ключевых слов
#define MAX_N 10		//максимальная размерность массива

typedef char LEX[MAX_LEX]; //лексема
typedef char TEXT[MAX_TEXT]; //исходный модуль текста

#define T_int			1	// int
#define T_double		2	// double
#define T_for			3	// for
#define T_return		4	// return
#define T_ident			10	// идентификатор
#define T_const_int		20	// целная константа
#define T_const_char	21	// символьная константа 
#define T_comma			31	// ,
#define T_dot_com		32	// ;
#define T_LB			33	// (
#define T_RB			34  // )
#define T_FLB			35	// {
#define T_FRB			36	// }
#define T_TC			37	// '
#define T_SQLB			38	// [
#define T_SQRB			39	// ]
#define T_plus			40  // +
#define T_minus			41  // -
#define T_un_minus		42  // --
#define T_un_plus		43  // ++
#define T_mult			44  // *
#define T_div			45  // /
#define T_mod			46  // %
#define T_save			47  // =
#define T_less			48	// <
#define T_more			49	// >
#define T_eq_less		50	// <=
#define T_eq_more		51	// >=
#define T_eq			52	// ==
#define T_not			53	// !
#define T_neq			54	// !=
#define T_error			100 // ошибочный тип
#define T_end			200	// конец текста