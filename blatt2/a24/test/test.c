#include<stdio.h>

int main(int argc,char* argv[])
{
	printf("Hello World\n");
	if(argc>1)
	{
		printf("Additional arguments you passed:\n");
        	for(int i=1;i<argc;i++)
		{
        	printf("%s\n",argv[i]);
		}
	}
        return 0;
}
