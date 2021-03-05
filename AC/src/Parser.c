#include "Parser.h"

//
//  Program -> { Statement_List } eof
//
AC_Result AC_isProgram(
	AC_TokenStream *token_stream
)
{
    AC_Token *curr_token;
    AC_Result res;

    res = AC_nextTokenTokenStream(token_stream, &curr_token);

    // Ensure the first token is a left bracket
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type != AC_L_BRACKET) {
            AC_DEBUG_TRACE_ARG(AC_NOT_GOOD, "Syntax Error!")
            return AC_SYNTAX_ERROR;
        }
    }
    else {
        AC_EXIT_FAILURE("Error reading from token stream!")
    }
        
    // Ensure the middle is a StatementList
        
    // Ensure the last token is a right bracket

    return AC_SUCCESS;
}


AC_Result AC_parseTokenStream(
	AC_TokenStream *token_stream
)
{
    //AC_printTokenStream(token_stream);

    if (AC_isProgram(token_stream) == AC_SUCCESS) {
        return AC_SUCCESS;
    }
    else {
        AC_EXIT_FAILURE("Program was not recognized by parser!")
    }

    AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "PARSING COMPLETED SUCCESSFULLY!")
    return AC_SUCCESS;
}