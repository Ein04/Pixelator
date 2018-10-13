#ifndef _TWODTREE_H_
#define _TWODTREE_H_

#include <utility>
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "stats.h"
using namespace std;
using namespace cs221util;


class twoDtree {
private:

   /**
    * The Node class is private to the tree class via the principle of
    * encapsulation---the end user does not need to know our node-based
    * implementation details.
    */
   class Node {
   public:
      Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a); // Node constructor

      pair<int,int> upLeft; 
      pair<int,int> lowRight;
      RGBAPixel avg;
      Node * left; // ptr to left subtree
      Node * right; // ptr to right subtree
      
   };
	
   
public:

   /**
    * twoDtree destructor.
    * Destroys all of the memory associated with the
    * current twoDtree. This function should ensure that
    * memory does not leak on destruction of a twoDtree.
    * 
    * @see twoDtree_given.cpp
    */
   ~twoDtree();

   /**
    * Copy constructor for a twoDtree. GIVEN
    * Since twoDtrees allocate dynamic memory (i.e., they use "new", we
    * must define the Big Three). This depends on your implementation
    * of the copy funtion.
    * @see twoDtree_given.cpp
    *
    * @param other The twoDtree  we are copying.
    */
   twoDtree(const twoDtree & other);

   /**
    * Overloaded assignment operator for twoDtrees. 
    * Part of the Big Three that we must define because the class
    * allocates dynamic memory. This depends on your implementation
    * of the copy and clear funtions.
    *
    * @param rhs The right hand side of the assignment statement.
    */
   twoDtree & operator=(const twoDtree & rhs);


   /**
    * Constructor that builds a twoDtree out of the given PNG.
    * Every leaf in the tree corresponds to a pixel in the PNG.
    * Every non-leaf node corresponds to a rectangle of pixels 
    * in the original PNG, represented by an (x,y) pair for the 
    * upper left corner of the rectangle and an (x,y) pair for 
    * lower right corner of the rectangle. In addition, the Node
    * stores a pixel representing the average color over the 
    * rectangle. 
    *
    * Every node's left and right children correspond to a partition
    * of the node's rectangle into two smaller rectangles. The node's
    * rectangle is split by the horizontal or vertical line that 
    * results in the two smaller rectangles whose sum of squared 
    * differences from their mean is as small as possible.
    *
    * The left child of the node will contain the upper left corner
    * of the node's rectangle, and the right child will contain the
    * lower right corner.
    *
   * This function will build the stats object used to score the 
   * splitting lines. It will also call helper function buildTree.
    */
   twoDtree(PNG & imIn);

   /**
    * Render returns a PNG image consisting of the pixels
    * stored in the tree. may be used on pruned trees. Draws
    * every leaf node's rectangle onto a PNG canvas using the 
    * average color stored in the node.
    */
   PNG render();

   /*
    *  Prune function trims subtrees as high as possible in the tree.
    *  A subtree is pruned (cleared) if at least pct of its leaves are within 
    *  tol of the average color stored in the root of the subtree. 
    *  Pruning criteria should be evaluated on the original tree, not 
    *  on a pruned subtree. (we only expect that trees would be pruned once.)
    *  
    * 
    */
   void prune(double pct, int tol);


private:
   Node* root; // ptr to the root of the twoDtree

   int height; // height of PNG represented by the tree
   int width; // width of PNG represented by the tree

   /* =================== private PA3 functions ============== */

   /**
    * Destroys all dynamically allocated memory associated with the
    * current twoDtree class.
    */
   void clear();

   /**
   * Copies the parameter other twoDtree into the current twoDtree.
   * Does not free any memory. Called by copy constructor and op=.
   * @param other The twoDtree to be copied.
   */
   void copy(const twoDtree & other);

   /**
   * Private helper function for the constructor. Recursively builds
   * the tree according to the specification of the constructor.
   * @param s Contains the data used to split the rectangles
   * @param ul upper left point of current node's rectangle.
   * @param lr lower right point of current node's rectangle.
   */
   Node * buildTree(stats & s,pair<int,int> ul, pair<int,int> lr);


   /**
   * Private helper function for the tree post-order traversal
   **/
   void clearNode(Node*& n);
   /**
   * Private helper function for the tree copy;
   **/
   Node* copyNode(Node* n);
   /**
   * Private helper function for the tree copy;
   **/
   void renderNode(PNG & pic, Node* n);
   /**
   * Private helper function for the pruneTree;
   **/
   // void pctNode(double pct, int tol, Node* n, pair<int,int> & pctPair);
   void pruneNode(double pct, int tol, Node* n);
   pair<int,int> leavesWithinTol(int tol, Node* rootN, Node* node);
   bool withinTol(int tol, RGBAPixel rootAvg, RGBAPixel leafAvg);

};

#endif
