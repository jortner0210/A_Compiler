#include "Parser.h"

//
//  Program -> { Statement_List } eof
//
AC_Result AC_isProgram(
	AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "Entering AC_isProgram")
    AC_Result res;
    AC_Token *curr_token;
    
    while (AC_nextTokenTokenStream(token_stream, &curr_token) == AC_SUCCESS) {

        AC_printToken(curr_token);
    }

    AC_backTrackTokenStream(token_stream);

    AC_nextTokenTokenStream(token_stream, &curr_token);
    AC_printToken(curr_token);
    
    return AC_SUCCESS;
}


AC_Result AC_parseTokenStream(
	AC_TokenStream *token_stream
)
{
    //AC_printTokenStream(token_stream);

    if (AC_isProgram(token_stream) == AC_SUCCESS) {
        AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "PARSING COMPLETED SUCCESSFULLY!")
        return AC_SUCCESS;
    }
    else {
        AC_PARSE_ERROR_MSG("Expected a Program!", 0, 0)
    }

    return AC_SUCCESS;
}
