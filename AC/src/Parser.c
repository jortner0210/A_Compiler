#include "Parser.h"

//
// Factor -> number
// Factor -> empty (is it okay for for empty to be ';')
//
AC_Result AC_isFactor(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isFactor")
    AC_Result res;
    AC_Token *curr_token;
        
    // CHECK FOR NUMBER
    if (AC_nextTokenTokenStream(token_stream, &curr_token) == AC_SUCCESS) {
        if (curr_token->tok_type == AC_NUMBER) {
            AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Looking for and found a Factor!")
            return AC_SUCCESS;
        }
        else if (curr_token->tok_type == AC_SEMI_COLON) {
            AC_backTrackTokenStream(token_stream);
            AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Found and empty Factor!")
            return AC_SUCCESS;
        }
        else {
            AC_EXIT_FAILURE("Expected a Factor")
        }
    }
}

//
// Term' -> empty
//
AC_Result AC_isTermPrime(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isTermPrime")
    AC_Result res;
    AC_Token *curr_token;
        
    // CHECK FOR EMPTY
    if (AC_nextTokenTokenStream(token_stream, &curr_token) == AC_SUCCESS) {
        if (curr_token->tok_type == AC_SEMI_COLON || curr_token->tok_type == AC_ADD_SUBTRACT) {
            AC_backTrackTokenStream(token_stream);
            AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Found and empty TermPrime!")
            return AC_SUCCESS;
        }
        else {
            AC_EXIT_FAILURE("Expected an empty term prime")
        }
    }
}

//
// Term -> Factor Term'
//
AC_Result AC_isTerm(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isTerm")
    AC_Result res;
    
    // CHECK FOR FACTOR
    res = AC_isFactor(token_stream);
    if (res != AC_SUCCESS) 
        AC_EXIT_FAILURE("Expected a Valid Factor")

    // CHECK FOR TERM PRIME
    res = AC_isTermPrime(token_stream);
    if (res != AC_SUCCESS) 
        AC_EXIT_FAILURE("Expected a Valid TermPrime")

    return AC_SUCCESS;    
}

//
// Expression' -> empty
// Expression' -> + Term Expression'
//
AC_Result AC_isExpressionPrime(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isExpressionPrime")
    AC_Result res;
    AC_Token *curr_token;
    
    if (AC_nextTokenTokenStream(token_stream, &curr_token) == AC_SUCCESS) {
        // CHECK FOR EMPTY
        if (curr_token->tok_type == AC_SEMI_COLON) {
            AC_backTrackTokenStream(token_stream);
            AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Found and empty ExpressionPrime!")
            return AC_SUCCESS;
        }
        // CHECK FOR OP(+)
        else if (curr_token->tok_type == AC_ADD_SUBTRACT) {
            AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Found an Op(+|-)!")
            // CHECK FOR TERM
            res = AC_isTerm(token_stream);
            if (res != AC_SUCCESS) 
                AC_EXIT_FAILURE("Expected a Valid Term")

            // CHECK FOR EXPRESSION PRIME
            res = AC_isExpressionPrime(token_stream);
            if (res != AC_SUCCESS) 
                AC_EXIT_FAILURE("Expected a Expression Prime")
        }
        else {
            AC_EXIT_FAILURE("Expected an empty string or an Op(+|-)")
        }
    }
}

//
// Expression -> Term
//
AC_Result AC_isExpression(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isExpression")
    AC_Result res;
      
    // CHECK FOR TERM
    res = AC_isTerm(token_stream);
    if (res != AC_SUCCESS) 
        AC_EXIT_FAILURE("Expected a Valid Term")

    // CHECK FOR EXPRESSION PRIME
    res = AC_isExpressionPrime(token_stream);
    if (res != AC_SUCCESS) 
        AC_EXIT_FAILURE("Expected a Expression Prime")

    return AC_SUCCESS;
}

//
// Statement -> Expression ;
//
AC_Result AC_isStatement(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isStatement")
    AC_Result res;
    AC_Token *curr_token;
    
    // CHECK FOR EXPRESSION
    res = AC_isExpression(token_stream);
    if (res != AC_SUCCESS) 
        AC_EXIT_FAILURE("Expected a Valid Expression")

    // CHECK FOR SEMI COLON
    if (AC_nextTokenTokenStream(token_stream, &curr_token) == AC_SUCCESS) {
        if (curr_token->tok_type == AC_SEMI_COLON) {
            AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Looking for and found Semi colon!")
            return AC_SUCCESS;
        }
        else {
            AC_EXIT_FAILURE("Expected a Valid Statement")
        }
    }
}

//
// Current Supported Grammer: Aim: LL(1) Grammar
//
// Program     -> Statement
//
// Statement   -> Expression ;
//
// Expression  -> Term Expression'
//
// Expression' -> empty
// Expression' -> + Term Expression'
//
// Term        -> Factor Term'
//
// Term'       -> empty
//
// Factor 	   -> number
// Factor 	   -> empty
//
AC_Result AC_isProgram(
	AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isProgram")
    AC_Result res;

    res = AC_isStatement(token_stream);
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Looking for and found valid statement!")
        return AC_SUCCESS;
    }
    else {
        AC_EXIT_FAILURE("Expected a Valid Statement")
    }
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
