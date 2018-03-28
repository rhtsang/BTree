#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()




int LeafNode::findMax(int value)
{
  int max = 0;

  if(count == leafSize && value > values[leafSize - 1])
    max = value;
  else if(count == leafSize && value < values[leafSize - 1])
  {
    max = values[leafSize - 1];
    //values[leafSize - 1] = 0;
    count--;
    sort(value);
  } // if value is not the max, return last element 
    //and insert value into correct place in values
  
  return max;
} // findMax() checks if the value to be inserted is the max




int LeafNode::getCount() const
{
  return count;
} // getCount() returns the current count


int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()



LeafNode* LeafNode::insert(int value)
{
  int min, newMin, max = findMax(value);
  
  if(count < leafSize)
  {
    min = getMinimum();
    sort(value);
    newMin = getMinimum();
    
    if(parent != NULL && newMin < min)  // if inserted a new min, update key
      parent->updateKey();
  } // there is space in leaf
  else if(leftSibling != NULL && leftSibling->getCount() < leafSize)
  {
    leftSibling->insert(this->getMinimum());
    removeMin(max);
    parent->updateKey(); // updates key because min changed
  } // look left
  else if(rightSibling != NULL && rightSibling->getCount() < leafSize)
  {
    rightSibling->insert(max);
  } // look right
  else // split
    return split(max);
    
  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()



void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()



void LeafNode::removeMin(int max)
{
  int index;
  
  // removes the first element, moves the list one forward
  for(index = 0; index < count; index++)
    values[index] = values[index + 1];
    
  values[count - 1] = max;  // set the last element to max
  //count--;
} // removeMin() removed the first element and set the last element to max



LeafNode* LeafNode::split(int max)
{
  int half;
  if(leafSize % 2 == 0)
    half = (leafSize / 2); // even number of elements to split
  else
    half = (leafSize / 2) + 1; // odd number of elements to split
  
  LeafNode* ptr = new LeafNode(leafSize, parent, this, rightSibling);
  
  if (rightSibling)
    rightSibling->setLeftSibling(ptr);
  setRightSibling(ptr);
  
  for(int index = half; index < leafSize; index++)
  {
    ptr->insert(values[index]);
    count--;
  } // insert the second half of the leaf into the new leaf
  
  ptr->insert(max); // insert max into new leaf
  
  return ptr;
} // LeafNode::split();




void LeafNode::sort(int value)
{
  int i;
  
  // starts at end, moves to the right
  for (i = count - 1; i >= 0 && value < values[i]; i--)
    values[i + 1] = values[i];
  values[i + 1] = value;
  
  count++;
}  // LeafNode::sort() places the value into the LeafNode's array's proper spot


