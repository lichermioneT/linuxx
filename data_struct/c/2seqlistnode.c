#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int seqNdatatype;
typedef struct seqlistnode
{
  seqNdatatype _data;
  struct seqlistnode* ps;
}seqlistnode;


seqlistnode* BuyListNode(seqNdatatype x)
{
  seqlistnode* node = (seqlistnode*)malloc(sizeof(seqlistnode));
  if(node == NULL)
  {
    perror("malloc failed");
  }
  node->_data = x;
  node->ps = NULL;

  return node;
}


void seqlistPrint(seqlistnode* ps)
{
  assert(ps);
  seqlistnode* cur = ps;
  while(cur != NULL)
  {
    printf("%d ", cur->_data);
    cur = cur->ps;
  }

  printf("\n");
}

void seqlistPushBack(seqlistnode** ps, seqNdatatype x)
{
  assert(ps);
  seqlistnode* newnode = BuyListNode(x);

  if(*ps == NULL)
  {
    (*ps) = newnode; 
  }
  else 
  {
    seqlistnode* cur = *ps;
    while(cur->ps != NULL)
    {
      cur = cur->ps;
    }
    cur->ps = newnode;
  }
}

void 

int main()
{

  // 指针,
  seqlistnode* s = NULL;
  seqlistPushBack(&s, 111);
  seqlistPushBack(&s, 111);
  seqlistPushBack(&s, 111);
  seqlistPushBack(&s, 111);
  seqlistPrint(s);







  return 0;
}


