#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main()
{

  int arr[] = {1,0,2,3,0,4,5,0};

  int n = sizeof(arr)/sizeof(arr[0]);  
  int cur = 0;
  int dest = -1;
 
  while(cur < n)
  {
    if(arr[cur])dest++;
    else dest+= 2;
    if(dest >= n) break;
    cur++;
  }
  
  if(dest == n)
  {
    arr[n-1] = 0;
    cur--;
    dest-=2;
  }

  while(cur >= 0)
  {
    if(arr[cur]) arr[dest--]=arr[cur--];
    else 
    {
      arr[dest--]=0;
      arr[dest--]=0;
      cur--;
    }
  }

  

  return 0;
}
