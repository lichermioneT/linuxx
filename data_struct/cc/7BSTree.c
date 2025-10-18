#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

typedef int BSTDataType;
typedef struct BSTreeNode
{
  struct BSTreeNode* left;
  struct BSTreeNode* right;
  BSTDataType data;
}BSTreeNode;

void BSTreeInsert(BSTreeNode** root,BSTDataType x)
{
  BSTreeNode* newNode = (BSTreeNode*)malloc(sizeof(BSTreeNode));
  if(newNode == NULL)
  {
    perror("malloc failed");
    exit(1);
  }
  
  newNode->data = x;
  newNode->left = NULL;
  newNode->right = NULL;
  
  BSTreeNode* cur = (*root);
  BSTreeNode* curInv = NULL;
  if(*root == NULL)
  {
    (*root) = newNode;
    return;
  }

  while(cur != NULL)
  {
    curInv = cur;

    if(cur->data > x)
    {
      cur = cur->left;
    }
    else if(cur->data < x)
    {
      cur = cur->right;
    }
    else 
    {
      return;
    }
  }

  if(curInv->data > x)
  {
    curInv->left = newNode;
  }
  else 
  {
    curInv->right = newNode;
  }
}

void BSTreeErase(BSTreeNode** root, BSTDataType x)
{
  assert(root);
  if(root == NULL) return; 
  BSTreeNode* cur = *root;
  BSTreeNode* curPrve = NULL;
  // 找到位置
  while(cur->data != x)
  {
    if(cur->data > x)
    {
      curPrve = cur;
      cur = cur->left;
    }
    else 
    {
      curPrve = cur;
      cur = cur->right;
    }
  }

  if(cur->left == NULL)
  {

  }



}



void InOrder(BSTreeNode* root)
{
  if(root == NULL)
    return;
  
  InOrder(root->left);
  printf("%d \n", root->data);
  InOrder(root->right);
}



int main()
{

  BSTreeNode* root = NULL;
  srand((unsigned long)time(NULL));

  for(int i = 0; i < 20; i++)
  {
    int r = rand() % 100 + 1;
    BSTreeInsert(&root, r);
  }

  InOrder(root);






  return 0;
}
