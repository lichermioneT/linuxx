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
  if(*root == NULL) return; 
  BSTreeNode* cur = *root;
  BSTreeNode* parent = NULL; // 注意这里，没有进入while循环的话，删除的就是根节点
  // 找到位置
  while(cur != NULL && cur->data != x ) // 注意判断顺序，小心cur->data会解引用的
  {
    parent = cur;
    if(cur->data > x)
    {
      cur = cur->left;
    }
    else 
    {
      cur = cur->right;
    }
  }
 
  // 没有找到
  if(cur == NULL)
    return;

  
  if(cur->left == NULL) // 情况1，没有左子树
  {
    if(parent == NULL)
    {
      *root = cur->right; // 根节点没得左子树，右子树当做根
    }
    else if(parent->left == cur)  // cur在parent的left
    {
      parent->left = cur->right;
    }
    else                         // cur在parent的right 
    {
      parent->right = cur->right;
    }
    free(cur);                // 释放cur
  }
  else if (cur->right == NULL)  // 情况2，没有右子树 
  {
    if(parent==NULL)
    {
      *root = cur->left;  // 根节点没得右子树，左子树当做根
    }
    else if (parent->left == cur) // cur在parent的left
    {
        parent->left = cur->left;
    }
    else                    // cur在parent的right
    {
      parent->right = cur->left;
    }
  
    free(cur);   // 释放cur
  }
  else              // 情况3，左右子树都的
  {
    BSTreeNode* minParent = cur;
    BSTreeNode* min = cur->right;  // 右边最小的
    while(min->left != NULL)  // 找到最小节点
    {
      minParent = min;
      min = min->left;
    }
  
    cur->data = min->data;
    
    if(minParent->left == min)
    {
      minParent->left = min->right; // min->right 为 NULL
    }
    else 
    {
        minParent->right = min->right; // 右孩子就是最小的节点了
    }
    free(min);
  }
}

void InOrder(BSTreeNode* root)
{
  if(root == NULL)
    return;
  
  InOrder(root->left);
  printf("%d ", root->data);
  InOrder(root->right);
}

int main()
{

  BSTreeNode* root = NULL;

  int arr[] = {3,4,5,1,8,9,90,1,22,334,88,99};
  int sz = sizeof(arr) / sizeof(arr[0]);
  for(int i = 0; i < sz; i++)
  {
    BSTreeInsert(&root, arr[i]);
    InOrder(root);
    printf("\n");
  }
  
  for(int i = 0; i < sz; i++)
  {
    BSTreeErase(&root, arr[i]);
    InOrder(root);
    printf("\n");
  }
  return 0;
}
