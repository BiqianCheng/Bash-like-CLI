#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
int main(){
        char* const arg1 = strdup("cat");
        char* const arg2 = strdup("README.md");
        char* const arg[3] = {arg1,arg2,NULL};
        execvp(arg[0],arg);  //works
}
