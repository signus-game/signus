
/*---------------------------------------------------------------------------

   File name 	:	dictionary.c
   Author 		:	Nicolas Devillard
   Created on	:	August 2000
   Description	:	Implements a dictionary for string variables.

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

#include "dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _ECLIPSE_
#include "xmemory.h"
#endif



/* The following defines a maximum value size for integers and doubles. */
#define MAXVALSZ	1024

/* This is the minimal allocated number of entries in a dictionary */
#define DICTMINSZ	128

/*---------------------------------------------------------------------------
  							Private functions
 ---------------------------------------------------------------------------*/

/* Doubles the allocated size associated to a pointer */
/* 'size' is the current allocated size. */
static void * mem_double(void * ptr, int size)
{
    void    *   newptr ;
 
    newptr = calloc(2*size, 1);
    memcpy(newptr, ptr, size);
    free(ptr);
    return newptr ;
}


/*---------------------------------------------------------------------------
  							Function codes
 ---------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_hash
  @memo		Compute the hash key for a string.
  @param	key		Character string to use for key.
  @return	1 unsigned int on at least 32 bits.
  @doc

  This hash function has been taken from an Article in Dr Dobbs Journal.
  This is normally a collision-free function, distributing keys evenly.
  The key is stored anyway in the struct so that collision can be avoided
  by comparing the key itself in last resort.
 */
/*--------------------------------------------------------------------------*/

unsigned dictionary_hash(char * key)
{
	int			len ;
	unsigned	hash ;
	int			i ;

	len = strlen(key);
	for (hash=0, i=0 ; i<len ; i++) {
		hash += (unsigned)key[i] ;
		hash += (hash<<10);
		hash ^= (hash>>6) ;
	}
	hash += (hash <<3);
	hash ^= (hash >>11);
	hash += (hash <<15);
	return hash ;
}


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

dictionary * dictionary_new(int size)
{
	dictionary	*	d ;

	/* If no size was specified, allocate space for DICTMINSZ */
	if (size<1) size=DICTMINSZ ;

	d = malloc(sizeof(dictionary));
	d->n = 0 ;
	d->size = size ;
	d->val  = malloc(size * sizeof(char*));
	d->key  = malloc(size * sizeof(char*));
	d->hash = malloc(size * sizeof(unsigned));
	return d ;
}


/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_del
  @memo		Delete a dictionary object
  @param	d	dictionary object to deallocate.
  @return	void
  @doc

  Deallocate a dictionary object and all memory associated to it.
 */
/*--------------------------------------------------------------------------*/

void dictionary_del(dictionary * d)
{
	int		i ;

	if (d==NULL) return ;
	for (i=0 ; i<d->n ; i++) {
		if (d->val[i]!=NULL)
			free(d->val[i]);
		if (d->key[i]!=NULL)
			free(d->key[i]);
	}
	free(d->val);
	free(d->key);
	free(d->hash);
	free(d);
	return ;
}



/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_get
  @memo		Get a value from a dictionary.
  @param	d		dictionary object to search.
  @param	key		Key to look for in the dictionary.
  @return	1 pointer to internally allocated character string.
  @doc

  This function locates a key in a dictionary and returns a pointer to its
  value, or NULL if no such key can be found in dictionary. The returned
  character pointer points to data internal to the dictionary object, you
  should not try to free it or modify it.
 */
/*--------------------------------------------------------------------------*/
char * dictionary_get(dictionary * d, char * key)
{
	unsigned	hash ;
	int			i ;

	hash = dictionary_hash(key);
	for (i=0 ; i<d->n ; i++) {
		if (hash==d->hash[i]) {
			if (key[0] == d->key[i][0]) {
				return d->val[i] ;
			}
		}
	}
	return NULL ;
}

/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_getchar
  @memo		Get a value from a dictionary, as a char.
  @param	d		dictionary object to search.
  @param	key		Key to look for in the dictionary.
  @param	def		Default value for the key if not found.
  @return 	char	
  @doc

  This function locates a key in a dictionary using dictionary_get,
  and returns the first char of the found string.
 */
/*--------------------------------------------------------------------------*/
char dictionary_getchar(dictionary * d, char * key, char def)
{
	char * v ;

	if ((v=dictionary_get(d,key))==NULL) {
		return def ;
	} else {
		return v[0] ;
	}
}


/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_getint
  @memo		Get a value from a dictionary, as an int.
  @param	d		dictionary object to search.
  @param	key		Key to look for in the dictionary.
  @param	def		Default value for the key if not found.
  @return	int
  @doc

  This function locates a key in a dictionary using dictionary_get,
  and applies atoi on it to return an int. If the value cannot be found
  in the dictionary, the default is returned.
 */
/*--------------------------------------------------------------------------*/
int dictionary_getint(dictionary * d, char * key, int def)
{
	char * v ;

	if ((v=dictionary_get(d,key))==NULL) {
		return def ;
	} else {
		return atoi(v);
	}
}

/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_getdouble
  @memo		Get a value from a dictionary, as a double.
  @param	d		dictionary object to search.
  @param	key		Key to look for in the dictionary.
  @param	def		Default value for the key if not found.
  @return	double
  @doc

  This function locates a key in a dictionary using dictionary_get,
  and applies atof on it to return a double. If the value cannot be found
  in the dictionary, the default is returned.
 */
/*--------------------------------------------------------------------------*/
double dictionary_getdouble(dictionary * d, char * key, double def)
{
	char * v ;

	if ((v=dictionary_get(d,key))==NULL) {
		return def ;
	} else {
		return atof(v);
	}
}


/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_set
  @memo		Set a value in a dictionary.
  @param	d		dictionary object to modify.
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

  Notice that if you dictionary_set a variable to NULL, a call to
  dictionary_get will return a NULL value: the variable will be found, and
  its value (NULL) is returned. In other words, setting the variable
  content to NULL is equivalent to deleting the variable from the
  dictionary. It is not possible (in this implementation) to have a key in
  the dictionary without value.
 */
/*--------------------------------------------------------------------------*/

void dictionary_set(dictionary * d, char * key, char * val)
{
	int			i ;
	unsigned	hash ;

	if (d==NULL || key==NULL) return ;
	
	/* Compute hash for this key */
	hash = dictionary_hash(key) ;
	/* Find if value is already in blackboard */
	if (d->n>0) {
		for (i=0 ; i<d->n ; i++) {
			if (hash==d->hash[i]) { /* Same hash value */
				if (key[0]==d->key[i][0]) {	 /* Same key */
					/* Found a value: modify and return */
					if (d->val[i]!=NULL)
						free(d->val[i]);
					if (val!=NULL) {
						d->val[i] = strdup(val);
					} else {
						d->val[i] = NULL ;
					}
					return ;
				}
			}
		}
	}

	/* Add a new value */
	/* See if dictionary needs allocation */
	if (d->n==d->size) {

		/* Reached maximum size: reallocate blackboard */
		d->val  = mem_double(d->val, d->size * sizeof(char*)) ;
		d->key  = mem_double(d->key, d->size * sizeof(char*)) ;
		d->hash = mem_double(d->hash, d->size * sizeof(unsigned)) ;

		/* Double size */
		d->size *= 2 ;
	}

	/* Copy key */

	d->key[d->n] = strdup(key);
	if (val!=NULL) {
		d->val[d->n] = strdup(val);
	} else {
		d->val[d->n] = NULL ;
	}
	d->hash[d->n] = hash;
	d->n ++ ;
	return ;
}


/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_setint
  @memo		Set a key in a dictionary, providing an int.
  @param	d		Dictionary to update.
  @param	key		Key to modify or add
  @param	val		Integer value to store (will be stored as a string).
  @return	void
  @doc

  This helper function calls dictionary_set() with the provided integer
  converted to a string using %d.
 */
/*--------------------------------------------------------------------------*/


void dictionary_setint(dictionary * d, char * key, int val)
{
	char	sval[MAXVALSZ];
	sprintf(sval, "%d", val);
	dictionary_set(d, key, sval);
}


/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_setdouble
  @memo		Set a key in a dictionary, providing a double.
  @param	d		Dictionary to update.
  @param	key		Key to modify or add
  @param	val		Double value to store (will be stored as a string).
  @return	void
  @doc

  This helper function calls dictionary_set() with the provided double
  converted to a string using %g.
 */
/*--------------------------------------------------------------------------*/


void dictionary_setdouble(dictionary * d, char * key, double val)
{
	char	sval[MAXVALSZ];
	sprintf(sval, "%g", val);
	dictionary_set(d, key, sval);
}



/*-------------------------------------------------------------------------*/
/**
  @name		dictionary_dump
  @memo		Dump a dictionary to an opened file pointer.
  @param	d	Dictionary to dump
  @param	f	Opened file pointer.
  @return	void
  @doc

  Dumps a dictionary onto an opened file pointer. Key pairs are printed out
  as [Key]=[Value], one per line. It is Ok to provide stdout or stderr as
  output file pointers.
 */
/*--------------------------------------------------------------------------*/

void dictionary_dump(dictionary * d, FILE * out)
{
	int		i ;

	if (d==NULL || out==NULL) return ;
	if (d->n<1) {
		fprintf(out, "empty dictionary\n");
		return ;
	}
	for (i=0 ; i<d->n ; i++) {
		fprintf(out, "%20s\t[%s]\n",
				d->key[i],
				d->val[i]);
	}
	return ;
}


#ifdef TEST
#define NVALS 20000
int main(int argc, char *argv[])
{
	dictionary	*	d ;
	char	*	val ;
	int			i ;
	char		cval[90] ;

	/* allocate blackboard */
	printf("allocating...\n");
	d = dictionary_new(0);
	
	/* Set values in blackboard */
	printf("setting %d values...\n", NVALS);
	for (i=0 ; i<NVALS ; i++) {
		sprintf(cval, "%04d", i);
		dictionary_set(d, cval, "salut");
	}
	printf("getting %d values...\n", NVALS);
	for (i=0 ; i<NVALS ; i++) {
		sprintf(cval, "%04d", i);
		val = dictionary_get(d, cval);
		if (val==NULL) {
			printf("cannot get value for key [%s]\n", cval);
		}
	}
	printf("deallocating...\n");
	dictionary_del(d);
#if _XMEMORY_
	xmemory_status();
#endif
	return 0 ;
}
#endif
