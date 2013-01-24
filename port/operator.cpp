/**
 * @file operator.cpp
 * @author kubanec
 * @date 24.1.2013
 *
 */

#include "ch.h"

void* operator new(size_t sz)
{
	void * temp;
	temp = chCoreAlloc(sz);
	if (temp == NULL)
		asm ("bkpt");

	return temp;
}

void operator delete(void* m)
{
	(void) m;
}

