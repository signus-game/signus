#ifndef _SYSTEM_INOTBUTOXUTOMHUNOBUTOBUTBOTBUTOBU_H
#define _SYSTEM_INOTBUTOXUTOMHUNOBUTOBUTBOTBUTOBU_H

#include "stream.h"

#define memalloc malloc
#define memfree free

// Mock implementation for utils-game code compatibility
static void multipath_fopen(File &f, const char *name, unsigned mode) {
	f.open(name, mode);
}

#endif
