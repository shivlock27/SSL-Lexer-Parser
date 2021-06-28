/*
Group #49 SSL

Members - 
Shivang Singh  2018A7PS0115H
Deepak George 2018A7PS0244H
Nishit Chouhan 2018A7PS0446H
Thakur Shivank Singh 2018A7PS0439H
*/

#include<bits/stdc++.h>
#include "Lexer.hpp"
using namespace std;

map<pair<string,string>,pair<string,vector<string>>> parser_table; //data structure to store the parse table
map<int,string> parser_input_ref; //maps token numbers and the lexemes
vector<string> parser_inp; //
vector<vector<string>> stack_contents; //stores the contents of the stack at each step of parsing
stack<string> input; //stores the input string obtained from the user in a stack to initiate the parsing process

//stores the terminal strings
vector<string> terminals = {"$","id","(",")","{","}","main","int","float","string","char","boolean",",",";","input","print","return","=","if","ifel","while","int_literal","float_literal","char_literal","string_literal","true","false","less_op","greater_op","less_eq_op","greater_eq_op","eq_eq_op","not_eq_op","logical_and","logical_or","!","+","-","^","&","|","*","/","%","ssc"};
//stores the non terminal strings
vector<string> non_terminals = {"S","PROGRAM","FUNCTION_LIST","FIRST_FUNCTION","FIRST_FUNCTION_PRE","FIRST_FUNCTION_POST""MORE_FUNCTIONS","MAIN","TYPE","FORMAL_PARAMS","FORMAL_PARAMS_POST","FORMAL_PARAMS_PRE","STMTS","STMT_PRE","STMT_POST","STMT","PRINT_POST","RETURN_POST","POS_3","POS_4","POS_5","ACTUAL_PARAMS","ACTUAL_PARAMS_P,RE""ACTUAL_PARAMS_POST","VARLIST_POST","CONDITION_ST","LOOP_ST","LITERAL","BOOL_LITERAL","REL_OP","LOG_OP","RELN_EXPR","RELN_EXPR'","EXPR","EXPR'","TERM","TERM'","FACTOR",};
//stores the productions that are used at each step of parsing
vector<pair<string,vector<string>>> productions;
//stores the level order traversal of the parse tree
map<int,vector<string>> level_order_traversal;
//structure that is used in the parse tree
typedef struct node
{
    string label; //the terminal or nonterminal that is a node in the tree
    vector<node*> children; //the children of the above mentioned node
}node;

//track the level of the tree when we are creating it, so that we can output the level order traversal at a later point
int ctr=0;

//function to create the parse tree
void create_parse_tree(node *curNode)
{
    if(curNode == NULL)//null condition, return
    {
        return;
    }
    string curLabel = curNode->label; //obtain the current node value
    if(find(terminals.begin(), terminals.end(),curLabel) != terminals.end()) //if current node is terminal then return
    {
        return;
    }
    vector<string> children = productions[ctr++].second; //get the child nodes
    for(auto i : children)//create new nodes for each of the children and add them to the children vector of the parent node
    {
        node *n1 = new node();
        n1->label = i;
        curNode->children.push_back(n1);
    }

    for(auto i : curNode->children)//recursively call create parse tree on the children of the current node
    {
        create_parse_tree(i);
    }
    
}
//function to print the parse tree
void print_parse_tree(node *curNode, int level)
{
    if(curNode == NULL)//return condition
    {
        return;
    }
    string curLabel = curNode->label;//get the value of the current node
    if(find(terminals.begin(), terminals.end(),curLabel) != terminals.end())//if the current node is a terminal then add it to the level order traversal vector
    {
        level_order_traversal[level].push_back(curLabel);
        return;
    }
    level_order_traversal[level].push_back(curLabel);//add to level order traversal array
    for(auto i : curNode->children) //increase the depth of the tree and recursively call print_parse_tree
    {
        print_parse_tree(i,level+1);
    }

}
//map the token numbers and the lexemes
void populate_parser_input_ref()
{
    parser_input_ref.insert({0 , "$" });
    parser_input_ref.insert({103 , "id" });
    parser_input_ref.insert({121 , "(" });
    parser_input_ref.insert({123 , ")" });
    parser_input_ref.insert({122 , "{" });
    parser_input_ref.insert({124 , "}" });
    parser_input_ref.insert({205 , "main" });
    parser_input_ref.insert({200 , "int" });
    parser_input_ref.insert({201 , "float" });
    parser_input_ref.insert({203 , "string" });
    parser_input_ref.insert({202 , "char" });
    parser_input_ref.insert({204 , "boolean" });
    parser_input_ref.insert({135 , "," });
    parser_input_ref.insert({125 , ";" });
    parser_input_ref.insert({215 , "input" });
    parser_input_ref.insert({212 , "print" });
    parser_input_ref.insert({213 , "return" });
    parser_input_ref.insert({113 , "=" });
    parser_input_ref.insert({206 , "if" });
    parser_input_ref.insert({207 , "ifel" });
    parser_input_ref.insert({210 , "while" });
    parser_input_ref.insert({104 , "int_literal" });
    parser_input_ref.insert({105 , "float_literal" });
    parser_input_ref.insert({107 , "char_literal" });
    parser_input_ref.insert({106 , "string_literal" });
    parser_input_ref.insert({208 , "true" });
    parser_input_ref.insert({209 , "false" });
    parser_input_ref.insert({109 , "less_op" });
    parser_input_ref.insert({111 , "greater_op" });
    parser_input_ref.insert({108 , "less_eq_op" });
    parser_input_ref.insert({110 , "greater_eq_op" });
    parser_input_ref.insert({112 , "eq_eq_op" });
    parser_input_ref.insert({114 , "not_eq_op" });
    parser_input_ref.insert({116 , "logical_and" });
    parser_input_ref.insert({118 , "logical_or" });
    parser_input_ref.insert({115 , "!" });
    parser_input_ref.insert({127 , "+" });
    parser_input_ref.insert({129 , "-" });
    parser_input_ref.insert({120 , "^" });
    parser_input_ref.insert({117 , "&" });
    parser_input_ref.insert({119 , "|" });
    parser_input_ref.insert({130 , "*" });
    parser_input_ref.insert({131 , "/" });
    parser_input_ref.insert({132 , "%" });

}
//function to perform parsing
int parser(stack<string> stk, stack<string> remaining_inp)
{
    int correct_syntax=1; //flag that indicates whether parsing was successful or not
    int need_parse_tree=0; //in case parse tree can be formed (for correct input), this flag is set to 1
    while(true)
    {
        // terminating Condition
        if(stk.empty() and remaining_inp.empty())
        {
            need_parse_tree=1;
            break;
        }
        //terminating condition
        else if (stk.empty())
        {
            cout<<endl<<"Parsing was completed but input string is not completely consumed and all non terminals are exhausted. "<<endl;
            break;
        }
        //terminating condition
        else if (remaining_inp.empty())
        {
            cout<<endl<<"Parsing was completed but the input string is completely consumed and the stack is not empty."<<endl;
            break;
        }
        //copy the contents of the stack into a string so that it can be displayed to the user at a later point in time if the input was incorrect
        stack<string> stkcopy(stk);
        vector<string> stktemp;
        while(!stkcopy.empty())
        {
            stktemp.push_back(stkcopy.top());
            stkcopy.pop();
        }
        stack_contents.push_back(stktemp);

        string top_of_stack = stk.top();

        string cur_inp = remaining_inp.top();

        
        if(find(terminals.begin(),terminals.end(),top_of_stack)!=terminals.end())//if the top of the stack is a terminal
        {
            if(top_of_stack==cur_inp)
            {
                remaining_inp.pop();
                stk.pop();
                // Match
            }
            else
            {
                // Error Not matching terminals
                correct_syntax=0;
                cout<<"Terminals do not match : - Expected : "<<top_of_stack<<" || Found : "<<cur_inp<<endl;
                remaining_inp.pop();
            }
        }
        else //if the top of the stack is a non terminal
        {
            string row = top_of_stack;
            string column = cur_inp;
            pair<string,string> key;
            key.first=row;
            key.second=column;
            pair<string,vector<string>> rule = parser_table[key];
            string lhs = rule.first;
            vector<string> rhs = rule.second; 
            //synch condition
            if(lhs=="SYNCH")
            {
                correct_syntax=0;
                cout<<"SYNCH was encountered in Parse table entry and "<<top_of_stack<<" was omitted."<<endl;
                stk.pop();
                continue;
            }
            //skip condition
            if(lhs=="SKIP")
            {
                correct_syntax=0;
                cout<<"SKIP was encountered in Parse table entry and current input symbol "<<cur_inp<<" was omitted."<<endl;
                remaining_inp.pop();
                continue;
            }
 
            // Null production
            if(rhs[0]=="ssc")
            {
                productions.push_back(rule);
                stk.pop();
                continue;
            }

            // Found a rule (not null production)
            productions.push_back(rule);
            stk.pop();
            for (auto it = rhs.rbegin(); it != rhs.rend(); it++)
            {
                stk.push(*it);
            }
        }
    }
    if(correct_syntax==0) //if incorrect input was provided
    {
        cout<<endl<<"Parsing was completed but either terminals did not match, SKIP was used or SYNCH was used"<<endl;
        return 0;
    }
    if(need_parse_tree==1) //if input was correct
    {
        cout<<"Parsing was successful and the level order traversal for the parse tree is generated as follows:- "<<endl;
        return 1;
    }
    return 0;
}
//reads from parser_table_final.txt and fills a datastructure with the parse table for easy querying
void populateParserTable()
{
   fstream parserfile;
   parserfile.open("parser_table_final.txt",ios::in); //open a file to perform read operation using file object
   std::vector<std::string> result; 
    if(parserfile.is_open())
    {   
        string s;

        while(getline(parserfile, s))
        { 
            std::istringstream iss(s); 
            pair<string,string> p1;

            for(std::string s; iss >> s; ){
                p1.first = s;
                iss>>s;
                p1.second = s;
                iss>>s;
                pair<string,vector<string>> p2;
                p2.first = s;
                vector<string> sub;
                iss>>s;
                for(;iss>>s;){
                    sub.push_back(s);
                }
                p2.second = sub;
                parser_table[p1] = p2;
            } 
        }
      parserfile.close();
    }
}

int main()
{
    populateParserTable(); //populate parse table
    populate_parser_input_ref(); //call function to make token lexeme pairs
    vector<pair<string,int>> token_list = Lex(); //call to lexer to get token list
    //populate parser_inp for each token obtained from lexer
    for(auto i : token_list)
    {
        parser_inp.push_back(parser_input_ref.at(i.second));
    }
    cout<<endl;
    parser_inp.push_back("$"); //dollar sign signifying end of input

    for (auto it = parser_inp.rbegin(); it != parser_inp.rend(); it++)
    {
        input.push(*it);
    }
    //stack for parsing, initially populated with the starting non terminal
    stack<string> st;
    st.push("$");
    st.push("PROGRAM");
    //if parsing is completed and the input given was correct, we create a parse tree and print its level order traversal
    if(parser(st,input)==1)
    {

        node *root = new node();
        root->label = "PROGRAM";
        create_parse_tree(root);
        print_parse_tree(root,0);
        for(auto it : level_order_traversal)
        {
            cout<<"Level "<<it.first<<" ";
            for(auto s : it.second)
            {
                cout<<s<<" ";
            }
            cout<<endl;
        }
    }
    else //if parsing was completed but the input had syntactic errors, then the stack contents at each step of parsing are printed and appropriate error messages are displayed 
    {
        cout<<endl<<"Input is not valid according to the Syntax of our language"<<endl<<endl;
        cout<<"The contents of the stack at each step are as follows :"<<endl;
        int ct=1;
        for(auto it:stack_contents)
        {
            cout<<"Step "<<ct++<<" : ";
            for(auto jt:it)
            {
                cout<<jt<<' ';
            }
            cout<<endl;
        }
    }

    return 0;
}

