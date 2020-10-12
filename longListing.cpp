#include "header.h"

int uNameLength;
int grpNameLength;
int startptr;
int endptr;
int curptr;
/**********************************************/
/* for printing long listing of files/folders */
/**********************************************/
void printListing(int sptr, int eptr){
	
	for(int i=sptr; i<eptr; i++){
		for(int j=0; j<(int)curDirListing[i].size(); j++){
			if(j==3) cout << setw(11) << right << curDirListing[i][j] << " ";
			else cout << curDirListing[i][j] << " ";
		} 
		cout << "\r\n";
	}
	
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_dim);

	setCursor(to_string(term_dim.ws_row), "1");

	write(STDOUT_FILENO, ":Normal mode:", 13);
}

/************************************************/
/* for setting up long listing of files/folders */
/************************************************/
bool setListing(char* path){
    struct dirent *de;  
    struct stat st;
	curDirListing.clear();
	
	startptr = 0;
	curptr = 0;
	endptr = 0;
	strcpy(cwd, path);
	
    DIR *dr = opendir(path); 
	
	clrscreen
    
    if (dr == NULL)  
    { 
        write(STDOUT_FILENO, ":Directory couldn't be opened\n", 30);
        return false; 
    } 		
	int dirfd = open(path, O_DIRECTORY);

    while ((de = readdir(dr)) != NULL){
        if (fstatat(dirfd, de->d_name, &st, 0) == -1) {
            fprintf(stderr, "stat(\"%s\") failed (%d: %s)\n",
                    de->d_name, errno, strerror(errno));
        }
        else if(de->d_name[0]!='.' || strlen(de->d_name)==1 || 
				(strlen(de->d_name)==2 && de->d_name[1]=='.')){

			char* time = calctime(st);
			strtok(time, "\n");
			char* user_name = getUname(st.st_uid);
			uNameLength = strlen(user_name);
			char* grp_name = getGname(st.st_gid);
			grpNameLength = strlen(grp_name);
			string perms = getPermssions(st);
			
			char* fpath = (char *) malloc(pathLength);
			strcpy(fpath, path);
			strcat(fpath, "/");
			strcat(fpath, de->d_name);
			curPaths[endptr] = fpath;

			vector<string> curListValue;
			curListValue.push_back(perms);
			curListValue.push_back((string)user_name);
			curListValue.push_back((string)grp_name);
			curListValue.push_back(to_string(st.st_size));
			curListValue.push_back((string)time);
			curListValue.push_back((string)de->d_name);
			curDirListing.push_back(curListValue);
			endptr++;
		}
    }
	
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_dim);
	printListing(startptr, min(endptr, term_dim.ws_row-1));

    closedir(dr);    

	disableCanon();
    return true; 
}