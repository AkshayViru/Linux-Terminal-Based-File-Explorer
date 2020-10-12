#include "header.h"

/**********************************************/
/* Handles window size change */
/**********************************************/
void winsz_handler(int sig) {
    
	if(isNormalMode){
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_dim);

		clrscreen
		printListing(0, min(endptr, term_dim.ws_row-1+startptr));

		startptr = 0;
		curptr = 0;
		setCursor("1", to_string(37+uname_len+gname_len));
	}
}

/**********************************************/
/* Sets cursor position to [row, col] */
/**********************************************/
void setCursor(string row, string col){
	stringstream ss;
	string tmpStr;
	char* curpos;
	ss << "\033[" << row << ";" << col <<"H";
	tmpStr = ss.str();
	curpos = &tmpStr[0];

	write(STDOUT_FILENO, curpos, 8);
}

/*******************************************************/
/* Checks for file/directory permission and prints error */
/*******************************************************/
bool hasPermission(char* path, int mode){
	if(access(path, mode)==-1){

		write(STDOUT_FILENO, ":Permission denied\n", 19);

		return false;
	}
	return true;
}

/************************************/
/* Get absolute path from relative */
/************************************/
string getAbsPath(string str){
	string res = "";
    if (str[0] == '~') res = string(pwd) + str.substr(1, str.length());
    else if (str[0] == '/') res = string(pwd) + str;
	else if (str[0] == '.' && str[1] == '.'){
		res = string(cwd).substr(0, string(cwd).find_last_of("/"))+str.substr(2, str.length());
		if(strcmp(cwd, pwd)==0) return "";
	} 
    else if (str[0] == '.' && str[1] == '/') res = string(cwd) + str.substr(1, str.length());
    else res = string(cwd) + "/" + str;
    return res;
}

/*********************************************************/
/* Returns true if directory exists at the given path */
/*********************************************************/
bool is_dir(char* path){
	struct stat s;
	if( stat(path,&s) == 0 && S_ISDIR(s.st_mode)) return true;
	else return false;
}







//for calculating modification time of files/folders
char* calctime(struct stat st){
	time_t rawtime = st.st_mtime;
	struct tm * timeinfo;

	timeinfo = localtime ( &rawtime );
	
    static const char mon_name[][4] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	static char result[26];
	sprintf(result, "%.3s%3d %.2d:%.2d\n",
		mon_name[timeinfo->tm_mon],
		timeinfo->tm_mday, timeinfo->tm_hour,
		timeinfo->tm_min);

  	return result;
}

//for getting groupname to be displayed during long listing 
char* getGname(gid_t st_gid){
	struct group* gp;
	gp = getgrgid(st_gid);

	gname_len = max((unsigned long)gname_len, strlen(gp->gr_name)+1);

	return gp->gr_name;
}

//for getting username to be displayed during long listing 
char* getUname(uid_t st_uid){
	struct passwd *pw;
	pw = getpwuid(st_uid);

	uname_len = max((unsigned long)uname_len ,strlen(pw->pw_name)+1);

	return pw->pw_name;
}

//for getting file/folder permissions to be displayed during long listing 
string getPermssions(struct stat st){
	string res;
	res += (S_ISDIR(st.st_mode)) ? "d" : "-";
    res += (st.st_mode & S_IRUSR) ? "r" : "-";
    res += (st.st_mode & S_IWUSR) ? "w" : "-";
    res += (st.st_mode & S_IXUSR) ? "x" : "-";
    res += (st.st_mode & S_IRGRP) ? "r" : "-";
    res += (st.st_mode & S_IWGRP) ? "w" : "-";
    res += (st.st_mode & S_IXGRP) ? "x" : "-";
    res += (st.st_mode & S_IROTH) ? "r" : "-";
    res += (st.st_mode & S_IWOTH) ? "w" : "-";
    res += (st.st_mode & S_IXOTH) ? "x" : "-";

	return res;
}
