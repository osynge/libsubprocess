#ifndef __PTRPTR_ABSTRACTION_H
#define __PTRPTR_ABSTRACTION_H

#ifndef PTRPTR_ABSTRACTION
#  if defined(__declspec) && !defined(__BEOS__)
#    define PTRPTR_ABSTRACTION(type) __declspec(dllimport) type __cdecl
#  else
#    define PTRPTR_ABSTRACTION(type) type
#  endif
#endif  /* not defined PTRPTR_ABSTRACTION */

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif




struct Ptrptr_Struct;
typedef struct Ptrptr_Struct *Ptrptr;


PTRPTR_ABSTRACTION(Ptrptr)
Ptrptr_Create();

PTRPTR_ABSTRACTION(int)
Ptrptr_Count(Ptrptr context);

PTRPTR_ABSTRACTION(int)
Ptrptr_Add(Ptrptr context,char *argument);

PTRPTR_ABSTRACTION(int)
Ptrptr_Insert(Ptrptr context, char *argument, int index);

PTRPTR_ABSTRACTION(char*)
Ptrptr_Item(Ptrptr context,int index);

PTRPTR_ABSTRACTION(int)
Ptrptr_Delete(Ptrptr context, int index);

PTRPTR_ABSTRACTION(void)
Ptrptr_Clear(Ptrptr context);

PTRPTR_ABSTRACTION(void )
Ptrptr_Free(Ptrptr context);

PTRPTR_ABSTRACTION(char **)
Ptrptr_Ptrptr(Ptrptr context);


#ifdef __cplusplus
}
#endif

#endif
