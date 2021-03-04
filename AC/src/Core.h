#ifndef AC_CORE_H
#define AC_CORE_H

typedef enum 
{
	AC_SUCCESS,

	AC_ERROR,
	AC_IO_ERROR,

	AC_EOF,

	AC_NOT_NUMERIC,

	AC_SYMBOL_TABLE_SYMBOL_NOT_FOUND,
	AC_SYMBOL_TABLE_SYMBOL_FOUND	

} AC_Result;


void AC_printResult(
	AC_Result result
);

#endif
