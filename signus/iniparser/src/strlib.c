
/*---------------------------------------------------------------------------
   
   File name 	:	strlib.c
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

/*---------------------------------------------------------------------------
   								Includes
 ---------------------------------------------------------------------------*/

#include "strlib.h"

#ifdef _ECLIPSE_
#include "static_sz.h"
#else
#define ASCIILINESZ	1024
#endif

#include <string.h>
#include <ctype.h>

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

char * strlwc(char * s)
{
    static char l[ASCIILINESZ+1];
    int i ;

    if (s==NULL) return NULL ;
    memset(l, 0, ASCIILINESZ+1);
    i=0 ;
    while (s[i] && i<ASCIILINESZ) {
        l[i] = (char)tolower((int)s[i]);
        i++ ;
    }
    l[ASCIILINESZ]=(char)0;
    return l ;
}



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

char * strupc(char * s)
{
    static char l[ASCIILINESZ+1];
    int i ;

    if (s==NULL) return NULL ;
    memset(l, 0, ASCIILINESZ+1);
    i=0 ;
    while (s[i] && i<ASCIILINESZ) {
        l[i] = (char)toupper((int)s[i]);
        i++ ;
    }
    l[ASCIILINESZ]=(char)0;
    return l ;
}



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

char * strskp(char * s)
{
    char * skip = s;
	if (s==NULL) return NULL ;
    while (isspace((int)*skip) && *skip) skip++;
    return skip ;
} 



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

char * strcrop(char * s)
{
    static char l[ASCIILINESZ+1];
	char * last ;

    if (s==NULL) return NULL ;
    memset(l, 0, ASCIILINESZ+1);
	strcpy(l, s);
	last = l + strlen(l);
	while (last > l) {
		if (!isspace((int)*(last-1)))
			break ;
		last -- ;
	}
	*last = (char)0;
    return l ;
}


