#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <stdio.h>
#include "edg_se_launcher.h"
#define APLICATION "EDG-SE-LIB-BDB-ABSTRACTION"
#define BUFF_SIZE 4096
#include "edg_se_ptrptr.h"
int test_fork()
{
	int child_stdin, child_stdout, child_stderr;
	int Launcher_launch_rc;
	int done;
	char * buf;
	int len;
	int rc;
	pid_t Launcher_launch_pid;
	Launcher context;
	context = Launcher_Create();
	
	
	/*
	child_stdin = Launcher_OpenPipe(context,0,LAUNCHER_PIPE_WRITE);
	*/
	/*
	child_stdout = Launcher_OpenPipe(context,1,LAUNCHER_PIPE_READ);
	fprintf(stdout,"child_stdout=%d\n",child_stdout);
	
	child_stderr = Launcher_OpenPipe(context,2,LAUNCHER_PIPE_READ);
	*/
	
	Launcher_SetExecutable(context,"/bin/cat");
	fprintf(stdout,"executable='%s'\n",Launcher_GetExecutable(context));
	Launcher_SetExecutable(context,"/bin/ls");
	Launcher_ArgumentsAdd(context,"/");
	Launcher_ArgumentsAdd(context,"/mnt");
	fprintf(stdout,"executable='%s'\n",Launcher_GetExecutable(context));
	Launcher_launch_rc = Launcher_Launch(context);
	if (0 == Launcher_Running(context))
	{
		printf("running\n");
	}
	else
	{
		printf("finnished\n");
	}
	Launcher_launch_pid = Launcher_GetPid(context);
	Launcher_Wait(context,WUNTRACED);
	fprintf(stdout,"Launcher_launch_pid='%d'\n",Launcher_launch_pid);
	buf = malloc(sizeof(char *) * BUFF_SIZE);
	do 
	{
		len = read(child_stdout, buf,BUFF_SIZE);
		done = len < BUFF_SIZE;
		buf[len] = '\0';
		fprintf(stdout,"fd,len,content,%d,%d,%s\n",child_stdout,len,buf);
				
	} while (!done);
	do 
	{
		len = read(child_stderr, buf,BUFF_SIZE);
		done = len < BUFF_SIZE;
		buf[len] = '\0';
		fprintf(stdout,"fd,len,content,%d,%d,%s\n",child_stderr,len,buf);
				
	} while (!done);
	free(buf);
	/*Launcher_launch_rc = Launcher_Wait(context);*/
	Launcher_Free(context);
	return 0;
	
}

int test_fork2()
{
int child_stdin, child_stdout, child_stderr;
	int Launcher_launch_rc;
	int done;
	char * buf;
	int len;
	int rc;
	pid_t Launcher_launch_pid;
	Launcher context;
	context = Launcher_Create();
	
	
	/*
	child_stdin = Launcher_OpenPipe(context,0,LAUNCHER_PIPE_WRITE);
	*/
	child_stdout = Launcher_PipeCreate(context,1,LAUNCHER_PIPE_READ);
	fprintf(stdout,"child_stdout=%d\n",child_stdout);
	
	child_stderr = Launcher_PipeCreate(context,2,LAUNCHER_PIPE_READ);
	
	
	Launcher_SetExecutable(context,"/bin/cat");
	fprintf(stdout,"executable='%s'\n",Launcher_GetExecutable(context));
	Launcher_SetExecutable(context,"/bin/ls");
	Launcher_ArgumentsAdd(context,"/");
	Launcher_ArgumentsAdd(context,"/mnt");
	Launcher_ArgumentsAdd(context,"/sdkfljflk");
	fprintf(stdout,"executable='%s'\n",Launcher_GetExecutable(context));
	Launcher_launch_rc = Launcher_Launch(context);
	if (0 == Launcher_Running(context))
	{
		printf("running\n");
	}
	else
	{
		printf("finnished\n");
	}
	
	Launcher_launch_pid = Launcher_GetPid(context);
	child_stdout = Launcher_PipeGet(context,1);
	child_stderr = Launcher_PipeGet(context,2);
	Launcher_Wait(context,WUNTRACED);
	fprintf(stdout,"Launcher_launch_pid='%d'\n",Launcher_launch_pid);
	buf = malloc(sizeof(char *) * BUFF_SIZE);
	do 
	{
		len = read(child_stdout, buf,BUFF_SIZE);
		done = len < BUFF_SIZE;
		buf[len] = '\0';
		fprintf(stdout,"fd,len,content,%d,%d,%s\n",child_stdout,len,buf);
				
	} while (!done);
	do 
	{
		len = read(child_stderr, buf,BUFF_SIZE);
		done = len < BUFF_SIZE;
		buf[len] = '\0';
		fprintf(stdout,"fd,len,content,%d,%d,%s\n",child_stderr,len,buf);
				
	} while (!done);
	free(buf);
	/*Launcher_launch_rc = Launcher_Wait(context);*/
	Launcher_Free(context);
	return 0;
	
}


int test_ptrptr()
{
	char **ptrptr;
	int counter;
	Ptrptr context;
	context = Ptrptr_Create();
	printf("Ptrptr_Count='%d'\n",Ptrptr_Count(context));
	counter = 0;
	while (counter < 1255)
	{
		Ptrptr_Add(context,"hello");
		counter++;
	}
	Ptrptr_Add(context,"hello");
	printf("Ptrptr_Count='%d'\n",Ptrptr_Count(context));
	Ptrptr_Clear(context);
	counter = 0;
	while (counter < 2)
	{
		Ptrptr_Add(context,"hello");
		counter++;
	}
	counter = 0;
	while (counter < Ptrptr_Count(context))
	{
		printf("Ptrptr_Items(%d)='%s'\n",counter,Ptrptr_Item(context,counter));
		counter++;
	}
	Ptrptr_Insert(context,"1",1);
	Ptrptr_Insert(context,"2",2);
	counter = 0;
	while (counter < Ptrptr_Count(context))
	{
		printf("Ptrptr_Items(%d)='%s'\n",counter,Ptrptr_Item(context,counter));
		counter++;
	}
	Ptrptr_Insert(context,"2",2);
	ptrptr = Ptrptr_Ptrptr(context);
	counter = 0;
	while (ptrptr[counter] != NULL)
	{
		printf("ptrptr[%d]='%s'\n",counter,ptrptr[counter]);
		counter++;
	}
	Ptrptr_Free(context);
	return 0;	
}	
int
test_execv()
{
	char * executable;
	Ptrptr context;
	executable = "/bin/ls";
	context = Ptrptr_Create();
	Ptrptr_Add(context,"/");
	Ptrptr_Add(context,"/");
	Ptrptr_Add(context,"/mnt");

	execv(executable,Ptrptr_Ptrptr(context));
	printf("execv failed\n");
	return 0;
}
int main()
{
	/*test_execv();*/
	test_fork2();
	/*test_ptrptr();*/
	return 0;	
}
