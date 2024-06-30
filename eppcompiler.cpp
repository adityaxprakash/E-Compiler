/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "eppcompiler.h"
using namespace std;

EPPCompiler::EPPCompiler()
{
    memory_size=0;
    output_file="targ.txt";
}

EPPCompiler::EPPCompiler(string out_file,int mem_limit)
{
    memory_size=mem_limit;
    output_file=out_file;
    for(int i=0;i<mem_limit;i++)
        least_mem_loc.push_heap(i);
}

void EPPCompiler::compile(vector<vector<string>> code)
{
    vector<string> compiled_lines;
    int lines=code.size();
    for(int i=0;i<lines;i++)
    {
        vector<string> temp_compiled;
        targ.parse(code[i]);
        if(code[i][0]=="del")
        {
            least_mem_loc.push_heap(targ.last_deleted);
        }
        else if(code[i][0]=="ret")
        {
        }
        else
        {
            if(targ.symtable->search(code[i][0])==-1)
            {

                int tbadded=least_mem_loc.get_min();
                least_mem_loc.pop();
                targ.symtable->assign_address(code[i][0],tbadded);
            }
        }
        temp_compiled=generate_targ_commands();
        int code_size=temp_compiled.size();
        for(int k=0;k<code_size;k++){
            compiled_lines.push_back(temp_compiled[k]);
        }
    }

    write_to_file(compiled_lines);
}

vector<string> EPPCompiler::generate_targ_commands()
{
    vector<string> line;
    ExprTreeNode* root=targ.expr_trees.back();
    if(root->id==":=")
    {
        if(root->left->type=="DEL")
        {
            line.push_back("DEL = mem["+std::to_string(targ.last_deleted)+"]");
        }
        else if(root->left->type=="VAR")
        {
            targ.expr_trees.push_back(root->right);
            line=generate_targ_commands();
            targ.expr_trees.pop_back();

            int ad=targ.symtable->search(root->left->id);
            line.push_back("mem["+std::to_string(ad)+"] = POP");
        }
        else if(root->left->type=="RET")
        {
            targ.expr_trees.push_back(root->right);
            line=generate_targ_commands();
            targ.expr_trees.pop_back();

            line.push_back("RET = POP");
        }
    }   
    else if(root->type=="VAL")
    {
        line.push_back("PUSH "+std::to_string(root->num));
    }
    else if(root->type=="VAR")
    {
        int ad=targ.symtable->search(root->id);
        line.push_back("PUSH mem["+std::to_string(ad)+"]");
    }
    else if(root->type=="ADD")
    {
        targ.expr_trees.push_back(root->right);
        line=generate_targ_commands();
        targ.expr_trees.pop_back();

        targ.expr_trees.push_back(root->left);
        vector<string> temp=generate_targ_commands();
        int temp_size=temp.size();
        for(int i=0;i<temp_size;i++)
            line.push_back(temp[i]);
        targ.expr_trees.pop_back();

        line.push_back("ADD");
    }
    else if(root->type=="SUB")
    {
        targ.expr_trees.push_back(root->right);
        line=generate_targ_commands();
        targ.expr_trees.pop_back();

        targ.expr_trees.push_back(root->left);
        vector<string> temp=generate_targ_commands();
        int temp_size=temp.size();
        for(int i=0;i<temp_size;i++)
            line.push_back(temp[i]);
        targ.expr_trees.pop_back();

        line.push_back("SUB");
    }   
    else if(root->type=="MUL")
    {
        targ.expr_trees.push_back(root->right);
        line=generate_targ_commands();
        targ.expr_trees.pop_back();

        targ.expr_trees.push_back(root->left);
        vector<string> temp=generate_targ_commands();
        int temp_size=temp.size();
        for(int i=0;i<temp_size;i++)
            line.push_back(temp[i]);
        targ.expr_trees.pop_back();

        line.push_back("MUL");
    }   
    else if(root->type=="DIV")
    {
        targ.expr_trees.push_back(root->right);
        line=generate_targ_commands();
        targ.expr_trees.pop_back();

        targ.expr_trees.push_back(root->left);
        vector<string> temp=generate_targ_commands();
        int temp_size=temp.size();
        for(int i=0;i<temp_size;i++)
            line.push_back(temp[i]);
        targ.expr_trees.pop_back();

        line.push_back("DIV");
    }
    return line;  
}

void EPPCompiler::write_to_file(vector<string> commands)
{
    ofstream outf;
    outf.open(output_file);
    int commands_size=commands.size();
    for(int i=0;i<commands_size;i++)
    {
        outf<<commands[i]<<"\n";
    }
    outf.close();
}

EPPCompiler::~EPPCompiler()
{
}

