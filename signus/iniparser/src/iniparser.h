
/*---------------------------------------------------------------------------

   File name 	:	iniparser.h
   Author 		:	N. Devillard
   Created on	:	March 2000
   Language		:	ANSI C
   Description	:	Parser for ini files

 *--------------------------------------------------------------------------*/

/*

	$Id$
	$Author$
	$Date$
	$Revision$

*/

#ifndef _INIPARSER_H_
#define _INIPARSER_H_

/*---------------------------------------------------------------------------
   								Includes
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dictionary.h"


/*-------------------------------------------------------------------------*/
/**
  @name     iniparser_dump
  @memo     Dump a dictionary to an opened file pointer.
  @param    d   Dictionary to dump.
  @param    f   Opened file pointer to dump to.
  @return   void
  @doc
 
  This function prints out the contents of a dictionary, one element by
  line, onto the provided file pointer. It is OK to specify stderr or
  stdout as output files. This function is meant for debugging purposes
  mostly.
 */
/*--------------------------------------------------------------------------*/
void iniparser_dump(dictionary * d, FILE * f);


/*-------------------------------------------------------------------------*/
/**
  @name     iniparser_dump_lua
  @memo     Dump a dictionary to an opened file pointer.
  @param    d   Dictionary to dump.
  @param    f   Opened file pointer to dump to.
  @return   void
  @doc
 
  This function prints out the contents of a dictionary, one element by
  line, onto the provided file pointer. It is OK to specify stderr or
  stdout as output files.
 
  The output syntax is fully compatible with lua tables.
 */
/*--------------------------------------------------------------------------*/
void iniparser_dump_lua(dictionary * d, FILE * f);



/*-------------------------------------------------------------------------*/
/**
  @name     iniparser_getstr
  @memo     Get the string associated to a key
  @param    d Dictionary to search
  @param    key Key string to look for
  @return   pointer to statically allocated character string
  @doc
 
  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found, a
  NULL value is returned. The returned char pointer is pointing to a
  statically allocated string in this module.
 */
/*--------------------------------------------------------------------------*/
            
char * iniparser_getstr(dictionary * d, char * key);


/*-------------------------------------------------------------------------*/
/**
  @name     iniparser_getint
  @memo     Get the string associated to a key, convert to an int
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notFound Value to return in case of error
  @return   integer
  @doc
 
  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notFound value is returned.
 */
/*--------------------------------------------------------------------------*/  

int iniparser_getint(dictionary * d, char * key, int notfound);


/*-------------------------------------------------------------------------*/
/**
  @name     iniparser_getdouble
  @memo     Get the string associated to a key, convert to a double
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notFound Value to return in case of error
  @return   double
  @doc
 
  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notFound value is returned.
 */
/*--------------------------------------------------------------------------*/  

double iniparser_getdouble(dictionary * d, char * key, double notfound);


/*-------------------------------------------------------------------------*/
/**
  @name     iniparser_getboolean
  @memo     Get the string associated to a key, convert to a boolean
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notFound Value to return in case of error
  @return   integer
  @doc
 
  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notFound value is returned.
 
  A true boolean is found if one of the following is matched:
  \begin{itemize}
  \item A string starting with 'y'
  \item A string starting with 'Y'
  \item A string starting with 't'
  \item A string starting with 'T'
  \item A string starting with '1'
  \end{itemize}
 
  A false boolean is found if one of the following is matched:
  \begin{itemize}
  \item A string starting with 'n'
  \item A string starting with 'N'
  \item A string starting with 'f'
  \item A string starting with 'F'
  \item A string starting with '0'
  \end{itemize}
 
  The notFound value returned if no boolean is identified, does not
  necessarily have to be 0 or 1.
 
 */
/*--------------------------------------------------------------------------*/  

int iniparser_getboolean(dictionary * d, char * key, int notfound);


/*-------------------------------------------------------------------------*/
/**
  @name     iniparser_setstr
  @memo     Set an entry in a dictionary.
  @param    ini     Dictionary to modify.
  @param    entry   Entry to modify (entry name)
  @param    val     New value to associate to the entry.
  @return   int 0 if Ok, -1 otherwise.
  @doc
 
  If the given entry can be found in the dictionary, it is modified to
  contain the provided value. If it cannot be found, -1 is returned.
 */
/*--------------------------------------------------------------------------*/
 
int iniparser_setstr(dictionary * ini, char * entry, char * val) ;


/*-------------------------------------------------------------------------*/
/**
  @name     iniparser_find_entry
  @memo     Finds out if a given entry exists in a dictionary
  @param    ini     Dictionary to search
  @param    entry   Name of the entry to look for
  @return   integer 1 if entry exists, 0 otherwise
  @doc
 
  Finds out if a given entry exists in the dictionary. Since sections
  are stored as keys with NULL associated values, this is the only way
  of querying for the presence of sections in a dictionary.
 */
/*--------------------------------------------------------------------------*/
 
int iniparser_find_entry(dictionary * ini, char * entry) ;

/*-------------------------------------------------------------------------*/
/**
  @name     iniparser_load
  @memo     Parse an ini file and return an allocated dictionary object
  @param    ininame Name of the ini file to read.
  @return   Pointer to newly allocated dictionary
  @doc
 
  This is the parser for ini files. This function is called, providing
  the name of the file to be read. It returns a dictionary object that
  should not be accessed directly, but through accessor functions
  instead.
 
  The returned dictionary must be freed using iniparser_freedict().
 */
/*--------------------------------------------------------------------------*/  


dictionary * iniparser_load(char * ininame);


/*-------------------------------------------------------------------------*/
/**
  @name     iniparser_freedict
  @memo     Free all memory associated to an ini dictionary
  @param    d Dictionary to free
  @return   void
  @doc
 
  Free all memory associated to an ini dictionary.
  It is mandatory to call this function before the dictionary object
  gets out of the current context.
 */
/*--------------------------------------------------------------------------*/  

void iniparser_freedict(dictionary * d);

#endif
