
/*---------------------------------------------------------------------------

   File name 	:	iniparser.c
   Author 		:	N. Devillard
   Created on	:	March 2000
   Description	:	Parser for ini files

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

#include "iniparser.h"
#include "strlib.h"

#ifdef _ECLIPSE_
#include "static_sz.h"
#include "comm.h"
#include "xmemory.h"
#else
#include "e_error.h"
#define ASCIILINESZ	1024
#endif


/*---------------------------------------------------------------------------
						Private to this module
 ---------------------------------------------------------------------------*/

/* Private: add an entry to the dictionary */
static void iniparser_add_entry(
	dictionary * d,
	char * sec,
	char * key,
	char * val)
{
	char		longkey[2*ASCIILINESZ+1];

	/* Make a key as section:keyword */
	if (key!=NULL) {
		sprintf(longkey, "%s:%s", sec, key);
	} else {
		strcpy(longkey, sec);
	}

	/* Add (key,val) to dictionary */
	dictionary_set(d, longkey, val);
	return ;
}



/*-------------------------------------------------------------------------*/
/**
  @name		iniparser_dump
  @memo		Dump a dictionary to an opened file pointer.
  @param	d	Dictionary to dump.
  @param	f	Opened file pointer to dump to.
  @return	void
  @doc

  This function prints out the contents of a dictionary, one element by
  line, onto the provided file pointer. It is OK to specify stderr or
  stdout as output files. This function is meant for debugging purposes
  mostly.
 */
/*--------------------------------------------------------------------------*/
void iniparser_dump(dictionary * d, FILE * f)
{
	int		i ;

	if (d==NULL || f==NULL) return ;
	for (i=0 ; i<d->n ; i++) {
		if (d->val[i]!=NULL) {
			fprintf(f, "[%s]=[%s]\n", d->key[i], d->val[i]);
		} else {
			fprintf(f, "[%s]=UNDEF\n", d->key[i]);
		}
	}
	return ;
}


/*-------------------------------------------------------------------------*/
/**
  @name		iniparser_dump_lua
  @memo		Dump a dictionary to an opened file pointer.
  @param	d	Dictionary to dump.
  @param	f	Opened file pointer to dump to.
  @return	void
  @doc

  This function prints out the contents of a dictionary, one element by
  line, onto the provided file pointer. It is OK to specify stderr or
  stdout as output files.

  The output syntax is fully compatible with lua tables.
 */
/*--------------------------------------------------------------------------*/
void iniparser_dump_lua(dictionary * d, FILE * f)
{
	int		i ;
	char	lua_s[ASCIILINESZ+1];
	int		j ;

	if (d==NULL || f==NULL) return ;
	fprintf(f, "ini={}\n");
	for (i=0 ; i<d->n ; i++) {
		memset(lua_s, 0, ASCIILINESZ+1);
		strcpy(lua_s, d->key[i]);
		j=0 ;
		while (lua_s[j]) {
			if (lua_s[j]==':' || lua_s[j]=='/')
				lua_s[j] = '.' ;
			j++ ;
		}
		if (d->val[i]!=NULL) {
			fprintf(f, "ini.%s='%s'\n", lua_s, d->val[i]);
		} else {
			fprintf(f, "ini.%s={}\n", lua_s);
		}
	}
	return ;
}


/*-------------------------------------------------------------------------*/
/**
  @name		iniparser_getstr
  @memo		Get the string associated to a key
  @param	d Dictionary to search
  @param	key Key string to look for
  @return	pointer to statically allocated character string
  @doc

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found, a
  NULL value is returned. The returned char pointer is pointing to a
  statically allocated string in this module.
 */
/*--------------------------------------------------------------------------*/


char * iniparser_getstr(dictionary * d, char * key)
{
	return dictionary_get(d, key);
}



/*-------------------------------------------------------------------------*/
/**
  @name		iniparser_getint
  @memo		Get the string associated to a key, convert to an int
  @param	d Dictionary to search
  @param	key Key string to look for
  @param	notFound Value to return in case of error
  @return	integer
  @doc

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notFound value is returned.
 */
/*--------------------------------------------------------------------------*/


int iniparser_getint(dictionary * d, char * key, int notfound)
{
	char	*	str ;

	str = iniparser_getstr(d, key);
	if (str==NULL) return notfound ;
	return atoi(str);
}


/*-------------------------------------------------------------------------*/
/**
  @name		iniparser_getdouble
  @memo		Get the string associated to a key, convert to a double
  @param	d Dictionary to search
  @param	key Key string to look for
  @param	notFound Value to return in case of error
  @return	double
  @doc

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notFound value is returned.
 */
/*--------------------------------------------------------------------------*/


double iniparser_getdouble(dictionary * d, char * key, double notfound)
{
	char	*	str ;

	str = iniparser_getstr(d, key);
	if (str==NULL) return notfound ;
	return atof(str);
}



/*-------------------------------------------------------------------------*/
/**
  @name		iniparser_getboolean
  @memo		Get the string associated to a key, convert to a boolean
  @param	d Dictionary to search
  @param	key Key string to look for
  @param	notFound Value to return in case of error
  @return	integer
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


int iniparser_getboolean(dictionary * d, char * key, int notfound)
{
	char	*	c ;
	int			ret ;

	c = iniparser_getstr(d, key);
	if (c==NULL) return notfound ;
    if (c[0]=='y' || c[0]=='Y' || c[0]=='1' || c[0]=='t' || c[0]=='T') {
        ret = 1 ;
    } else if (c[0]=='n' || c[0]=='N' || c[0]=='0' || c[0]=='f' || c[0]=='F') {
        ret = 0 ;
    } else {
        ret = notfound ;
    }
    return ret;
}


/*-------------------------------------------------------------------------*/
/**
  @name		iniparser_find_entry
  @memo		Finds out if a given entry exists in a dictionary
  @param	ini 	Dictionary to search
  @param	entry 	Name of the entry to look for
  @return	integer 1 if entry exists, 0 otherwise
  @doc

  Finds out if a given entry exists in the dictionary. Since sections
  are stored as keys with NULL associated values, this is the only way
  of querying for the presence of sections in a dictionary.
 */
/*--------------------------------------------------------------------------*/

int iniparser_find_entry(
	dictionary	*	ini,
	char		*	entry
)
{
	int			i ;
	unsigned	hash ;
	int			found ;

	found = 0 ;
	hash = dictionary_hash(entry);
	for (i=0 ; i<ini->n ; i++) {
		if (hash==ini->hash[i]) {
			if (!strcmp(entry, ini->key[i])) {
				found = 1 ;
				break ;
			}
		}
	}
	return found ;
}



/*-------------------------------------------------------------------------*/
/**
  @name		iniparser_setstr
  @memo		Set an entry in a dictionary.
  @param	ini		Dictionary to modify.
  @param	entry	Entry to modify (entry name)
  @param	val		New value to associate to the entry.
  @return	int 0 if Ok, -1 otherwise.
  @doc

  If the given entry can be found in the dictionary, it is modified to
  contain the provided value. If it cannot be found, -1 is returned.
  It is Ok to set val to NULL.
 */
/*--------------------------------------------------------------------------*/

int iniparser_setstr(
	dictionary	*	ini,
	char		*	entry,
	char		*	val
)
{
	dictionary_set(ini, entry, val);
	return 0 ;
}


/*-------------------------------------------------------------------------*/
/**
  @name		iniparser_load
  @memo		Parse an ini file and return an allocated dictionary object
  @param	ininame Name of the ini file to read.
  @return	Pointer to newly allocated dictionary
  @doc

  This is the parser for ini files. This function is called, providing
  the name of the file to be read. It returns a dictionary object that
  should not be accessed directly, but through accessor functions
  instead.

  The returned dictionary must be freed using iniparser_freedict().
 */
/*--------------------------------------------------------------------------*/

dictionary * iniparser_load(char * ininame)
{
	dictionary	*	d ;
	char		lin[ASCIILINESZ+1];
	char		sec[ASCIILINESZ+1];
	char		key[ASCIILINESZ+1];
	char		val[ASCIILINESZ+1];
	char	*	where ;
	FILE	*	ini ;
	int			lineno ;

	if ((ini=fopen(ininame, "r"))==NULL) {
		e_error("cannot open file %s\n", ininame);
		return NULL ;
	}

	/*
	 * Initialize a new dictionary entry
	 */
	d = dictionary_new(0);
	lineno = 0 ;
	while (fgets(lin, ASCIILINESZ, ini)!=NULL) {
		lineno++ ;
		where = strskp(lin); /* Skip leading spaces */
		if (*where==';' || *where=='#' || *where==0)
			continue ; /* Comment lines */
		else {
			if (sscanf(where, "[%[^]]", sec)==1) {
				/* Valid section name */
				strcpy(sec, strlwc(sec));
				iniparser_add_entry(d, sec, NULL, NULL);
			} else if (sscanf (where, "%[^=] = \"%[^\"]\"", key, val) == 2
				   ||  sscanf (where, "%[^=] = '%[^\']'",   key, val) == 2
				   ||  sscanf (where, "%[^=] = %[^;#]",     key, val) == 2) {
				strcpy(key, strlwc(strcrop(key)));
				/*
				 * sscanf cannot handle "" or '' as empty value,
				 * this is done here
				 */
				if (!strcmp(val, "\"\"") || !strcmp(val, "''")) {
					val[0] = (char)0;
				} else {
					strcpy(val, strcrop(val));
				}
				iniparser_add_entry(d, sec, key, val);
			}
		}
	}
	fclose(ini);
	return d ;
}



/*-------------------------------------------------------------------------*/
/**
  @name		iniparser_freedict
  @memo		Free all memory associated to an ini dictionary
  @param	d Dictionary to free
  @return	void
  @doc

  Free all memory associated to an ini dictionary.
  It is mandatory to call this function before the dictionary object
  gets out of the current context.
 */
/*--------------------------------------------------------------------------*/

void iniparser_freedict(dictionary * d)
{
	dictionary_del(d);
}
