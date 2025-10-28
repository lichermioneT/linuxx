#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main()
{

  int arr[] = {1,0,2,3,0,4,5,0};
  int sz = sizeof(arr)/sizeof(arr[0]);
  
  int cur = 0;
  for(int i = 0; i < sz; i++)
  {
    if(arr[i] == 0)
    {
      cur += 2;
    }
    else 
    {
      cur += 1;
    }

    if(cur >= sz - 1) break;
  }


  return 0;
}
