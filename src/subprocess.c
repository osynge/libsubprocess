#define DEBUG_LEVEL 0
#include "config.h"
#include "config_local.h"
#include <errno.h>



#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <asm/errno.h>
#include <string.h>

#include "ptrptr.h"
#include "subprocess.h"

/*Launcher Structure*/

struct Launcher_Struct {
	pid_t launched_process_id;
	struct Launcher_StructPipe *Launcher_Pipe_First;
	struct Launcher_StructPipe *Launcher_Pipe_Last;
	char *executable;
	int executable_allocated_size;
	int return_code;
	Ptrptr Attributes;
};

struct Launcher_StructPipe {
	struct Launcher_Struct * Launcher_owner;
	int file_descriptor_child;
	int file_descriptor_pairent;
	int file_descriptor_id_child;
	struct Launcher_StructPipe *Next;
	struct Launcher_StructPipe *Previous;
	enum Launcher_PipeType PipeType;
};





/*Prototypes*/

void
Launcher_Clear(Launcher context);
struct Launcher_StructPipe *
Launcher_StructPipe_Create(Launcher context);
void 
Launcher_StructPipe_Free(struct Launcher_StructPipe * pipe);




/*Public funtions*/

Launcher
Launcher_Create()
{
	Launcher contect;
	contect = (Launcher) malloc(sizeof (struct Launcher_Struct));
	contect->Launcher_Pipe_First = NULL;
	contect->Launcher_Pipe_Last = NULL;
	contect->executable = NULL;
	contect->executable_allocated_size = 0;
	contect->Attributes = Ptrptr_Create();
	contect->return_code = 0;
	return contect;
}

void
Launcher_Free(Launcher context)
{
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return;
	}
	state  = context;
	Launcher_Clear(state);
	if (state->executable != NULL)
	{
		free(state->executable);
	}
	Ptrptr_Free(state->Attributes);
	free(state);
	return;
}


int
Launcher_PipeCreate(Launcher context,int child_fd,enum Launcher_PipeType mode)
{
	struct Launcher_Struct *state;
	struct Launcher_StructPipe *new_pipe;
	if (NULL == context)
	{
		return -2;
	}
	state  = context;
	new_pipe = (struct Launcher_StructPipe *)Launcher_StructPipe_Create(state);	
	new_pipe->file_descriptor_child = -1;
	new_pipe->file_descriptor_pairent = -1;
	new_pipe->file_descriptor_id_child = child_fd;
	new_pipe->PipeType = mode;
	return new_pipe->file_descriptor_id_child;
}

void
Launcher_PipeFree(Launcher context,int child_fd)
{
	struct Launcher_StructPipe *pipe_to_close;
	struct Launcher_StructPipe *pipe_cursor;
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return;
	}
	state  = context;
	pipe_to_close = NULL;
	pipe_cursor = state->Launcher_Pipe_First;
	while ((pipe_to_close == NULL) && (pipe_cursor != NULL))
	{
		if (child_fd == pipe_cursor->file_descriptor_id_child)
		{
			pipe_to_close = pipe_cursor;
		}
		pipe_cursor = pipe_cursor->Next;
	}
	if (pipe_to_close != NULL)
	{
		Launcher_StructPipe_Free(pipe_to_close);
	}
	return;	
}

int
Launcher_PipeGet(Launcher context,int child_fd)
{
	struct Launcher_Struct *state;
	struct Launcher_StructPipe *pipe_cursor;
	int output;
	if (NULL == context)
	{
		return -2;
	}
	state  = context;
	output = -1;
	pipe_cursor = state->Launcher_Pipe_First;
	while ((output < 0 ) && (pipe_cursor != NULL))
	{
		if (child_fd == pipe_cursor->file_descriptor_id_child)
		{
			output = pipe_cursor->file_descriptor_pairent;
		}
		pipe_cursor = pipe_cursor->Next;
	}
	return output;
}
int
Launcher_PipeClose(Launcher context,int child_fd)
{
	struct Launcher_StructPipe *pipe_to_close;
	struct Launcher_StructPipe *pipe_cursor;
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	pipe_to_close = NULL;
	pipe_cursor = state->Launcher_Pipe_First;
	while ((pipe_to_close == NULL) && (pipe_cursor != NULL))
	{
		if (pipe_cursor->file_descriptor_id_child == child_fd)
		{
			close(pipe_cursor->file_descriptor_pairent);
			pipe_cursor->file_descriptor_pairent = -1;
			return 0;
		}
		pipe_cursor = pipe_cursor->Next;
	}
	return -1;;	
}


int
Launcher_SetExecutable(Launcher context,const char *executable)
{
	struct Launcher_Struct *state;
	int executable_path_size;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	if (0 != access(executable,X_OK))
	{
		fprintf(stderr,"Application path is not executable='%s'\n",executable);
		return -2;
	}
	executable_path_size = strlen(executable);
	if (state->executable_allocated_size < (2 * executable_path_size))
	{
		if (state->executable == NULL)
		{
			free(state->executable);
		}
		state->executable_allocated_size = 2 * executable_path_size;
		state->executable = (char *)malloc(sizeof(char) * state->executable_allocated_size);
		if (state->executable == NULL)
		{
			return -3;
		}		
	}
	if (NULL == strcpy(state->executable,executable))
	{
		return -4;
	}
	Ptrptr_Clear(state->Attributes);
	Ptrptr_Add(state->Attributes,state->executable);
	return 0;	
}

char *
Launcher_GetExecutable(Launcher context)
{
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return NULL;
	}
	state  = context;
	return state->executable;
}

int
Launcher_Launch(Launcher context)
{
	char **tmp;
	struct Launcher_StructPipe *pipe_cursor;
	struct Launcher_Struct *state;
	int filedes[2];
	if (NULL == context)
	{
		fprintf(stderr,"Calling contect is NULL\n");
		return -1;
	}
	state  = context;
	
	pipe_cursor = state->Launcher_Pipe_First;
	while (pipe_cursor != NULL)
	{
		
		switch (pipe_cursor->PipeType)
		{
			case LAUNCHER_PIPE_WRITE:
				if (0 !=  pipe(filedes))
				{
					return -3;		
				}
				pipe_cursor->file_descriptor_child = filedes[0];
				pipe_cursor->file_descriptor_pairent = filedes[1];

				break;
			case LAUNCHER_PIPE_READ:
				if (0 !=  pipe(filedes))
				{
					return -3;		
				}								
				pipe_cursor->file_descriptor_child = filedes[1];
				pipe_cursor->file_descriptor_pairent = filedes[0];
				break;
			case LAUNCHER_PIPE_IGNORE:
				pipe_cursor->file_descriptor_child = open("/dev/null",O_RDWR);
				pipe_cursor->file_descriptor_pairent = -1;
				break;		
			default :
				return -4;
	
		}
		pipe_cursor = pipe_cursor->Next;
	}
	state->launched_process_id = fork();
	if (state->launched_process_id == -1) 
	{
		fprintf(stderr, "Fork failure\n");			
		return(-2);
	}
		/* DEBUG CODE */
	
	tmp = Ptrptr_Ptrptr(state->Attributes);
	if (state->launched_process_id == 0) 
	{
		// This is child process

		int counter;
		pipe_cursor = state->Launcher_Pipe_First;
		while (pipe_cursor != NULL)
		{
			if (pipe_cursor->file_descriptor_pairent >= 0 )
			{
				#ifdef DEBUG_CODE
				fprintf(stderr, "Child Closing %d\n",pipe_cursor->file_descriptor_pairent);			
				#endif
				close(pipe_cursor->file_descriptor_pairent);
			}
			
			if ((pipe_cursor->file_descriptor_id_child >= 0) && (pipe_cursor->file_descriptor_child >= 0 ))
			{
				#ifdef DEBUG_CODE				
				fprintf(stderr, "Child Closing intended %d\n",pipe_cursor->file_descriptor_id_child);							
				#endif	
				close(pipe_cursor->file_descriptor_id_child);

				if ( pipe_cursor->file_descriptor_id_child != dup2(pipe_cursor->file_descriptor_child,pipe_cursor->file_descriptor_id_child))
				{
					if (EBADF == pipe_cursor->file_descriptor_id_child)
					{
						fprintf(stderr, "Old filedescriptor does not exist: %d\n",pipe_cursor->file_descriptor_id_child);			
					}
					
					fprintf(stderr, "error for dup2 for %d,%d\n",pipe_cursor->file_descriptor_child,pipe_cursor->file_descriptor_id_child);

					return -1;
				}
				#ifdef DEBUG_CODE				
				else
				{
					
					fprintf(stderr, "duped  %d,%d\n",pipe_cursor->file_descriptor_child,pipe_cursor->file_descriptor_id_child);							

				}				
				#endif	
			}
			pipe_cursor = pipe_cursor->Next;
		}
		// No we can execute the handler							
		
		execv(state->executable, tmp);
		// Well we might as well Shout our failure to launch the application
		fprintf(stderr,"execv failed calling '%s'\n",state->executable);
		counter = 0;
		while (NULL != tmp[counter])
		{
			fprintf(stderr, "arg=%d value=%s\n",counter,Ptrptr_Ptrptr(state->Attributes)[counter]);
			counter++;
		}

		exit(-1);
	}
	else
	{
		// This is pairent process
		// Close all pipes made to child process
		pipe_cursor = state->Launcher_Pipe_First;
		while (pipe_cursor != NULL)
		{
			/*fprintf(stderr, "Pairent Closing %d\n",pipe_cursor->file_descriptor_child);*/
			close(pipe_cursor->file_descriptor_child);
			/*pipe_cursor->file_descriptor_id_child = -1;*/
			pipe_cursor = pipe_cursor->Next;
		}
	}
	return 0;
}


pid_t
Launcher_GetPid(Launcher context)
{
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	return state->launched_process_id;

}



int
Launcher_Wait(Launcher context, int options)
{
	struct Launcher_Struct *state;
	int status;
	int *statusptr;
	int rc;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	statusptr = &status;
	rc = waitpid(state->launched_process_id,statusptr,options);
	if (rc == -1)
	{
		return errno;
	}
	if (statusptr != NULL)
	{	
		if WIFEXITED(status)
		{
			/*Exited fine*/
			state->return_code = WEXITSTATUS(status);
		}
		else
		{
			/*Exited bad*/
			state->return_code = -1;
		}
		
	}
	return 0;
	
}

int
Launcher_Running(Launcher context)
{
	struct Launcher_Struct *state;
	int status;
	int * current_state;
	int rc;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	current_state = &status;
	rc = waitpid(state->launched_process_id,current_state,WNOHANG);
	if (rc == 0)
	{
		return 1;
	}
	return 0;
	
}


int
Launcher_ReturnCodeGet(Launcher context)
{
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	return state->return_code;
}


/*Private funtions*/

void
Launcher_Clear(Launcher context)
{
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return;
	}
	state  = context;
	state->launched_process_id = 0;
	while (state->Launcher_Pipe_First != NULL)
	{
		Launcher_StructPipe_Free(state->Launcher_Pipe_First);
	}
	if (state->executable != NULL)
	{
		state->executable = '\0';
		state->executable_allocated_size = 0;
	}
	state->return_code = 0;
	return;
}

int
Launcher_ArgumentsAdd(Launcher context,char *argument)
{
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	return Ptrptr_Add(state->Attributes,argument);
}



int
Launcher_ArgumentsInsert(Launcher context,char *argument,int index)
{
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	return Ptrptr_Insert(state->Attributes,argument,index);
}


char *
Launcher_ArgumentsItem(Launcher context,int index)
{
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return NULL;
	}
	state  = context;
	return Ptrptr_Item(state->Attributes,index);
}


void
Launcher_ArgumentsDelete(Launcher context,int index)
{
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return;
	}
	state  = context;
	Ptrptr_Delete(state->Attributes,index);
	return ;
}


/*Launcher_StructPipe Public funtions*/

struct Launcher_StructPipe *
Launcher_StructPipe_Create(Launcher context)
{
	struct Launcher_StructPipe *new_pipe;
	struct Launcher_Struct *state;
	if (NULL == context)
	{
		return NULL;
	}
	state  = context;
	new_pipe = (struct Launcher_StructPipe *) malloc(sizeof (struct Launcher_StructPipe));
	if (new_pipe == NULL)
	{
		return NULL;
	}
	if (state->Launcher_Pipe_First == NULL)
	{
		state->Launcher_Pipe_First = new_pipe;
		state->Launcher_Pipe_Last = new_pipe;
		new_pipe->Next = NULL;
		new_pipe->Previous = NULL;
	}
	else
	{
		new_pipe->Next = NULL;
		new_pipe->Previous = state->Launcher_Pipe_Last;
		state->Launcher_Pipe_Last->Next = new_pipe;
		state->Launcher_Pipe_Last = new_pipe;
	}
	new_pipe->file_descriptor_child = -1;
	new_pipe->file_descriptor_pairent = -1;
	new_pipe->Launcher_owner = context;
	new_pipe->PipeType = LAUNCHER_PIPE_IGNORE;
	return new_pipe;
}

void Launcher_StructPipe_Free(struct Launcher_StructPipe * pipe)
{
	struct Launcher_Struct *state;
	if (NULL == pipe->Launcher_owner)
	{
		return;
	}
	state  = pipe->Launcher_owner;
	if ((state->Launcher_Pipe_First != pipe) && (state->Launcher_Pipe_Last == pipe))
	{
		pipe->Next->Previous = pipe->Previous;
		pipe->Previous->Next = pipe->Next;
	}
	if (state->Launcher_Pipe_First == pipe)
	{
		state->Launcher_Pipe_First = pipe->Next;
		if (state->Launcher_Pipe_First != NULL)
		{
			state->Launcher_Pipe_First->Previous = NULL;
		}
		
	}
	if (state->Launcher_Pipe_Last == pipe)
	{
		state->Launcher_Pipe_Last = pipe->Previous;
		if (state->Launcher_Pipe_Last != NULL)
		{
			state->Launcher_Pipe_Last->Next = NULL;
		}
	}
	if (0 >= pipe->file_descriptor_child)
	{
		close(pipe->file_descriptor_child);
	}
	if (0 >= pipe->file_descriptor_pairent)
	{
		close(pipe->file_descriptor_pairent);
	}
	free(pipe);
}




/*Launcher_StructPipe Private funtions*/



