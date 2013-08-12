#ifndef __LAUNCHER_ABSTRACTION_H
#define __LAUNCHER_ABSTRACTION_H

#ifndef LAUNCHER_ABSTRACTION
#  if defined(__declspec) && !defined(__BEOS__)
#    define LAUNCHER_ABSTRACTION(type) __declspec(dllimport) type __cdecl
#  else
#    define LAUNCHER_ABSTRACTION(type) type
#  endif
#endif  /* not defined LAUNCHER_ABSTRACTION */

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif




struct Launcher_Struct;
typedef struct Launcher_Struct *Launcher;



enum Launcher_PipeType {
  LAUNCHER_PIPE_READ = 0,
  LAUNCHER_PIPE_WRITE,
  LAUNCHER_PIPE_IGNORE
};


LAUNCHER_ABSTRACTION(Launcher)
Launcher_Create();


/*
Returns child_fd on success or -ve number on Failure
*/
LAUNCHER_ABSTRACTION(int)
Launcher_PipeCreate(Launcher context,int child_fd,enum Launcher_PipeType mode);

/*
Returns a file ID that corresponds to the other end of the unnamed pipe
connected to child_fd with respect to the child process.
Returns -ve number on Failure
*/

LAUNCHER_ABSTRACTION(int)
Launcher_PipeGet(Launcher context,int child_fd);

LAUNCHER_ABSTRACTION(int)
Launcher_PipeClose(Launcher context,int child_fd);

LAUNCHER_ABSTRACTION(void)
Launcher_PipeFree(Launcher context,int child_fd);


/*Note Clears arguments and makes first argument the exectuable path*/
LAUNCHER_ABSTRACTION(int)
Launcher_SetExecutable(Launcher context,const char *executable);

LAUNCHER_ABSTRACTION(char *)
Launcher_GetExecutable(Launcher context);

/*
Returns Count as a Return code
*/
LAUNCHER_ABSTRACTION(int)
Launcher_ArgumentsAdd(Launcher context,char *argument);

LAUNCHER_ABSTRACTION(int)
Launcher_ArgumentsInsert(Launcher context,char *argument,int index);

LAUNCHER_ABSTRACTION(char *)
Launcher_ArgumentsItem(Launcher context,int index);

LAUNCHER_ABSTRACTION(void)
Launcher_ArgumentsDelete(Launcher context,int index);


LAUNCHER_ABSTRACTION(void)
Launcher_ArgumentsClear(Launcher context,char *argument);


LAUNCHER_ABSTRACTION(char**)
Launcher_ArgumentsGet(Launcher context);


LAUNCHER_ABSTRACTION(int)
Launcher_Launch(Launcher context);

LAUNCHER_ABSTRACTION(pid_t)
Launcher_GetPid(Launcher context);

/*
options can be WNOHANG or WUNTRACED
*/

LAUNCHER_ABSTRACTION(int)
Launcher_Wait(Launcher context,int options);
/*
Return Code 1 if running and 0 is not running, -1 on error
*/
LAUNCHER_ABSTRACTION(int)
Launcher_Running(Launcher context);

/*
The value is set in the Launcher_Wait command once the application 
is terminated 
*/

LAUNCHER_ABSTRACTION(int)
Launcher_ReturnCodeGet(Launcher context);


LAUNCHER_ABSTRACTION(void )
Launcher_Free(Launcher context);

#ifdef __cplusplus
}
#endif

#endif
