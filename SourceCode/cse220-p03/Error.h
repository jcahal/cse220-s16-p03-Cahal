/***************************************************************************************************************
 * FILE: Error.h
 *
 * DESCRIPTION
 * Functions for error handling.
 *
 * CSE 220
 **************************************************************************************************************/
#ifndef ERROR_H
#define ERROR_H

//==============================================================================================================
// TYPE DEFINITIONS
//==============================================================================================================

// Program exit codes when errors occur.
typedef enum {
	ErrorNone			=   0,
	ErrorArg          	=  -1,
	ErrorArgDup 		=  -2,
	ErrorArgInputFile	=  -3,
	ErrorArgInvOpt		=  -4,
	ErrorArgRot			=  -5,
	ErrorArgUnexpStr	=  -6,
	ErrorBmpInv			=  -7,
	ErrorBmpCorrupt		=  -8,
	ErrorFileOpen		=  -9,
	ErrorFileRead		= -10,
	ErrorFileWrite		= -11
} tError;

//==============================================================================================================
// FUNCTION DECLARATIONS
//==============================================================================================================

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: ErrorExit
 *
 * DESCRIPTION
 * Called when an error occurs and the program must be terminated. This function works similarly to printf(). A
 * typical call would be something like,
 *
 *     ErrorExit(ArgError, "An error code %d occurred in %s", ArgError, some_string_var);
 *
 * pError would be initialized to ArgError when the function is called. pFmt would be initialized to "An error
 * code %d occurred in %s", and there would be two variadic arguments: ArgError which had better be a tError
 * variable (we don't do any type checking), and some_string_var which must be of data type char *.
 *
 * PARAMETERS
 * pError	A value of the enumerated type tError which is sent back to the OS to indicate what error occurred.
 * pFmt		A format string similar to that of printf(). Valid format specifiers are %c, %d, and %s.
 *
 * NOTES
 * This is a variadic function because the ... following the required pFmt parameter states that there may be
 * 0, 1, 2, ... optional parameters. Variadic arguments are handled by preprocessor macros defined in stdarg.h,
 * in particular: va_list, va_start, va_arg, and va_end.
 *------------------------------------------------------------------------------------------------------------*/
void ErrorExit(tError pError, char *pFmt, ...);

#endif
