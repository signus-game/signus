
/*---------------------------------------------------------------------------
   
   File name 	:	strlib.h
   Author 		:	N. Devillard
   Created on	:	January 2001
   Description	:	Various string handling routines.

 *--------------------------------------------------------------------------*/

/*
	$Id$
	$Author$
	$Date$
	$Revision$
*/

#ifndef _STRLIB_H_
#define _STRLIB_H_

/*---------------------------------------------------------------------------
   								Includes
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------
  							Function codes
 ---------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/**
  @name		strlwc
  @memo		Convert a string to lowercase.
  @param	s	String to convert.
  @return	ptr to statically allocated string.
  @doc

  This function returns a pointer to a statically allocated string
  containing a lowercased version of the input string. Do not free
  or modify the returned string! Since the returned string is statically
  allocated, it will be modified at each function call (not re-entrant).
 */
/*--------------------------------------------------------------------------*/

char * strlwc(char * s);



/*-------------------------------------------------------------------------*/
/**
  @name		strupc
  @memo		Convert a string to uppercase.
  @param	s	String to convert.
  @return	ptr to statically allocated string.
  @doc

  This function returns a pointer to a statically allocated string
  containing an uppercased version of the input string. Do not free
  or modify the returned string! Since the returned string is statically
  allocated, it will be modified at each function call (not re-entrant).
 */
/*--------------------------------------------------------------------------*/

char * strupc(char * s);



/*-------------------------------------------------------------------------*/
/**
  @name		strskp
  @memo		Skip blanks until the first non-blank character.
  @param	s	String to parse.
  @return	Pointer to char inside given string.
  @doc

  This function returns a pointer to the first non-blank character in the
  given string.

 */
/*--------------------------------------------------------------------------*/

char * strskp(char * s);


/*-------------------------------------------------------------------------*/
/**
  @name		strcrop
  @memo		Remove blanks at the end of a string.
  @param	s	String to parse.
  @return	ptr to statically allocated string.
  @doc

  This function returns a pointer to a statically allocated string,
  which is identical to the input string, except that all blank
  characters at the end of the string have been removed.
  Do not free or modify the returned string! Since the returned string
  is statically allocated, it will be modified at each function call
  (not re-entrant).
 */
/*--------------------------------------------------------------------------*/

char * strcrop(char * s);


#endif
