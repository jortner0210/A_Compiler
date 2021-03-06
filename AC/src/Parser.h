#ifndef AC_PARSER_H
#define AC_PARSER_H

//
//	The purpose of the Parser is to match a stream of catorized words
//	against the rules that specify syntax for the language. To determine
//	if the input stream is a sentence in the source language.
//
//	- i.e. 
//		Grammatical Rules:
//		 1) Sentence -> Subject verb Object endmark
//		 2) Subject  -> noun
//		 3) Subject  -> Modifier noun
//		 4) Object   -> noun
//		 5) Object   -> Modifier noun
//		 6) Modifier -> adjective
//
//		Sentence: "Compilers are engineered objects."
//		 
//		 Start  - Sentence
//		 Rule 1 - Subject verb Object endmark
//		 Rule 2 - noun verb Object endmark
//		 Rule 5 - noun verb Modifier noun endmark
//		 Rule 6 - noun verb adjective noun endmark
//

//
// Current Supported Grammer: Aim: LL(1) Grammar
//
// Program     -> Expression
//
// Expression  -> Term Expression' 
//
// Expression' -> empty
// Expression' -> + Term Expression'
//
// Term        -> Factor Term'
//
// Term'       -> empty
// Term'       -> * Factor Term'
//
// Factor 	   -> number
//

#include "Scanner.h"

AC_Result AC_parseTokenStream(
	AC_TokenStream *token_stream
);

AC_Result AC_isProgram(
	AC_TokenStream *token_stream
);

AC_Result AC_isExpression(
    AC_TokenStream *token_stream
);

AC_Result AC_isExpressionPrime(
    AC_TokenStream *token_stream
);

AC_Result AC_isTerm(
    AC_TokenStream *token_stream
);

AC_Result AC_isTermPrime(
    AC_TokenStream *token_stream
);

AC_Result AC_isFactor(
    AC_TokenStream *token_stream
);


#endif
