#include "constants.h"
#include "utils.h"

int getCountOfProcessesInNextMinute(void)
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


int getNextDuration(void)
{
 int Duration[]={DURATION_0,DURATION_1,DURATION_2,DURATION_3};
// srand(time(NULL));
 int n = rand()%COUNT_OF_DURATIONS;
  return Duration[n];
}

void doNothing(void){/*do nothing**/}
