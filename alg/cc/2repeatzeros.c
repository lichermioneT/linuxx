#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main()
{

  int arr[] = {1,0,2,3,0,4,5,0};
  int sz = sizeof(arr)/sizeof(arr[0]);

  int i = 0;
  int j = 0;

  while(i < sz  && j < sz)
  {
    if(arr[i] == 0)
    {
      j+=2;
    }
    else 
    {
      i++;
    }
  }

  i--;
  j--;

  while(i >= 0 && j >= 0)
  {
    if(j < sz)
    {
      arr[j] = arr[i];
    }

    if(arr[i] == 0)
    {

    }
  }






  return 0;
}
