#include "seqlist.h"


int main(int argc, char* argv[])
{

  int arr[] = {1,2,3,4,5,6,7,8,9,10};
  struct seqlist s;
  seqlistInit(&s, 10); 
  size_t len = sizeof(arr) / sizeof(arr[0]);
  for(int i = 0; i < len; i++)
  {
    seqlistPushBack(&s, arr[i]); 
  }

  seqlistPrint(&s);




  return 0;
}


