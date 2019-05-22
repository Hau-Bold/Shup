#include "utils.h"

void printSettingsInfomation(char* user, char* host,char* cwd)
{
     fprintf(stdout,"%s%s%s@%s%s%s:%s%s%s%s",GREEN,user,DEFAULT,RED,host,DEFAULT,cwd,MAGENTA,getPrompt(user),DEFAULT);
}

char* getPrompt(char* user)
{
      if(strcmp(user,ROOT)==0)
	{
         return PROMPT_ROOT;
	}
      return PROMPT_OTHER;
}

void escape(char* toEscape)
{
	int index=0;
	while(toEscape[index]!=0)
	{
	 index++;
	}
	toEscape[index]='\0';
}

char* readLine(void)
{
	char*buffer = (char*)malloc(BUFFER_SIZE);

	if(!buffer)
	{
	fprintf(stderr,MEMORY_ALLOC_ERROR,RED,DEFAULT);
	exit(EXIT_FAILURE);  //TERMINATE EVERYTHING
	}
        
                  
        // start reading command:
        int position=0;//postion of cursor
        int chunks=0;//if buffer needs mor that 256 signs, we reallocate buffer for 512 signs
        while(1)
	{
	 int sign = getc(stdin);
         if(sign=='\n')
	 {
	  buffer[position]='\0';
	  fflush(stdin);
          return position==0 ? NULL: buffer;//we didn't read anything
	 }
           buffer[position]=sign;          
           position++;
           
          //if size of buffer exceed, reallocate:
         if(position >= BUFFER_SIZE)
	 {
	   chunks++;
           buffer = realloc(buffer, chunks*BUFFER_SIZE);
	 }
	}
}



char** split(char* string,char* delimiter,int* ctArgs)
{
        char** buffer = (char**)malloc(BUFFER_SIZE* sizeof(char*));
	char* ptr = strtok(string,delimiter);
	int position = 0;	
	int chunks =0;
	 while(ptr)
	 {
	    //found first segment  
	    //no need to escape ptr.
             buffer[position]=ptr;
             position++;

	    //if size of buffer exceed, reallocate:
            if(position >= BUFFER_SIZE)
	      {
               chunks++;
               buffer = realloc(buffer, chunks * BUFFER_SIZE);
	      }

             ptr = strtok(NULL,delimiter);
	 }
          *ctArgs = position;
	return buffer;
}

char* getPathFromTree(char* arg)
{
    int loc = strlen(arg);
    char* cct = (char*)malloc((loc+5+1+1));
    strcpy(cct,"which ");
    strcat(cct,arg);
    FILE* file = popen(cct,"r");
    if(file)
      {
            char* response;
 	    char path[1024];//load location in path and trim it!
            if(fgets(path, sizeof(path)-1,file)!=NULL)
              {
                  int s = strlen(path);
                  response = (char*)malloc((s));
                    for(int i=0;i<s-1;i++)
                       {
                        response[i]=path[i];
                       }
           
               }
         pclose(file);
	 free(cct);
        return response;
       }
    else
      {
      free(cct);
      if(VERBOSE)
      {	
      fprintf(stderr,"%sError of message: %s%s\n",RED,strerror(errno),DEFAULT);
      }
      return NULL;
      }
}

void launchArguments(int argc,char* argv[])
{  
        /*user wants to launch arguments**/
	int pid=0,status=0;
        pid = fork();

        if(pid < 0)
          {
          /**fork has failed*/
          fprintf(stderr,ERROR_FORK,RED,DEFAULT);
          } 
        
        else if(pid==0)
	{
         //child process to launch arguments
          if(argc > 0)
            {
            //there are arguments passed!
	    char* new_argv[argc+1];

            //copy arguments;
            for(int id=0; id<argc; id++)
	    {
		    new_argv[id]=argv[id];
	    }
	   /** apppend NULL */ 
            new_argv[argc]=(char*)NULL;

            int val = execv(new_argv[0],new_argv);//may program be launched direct, i.e path is passed correctly?
                  
               if(val)
               {
		       if(VERBOSE)
		       {
                       fprintf(stderr,"%sError of message: %s%s\n",RED,strerror(errno),DEFAULT);
		       }
                       char*path = getPathFromTree(new_argv[0]);
		       new_argv[0]=path;
		     
                       val=execv(new_argv[0],new_argv);

                       if(val)
                       {
			  if(VERBOSE)
			  {	  
                          fprintf(stderr,"%sError of message: %s%s\n",RED,strerror(errno),DEFAULT);
			  }
                          exit(EXIT_FAILURE);
                       }
		       free(path);
               }
               exit(EXIT_SUCCESS);
             }
	     else
	     {       /*there is nothing to launch,termate this child**/
		     exit(EXIT_SUCCESS);
	     }
         }
        else
        {
         /*parent process**/
         if((pid=wait(&status))==-1)
           {
            if(VERBOSE)
	    {		   
            fprintf(stderr,ERROR_WAIT,RED,DEFAULT);
	    }
           }
         else
          {
          if(WIFEXITED(status))
            {
             if(VERBOSE)
	     {		   
              fprintf(stdout,"%s son with pid %d exited with status %d\n%s",GREEN,pid,WEXITSTATUS(status),DEFAULT);
	     }
            }
           else if(WIFSIGNALED(status))
            {

             if(VERBOSE)
	     {		   
             fprintf(stdout,"%s son with pid %d ended because of signal %d\n%s",GREEN,pid,WTERMSIG(status),DEFAULT);
	     }
            }
           else
            {
             if(VERBOSE)
	     {		   
             fprintf(stdout,"%sson with pid %d didn't end normally: status: %d\n%s",RED,pid,WEXITSTATUS(status),DEFAULT);           
             }
	    }
          }
        }
}


