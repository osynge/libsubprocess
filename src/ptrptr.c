#include "config.h"
#include "config_local.h"

#include "ptrptr.h"
#include <stdlib.h>
#include <string.h>
#define PTRPTR_BUFFERSIZEDEFAULT 256
#include <stdio.h>



typedef struct Ptrptr_Item *Item;



struct Ptrptr_Struct {
	char **ptrptr;
	int  ptrptr_allocated;
	int  ptrptr_count;
};


int
Ptrptr_Extend(Ptrptr context);

Ptrptr
Ptrptr_Create()
{
	Ptrptr state;
	state = malloc(sizeof(struct Ptrptr_Struct));
	state->ptrptr = NULL;
	state->ptrptr_allocated = 0;
	state->ptrptr_count = 0;
	Ptrptr_Extend(state);
	return state;
}
void 
Ptrptr_Free(Ptrptr context)
{
	struct Ptrptr_Struct *state;
	int counter;
	if (NULL == context)
	{
		return;
	}
	state  = context;
	counter = 0;
	while (counter < state->ptrptr_allocated)
	{
		if (state->ptrptr[counter] != NULL)
		{
			free(state->ptrptr[counter]);
			state->ptrptr[counter] = NULL;
			
		}
		counter ++;
	}
	free(state->ptrptr);
	free(state);
}

int
Ptrptr_Count(Ptrptr context)
{
	struct Ptrptr_Struct *state;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	return state->ptrptr_count;
}


int
Ptrptr_Add(Ptrptr context,char *argument)
{
	char *arg_cpy;
	struct Ptrptr_Struct *state;
	int counter;
	int argument_length;

	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	if (Ptrptr_Extend(context) < 0)
	{
		return -4;
	}
	if (NULL == argument)
	{
		return -5;
	}
	argument_length = strlen(argument);
	arg_cpy = (char *)malloc(sizeof(char) * (argument_length +1));
	if (arg_cpy != strncpy(arg_cpy,argument,argument_length))
	{
		free(arg_cpy);
		return -2;
	}
	/*Now make sure string is NULL terminated*/
	arg_cpy[argument_length] = '\0';
	counter = state->ptrptr_count;
	while (counter < state->ptrptr_allocated)
	{
		if ( state->ptrptr[counter] == NULL)
		{
			state->ptrptr[counter] = arg_cpy;
			state->ptrptr_count++;
			return counter;
		}
	}
	return -3;
	
}


int
Ptrptr_Insert(Ptrptr context, char *argument,int index)
{
	struct Ptrptr_Struct *state;
	char *arg_cpy;
	char *tmp;
	int counter;
	int argument_length;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	if (Ptrptr_Extend(context) < 0)
	{
		return -4;
	}
	if ((0 <= index ) && ( index <= state->ptrptr_count))
	{
		argument_length = strlen(argument);
		arg_cpy = (char *)malloc(sizeof(char) * (argument_length +1));
		if (arg_cpy != strncpy(arg_cpy,argument,argument_length))
		{
			free(arg_cpy);
			return -2;
		}
		/*Now make sure string is NULL terminated*/
		arg_cpy[argument_length] = '\0';
		counter = index;
		while ((counter < state->ptrptr_allocated) && (arg_cpy != NULL))
		{

			tmp = state->ptrptr[counter];
			state->ptrptr[counter] = arg_cpy;
			arg_cpy = tmp;
			counter ++;
		}
		state->ptrptr_count++;
	}	
	return state->ptrptr_count;
}



char*
Ptrptr_Item(Ptrptr context,int index)
{
	struct Ptrptr_Struct *state;
	if (NULL == context)
	{
		return NULL;
	}
	state  = context;
	if (state->ptrptr == NULL)
	{
		return NULL;
	}
	if ((0 <= index )&& ( index < state->ptrptr_count))
	{
		return state->ptrptr[index];
	}
	return NULL;
}



char **
Ptrptr_Ptrptr(Ptrptr context)
{
	struct Ptrptr_Struct *state;
	if (NULL == context)
	{
		return NULL;
	}
	state  = context;
	if (Ptrptr_Extend(context) < 0)
	{
		return NULL;
	}
	return state->ptrptr;
}



void
Ptrptr_Clear(Ptrptr context)
{
	struct Ptrptr_Struct *state;
	int counter;
	if (NULL == context)
	{
		return;
	}
	state  = context;
	if (state->ptrptr == NULL)
	{
		return;
	}
	counter = 0;
	while (counter < state->ptrptr_allocated)
	{
		if ( state->ptrptr[counter] != NULL)
		{
			free(state->ptrptr[counter]);
			state->ptrptr[counter] = NULL;
		}
		counter ++;
	}
	state->ptrptr_count = 0;
	return;
}

int
Ptrptr_Delete(Ptrptr context, int index)
{
	struct Ptrptr_Struct *state;
	int counter;
	if (NULL == context)
	{
		return -1;
	}
	state  = context;
	if (state->ptrptr == NULL)
	{
		return -2;
	}
	if ((0 <= index )&& ( index < state->ptrptr_count) &&( state->ptrptr[index] != NULL))
	{
		free(state->ptrptr[index]);
		state->ptrptr[index] = NULL;
		counter = index;
		while (counter < state->ptrptr_allocated)
		{
			if (counter > index)
			{
				state->ptrptr[counter -1] = state->ptrptr[counter];
				state->ptrptr[counter] = NULL;
			}
			counter ++;
		}
		state->ptrptr_count--;
	}	
	return state->ptrptr_count;
}


/*Private functions*/



int
Ptrptr_Extend(Ptrptr context)
{
	char **ptrptr_tmp1;
	struct Ptrptr_Struct *state;
	int new_count;
	int counter;	
	if (NULL == context)
	{
		return -2;
	}
	state  = context;
	if (state->ptrptr_count + 2 > state->ptrptr_allocated)  
	{
		new_count = (state->ptrptr_allocated + 256);
		ptrptr_tmp1 = (char **)malloc(sizeof(char *) * new_count);
		counter = 0;
		while (counter < new_count)
		{
			ptrptr_tmp1[counter] = NULL;
			counter++;
		}
		if (state->ptrptr != NULL)
		{
			if (ptrptr_tmp1 != memcpy(ptrptr_tmp1,state->ptrptr,(state->ptrptr_allocated *sizeof(char *)) ))
			{
				free(ptrptr_tmp1);
				return -1;
			}
		}
		state->ptrptr = ptrptr_tmp1;
		state->ptrptr_allocated = new_count;
	}	
	return 0;
}
