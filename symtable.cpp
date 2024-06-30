/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"
using namespace std;

SymNode* searcher(SymNode* root, string k)
{
    if(root==NULL)
        return NULL;
    if(root->key==k)
        return root;
    if(root->key<k)
        return searcher(root->right,k);
    return searcher(root->left,k);
}

SymbolTable::SymbolTable()
{
    size=0;
    root=NULL;
}

SymNode* fixRoot(SymNode* root)
{
    if(root==NULL)
        return NULL;
    while(root->par)
        root=root->par;
    return root;
}

void treeFixUp(SymNode* node)
{
    if(node==NULL){
        return;
    }
    int leftHeight=-1;
    int rightHeight=-1;
    SymNode* leftChild=node->left;
    SymNode* rightChild=node->right;
    node->height=0;
    if(leftChild!=NULL)
    {
        node->height=max(node->height,leftChild->height+1);
        leftHeight=leftChild->height;
    }
    if(rightChild!=NULL)
    {
        node->height=max(node->height,rightChild->height+1);
        rightHeight=rightChild->height;
    }
    if(leftHeight-rightHeight<=-2)
    {
        int rleftHeight=-1;
        int rrightHeight=-1;
        if(rightChild->left!=NULL)
        {
            rleftHeight=rightChild->left->height;
        }
        if(rightChild->right!=NULL)
        {
            rrightHeight=rightChild->right->height;
        }
        if(rleftHeight>rrightHeight)
        {
            node=node->LeftRightRotation();
        }
        else
        {   
            node=node->LeftLeftRotation();
        }
    }
    else if(leftHeight-rightHeight>=2)
    {
        int lleftHeight=-1;
        int lrightHeight=-1;
        if(leftChild->left!=NULL)
        {
            lleftHeight=leftChild->left->height;
        }
        if(leftChild->right!=NULL)
        {
            lrightHeight=leftChild->right->height;
        }
        if(lleftHeight>=lrightHeight)
        {
            node=node->RightRightRotation();
        }
        else
        {
            node=node->RightLeftRotation();
        }
    }
    treeFixUp(node->par);
}

SymNode* deleter(string k, SymNode* root)
{
    SymNode* tbd=searcher(root,k);
    if(tbd==NULL)
        return NULL;
    SymNode* p=tbd->par;
    if(tbd->left==NULL && tbd->right==NULL)
    {
        if(p!=NULL && p->left==tbd)
            p->left=NULL;
        else if(p!=NULL && p->right==tbd)
            p->right=NULL;
        else
        {
            root=NULL;
            delete tbd;
            return root;
        }
        delete tbd;
        treeFixUp(p);
        return fixRoot(root);
    }
    else if(tbd->left==NULL)
    {
        tbd->right->par=p;
        if(p==NULL)
        {
            SymNode* temp=tbd->right;
            delete tbd;
            return temp;
        }
        else
        {
            if(p->left==tbd)
                p->left=tbd->right;
            else if(p->right==tbd)
                p->right=tbd->right;
            delete tbd;
            treeFixUp(p);
            return fixRoot(root);
        }
    }
    else if(tbd->right==NULL)
    {
        tbd->left->par=p;
        if(p==NULL)
        {
            SymNode* temp=tbd->left;
            delete tbd;
            return temp;
        }
        else
        {
            if(p->left==tbd)
                p->left=tbd->left;
            else if(p->right==tbd)
                p->right=tbd->left;
            delete tbd;
            treeFixUp(p);
            return fixRoot(root);
        }
    }
    SymNode* successor=tbd->right;
    while(successor->left)
        successor=successor->left;
    tbd->key=successor->key;
    tbd->address=successor->address;
    SymNode* parent=successor->par;
    if(successor->left==NULL && successor->right==NULL)
    {
        if(parent!=NULL && parent->left==successor)
            parent->left=NULL;
        else if(parent!=NULL && parent->right==successor)
            parent->right=NULL;
        delete successor;
        treeFixUp(parent);
        
    }
    else if(successor->left==NULL)
    {
        successor->right->par=parent;
        if(parent->left==successor)
            parent->left=successor->right;
        else if(parent->right==successor)
            parent->right=successor->right;
        delete successor;
        treeFixUp(parent);
    }
    else if(successor->right==NULL)
    {
        successor->left->par=parent;
        if(parent->left==successor)
            parent->left=successor->left;
        else if(parent->right==successor)
            parent->right=successor->left;
        delete successor;
        treeFixUp(parent);
    }  
    return fixRoot(root);            
}

void SymbolTable::remove(string k)
{   
    if(searcher(root,k)==NULL)
        return;
    size--;
    root=deleter(k,root);
}

void SymbolTable::insert(string k)
{
    if(searcher(root,k)!=NULL)
        return;
    if(size==0)
    {
        size++;
        root=new SymNode(k);
        return;
    }
    size++;
    SymNode* newNode= new SymNode(k);
    SymNode* curr=root;
    while(true)
    {
        if(k<curr->key && curr->left==NULL){
            newNode->par=curr;
            curr->left=newNode;
            treeFixUp(curr);
            root=fixRoot(root);
            return;
        }
        if(k>curr->key && curr->right==NULL){
            newNode->par=curr;
            curr->right=newNode;
            treeFixUp(curr);
            root=fixRoot(root);
            return;
        }
        if(k<curr->key)
            curr=curr->left;
        else if(k>curr->key)
            curr=curr->right;
    }
}

int SymbolTable::search(string k)
{
    SymNode* node=searcher(root,k);
    if(node==NULL)
        return -2;
    return node->address;
}

void SymbolTable::assign_address(string k,int idx)
{
    SymNode* node=searcher(root,k);
    if(node==NULL)
        return;
    node->address=idx;
}

int SymbolTable::get_size()
{
    return size;
}

SymNode* SymbolTable::get_root()
{
    return root;
}

void destroyer(SymNode* root)
{
    if(root==NULL)
        return;
    destroyer(root->left);
    destroyer(root->right);
    delete root;
}

SymbolTable::~SymbolTable()
{
    destroyer(root);
}

