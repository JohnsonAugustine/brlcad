/*
	Author:	Gary S. Moss
	U. S. Army Ballistic Research Laboratory
	Aberdeen Proving Ground
	Maryland 21005-5066
	(301)278-6651 or AV-298-6651

	$Header$ (BRL)

*/
/**
	<Sc.h> -- MUVES "Sc" (Screen manager) package definitions
**/

/**

	The Sc package provides an interface to the termcap library
	(termlib(3)) for doing cursor movement, character attribute,
	and other low-level screen management operations.  It is
	intended for use in building terminal-independent full-screen
	user interfaces.  Do to its use of the tgoto() termlib(3)
	routine, programs that use this package should turn off TAB3
	in the terminal handler.

**/
#ifndef Sc_H_INCLUDE
#define Sc_H_INCLUDE
#include "burst.h"

/**
	bool	ScInit( FILE *fp )

	ScInit() must be invoked before any other function in the Sc
	package.  Stream fp must be open for writing and all terminal
	control sequences will be sent to fp, giving the application
	the option of using STDOUT for other things.  Besides setting
	the output stream, ScInit() does the following:

	Initializes the terminal.

	Fills terminal name and capabilities into these externals:

	char	ScTermname[ScTERMSIZ]	(terminal name from $TERM)
	char	ScTermcap[ScTCAPSIZ]	(terminal capabilities entry)

	Gets terminal control strings into external variables (here
	are some that don't have individual functions to output them,
	the ones that DO are below):

	char	*ScBC	(backspace character)	
	char	*ScPC	(padding character)
	char	*ScUP	(move the cursor up one line)
	char	*ScTI	(initialize the terminal)

	Gets individual terminal parameters into these externals:

	int	ScLI	(number of lines on screen)
	int	ScCO	(number of columns on screen)

	Returns "true" for success, "false" for failure and prints
	appropriate diagnostics on STDERR if $TERM is not set or
	there is a problem in retrieving the corresponding termcap
	entry.
**/
#if _STDC_
extern bool	ScInit( FILE *fp );
#else
extern bool	ScInit();
#endif

/**

	Below are functions paired with terminal control strings that
	they output to the stream specified with ScInit().  It is not
	recommended that the control strings be used directly, but it
	may be useful in certain applications to check their value;
	if ScInit() has not been invoked or the corresponding terminal
	capability does not exist, its control string will be NULL,
	AND the function will return "false".  Otherwise, they will
	return "true" (assuming that it worked).  There is no way to
	be sure of this.

	char	*ScCE 	(clear from under the cursor to end of line)
	bool	ScClrEOL( void )

	char	*ScCS	(change scrolling region)
	bool	ScClrScrlReg( void )

	char	*ScSE	(end standout mode)
	bool	ScClrStandout( void )

	char	*ScCL	(clear screen, and home cursor)
	bool	ScClrText( void )

	char	*ScAL	(insert a line under the cursor)
	bool	ScInsertLn( void )

	char	*ScDL	(delete the line under the cursor)
	bool	ScDeleteLn( void )

	char	*ScSR	(scroll text backwards 1 line)
	bool	ScDnScroll( void )

	char	*ScHO	(move cursor to top-left corner of screen)
	bool	ScHmCursor( void )

	char	*ScCM	(move cursor to column and row <x,y>)
	bool	ScMvCursor( x, y )

	char	*ScCS	(set scrolling region from top to btm incl.)
	bool	ScSetScrlReg( top, btm )

	char	*ScSO	(begin standout mode)
	bool	ScSetStandout( void )

	char	*ScSF	(scroll text forwards 1 line)
	bool	ScUpScroll( void )

**/
extern char	*ScBC;
extern char	*ScPC;
extern char	*ScUP;
extern char	*ScCS;
extern char	*ScSO;
extern char	*ScSE;
extern char	*ScCE;
extern char	*ScCL;
extern char	*ScHO;
extern char	*ScCM;
extern char	*ScTI;
extern char	*ScDL;
extern char	*ScSR;
extern char	*ScSF;

extern int	ScLI;
extern int	ScCO;

#if _STDC_
extern bool	ScClrEOL( void );
extern bool	ScClrScrlReg( void );
extern bool	ScClrStandout( void );
extern bool	ScClrText( void );
extern bool	ScDeleteLn( void );
extern bool	ScDnScroll( void );
extern bool	ScHmCursor( void );
extern bool	ScInsertLn( void );
extern bool	ScMvCursor( int x, int y );
extern bool	ScSetScrlReg( int top, int btm );
extern bool	ScSetStandout( void );
extern bool	ScUpScroll( void );
#else
extern bool	ScClrEOL();
extern bool	ScClrScrlReg();
extern bool	ScClrStandout();
extern bool	ScClrText();
extern bool	ScDeleteLn();
extern bool	ScDnScroll();
extern bool	ScHmCursor();
extern bool	ScInsertLn();
extern bool	ScMvCursor();
extern bool	ScSetScrlReg();
extern bool	ScSetStandout();
extern bool	ScUpScroll();
#endif

#define ScTCAPSIZ	1024
#define ScTERMSIZ	80

extern char	ScTermcap[];
extern char	ScTermname[];

#endif		/* Sc_H_INCLUDE */


