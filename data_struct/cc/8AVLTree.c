#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef int AVLTreeDataType;
typedef struct AVLTreeNode
{
  AVLTreeDataType data;
  struct AVLTreeNode* left;
  struct AVLTreeNode* right;
  struct AVLTreeNode* parent;
  int heigt;
}AVLTreeNode;


void RotateL(AVLTreeNode** root, AVLTreeNode* parent)
{
  assert(parent);
  AVLTreeNode* parentR = parent->right;
  AVLTreeNode* parentRL = parentR->left;
  AVLTreeNode* pparent = parent->parent;

// 1.parent
  parent->right = parentRL;
  if(parentRL != NULL)   // 处理不为空的情况
    parentRL->parent = parent; 
  
  subR->left = parent;
  AVLTreeNode* ppnode = parent->parent;
  parent->parent = subR;

// 2subL 
  // 平行处理的
  //1. parent是树的根，现在subR是根
  if(parent == *root)
  {
    *root = subR;
    subR->parent = NULL;
  }
  else // 2.parent不是这颗树的根节点了
  {
    if(ppnode->left == parent)
    {
      ppnode->left = subR;
    }
    else 
    {
      ppnode->right =  subR;
    }
    subR->parent = ppnode;
  }
  
  subR->heigt = 0;
  parent->heigt = 0;
}

void RotateR(AVLTreeNode** root, AVLTreeNode* parent)
{
  assert(root);
  assert(parent);
  
  AVLTreeNode* subL = parent->left;
  AVLTreeNode* subLR = subL->right;
  
  parent->left = subLR;
  if(subLR != NULL)
  {
    subLR->parent = subL;
  }

  subL->right = parent;
  AVLTreeNode* ppnode = parent->parent;
  parent->parent = subL;
  
  if(parent->parent == *root)
  {
    *root = subL;
    subL->parent = NULL;
  }
  else 
  {
      if(ppnode->left == parent)
      {
        ppnode->left = subL;
      }
      else 
      {
        ppnode->right = subL;
      }
      subL->parent = ppnode;
  }
  
  subL->heigt = 0;
  parent->heigt = 0;
}

void RotateLR(AVLTreeNode** root, AVLTreeNode* parent)
{

}

void RotateRL(AVLTreeNode** root, AVLTreeNode* parent)
{

}

void AVLTreeInsert(AVLTreeNode** root, AVLTreeDataType x)
{
  assert(root);
  AVLTreeNode* newNode = (AVLTreeNode*)malloc(sizeof(AVLTreeNode));
  if(newNode == NULL)
  {
    perror("malloc failed");
    exit(1);
  }
  newNode->data = x;
  newNode->left = NULL;
  newNode->parent = NULL;
  newNode->parent = NULL;
  newNode->heigt = 0;
  if(*root == NULL)
  {
    *root = newNode;
    return;
  }
  
  AVLTreeNode* cur = *root;
  AVLTreeNode* parent = NULL;
  while(cur != NULL)
  {
    parent = cur;
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
      return; // 直接退出
    }
  }
  
  // 放左边还是右边
  if(parent->data > x)
  {
    parent->left = newNode;
    newNode->parent = parent; // 和父亲链接起来
  }
  else 
  {
    parent->right = newNode;
    newNode->parent = parent; // 和父亲链接起来
  }

  // 更新平衡因子
  // 只会影响它的祖先
  // 其它部分不受这次插入的影响
  
  // 新增节点的height==0；
  // 父亲的左-1  左边height--
  // 父亲的右+1  右边height++
  
  // 也不是所有的祖先会受影响的
  // parent->heigt == 0;说明parent的高度不变，更新完成，插入完成。(-1,1)---(1,-1)
  // 原来是1或者-1，然后把矮的那一边填上了。
  // 父亲的高度不变的
  
  // 更新完成，parent->heigt==1  -1
  // 父亲的左-1  左边height--
  // 父亲的右+1  右边height++
  // 变成1，右边高，变成-1，左边高
  // 对上层有影响，继续更新
  
  // 更新完成
  // parent->heigt == 2/-2
  // 旋转处理

// 更新到根节点的位置
  while(parent)
  {
    if(cur == parent->right)
    {
      parent->heigt++;
    }
    else 
    {
      parent->right--;
    }

    if(parent->heigt == 0)
    {
      break;
    }
    else if (parent->heigt == 1 || parent->heigt == -1)
    {
      // parent的子树变化了，继续更新
      cur = parent;
      parent = parent->parent;
    }
    else if (parent->heigt == 2 || parent->heigt == -2)
    {
      if(parent->heigt == 2)
      {
        if(cur->heigt == 1)
        {
          RotateL(root, parent);
        }
        else if(cur->heigt == 1)
        {

        }
      }
      else if (parent->heigt == -2)
      {
        if(cur->heigt == -1)
        {
          RotateR(root, parent);
        }
        else if (cur->heigt == 1)
        {

        }
      }

  }


}

// 左单旋转
// 2               parent=2
//   1             subr = 1
//     0           
// subR的左边给parent右边
// parent变成subR的左边
// 

// parent->right = subR->left 
// subR->left = parent
// 更新完成之后，parent->heigt == 0; subR->heigt==0

int main()
{

  AVLTreeNode* root = NULL;















  return 0;
}
