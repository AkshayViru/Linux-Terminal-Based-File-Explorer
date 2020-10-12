/************************************************************************
**      This File is for includeing all header file, extern            **    
**      variables and global function required by other cpp files.     **
**                                                                     **
************************************************************************/

#include <iostream>
#include <iomanip>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <termios.h>
#include <time.h>
#include <sys/ioctl.h>
#include <pwd.h>
#include <vector>
#include <stack>
#include <signal.h>
#include <fstream>
#include <grp.h>
#include <iterator>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

#define pathLength 128
#define clearRow write(STDOUT_FILENO, "\33[2K\r", 5);
#define clrscreen cout << "\033c";

extern char* curPaths[200];
extern vector<vector<string>> curDirListing;
extern int uNameLength;
extern int grpNameLength;
extern bool small_window;
extern int startptr;
extern int endptr;
extern int curptr;
extern bool isNormalMode;
extern char tmpCwd2[pathLength];
extern stack<string> back_st;
extern stack<string> for_st;
extern char pwd[pathLength];
extern char cwd[pathLength];
extern struct termios orig_termios;
extern vector<string> testt;
extern bool isMove;
extern bool isGoto;
extern bool searchResult;
extern int uname_len;
extern int gname_len;
extern struct winsize term_dim;


void enableCanon();
void disableCanon();
void disableCanon();
void winsz_handler(int);
void setCursor(string, string);
bool hasPermission(char*, int);
string getAbsPath(string);
bool is_dir(char*);
void fcopy_move(string, string);
void dcopy_move(string, string);
void copy_move_cmd(vector<string>);
void rename(vector<string>);
void create_file(string, string);
void create_dir(string, string);
void delete_file(string);
void delete_dir(string);
void goto_(string);
void search(string, string);
void getCMD(string);
void cmdMode();
void normalMode();
char* calctime(struct stat);
char* getGname(gid_t);
char* getUname(uid_t);
string getPermssions(struct stat);
void printListing(int, int);
bool setListing(char*);



