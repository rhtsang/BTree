#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  int getMinimum()const;
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  BTreeNode* matchKey(int value);
  void print(Queue <BTreeNode*> &queue);
  void removeMin(BTreeNode *newNode);
  void sortKey(BTreeNode *newNode); // insert a child to the middle of the node
  InternalNode* split(BTreeNode *newNode);
  void updateKey(); // when a min changes
}; // InternalNode class

#endif
