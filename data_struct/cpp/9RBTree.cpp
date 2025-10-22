#include <iostream>
using namespace std;

enum Colour
{
  BLACK,
  RED
};

template<class K, class V>
class RBTreeNode
{
  RBTreeNode<K,V>*  _letf;
  RBTreeNode<K,V>* _right;
  RBTreeNode<K,V>* _parent;
  Colour _col;
  pair<K,V> _kv;
  RBTreeNode(const pair<K,V>& kv)
    :_letf(nullptr)
    ,_right(nullptr)
    ,_parent(nullptr)
    ,_col(RED)
    ,_kv(kv)
  {}
};


template<class K, class V>
class RBTree 
{
  typedef RBTreeNode<K,V> Node;
public:
  bool insert(const pair<K,V>& kv)
  {
    if(_root == nullptr)
    {
      _root = new Node(kv);
      _root->_col = BLACK; // 根节点是黑色的
      return true;
    }

    Node* parent = nullptr;
    Node* cur = _root;

    while(cur != nullptr)
    {
      if(cur->_kv.first < kv.first)
      {
        parent = cur;
        cur = cur->_right;
      }
      else if(cur->_kv.first > kv.first)
      {
        parent = cur;
        cur = cur->_letf;
      }
      else 
      {
        return false;
      }
    }

    cur = new Node(kv);
    if(parent->_kv.first < kv.first)
    {
      parent->_right = cur;
      cur->_parent = parent;
    }
    else 
    {
      parent->_letf = cur;
      cur->_parent = parent;
    }

    
    cur->_col = RED; // 新节点只能是红色的,因为黑的破坏平衡了（黑色数量相同）
   // 关键看叔叔
   // 
   // 1.parent是红的，一定不是根节点 
   // cur红 parent红 叔叔存在且是红色的
   // p和u变成黑色，g变成红。没有连续的红节点了，且子树中每条路径的黑节点数量不变。
   //   1.g是根节点了。变黑就行了
   //   2.不是跟呢？g可能是红色的
   //     g的父亲是黑色，没有影响
   //     g的父亲是红色，继续向上调整
   //
   // cur存在且为红，p和u变黑，g变红，继续往上处理
   // ps:这里只关注颜色，p,g,u,几个节点在左右没有影响的
   //
   //
   //
   // 2.cur红色，p红色，g黑色的，p不存在的
   //   cur就是新增的
   // 2.cur红色，p红色，g黑色的，p存在且是黑色的
   //   cur不是新增的，cur原来是黑色的，这里是cur变红，由第一种情况变来的
   //   旋转+变色
   //   左单旋转：g红 p黑
   //   右单旋转：
   //
   // 3.双旋转
   //   cur红色的，parent红色的，g是黑色的 u不存在，或者为黑的
   //
   //
   //
   //
  
  while(parent != nullptr && parent->_col == RED)
  {
    // 红黑树关键看叔叔
    
    Node* grandfather = parent->_parent;

    if(grandfather->_letf == parent)
    {
      Node* uncle = grandfather->_right;
      // 叔叔存在且颜色为红色的
      if(uncle != nullptr && uncle->_col == RED)
      {
        parent->_col = BLACK;
        uncle->_col = BLACK;
        grandfather->_col = RED;

        // 继续往上处理
        cur = grandfather;
        parent = cur->_parent;
      }
      else  // 叔叔不存在，或者叔叔存在且为黑色
      {
        // 双旋---》变成单旋转
        if(cur == parent->_right)
        {
          RotateL(parent);
          swap(parent, cur);
        }
        
        // 第二种情况
        //
        RoteteR(grandfather);
        grandfather->_col = RED;
        parent->_col = BLACK;
        break;
      }
    }
    else 
    {
      Node* uncle = grandfather->_letf;
      if(grandfather->_right == parent)
      {
        if(uncle != nullptr  && uncle->_col == RED)
        {
          parent->_col = BLACK;
          uncle->_col = BLACK;
          grandfather->_col = RED;
          
          // 继续向上处理
          cur = grandfather;
          parent = cur->_parent;
        }
        else  // 叔叔不存在，存在且为黑色
        {
          if(cur == parent->_letf)
          {
             RoteteR(parent);
             swap(parent, cur);
          }

          RotateL(grandfather);
          grandfather->_col = RED;
          parent->_col = BLACK;
          break;
        }
      }
    }
    
    

    
  }
  













    _root->_col = BLACK;
    return true;
  }



private: 
  Node* _root = nullptr;
};



int main()
{


















  return 0;
}
