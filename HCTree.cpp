#include "HCTree.h"
#include "HCNode.h"
#include <queue>
#include <cstddef>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>



/** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
void HCTree::build(const vector<int>& freqs){
  // creates the priority queue
  std::priority_queue<HCNode, vector<HCNode*>, HCNodePtrComp> pq;
  
  // Sets all leaf nodes into the leaves vector.
    for (int index = 0; index < freqs.size(); index++){
      if (freqs[index] != 0){
        unsigned char symb = index;
        HCNode* leaf = new HCNode(freqs[index], symb);
        leaves[index] = leaf;
        pq.push(leaf);
      }
    }
    /*while (!pq.empty()){
      cout << "Value: " << pq.top()->count << " Symbol: " << pq.top()->symbol << endl;
      pq.pop();
    }
 */
    while (pq.size() > 1){
      HCNode* first = pq.top();  //Gets the smallest element. 
      pq.pop();
      HCNode* second = pq.top(); //Gets the second smallest element.
      pq.pop();

    // Gets the sum of the two elements, creates an internal node.
      int sum = first->count + second->count;
      HCNode* internal = new HCNode(sum, 0);
      internal->c0 = first;  // Sets the internal node's children
      internal->c1 = second;
      first->p = internal;  // Sets the children's node's parent.
      second->p = internal;
      pq.push(internal);
    }
    
    root = pq.top();
    pq.pop();
    cout << "root node: " << root->count << endl;
    for (int index = 0; index < leaves.size(); index++){
    if (leaves[index] != 0){
      cout << "Node: " << leaves[index]->symbol << " freq: " <<leaves[index]->count << endl;
      if (leaves[index]->p != 0){
         cout << "Parent node of: " << leaves[index]->count << " is: " << leaves[index]->p->count << endl;
      }
    }
    
  }

}


/** Write to the given ofstream
     *  the sequence of bits (as ASCII) coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
     *  BE USED IN THE FINAL SUBMISSION.
     */
void HCTree::encode(byte symbol, ofstream& out) const{
  HCNode *tmp;
  HCNode *curr;
  stack<int> intStack;

  std::cout << "Symbol we are looking to code: " << symbol << endl;

  // Gets the leaf of corresponding symbol
  for (int index = 0; index < leaves.size(); index++){
    if (leaves[index] != 0 && leaves[index]->symbol == symbol){
      tmp = leaves[index];
      curr = tmp->p;
      break;
    }
  }
  
  std::cout << "tmp's symbol: " << tmp->symbol << endl;
  // Loops until code is in stack
  while (tmp != root){
    curr = tmp->p;
    if (curr->c0 == tmp){
      intStack.push(0);
      std::cout << "Pushing 0 to stack" << endl;
    }
    if (curr->c1 == tmp){
      intStack.push(1);
      std::cout << "Pushing 1 to stack" << endl;
    }
    tmp = curr;
  }
  // Outputs code to file.
  while (!intStack.empty()){
    out << intStack.top();
    std::cout << "Number written to out: " << intStack.top() << endl;
    intStack.pop();
  }
  //out.close();
}

/** Return the symbol coded in the next sequence of bits (represented as 
     *  ASCII text) from the ifstream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
     *  IN THE FINAL SUBMISSION.
     */
int HCTree::decode(ifstream& in) const{
  HCNode* curr = root;
  unsigned char symb = in.get();
  std::cout << "Decode: ";
  while (in.good()){
    if (symb == '0'){
      curr = curr->c0;
    }
    else if (symb == '1'){
      curr = curr->c1;
    }
    std::cout << symb;
    symb = in.get();
  }
  std::cout << endl;
  std::cout << "Symbol returned: " << curr->symbol << endl;
  return curr->symbol;
}
