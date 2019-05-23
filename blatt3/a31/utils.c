#include "utils.h"

int getNextCountOfProcesses(void)
{
	int n = rand()%100;
        if(n < 30)
        {
         return 0;
        }
        else if(n < 60)
        {
        return 1;
        }
        else if(n < 80)
        {
        return 2;
        }
        return 3;
}




