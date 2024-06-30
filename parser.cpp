/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "parser.h"
using namespace std;


Parser::Parser()
{
    symtable=new SymbolTable();
    last_deleted=-1;
}

bool check_number(string c)
{
    if(c.size()==0)
        return false;
    if(c[0]=='-')
    {
        c=c.substr(1,c.size()-1);
    }
    if(c.size()==0)
        return false;
    string t="0123456789";
    int len_c=c.size();
    for(int i=0;i<len_c;i++)
    {
        bool check=false;
        for(int j=0;j<10;j++)
        {
            if(c[i]==t[j])
                check=true;
        }
        if(check==false)
            return false;
    }
    return true;
}

void makeTree(vector<ExprTreeNode*> &trees_stack)
{
    ExprTreeNode* n3= trees_stack.back();
    trees_stack.pop_back();
    ExprTreeNode* n2= trees_stack.back();
    trees_stack.pop_back();
    ExprTreeNode* n1= trees_stack.back();
    trees_stack.pop_back();
    n2->left=n1;
    n2->right=n3;
    trees_stack.push_back(n2);
}

void Parser::parse(vector<string> expression)
{
    vector<ExprTreeNode*> tempstack;
    int len=expression.size();
    for(int i=0;i<len;i++)
    {
        string current=expression[i];
        if(current=="(")
            continue;
        else if(current==":=")
        {
            ExprTreeNode* newExp=new ExprTreeNode();
            newExp->id=current;
            newExp->type="VAR";
            tempstack.push_back(newExp);
        }
        else if(current=="ret")
        {
            ExprTreeNode* newExp=new ExprTreeNode();
            newExp->type="RET";
            tempstack.push_back(newExp);
        }
        else if(current=="del")
        {
            ExprTreeNode* newExp=new ExprTreeNode();
            newExp->type="DEL";
            tempstack.push_back(newExp);
        }
        else if(check_number(current))
        {
            ExprTreeNode* newExp = new ExprTreeNode("VAL",std::stoi(current));
            tempstack.push_back(newExp);
        }
        else if(current=="+")
        {
            ExprTreeNode* newExp = new ExprTreeNode();
            newExp->type="ADD";
            tempstack.push_back(newExp);
        }
        else if(current=="-")
        {
            ExprTreeNode* newExp = new ExprTreeNode();
            newExp->type="SUB";
            tempstack.push_back(newExp);
        }
        else if(current=="*")
        {
            ExprTreeNode* newExp = new ExprTreeNode();
            newExp->type="MUL";
            tempstack.push_back(newExp);
        }
        else if(current=="/")
        {
            ExprTreeNode* newExp = new ExprTreeNode();
            newExp->type="DIV";
            tempstack.push_back(newExp);
        }
        else if(current!=")")
        {
            ExprTreeNode* newExp = new ExprTreeNode();
            newExp->id=current;
            newExp->type="VAR";
            tempstack.push_back(newExp);
        }
        else
        {
            int l=tempstack.size();
            if(l>3)
            {
                makeTree(tempstack);
            }
            else
                continue;
        }
    }
    makeTree(tempstack);
    ExprTreeNode* root=tempstack.back();
    tempstack.pop_back();
    expr_trees.push_back(root);
    if(expression[0]=="ret")
        return;
    else if(expression[0]=="del")
    {
        last_deleted=symtable->search(expression[2]);
        symtable->remove(expression[2]);
    }
    else
    {
        if(symtable->search(expression[0])==-2)
        {
            symtable->insert(expression[0]);
        }
    }
}



void destr(ExprTreeNode* root)
{
    if(root==nullptr)
        return;
    destr(root->left);
    destr(root->right);
    delete root;
}

Parser::~Parser() 
{
    delete symtable;
    int expr_trees_size=expr_trees.size();
    for(int i=0;i<expr_trees_size;i++)
    {
        destr(expr_trees[i]);
    }
}