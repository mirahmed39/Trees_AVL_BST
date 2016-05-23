#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "dsexceptions.h"
#include "SequenceMap.h"
#include <algorithm>
#include <iostream>
using namespace std;

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinarySearchTree
{
  public:
    BinarySearchTree( ) : root_{ nullptr }
    {
    }

    /**
     * Copy constructor
     */
    BinarySearchTree( const BinarySearchTree & rhs ) : root_{ nullptr }
    {
        root_ = clone( rhs.root_ );
    }

    /**
     * Move constructor
     */
    BinarySearchTree( BinarySearchTree && rhs ) : root_{ rhs.root_ }
    {
        rhs.root_ = nullptr;
    }

    /**
     * Destructor for the tree
     */
    ~BinarySearchTree( )
    {
        makeEmpty( );
    }

    /**
     * Copy assignment
     */
    BinarySearchTree & operator=( const BinarySearchTree & rhs )
    {
        BinarySearchTree copy = rhs;
        std::swap( *this, copy );
        return *this;
    }

    /**
     * Move assignment
     */
    BinarySearchTree & operator=( BinarySearchTree && rhs )
    {
        std::swap( root_, rhs.root_ );
        return *this;
    }


    /**
     * Find the smallest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMin( root_ )->map_;
    }

    /**
     * Find the largest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMax( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMax( root_ )->map_;
    }

    /**
     * Returns true if x is found in the tree.
     */
    bool contains( const Comparable & x, int &counter ) const
    {
        return contains( x, root_, ++counter );
    }

    /**
      * finds x and prints it.
      */
    void printElement( const Comparable &x) {
        findAndPrint(x, root_);
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty( ) const
    {
        return root_ == nullptr;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void printTree( ostream & out = cout ) const
    {
        if( isEmpty( ) )
            out << "Empty tree" << endl;
        else
            printTree( root_, out );
        out << endl;
    }

    /**
     * Make the tree logically empty.
     */
    void makeEmpty( )
    {
        makeEmpty( root_ );
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( const Comparable & x )
    {
        insert( x, root_ );
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( Comparable && x )
    {
        insert( std::move( x ), root_ );
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove( const Comparable & x, int &counter )
    {
        remove( x, root_, ++counter );
    }

    /**
      * counts the total nodes form the beginning of the tree.
      */
    int countNodes() const {
        return totalNodes(root_);
    }

    /**
      * counts depth of each node in the tree and adds to total.
      */
    int depthOfTree( int depth ) {
        return findDepth(root_, depth);
    }

  private:
    struct BinaryNode
    {
        Comparable element_;
        BinaryNode *left_;
        BinaryNode *right_;

        BinaryNode( const Comparable & ele, BinaryNode *lt, BinaryNode *rt )
          : element_{ ele }, left_{ lt }, right_{ rt } { }

        BinaryNode( Comparable && ele, BinaryNode *lt, BinaryNode *rt )
          : element_{ std::move( ele ) }, left_{ lt }, right_{ rt } { }
    };

    BinaryNode *root_;


    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that root_s the subtree.
     * Set the new root of the subtree.
     */
    void insert( const Comparable & x, BinaryNode * & t )
    {
        if( t == nullptr )
            t = new BinaryNode{ x, nullptr, nullptr };
        else if( x < t->element_ && !x.isEmpty())
            insert( x, t->left_ );
        else if( t->element_ < x && !x.isEmpty())
            insert( x, t->right_ );
        else
            t->element_.Merge(x);  // Duplicate, Merge!!!
    }

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert( Comparable && x, BinaryNode * & t )
    {
        if( t == nullptr )
            t = new BinaryNode{ std::move( x ), nullptr, nullptr };
        else if( x < t->element_ && !x.isEmpty() )
            insert( std::move( x ), t->left_ );
        else if( t->element_ < x && !x.isEmpty() )
            insert( std::move( x ), t->right_ );
        else
            t->element_.Merge(x);  // Duplicate; do nothing
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void remove( const Comparable & x, BinaryNode * & t, int &counter ) {
        if( t == nullptr )
            return;// Item not found; do nothing
        if( x < t->element_) {
            remove( x, t->left_, ++counter );
        }
        else if( t->element_ < x) {
            remove( x, t->right_, ++counter );
        }
        else if( t->left_ != nullptr && t->right_ != nullptr ) // Two children
        {
            t->element_ = findMin( t->right_ )->element_;
            remove( t->element_, t->right_, ++counter );
        }
        else
        {
            BinaryNode *oldNode = t;
            t = ( t->left_ != nullptr ) ? t->left_ : t->right_;
            delete oldNode;
        }
    }


    /**
     * Return the total number of nodes of of the tree
     */
    int totalNodes( BinaryNode *t ) const {
        int count = 0;
        if( t != nullptr ) {
            count = 1 + totalNodes( t->left_ ) + totalNodes( t->right_ );
        }
        return count;
    }

    int findDepth(BinaryNode *t, int depth) {
        if (t == nullptr)
            return 0;
        else
            return depth + findDepth(t->left_, depth+1) + findDepth(t->right_, depth+1);
    }

    /**
     * Internal method to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    BinaryNode * findMin( BinaryNode *t ) const
    {
        if( t == nullptr )
            return nullptr;
        if( t->left_ == nullptr )
            return t;
        return findMin( t->left_ );
    }

    /**
     * Internal method to find the largest item in a subtree t.
     * Return node containing the largest item.
     */
    BinaryNode * findMax( BinaryNode *t ) const
    {
        if( t != nullptr )
            while( t->right_ != nullptr )
                t = t->right_;
        return t;
    }


    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the subtree.
     */
     bool contains( const Comparable & x, BinaryNode *t, int &counter ) const
    {
        if( t == nullptr )
            return false;
        else if( x < t->element_ )
            return contains( x, t->left_, ++counter );
        else if( t->element_ < x )
            return contains( x, t->right_, ++counter );
        else
            return true;    // Match
    }

    bool findAndPrint( const Comparable &x, BinaryNode *t ) {
        if( t == nullptr )
            return false;
        else if( x < t->element_ )
            return findAndPrint( x, t->left_ );
        else if( t->element_ < x )
            return findAndPrint( x, t->right_ );
        else
            cout << t->element_;    // Match
    }
/****** NONRECURSIVE VERSION*************************
    bool contains( const Comparable & x, BinaryNode *t ) const
    {
        while( t != nullptr )
            if( x < t->element_ )
                t = t->left_;
            else if( t->element_ < x )
                t = t->right_;
            else
                return true;    // Match

        return false;   // No match
    }
*****************************************************/

    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty( BinaryNode * & t )
    {
        if( t != nullptr )
        {
            makeEmpty( t->left_ );
            makeEmpty( t->right_ );
            delete t;
        }
        t = nullptr;
    }

    /**
     * Internal method to print a subtree rooted at t in sorted order.
     */
    void printTree( BinaryNode *t, ostream & out ) const
    {
        if( t != nullptr )
        {
            printTree( t->left_, out );
            out << t->element_ << " ";
            printTree( t->right_, out );
        }
    }

    /**
     * Internal method to clone subtree.
     */
    BinaryNode * clone( BinaryNode *t ) const
    {
        if( t == nullptr )
            return nullptr;
        else
            return new BinaryNode{ t->map_, clone( t->left_ ), clone( t->right_ ) };
    }
};

#endif

