#ifndef __DEF_H
#define __DEF_H
//------------------------------------------------------------------------------
#define BIT(B)					(1u<<B)// преобр. номера бита в позицию в байте
#define TBIT0(ADDRESS,B)		(!(ADDRESS & (BIT(B))))// проверка бита на ноль
#define TBIT1(ADDRESS,B)		(ADDRESS & (BIT(B)))// проверка бита на единицу
#define SBIT(ADDRESS,B)			(ADDRESS |= BIT(B))// установка бита
#define CBIT(ADDRESS,B)			(ADDRESS &=~BIT(B))// сброс бита
#define SMASK(ADDRESS,BYTE)		(ADDRESS |= BYTE)// установка битов по байтовой маске
#define CMASK(ADDRESS,BYTE)		(ADDRESS &= ~(BYTE))// очистка битов по байтовой маске*/
//------------------------------------------------------------------------------
#define ARRAY_SIZE(x)			(sizeof(x) / sizeof((x)[0]))
//------------------------------------------------------------------------------
#if !defined(DEBUG_SEPARATOR_LINE)
	#define DEBUG_SEPARATOR_LINE	"=======================\n"
#endif
//------------------------------------------------------------------------------
#if !defined(PRINT_FUN_NAME_AND_LINE)
#define PRINT_FUN_NAME_AND_LINE() \
							({	printf(DEBUG_SEPARATOR_LINE); \
								printf("%s line:%i\n",__func__,__LINE__);})
#endif
//------------------------------------------------------------------------------
#define ABS(X)  ((X) > 0 ? (X) : -(X))
//------------------------------------------------------------------------------
#endif
