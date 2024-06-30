/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symnode.h"
using namespace std;

void insertOnParent(SymNode* node, SymNode* newNode)
{
    if(node==NULL)
        return;
    if(node->par==NULL)
    {
        return;
    }
    if(node->par->left==node){
        node->par->left=newNode;
        return;
    }
    if(node->par->right==node){
        node->par->right=newNode;
        return;
    }   
}

void HeightFixUp(SymNode* node)
{
    if(node==NULL)
        return;
    node->height=0;
    if(node->left!=NULL)
        node->height=max(node->height,node->left->height+1);
    if(node->right!=NULL)
        node->height=max(node->height,node->right->height+1);
    return;
}

SymNode::SymNode()
{
    key="";
    height=0;
    par=NULL;
    left=NULL;
    right=NULL;
}

SymNode::SymNode(string k)
{
    key=k;
    height=0;
    par=NULL;
    left=NULL;
    right=NULL;
}

SymNode* SymNode::LeftLeftRotation()
{
    SymNode* b=this->right;
    insertOnParent(this,b);
    b->par=this->par;
    this->par=b;
    this->right=b->left;
    if(this->right)
        this->right->par=this;
    b->left=this;
    HeightFixUp(this);
    HeightFixUp(b);
    return b;
}

SymNode* SymNode::RightRightRotation()
{
    SymNode* b=this->left;
    insertOnParent(this,b);
    b->par=this->par;
    this->par=b;
    this->left=b->right;
    if(this->left)
        this->left->par=this;
    b->right=this;
    HeightFixUp(this);
    HeightFixUp(b);
    return b;
}

SymNode* SymNode::LeftRightRotation()
{
    SymNode* b=this->right->left;
    insertOnParent(this,b);
    SymNode* c=this->right;
    c->par=b;
    c->left=b->right;
    if(c->left)
        c->left->par=c;
    b->right=c;
    b->par=this->par;
    this->par=b;
    this->right=b->left;
    if(this->right)
        this->right->par=this;
    b->left=this;
    HeightFixUp(this);
    HeightFixUp(c);
    HeightFixUp(b);
    return b;
}

SymNode* SymNode::RightLeftRotation()
{
    SymNode* b=this->left->right;
    insertOnParent(this,b);
    SymNode* c=this->left;
    c->par=b;
    c->right=b->left;
    if(c->right)
        c->right->par=c;
    b->left=c;
    b->par=this->par;
    this->par=b;
    this->left=b->right;
    if(this->left)
        this->left->par=this;
    b->right=this;
    HeightFixUp(this);
    HeightFixUp(c);
    HeightFixUp(b);
    return b;
}

SymNode::~SymNode()
{

}

