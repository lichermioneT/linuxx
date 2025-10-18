#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef  int STDataType;
typedef struct Stack
{
    STDataType* data;
    size_t top;          // 栈顶部
    size_t capacity;    // 栈的容量
}Stack;

void StackInit(Stack* ps)
{
  assert(ps);
  STDataType* temp = (STDataType*)malloc(sizeof(STDataType)*10);
  if(temp == NULL)
  {
    perror("malloc failed");
    exit(1);
  }

  ps->data = temp;
  ps->capacity = 10;
  ps->top = 0;
}

void StackDestory(Stack* ps)
{
  assert(ps);
  free(ps->data);
  ps->data = NULL;
  ps->top = ps->capacity = 0;
}

void StackPrint(Stack* ps)
{
  assert(ps);
  for(size_t i = 0; i < ps->top; i++)
  {
    printf("%d ", ps->data[i]);
  }
}

void StackPush(Stack* ps, STDataType x)
{
  assert(ps);
  
  if(ps->top == ps->capacity)
  {
    size_t newCapacity = ps->capacity == 0 ? 4 : ps->capacity *2;
    STDataType* temp = (STDataType*)realloc(ps->data, sizeof(STDataType) * newCapacity);
    
    if(temp == NULL)
    {
      perror("realloc failed");
      exit(1);
    }

    ps->data = temp;
    ps->capacity = newCapacity;
  }

  ps->data[ps->top] = x;
  ps->top++;
}


// 出栈
void StackPop(Stack* ps)
{
  assert(ps);
  assert(ps->top > 0);

  ps->top--;
}


// 栈顶数据
STDataType StackTop(Stack* ps)
{
  assert(ps);
  assert(ps->top > 0);

  return (ps->data[ps->top-1]);
}

size_t StackSize(Stack* ps)
{
  assert(ps);
  
  return ps->top;
}

int StackEmpty(Stack* ps)
{
  assert(ps);

  return ps->top == 0 ? 1 : 0;
}



int main()
{

// 栈是先进去后出来
  Stack s;
  StackInit(&s);  
  for(int i = 0; i < 20; i++)
  {
    StackPush(&s, i);
  }
  
  for(int i = 0; i < 20; i++)
  {
    StackPop(&s);
    StackPrint(&s);
    printf("\n");
  }

  StackPrint(&s);
  printf("\n");

  StackDestory(&s);

  return 0;
}
