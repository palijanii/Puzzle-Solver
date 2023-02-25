#include "chain.h"
#include <cmath>
#include <iostream>

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  clear();
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::insertAfter(Node * p, const Block &ndata) {
  Node *temp = new Node(ndata);
  if (p == NULL) {
    temp->next = head_;
    if (head_ != NULL) {
    temp->next->prev = temp;
    }
    head_ = temp;
  } else {
    if (p->next != NULL) {
    temp->next = p->next;
    p->next->prev = temp;
    temp->prev = p;
    p->next = temp;
    } else {
      temp->prev = p;
      p->next = temp;
      temp->next = NULL;
    }
   }
  length_++;  
  return temp;
}

/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::swap(Node *p, Node *q) {
  if (p == NULL || q == NULL || p == q) return;
  
  if (p == head_) {
    head_ = q;
  }
  if (q == head_) {
    head_ = p;
  }
  
  if (p->next != q && q->next != p) {
    Node *pprev = p->prev;
    Node *pnext = p->next;
    Node *qprev = q->prev;
    Node *qnext = q->next;
    p->next = NULL;
    p->prev = NULL;
    q->next = NULL;
    q->prev = NULL;
    if (pprev != NULL) {
      pprev->next = q;
      q->prev = pprev;
    } else {
      q->prev = NULL;
      head_ = q;
    }
    if (pnext != NULL) {
      pnext->prev = q;
      q->next = pnext;
    } else {
      q->next = NULL;
    }
    if (qprev != NULL) {
      qprev->next = p;
      p->prev = qprev;
    } else {
      p->prev = NULL;
      head_ = p;
    }
    if (qnext != NULL) {
      qnext->prev = p;
      p->next = qnext;
    } else {
      p->next = NULL;
    }
  } else {
    if (q->next == p) {
      Node* pp = p;
      p = q;
      q = pp;
    } // we know p->next == q for sure
    Node *pprev = p->prev;
    Node *qnext = q->next;
    p->prev = q;
    q->next = p;
    if (pprev != NULL) {
      pprev->next = q;
      q->prev = pprev;
    } else {
      q->prev = NULL;
      head_ = q;
    }
    if (qnext != NULL) {
      qnext->prev = p;
      p->next = qnext;
    } else {
      p->next = NULL;
    }
  }
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  // if (head_ == NULL || head_->next == NULL) {
  //   delete head_;
  //   head_ = NULL;
  // } else {
  //   Node * temp = head_;
  //   head_  = head_->next;
  //   delete temp;
  //   temp = NULL;
  //   clear();
  // }
  Node *tmp = head_;
  while (tmp != NULL) {
    Node *tmp2 = tmp->next;
    delete tmp;
    tmp = tmp2;
  }
  head_ = NULL;
  length_ = 0;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
  clear();
  if (other.empty()) return;
  Node *curr = NULL;
  for (Node* p=other.head_; p!=NULL; p = p->next) {
     curr = insertAfter(curr, p->data);
  }

  // Node * head = tempHead;
  // Node * tempOther = other.head_;
  // for (int i=0; i<other.size()-1; i++) {
  //   Node * temp = new Node(tempOther->next->data);
  //   tempOther = tempOther->next;
  //   tempHead->next = temp;
  //   tempHead->next->prev = tempHead;
  //   tempHead = tempHead->next;
  // }
  // head_ =  head;
}

/* Modifies the current chain: 
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block 
 *	is big for all other blocks.
 *	For each block B, find the distanceTo B from every other block
 *	and take the minimum of these distances as B's "value".
 *	Choose the block B with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block B, find the node with
 *    the block that is the closest match to follow B (to the right)
 *    among the remaining blocks, move (swap) it to follow B's node,
 *    then repeat to unscramble the chain/image.
 */
void Chain::unscramble() {
  Node *current = head_;
  Node *tmp = NULL;
  
  double highestScore = 0;
  // vector<Node*> nodes;
  // vector<double> scores;
  std::cout << "unscramble is initialized\n";
  for (int i=0; i < size(); i++) {
    // Node *lowestNodeScore = nullptr;
    
    double lowestScore = calcScore(current);
    if (lowestScore > highestScore) {
      highestScore = lowestScore;
      tmp = current;
    }
    // nodes.push_back(lowestNodeScore);
    // scores.push_back(lowestScore);  
    current = current->next;
  }
  swap(head_, tmp);
  current = head_;
  if (current->prev != NULL) {
  }
  
  while (current->next != NULL) {
    setUpNextBlocK(current);
    current = current->next;
  }

}

double Chain::calcScore(Node * current) {
  Node *tmp = head_;
  double lowestScore = 1.0;
  while (tmp != NULL) {
    if (tmp != current) {
      double score = tmp->data.distanceTo(current->data);
      if (score < lowestScore) {
        lowestScore = score;
      }
    }
    tmp = tmp->next;
  
  }
  return lowestScore;
  
}

void Chain::setUpNextBlocK(Node * current) {
    std::cout << "287\n";

  Node * tmp = current->next;
  double lowestScore = 1;
  Node * possibleRightNode; 
    std::cout << "292\n";
  while (tmp != NULL) {
    double score = current->data.distanceTo(tmp->data);
    if (score < lowestScore) {
      lowestScore = score;
      possibleRightNode = tmp;
    }
    tmp = tmp->next;
  }
    std::cout << "301\n";
    if (current->next == NULL) {
    std::cout << "current next is NULL\n";

    }
    if (possibleRightNode == NULL) {
    std::cout << "possibleRightNode is NULL\n";
    }
    if (current->next == possibleRightNode) {
      return;
    }
    swap(current->next, possibleRightNode);
    std::cout << "303\n";
}

