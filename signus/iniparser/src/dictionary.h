
/*---------------------------------------------------------------------------
 
   File name 	:	dictionary.h
   Author 		:	Nicolas Devillard
   Created on	:	August 2000
   Description	:	dictionary implements a dictionary for string variables.

 *--------------------------------------------------------------------------*/

/*
	$Id$
	$Author$
	$Date$
	$Revision$
*/

#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

/*---------------------------------------------------------------------------
   								Includes
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*---------------------------------------------------------------------------
   								New types
 ---------------------------------------------------------------------------*/

typedef struct _dictionary_ {
	int				n ;
	int				size ;
	char 		**	val ;
	char 		**  key ;
	unsigned	 *	hash ;
} dictionary ;


/*---------------------------------------------------------------------------
  							Function prototypes
 ---------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/**
  @name     dictionary_hash
  @memo     Compute the hash key for a string.
  @param    key     Character string to use for key.
  @return   1 unsigned int on at least 32 bits.
  @doc
 
  This hash function has been taken from an Article in Dr Dobbs Journal.
  This is normally a collision-free function, distributing keys evenly.
  The key is stored anyway in the struct so that collision can be avoided
  by comparing the key itself in last resort.
 */
/*--------------------------------------------------------------------------*/
 
unsigned dictionary_hash(const char * key);


/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_new
  @memo		Create a new dictionary object.
  @param	size	Optional initial size of the dictionary.
  @return	1 newly allocated dictionary objet.
  @doc

  This function allocates a new dictionary object of given size and returns
  it. If you do not know in advance (roughly) the number of entries in the
  dictionary, give size=0.
 */
/*--------------------------------------------------------------------------*/

dictionary * dictionary_new(int size);


/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_del
  @memo		Delete a dictionary object
  @param	vd	dictionary object to deallocate.
  @return	void
  @doc

  Deallocate a dictionary object and all memory associated to it.
 */
/*--------------------------------------------------------------------------*/

void dictionary_del(dictionary * vd);


/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_get
  @memo		Get a value from a dictionary.
  @param	vd		dictionary object to search.
  @param	key		Key to look for in the dictionary.
  @return	1 pointer to internally allocated character string.
  @doc

  This function locates a key in a dictionary and returns a pointer to its
  value, or NULL if no such key can be found in dictionary. The returned
  character pointer points to data internal to the dictionary object, you
  should not try to free it or modify it.
 */
/*--------------------------------------------------------------------------*/
const char * dictionary_get(dictionary * vd, const char * key);

/*-------------------------------------------------------------------------*/
/**
  @name     dictionary_getchar
  @memo     Get a value from a dictionary, as a char.
  @param    d       dictionary object to search.
  @param    key     Key to look for in the dictionary.
  @param    def     Default value for the key if not found.
  @return   char    
  @doc

  This function locates a key in a dictionary using dictionary_get,
  and returns the first char of the found string.
 */
/*--------------------------------------------------------------------------*/
char dictionary_getchar(dictionary * d, const char * key, char def) ;

/*-------------------------------------------------------------------------*/
/**
  @name     dictionary_getint
  @memo     Get a value from a dictionary, as an int.
  @param    d       dictionary object to search.
  @param    key     Key to look for in the dictionary.
  @param    def     Default value for the key if not found.
  @return   int
  @doc
 
  This function locates a key in a dictionary using dictionary_get,
  and applies atoi on it to return an int. If the value cannot be found
  in the dictionary, the default is returned.
 */
/*--------------------------------------------------------------------------*/
int dictionary_getint(dictionary * d, const char * key, int def);


/*-------------------------------------------------------------------------*/
/**
  @name     dictionary_getdouble
  @memo     Get a value from a dictionary, as a double.
  @param    d       dictionary object to search.
  @param    key     Key to look for in the dictionary.
  @param    def     Default value for the key if not found.
  @return   double
  @doc
 
  This function locates a key in a dictionary using dictionary_get,
  and applies atof on it to return a double. If the value cannot be found
  in the dictionary, the default is returned.
 */
/*--------------------------------------------------------------------------*/
double dictionary_getdouble(dictionary * d, const char * key, double def);



/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_set
  @memo		Set a value in a dictionary.
  @param	vd		dictionary object to modify.
  @param	key		Key to modify or add.
  @param	val 	Value to add.
  @return	void
  @doc

  If the given key is found in the dictionary, the associated value is
  replaced by the provided one. If the key cannot be found in the
  dictionary, it is added to it.

  It is Ok to provide a NULL value for val, but NULL values for the dictionary
  or the key are considered as errors: the function will return immediately
  in such a case.
 */
/*--------------------------------------------------------------------------*/

void dictionary_set(dictionary * vd, const char * key, const char * val);


/*-------------------------------------------------------------------------*/
/**
  @name     dictionary_setint
  @memo     Set a key in a dictionary, providing an int.
  @param    d       Dictionary to update.
  @param    key     Key to modify or add
  @param    val     Integer value to store (will be stored as a string).
  @return   void
  @doc
 
  This helper function calls dictionary_set() with the provided integer
  converted to a string using %d.
 */
/*--------------------------------------------------------------------------*/
 
 
void dictionary_setint(dictionary * d, const char * key, int val);



/*-------------------------------------------------------------------------*/
/**
  @name     dictionary_setdouble
  @memo     Set a key in a dictionary, providing a double.
  @param    d       Dictionary to update.
  @param    key     Key to modify or add
  @param    val     Double value to store (will be stored as a string).
  @return   void
  @doc
 
  This helper function calls dictionary_set() with the provided double
  converted to a string using %g.
 */
/*--------------------------------------------------------------------------*/
 
 
void dictionary_setdouble(dictionary * d, const char * key, double val);



/*-------------------------------------------------------------------------*/
/**
  @name     dictionary_dump
  @memo     Dump a dictionary to an opened file pointer.
  @param    d   Dictionary to dump
  @param    f   Opened file pointer.
  @return   void
  @doc
 
  Dumps a dictionary onto an opened file pointer. Key pairs are printed out
  as [Key]=[Value], one per line. It is Ok to provide stdout or stderr as
  output file pointers.
 */
/*--------------------------------------------------------------------------*/
 
void dictionary_dump(dictionary * d, FILE * out);

#endif
