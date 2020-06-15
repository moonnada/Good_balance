#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

void printFile(istream &dictfile);
bool balanceFile(istream &dictfile);
bool SingleQuote(istream &dictfile, string &str, int &i, int &k);
bool DoubleQuote(istream &dictfile, string &str, int &i, int &k);
bool openBlockComment(istream &dictfile, string &str, int &i, int &k);


int main()   //main function
{
    ifstream inFile;
    ofstream outFile;
    string fileName, line;
    cout << "Please enter filename for C++ code: ";
    cin >> fileName;
    inFile.open(fileName.c_str());
    
    if (!inFile) {
        cout << "File not found!" << endl;
        return (1);
    }
    
    printFile(inFile);
    
    if (balanceFile(inFile))
        cout << "balance ok" << endl;
    inFile.open(fileName);
    getline(inFile, line);
    outFile.open(fileName);
    outFile<<line<<endl;
    inFile.close();
    outFile.close();
    return 0;
}



void printFile(istream &dictfile){
    string str;
    do{
        getline(dictfile,str);
        cout<<str<<endl;
    }while(dictfile.peek()!=EOF);  //check each line
    dictfile.clear();
    dictfile.seekg(0, dictfile.beg);
}

bool SingleQuote(istream &dictfile, string &str, int &i, int &k){  //check single quote
    while (i<str.length()){     //check a length of the rest ot a line
        i++;
        if(str[i]=='\''){
            cout << "pair matching \' and \'" << endl;
            return true;
        }
    }
    cout << "unbalanced ' on line " << k << endl;
    return false;
}

bool DoubleQuote(istream &dictfile, string &str, int &i, int &k){ //check double quote
    while (i<str.length()){   //check a length of the rest ot a line
        i++;
        if(str[i]=='\"'){
            cout << "pair matching \" and \"" << endl;
            return true;
        }
    }
    cout << "unbalanced \" on line " << k << endl;
    return false;
}

bool openBlockComment(istream &dictfile, string &str, int &i, int &k){
    while (i<str.length()){   //check a length of the rest ot a line
        if(str[i]=='*' && str[i+1]=='/'){
            cout << "pair matching /* and */" << endl;
            return true;
        }
        i++;
    }
    int temp = k;
    while (dictfile.peek()!= EOF){   //check different lines
        k++;
        getline(dictfile, str);
        for(int i=0; i < str.length(); i++){
            if(str[i]=='*' && str[i+1]=='/'){
                cout << "pair matching /* and */" << endl;
                return true;
            }
        }
    }
    cout << "unbalanced /* on line " << temp << endl;
    return false;
}



bool balanceFile(istream &dictfile){
    string str;
    stack<char> a;
    stack<int> b;
    int k = 0;
    while (dictfile.peek()!= EOF){    //check each line
        k++;
        getline(dictfile, str);
        for(int i=0; i < str.length(); i++){     //check brackets on each line
            if(str[i]=='('|| str[i]=='{' || str[i]=='['){
                a.push(str[i]);
                b.push(k);
            }
            else if(str[i]==')' && a.top()=='('){   //check matching ( and )
                a.pop();
                b.pop();
                cout << "pair matching ( and )" << endl;
            }
            else if(str[i]==')' && a.top()!='('){
                cout << "unmatched ) symbol at line " << k << endl;
                return false;
            }
            else if(str[i]=='}' && a.top()=='{'){     //check matching { and }
                a.pop();
                b.pop();
                cout << "pair matching { and }" << endl;
            }
            else if(str[i]=='}' && a.top()!='{'){
                cout << "unmatched } symbol at line " << k << endl;
                return false;
            }
            else if(str[i]==']' && a.top()=='['){      //check matching [ and ]
                a.pop();
                b.pop();
                cout << "pair matching [ and ]." << endl;
            }
            else if(str[i]==']' && a.top()!='['){
                cout << "unmatched ] symbol at line " << k << endl;
                return false;
            }
            else if(str[i] == '/' && str[i+1] == '*'){   //check /*
                if(openBlockComment(dictfile, str, i, k)==false)
                    return false;
            }
            else if(str[i] == '\''){    //check single quote
                if(SingleQuote(dictfile, str, i, k)==false)
                    return false;
            }
            else if(str[i] == '"'){    //check double quote
                if(DoubleQuote(dictfile, str, i, k)==false)
                    return false;
            }
            else if(str[i] == '/' && str[i+1] == '/'){
                while(i<str.length())
                    i++;
            }
            else if(a.empty()){    //in case of char of stack is empty then check matching ),},]
                if(str[i]==')'){
                    cout << "unmatched ) symbol at line " << k << endl;
                    return false;
                }
                if(str[i]=='}'){
                    cout << "unmatched } symbol at line " << k << endl;
                    return false;
                }
                if(str[i]==']'){
                    cout << "unmatched ] symbol at line " << k << endl;
                    return false;
                }
            }
        }
    }
   
    while(!a.empty()){    //in case of char of stack is not empty then check matching or non matching open bracket
        if(a.top() == '('){
            a.pop();
            cout << "unmatched ( symbol at line " << b.top() << endl;
            b.pop();
            return false;
        }
        else if(a.top() == '{'){
            a.pop();
            cout << "unmatched { symbol at line " << b.top() << endl;
            b.pop();
            return false;
        }
        else if(a.top() == '['){
            a.pop();
            cout << "unmatched [ symbol at line " << b.top() << endl;
            b.pop();
            return false;
        }

        else{
            a.pop();
            cout << "There was an error with this file." << endl;
        }
    }
    return true;
}



