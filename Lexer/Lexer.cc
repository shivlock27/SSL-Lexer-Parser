/*
Group #49 SSL

Members - 
Shivang Singh  2018A7PS0115H
Deepak George 2018A7PS0244H
Nishit Chouhan 2018A7PS0446H
Thakur Shivank Singh 2018A7PS0439H
*/

#include<bits/stdc++.h>
#include<fstream>

using namespace std;

FILE * fp;  // File pointer that is used to read the input file character by character
int line_count=1;  // Maintains the current line number
char c;   // Stores the character that is to be processed by the lexer
//Vector that contains all the keywords present in the language
vector<string> keywords = {"int", "float", "char", "string", "bool",
                            "main","if","ifel","true","false","while","void","print","return","break","input"};

map<string,int> keywords_map;  //Map that stores the keywords and their corresponding token numbers

//Function that maps each keyword to its corresponding token number
void populateMap()
{
   for(int i=0;i<keywords.size();i++)
   {
      pair<string,int> p;
      p.first = keywords[i];
      p.second = 200+i;
      keywords_map.insert(p);
   }  
}
//Function to print the tokens found
void printer(string lexeme, int token_number, int line_number)
{
   cout<<"Token "<<token_number<<" string "<<lexeme<<" Line Number:"<<line_number<<endl;
}

int main()
{
   fp = freopen("input.txt", "r", stdin); //Open file that contains the input program to be scanned
   populateMap(); //calling function to map keywords to their token numbers
   c = fgetc(fp); //read character 
   while(1) {
      //Entry condition for an identifier
      if((c>='A' and c<='Z') or (c>='a' and c<='z') or (c=='_'))
      {
   
         string lexeme = "";
         lexeme.push_back(c);
         c = fgetc(fp);
         //continue to push back characters into the lexeme as long as they're letters, underscore or numbers
         while((c>='A' and c<='Z') or (c>='a' and c<='z') or (c=='_') or (c>='0' and c<='9'))
         {
            lexeme.push_back(c);
            c = fgetc(fp);
         }
         //check if the lexeme constructed thus far is a keyword
         if(keywords_map.find(lexeme)!=keywords_map.end())
         {
            printer(lexeme,keywords_map[lexeme],line_count); //lexeme recognized as keyword 
         }
         else
         {
            printer(lexeme,103,line_count); // lexeme recognized as identifier
         }
         //Check for termination of input stream
         if(feof(fp))
         {
            break;
         }
      }
      //Entry condition for numbers (Integers and Floats)
      else if(c>='0' and c<='9')
      {
         
            int poss_token_num = 104; //start with assumption that it will possibly be an integer
            string temp_num = "";
            temp_num.push_back(c); //Push back the first digit
            bool invalid_float = false;
            bool digit_after = false;
            c = fgetc(fp);
            while(c>='0' and c<='9') //push back all the subsequent digits
            {
               temp_num.push_back(c);
               c = fgetc(fp);
            }
            if(c=='.') //On encountering floating point, change the token number as it is now a floating point number
            {
               poss_token_num=105;
               temp_num.push_back(c);
               c = fgetc(fp);
              //continue to push back the digits that follow
               while((c>='0' and c<='9') or c=='.')
               {
                  if(c=='.')//On encountering the second floating point, display a lexical error
                  {
                     temp_num.push_back(c);
                     cout<<"Lexical error : Too many decimal points "<<temp_num<<" Line Number : "<<line_count<<endl;
                     c = fgetc(fp);
                     invalid_float = true; 
                     break;
                  }
                  else //Ensure that there are digits that follow after the floating point
                  {
                     digit_after = true;
                  }
                  temp_num.push_back(c); //push back following digits in the number
                  c = fgetc(fp);
               }
               if(!digit_after) //if there are no digits after floating point, display a lexical error
               {
                  cout<<"Lexical error : No digits after floating point "<<temp_num<<" Line Number : "<<line_count<<endl;
               }
               else
               {
                  if(!invalid_float) 
                  {
                     if(c=='N') //N signifies if the floating point number is negative
                     {
                        temp_num.push_back(c); //N needs to be pushed back into the current lexeme string as it is part of the floating point number
                        printer(temp_num,poss_token_num,line_count);
                        c =fgetc(fp);
                     }
                     else
                     {
                        printer(temp_num,poss_token_num,line_count); //it is a positive floating point number
                     }
                  } 
               }   
            }
            else if(c=='N') //M signifies negative integer here
            {
               temp_num.push_back(c); 
               c = fgetc(fp);
               printer(temp_num,104,line_count);
            }
            else
            {
               printer(temp_num,104,line_count); //Lexeme recognized as positive integer
            }
            
         
      }
      //Entry condition for a string literal
      else if(c=='\"')
      {
         string temp_lit="";
         c=fgetc(fp);
         while(c!='\"')
         {
            //if we hit the end of file and the closing quotation mark isn't found, display a lexical error
            if(feof(fp))
            {
               cout<<"Lexical error : String not terminated "<<temp_lit<<" Line Number : "<<line_count<<endl;
               break;
            }
            if(c=='\n') //increment line count when encoutnering newline character
            {
               line_count++;
            }
            temp_lit.push_back(c); //continue to add the characters encountered into the string literal
            c=fgetc(fp);
            
         }
         if(c=='\"') //closing quotation mark found
         {
            printer(temp_lit,106,line_count); //identify the lexeme as string literal
            c=fgetc(fp);
         }
      }
      //Entry condition for string or char literal
      else if(c=='\'')
      {
         string temp_lit="";
         c=fgetc(fp);
         while(c!='\'')
         {
            //Closing quotation mark not found, display a lexical error
            if(feof(fp))
            {
               cout<<"Lexical error : String not terminated "<<temp_lit<<" Line Number : "<<line_count<<endl;
               break;
            }
            if(c=='\n') //Increment linecount if a newline character is encountered
            {
               line_count++;
            }
            temp_lit.push_back(c); //Continue to push back the characters into string literal
            c=fgetc(fp);
            
         }
         if(c=='\'') //closing quotation mark encountered
         {
            if(temp_lit.length()==1) //character is of length 1
            {
               printer(temp_lit,107,line_count); // Recognize lexeme as character
            }
            else
            {
               printer(temp_lit,106,line_count);  // Recognize lexeme as string
            }
            c=fgetc(fp);
         }
      }
      //Entry condition for comments
      else if(c=='#')
      {
         
         c=fgetc(fp);
         while(c!='#')
         {
            //If comments are not closed, display a lexical error
            if(feof(fp))
            {
               cout<<"Lexical error : Comments not terminated with # Line Number : "<<line_count<<endl;
               break;
            }
            //Increment linecount if newline is encountered
            if(c=='\n')
            {
               line_count++;
            }
            c=fgetc(fp);
         }
         if(c=='#') //Closing of comments
         {
            c=fgetc(fp);
         }
      }
      //Entry condition for all other valid special characters/operators
      else
      {
         //Entry condition for < or <=
         if(c=='<')
         {
            c=fgetc(fp);
            if(c=='=') 
            {
               c=fgetc(fp);
               printer("<=",108,line_count); //Recognize lexeme as <=
            }
            else
            {
               printer("<",109,line_count); //Recognize lexeme as <
            }
         }  
         //Entry condition for > or >=
         else if(c=='>')
         {  
            c=fgetc(fp);
            if(c=='=') //Recognize lexeme as >=
            {
               c=fgetc(fp);
               printer(">=",110,line_count);
            }
            else
            {
               printer(">",111,line_count); //Recognize lexeme as >
            }  
         }
         //Entry condition for = or == 
         else if(c=='=')
         {
            c=fgetc(fp);
            if(c=='=') 
            {
               c=fgetc(fp);
               printer("==",112,line_count); //recognize lexeme as ==
            }
            else
            {
               printer("=",113,line_count); //Recognize lexeme as =
            }
         }
         //Entry condition for ! or !=
         else if(c=='!')
         {
            c=fgetc(fp);
            if(c=='=') 
            {
               c=fgetc(fp);
               printer("!=",114,line_count); //Recognize lexeme as !=
            }
            else
            {
               printer("!",115,line_count); //Recognize lexeme as !
            }
         }
         //Entry condition for & or &&
         else if(c=='&')
         {
            c=fgetc(fp);
            if(c=='&')
            {
               c=fgetc(fp);
               printer("&&",116,line_count); //Recognize lexeme as &&
            }
            else
            {
               printer("&",117,line_count); //Recognize lexeme as &
            }
         }
         //Entry condition for | or ||
         else if(c=='|')
         {
            c=fgetc(fp);
            if(c=='|')
            {
               c=fgetc(fp);
               printer("||",118,line_count); //Recognize lexeme as ||
            }
            else
            {
               printer("|",119,line_count); // Recognize lexeme as |
            }
         }
         //Entry condiiton for ^
         else if(c=='^')
         {
            printer("^",120,line_count); //Recognize lexeme as ^
            c = fgetc(fp);
         }
         //Entry condition for (
         else if(c=='(')
         {
            printer("(",121,line_count); //Recognize lexeme as (
            c = fgetc(fp);
         }
         //Entry condition for {
         else if(c=='{')
         {
            printer("{",122,line_count);//Recognize lexeme as {
            c = fgetc(fp);
         }
         //Entry condition for )
         else if(c==')')
         {
            printer(")",123,line_count); //Recognize lexeme as )
            c = fgetc(fp);
         }
         //Entry condition for }
         else if(c=='}')
         {
            printer("}",124,line_count); //Recognize lexeme as }
            c = fgetc(fp);
         }
         //Entry condition for ;
         else if(c==';')
         {
            printer(";",125,line_count); //Recognize lexeme as ;
            c = fgetc(fp);
         }
         //Entry condition for + or ++
         else if(c=='+')
         {
            c=fgetc(fp);
            if(c=='+')
            {
               c=fgetc(fp);
               printer("++",126,line_count); //Recognize lexeme as ++
            }
            else
            {
               printer("+",127,line_count); //Recognize lexeme as +
            }
         }
         //Entry condition for - or --
         else if(c=='-')
         {
            c=fgetc(fp);
            if(c=='-')
            {
               c=fgetc(fp);
               printer("--",128,line_count); //Recognize lexeme as --
            }
            else
            {
               printer("-",129,line_count); //Recognize lexeme as -
            }
         }
         //Entry condition for * operator
         else if(c=='*')
         {
            printer("*",130,line_count); //Recognize lexeme as *
            c = fgetc(fp);
         }
         //Entry condition for / operator
         else if(c=='/')
         {
            printer("/",131,line_count); //Recognize lexeme as /
            c = fgetc(fp);
         }
         //Entry condition for % operator
         else if(c=='%')
         {
            printer("%",132,line_count); //Recognize lexeme as %
            c = fgetc(fp);
         }
         //Entry condition as [
         else if(c=='[')
         {
            printer("[",133,line_count); //Recognize lexeme as [
            c = fgetc(fp);
         }
         //Entry condition as ]
         else if(c==']')
         {
            printer("]",134,line_count); //Recognize lexeme as ]
            c = fgetc(fp);
         }
         //Entry condition for ,
         else if(c==',')
         {
            printer(",",135,line_count); //Recognize lexeme as ,
            c = fgetc(fp);
         }
         //Entry condition for newline, increment newline
         else if(c=='\n')
         {
            line_count++;
            c = fgetc(fp);
         }
         //Entry condition for spaces, newlines or tabs
         else if(c==' ' or c=='\t' or c=='\r')
         {
            c=fgetc(fp);
         }
         //Alphabet is not recognized as valid
         else
         {
            cout<<"Lexical error : Invalid Character "<<c<< "Line Number :"<<line_count<<endl;
            c = fgetc(fp);
         }

      }
      //break if we encounter end of file
      if(feof(fp)) 
      {
         break;
      }
      
   }
   fclose(fp);
   
}