#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>


#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <cstring>

#define _MY_DEBUG

using namespace std;

class CException {

    string strDesc;
public:
    CException(const char *p) {
        strDesc = p;
    }
};

void singleDelimiter() {

    string input = "aaa,bbb,  ccc";
    istringstream ss(input);
    string token;

    while (getline(ss, token, ',')) {
        cout << token << '\n';
    }

}

void multipleDelimiters(const string &inputString, vector<string> &wordVector) {
    // string inputString = "aaa, ;bbb, 'ccc";
    // vector<string> wordVector;

    stringstream stringStream(inputString);
    string line;
    while (getline(stringStream, line)) {
        size_t prev = 0, pos;
        while ((pos = line.find_first_of(" '", prev)) != string::npos) {
            if (pos > prev)
                wordVector.push_back(line.substr(prev, pos - prev));
            prev = pos + 1;
        }
        if (prev < line.length())
            wordVector.push_back(line.substr(prev, string::npos));
    }

    //for_each(wordVector.begin(), wordVector.end(), []( const string &its){cout << its <<endl;} );
}

void runcommand(char *argv[]) {
    pid_t cpid = 0;
    int ret = 0;
    int status = 0;
    int fd[2] = {0, 0};

    ret = pipe(fd);

    if (ret == -1) {
        perror("pipe error");
    }

    cpid = fork();

    if (cpid == -1) {
        //throw CException("Fork Error!");
        perror("fork error");
        exit(EXIT_FAILURE);
    } else if (cpid == 0) {  // Child process
        /*
#ifdef  _MY_DEBUG
        cout << "Child's parent's PID is " <<  (long) getpid() <<endl;
#endif
        ret = dup2(fd[1], STDOUT_FILENO);
        if (ret == -1) {
            perror("dup2 error");
        }

        ret = close(fd[1]);
        if (ret == -1) {
            perror("close error index one");
        }

        ret = close(fd[0]);
        if (ret == -1) {
            perror("close error index zero");
        }
*/
#ifdef  _MY_DEBUG
        cout << "execvp "  << argv[0] << "  "<< argv[1] << endl;
#endif

        ret = execvp(argv[0], argv);
        if ( ret == -1) {
            perror("execvp error");            
        }

        exit(EXIT_SUCCESS);
    }
    else{
#ifdef  _MY_DEBUG
        cout << "Parent PID is " << cpid <<endl;
#endif
        ret = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
        //ret = wait(&status);
        if (ret == -1) {
            perror("waitpid error");
        }

        if(WEXITSTATUS(status) == 0)
            exit(EXIT_SUCCESS);
        else
            exit(EXIT_FAILURE);
        
    }

}


void parser() {
    vector<string> wordVector;

    string strInput;
    cout << "$";
    getline(cin, strInput);

    //strInput = "echo one && echo two; ";
    //strInput = "echo one";
    multipleDelimiters(strInput, wordVector);

    int size = wordVector.size();
    if(size){
        char ** argv = nullptr;

        argv = new char * [size];

        for(int i=0;i<size;++i){

            string itStr = wordVector[i];
            argv[i] = new char [itStr.length()+1];
            strcpy(argv[i], itStr.c_str());
        }

        runcommand(argv);

        for(int i=0;i<size;++i){
            delete argv[i] ;
        }

        delete [] argv;
    }

}

void blp_1(){
        pid_t pid;
        char *message;
        int n;
        int exit_code;
        printf("fork program starting\n");
        pid = fork();
        switch(pid)
        {
            case -1:
                perror("fork failed");
                exit(1);
            case 0:
                message = "This is the child";
                n = 5;
                exit_code = 37;
                break;
            default:
                message = "This is the parent";
                n = 3;
                exit_code = 0;
                break;
        }
        for(; n > 0; n--) {
            puts(message);
            sleep(1);
        }

        if (pid != 0) {
            int stat_val;
            pid_t child_pid;
            child_pid = wait(&stat_val);
            printf("Child has finished: PID = %d\n", child_pid);
            if(WIFEXITED(stat_val))
                printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
            else
            printf("Child terminated abnormally\n");
        }
        exit(exit_code);

}

void ppArrTest(){

    char ** ppArgv = new char * [4];


    for(int i=0;i<4;++i){
        stringstream ss;
        ss << i;

        ppArgv[i] = new char[4];
        strcpy(ppArgv[i], ss.str().c_str());
    }


    cout << "deleting.."<<endl;

    for(int i=0;i<4;++i){

        delete ppArgv[i];
    }

    delete [] ppArgv;
}

int main() {

    //singleDelimiter();

    //multipleDelimiters();

    parser();

    //blp_1();


    return 0;
}
