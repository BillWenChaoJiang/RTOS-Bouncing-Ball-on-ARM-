/*----------------------------------------------------------------------------
* Name:    Retarget.c
* Purpose: 'Retarget' layer for target-dependent low level functions
* Note(s):
*----------------------------------------------------------------------------
* This file is part of the uVision/ARM development tools.
* This software may only be used under the terms of a valid, current,
* end user licence from KEIL for a compatible version of KEIL software
* development tools. Nothing else gives you the right to use this software.
*
* This software is supplied "AS IS" without warranties of any kind.
*
* Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <rt_misc.h>
#include "GLCD_Scroll.h"

#pragma import( __use_no_semihosting_swi )

/*----------------------------------------------------------------------------
Write character to Serial Port
*----------------------------------------------------------------------------*/
int sendchar( int c ) {

	if ( c == '\r' || c == '\n' )  {
		append_char('\n');
	} else {
		append_char(c);
	}

	return c;
}


/*----------------------------------------------------------------------------
Read character from Serial Port   (blocking read)
*----------------------------------------------------------------------------*/
int getkey (void ) {

	return -1;
}


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


int fputc( int ch, FILE *f ) {

	return sendchar(ch);
}


int fgetc( FILE *f ) {
	int ch = getkey();

	sendchar( ch );

	return ch;
}


int ferror( FILE *f ) {

	/* Your implementation of ferror */
	return EOF;
}


void _ttywrch( int ch ) {

	sendchar( ch );
}


void _sys_exit( int return_code ) {

	label: goto label;  /* endless loop */
}
