
#include "e_error.h"

void e_warning(char *fmt, ...)
{
	va_list ap ;
	fprintf(stderr, "warning: ");
    va_start(ap, fmt) ;
    vfprintf(stderr, fmt, ap) ;
	fprintf(stderr, "\n");
	va_end(ap);
}

void e_error(char *fmt, ...)
{
	va_list ap ;
	fprintf(stderr, "*** error: ");
    va_start(ap, fmt) ;
    vfprintf(stderr, fmt, ap) ;
	fprintf(stderr, "\n");
	va_end(ap);
}

void e_comment(int level, char *fmt, ...)
{
	va_list ap ;
    va_start(ap, fmt) ;
    vfprintf(stderr, fmt, ap) ;
	fprintf(stderr, "\n");
	va_end(ap);
}

