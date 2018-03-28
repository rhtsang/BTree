#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()




int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()




InternalNode* InternalNode::insert(int value)
{
  BTreeNode *newNode = matchKey(value);
  
  if(newNode)
  {
    if(count < internalSize)
    {
      sortKey(newNode);
    } // if there is open space in the internal node
    else if(leftSibling != NULL && leftSibling->getCount() < internalSize)
    {
      InternalNode *left = dynamic_cast<InternalNode*>(leftSibling);
      // dynamic casting because the virtual table shit is being a bitch and wont recognize the insert(BTreeNode*) function
      left->insert(children[0]); // move the first node to the left
      removeMin(newNode); //should remove first node + sortKey(newNode)
      //parent->updateKey(); // not necessary because sortKey calls it
    } // look left
    else if(rightSibling != NULL && rightSibling->getCount() < internalSize)
    {
      InternalNode *right = dynamic_cast<InternalNode*>(rightSibling);
      // same SAME
      sortKey(newNode);
      right->insert(children[count - 1]); // move the last node to the right
      count--;
      // hmmm what if the one splited is the last node???? that means newNode needs to go to rightSibiling 
      //sortKey(newNode); // put the newNode into the current list (fills the spot)
    } // look right
    else // split
      return split(newNode);
  } // if the children splitted 
  
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()




void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  
  // set the current node to be the root
  oldRoot->setParent(this);
  node2->setParent(this);
  
  // set the keys and children of the root
  // inserting at count = 0
  keys[count] = oldRoot->getMinimum();
  children[count++] = oldRoot;
  // inserting at count = 1
  keys[count] = node2->getMinimum();
  children[count++] = node2;

} // InternalNode::insert()




void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  // students may write this
  newNode->setParent(this); // change parent
  sortKey(newNode); // sort the node into new parent
  
} // InternalNode::insert()



BTreeNode* InternalNode::matchKey(int value)
{
  int index;
  // loops stops when it first passes the key that matches the value
  for(index = 0; index < count && keys[index] < value; index++);
  
  if(index == 0)
    return children[index]->insert(value);
  else
    return children[index-1]->insert(value);
    
} // matches the value to a key




void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()





void InternalNode::removeMin(BTreeNode *newNode)
{
  int index;
  
  // removes the first child, moves the list one forward
  for(index = 0; index < count; index++)
  {
    keys[index] = keys[index + 1];
    children[index] = children[index + 1];
  }
  
  count--;
  sortKey(newNode); // put the new node in (doesnt have to be at the end)
  //values[count - 1] = max;  // set the last element to max
  
} // removeMin() removed the first child and inserts the newNode using sortKey





void InternalNode::sortKey(BTreeNode *newNode)
{
  int i;
  
  for (i = count - 1; i >= 0 && newNode->getMinimum() < keys[i]; i--)
  {
    keys[i + 1] = keys[i];
    children[i + 1] = children[i];
  } // starts at end, moves to the right one at a time till the correct index
  
  keys[i + 1] = newNode->getMinimum();
  children[i + 1] = newNode;
  
  count++;
  
  updateKey();
} // sortKey() puts the newNode in the right position in keys and children



InternalNode* InternalNode::split(BTreeNode *newNode)
{
  int half;
  if(internalSize % 2 == 0)
    half = (internalSize / 2); // even number of elements to split
  else
    half = (internalSize / 2) + 1; // odd number of elements to split
  
  InternalNode* ptr = new InternalNode(internalSize, leafSize, parent, this, rightSibling);

  if (rightSibling)
    rightSibling->setLeftSibling(ptr);
  setRightSibling(ptr);
  
  ptr->insert(children[count - 1]); // insert the last element into ptr because it has to go there 4 sho
  count--;
  sortKey(newNode); //insert newNode into current list so that the list is at maximum again
  
  for(int index = half; index < internalSize; index++)
  {
    ptr->insert(children[index]);
    
    count--;
  } // insert the second half of the leaf into the new leaf
  
  //ptr->insert(max); // insert max into new leaf --> no max for internalnode. no need
  
  return ptr;
} // LeafNode::split();




void InternalNode::updateKey()
{
  int i;//, min, newMin;
  //min = getMinimum(); // old 
  //min = keys[0]; // new
 
  for(i = 0; i < count; i++)
  {
    if(children[i]->getMinimum() != keys[i])
      keys[i] = children[i]->getMinimum();
  } // goes through the list to see if any key has changed
  
 // newMin = getMinimum();
  if(parent)// && newMin < min)
    parent->updateKey();  // if min changes, update the key of parent
    
} // updateKey() updates minimum if needed