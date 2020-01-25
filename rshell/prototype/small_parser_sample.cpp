#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using  namespace std;

void singleDelimiter(){

    string input = "aaa,bbb,  ccc";
    istringstream ss(input);
    string token;

    while (getline(ss, token, ',')) {
        cout << token << '\n';
    }

}

void multipleDelimiters(const string & inputString){
   // string inputString = "aaa, ;bbb, 'ccc";
    vector<string> wordVector;

    stringstream stringStream(inputString);
    string line;
    while(getline(stringStream, line))
    {
        size_t prev = 0, pos;
        while ((pos = line.find_first_of(" '", prev)) != string::npos)
        {
            if (pos > prev)
                wordVector.push_back(line.substr(prev, pos-prev));
            prev = pos+1;
        }
        if (prev < line.length())
            wordVector.push_back(line.substr(prev, string::npos));
    }

    for_each(wordVector.begin(), wordVector.end(), []( const string &its){cout << its <<endl;} );
}


void parser(){
    string strInput;
    //cout << "$";
    //getline(cin, strInput);

    strInput = "echo one && echo two; ";
    multipleDelimiters(strInput);
}

int main() {

    //singleDelimiter();

    //multipleDelimiters();

     parser();

    return 0;
}
