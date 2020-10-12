#include "header.h"

#define pathLength 128

char* curPaths[200];
vector<vector<string>> curDirListing;
bool isNormalMode;
char tmpCwd2[pathLength];
stack<string> back_st;
stack<string> for_st;
char pwd[pathLength];
char cwd[pathLength];
vector<string> testt;
bool isMove;
bool isGoto;
int uname_len;
int gname_len;
struct winsize term_dim;

int main(int argc, char **argv){

    getcwd(pwd, pathLength);
	strcpy(cwd, pwd);
	
	if (argc == 2) {
        strcpy(pwd, argv[1]);
    }

	if(!setListing(pwd)){
        cout << "Something went wrong.";
    }
}
