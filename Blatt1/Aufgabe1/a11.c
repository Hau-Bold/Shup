#include<stdio.h>

int main(int argc,char* argv[])
{
printf("count of arguments: %i\n",argc);
for(int ctoa=0;ctoa<argc;ctoa++)
{
printf("Nummer des Aufrufparameters: %i Wert des Aufrufparameters: %s\n",ctoa,argv[ctoa]);
}
return 0;
}
