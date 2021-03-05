#include "Parser.h"

AC_Result AC_readAndCheckTokenStream(
    AC_TokenStream *token_stream, 
    AC_TokenType AC_TokenStream, 
    const char *error_message
)
{
    AC_Token *curr_token;
    AC_Result res;
    res = AC_nextTokenTokenStream(token_stream, &curr_token);      
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type != AC_TokenStream) {
            if (error_message == NULL) {
                return AC_SYNTAX_ERROR;
            }
            else {
                AC_PARSE_ERROR_MSG(error_message, token_stream->curr_line_num, token_stream->curr_char_num)
                return AC_SYNTAX_ERROR;
            }
        }
        else {
            return AC_SUCCESS;
        }
    }
    else {
        AC_EXIT_FAILURE("End of file reached!")
    }
}

//
//  Program -> { Statement_List } eof
//
AC_Result AC_isProgram(
	AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "Entering AC_isProgram")
    AC_Result res;

    // OPTION 1:
	//  - CHECK FOR EMPTY
	if (token_stream->end_reached)
		AC_PARSE_ERROR_MSG("Expected a Valid Program", token_stream->curr_line_num, token_stream->curr_char_num)


    // OPTION 2:
        
	//  - CHECK FOR EXPRESSION
    res = AC_isStatement(token_stream);
    if (res != AC_SUCCESS) {
        AC_PARSE_ERROR_MSG("Expected a Statement", token_stream->curr_line_num, token_stream->curr_char_num)
    }

    return AC_SUCCESS;
}

AC_Result AC_isStatement(
	AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "Entering AC_isStatement")
    AC_Token *curr_token;
    AC_Result res;

    // OPTION 1:
    //  - CHECK FOR EXPRESSION
    res = AC_isExpression(token_stream);
    if (res != AC_SUCCESS) {
        AC_PARSE_ERROR_MSG("Expected an Expression", token_stream->curr_line_num, token_stream->curr_char_num)
    }

    // - CHECK FOR SEMI COLON
    res = AC_readAndCheckTokenStream(token_stream, AC_SEMI_COLON, "Expected a Semi Colon");

    return AC_SUCCESS;
}

AC_Result AC_isExpression(
	AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "Entering AC_isExpression")
    AC_Result res;

    // OPTION 1:
    //  - CHECK FOR TERM
    //  - CHECK FOR EXPRESSION TAIL
    res = AC_isTerm(token_stream);
    //if (res != AC_SUCCESS) {
    //    AC_PARSE_ERROR_MSG("Expected an Term", token_stream->curr_line_num, token_stream->curr_char_num)
    //}

    res = AC_isExpressionTail(token_stream);
    //if (res != AC_SUCCESS) {
    //    AC_PARSE_ERROR_MSG("Expected an Expression Tail", token_stream->curr_line_num, token_stream->curr_char_num)
    //}

    AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isExpression: SUCCESS")
    return AC_SUCCESS;
}

AC_Result AC_isExpressionTail(
	AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "Entering AC_isExpressionTail")
    AC_Result res;

    res = AC_readAndCheckTokenStream(token_stream, AC_SEMI_COLON, NULL);    
    if (res == AC_SUCCESS){
        AC_backTrackTokenStream(token_stream);
        AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isExpressionTail: Option 1 - SUCCESS")
        return AC_SUCCESS;
    }
    AC_backTrackTokenStream(token_stream);

    // OPTION 1:
    //  - CHECK FOR EMPTY
    //res = AC_readAndCheckTokenStream(token_stream, AC_SEMI_COLON, NULL);
    //if (res == AC_SUCCESS) {
    //    AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isExpressionTail: Option 1 - SUCCESS")
    //    return AC_SUCCESS;
    //}
    //AC_backTrackTokenStream(token_stream);

    // OPTION 2:
    //  - CHECK FOR OP(+|-)
    res = AC_readAndCheckTokenStream(token_stream, AC_ADD_SUBTRACT, NULL);
    if (res == AC_SUCCESS) {
        //  - CHECK FOR EXPRESSION 
        res = AC_isTerm(token_stream);
        //if (res != AC_SUCCESS) {
        //    AC_PARSE_ERROR_MSG("Expected a Term", token_stream->curr_line_num, token_stream->curr_char_num)
        //}

        res = AC_isExpressionTail(token_stream);
        //if (res != AC_SUCCESS) {
        //    AC_PARSE_ERROR_MSG("Expected an Expression Tail", token_stream->curr_line_num, token_stream->curr_char_num)
        //}
    }
    else {
        AC_backTrackTokenStream(token_stream);
    }

    AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isExpressionTail: Option 2 - SUCCESS")
    return AC_SUCCESS;
}

AC_Result AC_isTerm(
	AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "Entering AC_isTerm")
    AC_Result res;

    // OPTION 1:
    //  - CHECK FOR FACTOR
    res = AC_isFactor(token_stream);
    //if (res != AC_SUCCESS) {
    //    AC_PARSE_ERROR_MSG("Expected a Factor", token_stream->curr_line_num, token_stream->curr_char_num)
    //}

    //  - CHECK TERM TAIL
    res = AC_isTermTail(token_stream);
    //if (res != AC_SUCCESS) {
    //    AC_PARSE_ERROR_MSG("Expected a Term Tail", token_stream->curr_line_num, token_stream->curr_char_num)
   // }

    AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isTerm: SUCCESS")
    return AC_SUCCESS;
}

AC_Result AC_isTermTail(
	AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "Entering AC_isTermTail")
    AC_Result res;

    // OPTION 1:
    //  - CHECK FOR EMPTY
    //res = AC_readAndCheckTokenStream(token_stream, AC_SEMI_COLON, NULL);
    //if (res == AC_SUCCESS) {
    //    AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isTermTail: Option 1 - SUCCESS")
    //    return AC_SUCCESS;
    //}
    //AC_backTrackTokenStream(token_stream);
    //res = AC_readAndCheckTokenStream(token_stream, AC_ADD_SUBTRACT, NULL);
    //if (res == AC_SUCCESS) {
    //    AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isTermTail: Option 1 - SUCCESS")
    //    return AC_SUCCESS;
    //}
    //AC_backTrackTokenStream(token_stream);
    
    res = AC_readAndCheckTokenStream(token_stream, AC_SEMI_COLON, NULL);    
    if (res == AC_SUCCESS){
        printf("semi colon\n");
        AC_backTrackTokenStream(token_stream);
        AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isTermTail: Option 1 - SUCCESS")
        return AC_SUCCESS;
    }

    // OPTION 2:
    //  - CHECK FOR OP(*|/)
    AC_backTrackTokenStream(token_stream);
    res = AC_readAndCheckTokenStream(token_stream, AC_MULTIPLY_DIVIDE, NULL);
    if (res == AC_SUCCESS) {
        // CHECK FOR FACTOR
        AC_isFactor(token_stream);

        // CHECK FOR EXPRESSION TAIL
        AC_isExpressionTail(token_stream);
    }
    else {
        printf("backtrack\n");
        AC_backTrackTokenStream(token_stream);
    }

    AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isTermTail: Option 2 - SUCCESS")
    return AC_SUCCESS;
}

AC_Result AC_isFactor(
	AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "Entering AC_isFactor")
    AC_Result res;

    // CHECK FOR SEMI COLON
    res = AC_readAndCheckTokenStream(token_stream, AC_SEMI_COLON, NULL);
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isFactor: Semi Colon Found")
        AC_backTrackTokenStream(token_stream);
        return AC_SUCCESS;
    }
    
    // CHECK FOR NUMBER
    AC_backTrackTokenStream(token_stream);
    res = AC_readAndCheckTokenStream(token_stream, AC_NUMBER, "Expected a Number");
    AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Returning AC_isFactor: Number Found")
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
