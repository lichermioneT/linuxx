#include <stdio.h>

void swap(int* p1, int* p2)
{
  int temp = *p1;
  *p1 = *p2;
  *p2 =temp;
}


int main()
{

/*
  int arr[] = {1,0,3,4,50,0,0,3,6,7,99};
  int sz  = sizeof(arr)/sizeof(arr[0]);
  
  for(int i = 0; i < sz; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");
  // 双指针
  // cur指针：遍历数组
  // dest指针：非零元素的最后的下一个位置
  
  int dest = 0;
  for(int i = 0; i < sz; i++)
  {
    if(arr[i] != 0)
      arr[dest++] = arr[i];
  }
  
  for(dest; dest < sz; dest++)
  {
    arr[dest] = 0;
  }

  for(int i = 0; i < sz; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");

*/

// 快速排序

  int arr[] = {1,0,3,4,50,0,0,3,6,7,99};
  int sz = sizeof(arr)/sizeof(arr[0]);

  for(int i = 0; i < sz; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");


  int slow = 0;
  for(int i = 0; i <sz; i++)
  {
    if(arr[i] != 0)
    {
      swap(&arr[slow], &arr[i]);
      slow++;
    }
  }
  


  for(int i = 0; i < sz; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");













  return 0;
}
