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
	AC_SYMBOL_TABLE_SYMBOL_FOUND,

	AC_END_OF_TOKEN_STREAM,

	AC_SYNTAX_ERROR,

    AC_NOT_FOUND

} AC_Result;


void AC_printResult(
	AC_Result result
);

#ifdef NDEBUG
    #define DEBUG_TEST 0
    #define AC_ENABLE_VALIDATION_LAYERS 0
#else
    #define DEBUG_TEST 1
    #define AC_ENABLE_VALIDATION_LAYERS 1
#endif

#define AC_NOT_GOOD "\x1B[31m" // red
#define AC_ALL_GOOD "\x1B[32m" // green
#define AC_FINE     "\x1B[33m" // warning

#define KNRM  "\x1B[0m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


#define AC_SCANNER_CHAR_ERROR(wrong_char, line_num, char_num) 								 \
	do {                                                									 \
        printf("%s", AC_NOT_GOOD);                      									 \
        printf("Scanning Error: Unidentified char: %c, line number: %d, char number: %d\n",  \
			   wrong_char, 																	 \
			   line_num, 																	 \
			   char_num); 																	 \
        printf("%s\n", KNRM);                          										 \
        exit(EXIT_FAILURE);                             									 \
    } while (0);


#define AC_PARSE_ERROR_MSG(msg, line_num, char_num)        					   \
    do {                                                					   \
        printf("%s", AC_NOT_GOOD);                      					   \
        printf("Parse Error: Message: %s, line number: %d, char number: %d\n", \
			   msg, 														   \
			   line_num, 													   \
			   char_num); 													   \
        printf("%s\n", KNRM);                          						   \
        exit(EXIT_FAILURE);                             					   \
    } while (0);


#define AC_DEBUG_TRACE_FMT(type, fmt, ...)                 \
    do {                                                   \
        if (DEBUG_TEST) {                                  \
            printf("%s", type);                            \
            printf("AC DEBUG TRACE -- %s:%d:%s() -- " fmt, \
                   __FILE__,                               \
                   __LINE__,                               \
                   __func__,                               \
                   __VA_ARGS__);                           \
            printf("%s\n", KNRM);                          \
        }                                                  \
    } while (0);

#define AC_DEBUG_TRACE_ARG(type, arg)\
    AC_DEBUG_TRACE_FMT(type, "%s", arg)

	
#define AC_EXIT_FAILURE_FMT(fmt, ...)					\
    do {                                                \
        printf("%s", AC_NOT_GOOD);						\
        printf("AC EXIT FAILURE -- %s:%d:%s() -- " fmt, \
               __FILE__,                                \
               __LINE__,                                \
               __func__,								\
			   __VA_ARGS__);                            \
        printf("%s\n", KNRM);                           \
        exit(EXIT_FAILURE);                             \
    } while (0); 

#define AC_EXIT_FAILURE(msg)\
    AC_EXIT_FAILURE_FMT("%s", msg)

#endif
