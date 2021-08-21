#pragma once

#include <vector>

#include <QtWidgets>

template < class T > class TreeNode
{
public:
    TreeNode()
    {
        this->parent = nullptr;
    }

    TreeNode(const T& t , TreeNode* parent = nullptr )
    {
        this->t = t;
        this->parent = parent;
    }

    // copy constructor
    TreeNode(const TreeNode& obj )
    {
        this->t = obj.t;
        this->parent = obj.parent;
        this->children = obj.children;
        // fix the parent pointers
        for ( uint i = 0 ; i < this->children.size() ; ++i )
        {
            this->children.at(i).parent = this;
        }
    }

    // copy assignment operator
    TreeNode& operator=(const TreeNode& obj )
    {
        if ( this != &obj )
        {
            this->t = obj.t;
            this->parent = obj.parent;
            this->children = obj.children;
            // fix the parent pointers
            for ( uint i = 0 ; i < this->children.size() ; ++i )
            {
                this->children.at(i).parent = this;
            }
        }
        return *this;
    }

    virtual ~TreeNode()
    {

    }

    void addChild( const T& t )
    {
        this->children.push_back(TreeNode(t , this ) );
    }
    void addChild(TreeNode node){
        this->children.push_back(node);
    }

    // remove a child by value, note: if the node has multiple children with the same value, this will only delete the first child
    void removeChild( const T& t )
    {
        for ( uint i = 0 ; i < this->children.size() ; ++i )
        {
            if ( this->children.at(i).t == t )
            {
                this->children.erase( this->children.begin()+i );
                return;
            }
        }
    }

    void removeChildByIndex( const int index )
    {
        this->children.erase( this->children.begin()+index );
    }

    void setValue( const T& t )
    {
        this->t = t;
    }

    T& getValue()
    {
        return this->t;
    }

    const T& getValue() const
    {
        return this->t;
    }

    TreeNode& getParent()
    {
        return *this->parent;
    }

    const TreeNode& getParent() const
    {
        return *this->parent;
    }

    std::vector< TreeNode >& getChildren()
    {
        return this->children;
    }

    const std::vector< TreeNode >& getChildren() const
    {
        return this->children;
    }

    // the type has to have an overloaded std::ostream << operator for print to work
    void print( const int depth = 0 ) const
    {
        for ( int i = 0 ; i < depth ; ++i )
        {
            if ( i != depth-1 ) qDebug() << "    ";
            else std::cout << "|-- ";
        }
        std::cout << this->t << std::endl;
        for ( uint i = 0 ; i < this->children.size() ; ++i )
        {
            this->children.at(i).print( depth+1 );
        }
    }


private:
    T t;
    TreeNode* parent;
    std::vector< TreeNode > children;
};