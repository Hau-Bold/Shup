#include<stdio.h>
#include<stdlib.h>

#define MAX_CHARACTERS 100
int main(int argc,char* argv[])
{
printf("\033[;31m");
if(argc > 2)
{
printf("Nur erster Parameter wird berücksichtig\n");
}
int ctNames = atoi(argv[1]);

printf("Anzahl der Namen: %i\n",ctNames);
char names[ctNames][MAX_CHARACTERS];

for(int i=0;i<ctNames;i++)
{
printf("Eingabe %i-ter Name:\n",i+1);
fgets(names[i],MAX_CHARACTERS,stdin);
}
for(int i=ctNames-1;i>-1;i--)
{
printf("%i-ter Name: %s\n",i+1,names[i]);
}
printf("\x1B[0m");
printf("Ende\n");
return 0;
}
