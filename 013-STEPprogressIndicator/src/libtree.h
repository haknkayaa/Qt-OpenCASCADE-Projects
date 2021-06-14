#pragma once

#include <vector>



template < class T > class lstdTreeNode
{
public:
    lstdTreeNode()
    {
        this->parent = nullptr;
    }

    lstdTreeNode(const T& t , lstdTreeNode* parent = nullptr )
    {
        this->t = t;
        this->parent = parent;
    }

    // copy constructor
    lstdTreeNode(const lstdTreeNode& obj )
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
    lstdTreeNode& operator=(const lstdTreeNode& obj )
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

    virtual ~lstdTreeNode()
    {

    }

    void addChild( const T& t )
    {
        this->children.push_back(lstdTreeNode(t , this ) );
    }
    void addChild(lstdTreeNode node){
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

    lstdTreeNode& getParent()
    {
        return *this->parent;
    }

    const lstdTreeNode& getParent() const
    {
        return *this->parent;
    }

    std::vector< lstdTreeNode >& getChildren()
    {
        return this->children;
    }

    const std::vector< lstdTreeNode >& getChildren() const
    {
        return this->children;
    }

    // the type has to have an overloaded std::ostream << operator for print to work
    void print( const int depth = 0 ) const
    {
        for ( int i = 0 ; i < depth ; ++i )
        {
            if ( i != depth-1 ) std::cout << "    ";
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
    lstdTreeNode* parent;
    std::vector< lstdTreeNode > children;
};