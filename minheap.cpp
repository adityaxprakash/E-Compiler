/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "minheap.h"
using namespace std;

//Write your code below this line

MinHeap::MinHeap()
{

}

string binaryrep(int n)
{
    string binary="";
    while(n!=0)
    {
        int d=n%2;
        binary=std::to_string(d)+binary;
        n=n/2;
    }
    return binary;
}

void MinHeap::push_heap(int num)
{
    size++;
    if(size==1)
    {
        root=new HeapNode(num);
        return;
    }
    HeapNode* newNode=new HeapNode(num);
    string bin=binaryrep(size);
    HeapNode* toInsert=root;
    int i;
    int bin_size=bin.size();
    for(i=1;i<bin_size-1;i++)
    {
        if(bin[i]=='1')
            toInsert=toInsert->right;
        else
            toInsert=toInsert->left;
    }
    if(bin[i]=='1'){
        toInsert->right=newNode;
    }
    else{
        toInsert->left=newNode;
    }   
    newNode->par=toInsert;
    while(newNode->par!=nullptr)
    {
        if(newNode->par->val<=newNode->val)
        {
            return;
        }
        int temp=newNode->val;
        newNode->val=newNode->par->val;
        newNode->par->val=temp;
        newNode=newNode->par;
    } 
    return;
}

int MinHeap::get_min()
{ 
    if(size>0)  
        return root->val;
    return -1;
}

void MinHeap::pop()
{
    if(size==0)
        return;
    string bin=binaryrep(size);
    HeapNode* curr=root;
    int bin_size=bin.size();
    for(int i=1;i<bin_size;i++)
    {
        if(bin[i]=='1')
            curr=curr->right;
        else
            curr=curr->left;
    }
    root->val=curr->val;
    if(curr->par!=NULL && curr->par->left==curr)
        curr->par->left =NULL;
    else if(curr->par!=NULL && curr->par->right==curr)
        curr->par->right=NULL;
    else
    {
        root=NULL;
        delete curr;
        size--;
        return;
    }
    delete curr;
    curr=root;
    while(curr->left!=NULL)
    {
        int minInd=0;
        int minVal=curr->val;
        if(curr->left!=NULL)
        {
            if(curr->left->val<minVal)
            {
                minVal=curr->left->val;
                minInd=1;
            }
        }
        if(curr->right!=NULL)
        {
            if(curr->right->val<minVal)
            {
                minVal=curr->right->val;
                minInd=2;
            }
        }
        if(minInd==0)
            break;
        else if(minInd==1)
        {
            curr->left->val=curr->val;
            curr->val=minVal;
            curr=curr->left;            
        }
        else if(minInd==2)
        {
            curr->right->val=curr->val;
            curr->val=minVal;
            curr=curr->right;   
        }
    }
    size--;
    return;
}

void destro(HeapNode* root)
{
    if(root==nullptr)
        return;
    destro(root->left);
    destro(root->right);
    delete root;
}

MinHeap::~MinHeap()
{
    destro(root);
}

