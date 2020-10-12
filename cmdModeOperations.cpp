#include "header.h"

/**********************************************/
/* copies file from frm to to */
/**********************************************/
void fcopy_move(string frm, string to){

	std::ifstream  src(frm, std::ios::binary);
    std::ofstream  dst(to, std::ios::binary);

    dst << src.rdbuf();

	struct stat st;
	stat(&frm[0], &st);
	chown(&to[0], st.st_uid, st.st_gid);
	chmod(&to[0], st.st_mode);

	if(isMove) remove(&frm[0]);
}

/**********************************************/
/* recursively copies directory from frm to to */
/**********************************************/
void dcopy_move(string frm, string to){
	
	DIR* dr = opendir(&frm[0]);
	struct dirent* de;
	
	while((de = readdir(dr))){
		
		if(strcmp(de->d_name, ".")==0 || strcmp(de->d_name, "..")==0 ) continue;

		string new_frm = frm + "/" + de->d_name;
		string new_to = to + "/" + de->d_name;
		
		if(is_dir(&new_frm[0])){
			if (mkdir(&new_to[0], 0755) != 0) {
				write(STDOUT_FILENO, ":Directory Exists.\n", 19);
				return;
			}
			dcopy_move(new_frm, new_to);
			if(isMove) rmdir(&frm[0]);
		}
		else{
			fcopy_move(new_frm, new_to);
		}
	}
	closedir(dr);
}

/**********************************************/
/* For Copy/Move command */
/**********************************************/
void copy_move_cmd(vector<string> cmd_tokens){
	if(cmd_tokens.size()<3){
		write(STDOUT_FILENO, ":Too few arguments.\n", 20);
	}
	string des_dir = cmd_tokens[cmd_tokens.size()-1];
	des_dir = getAbsPath(des_dir);
	char* des = &des_dir[0];

	if(!hasPermission(des, X_OK)) return;

	if (is_dir(des)) {
        int tokenCount = cmd_tokens.size();
		for(int i=1; i < tokenCount-1; i++){
			string frm = getAbsPath(cmd_tokens[i]);
			int lst = frm.find_last_of("/");
			string to = des_dir + "/" + frm.substr(lst + 1, frm.length() - lst);

			if (is_dir(&frm[0])) {
				if (mkdir(&to[0], 0755) != 0) {
					write(STDOUT_FILENO, ":Directory Exists.\n", 19);
					return;
				}
				dcopy_move(frm, to);
				if(isMove) rmdir(&frm[0]);
			}
			else {
				fcopy_move(frm, to);
			}
		}
	}		 
	else {
		write(STDOUT_FILENO, ":Destination not found.\n", 24);
	}
}

void rename(vector<string> cmd_tokens){
	if(cmd_tokens.size()!=3){
		write(STDOUT_FILENO, ":Invalid Argument Count.\n", 25);
		return;
	}
	string old_path = getAbsPath(cmd_tokens[1]);
	string new_path = getAbsPath(cmd_tokens[2]);

	if(rename(&old_path[0], &new_path[0]) == -1) 
		write(STDOUT_FILENO, ":Error\n", 7);
}

void create_file(string fname, string des){
	string path = getAbsPath(des);
	path = path + "/" + fname;

	FILE *p = fopen(&path[0], "rb+");
	if(p == NULL){
		string dir_path = path.substr(0, path.find_last_of("/"));
		if(!hasPermission(&dir_path[0], X_OK)) return;

		p = fopen(&path[0], "wb");
	}
	else{
		write(STDOUT_FILENO, ":File Exists\n", 13);
	}

	fclose(p);
}

void create_dir(string fname, string des){
	string path = getAbsPath(des);
	path = path + "/" + fname;

	if (mkdir(&path[0], 0755) != 0) {
		write(STDOUT_FILENO, ":Directory Exists.\n", 14);
		return;
	}
}

void delete_file(string path){
	string dir_path = path.substr(0, path.find_last_of("/"));

	if(!hasPermission(&dir_path[0], X_OK)) return;

	remove(&path[0]);
}

/*****************************************************************/
/* Recursively deletes inner directories/folders and then outer */
/*****************************************************************/
void delete_dir(string path){
	if(!hasPermission(&path[0], X_OK)) return;
	
	if(!is_dir(&path[0])){
		write(STDOUT_FILENO, ":Directory not found.\n", 22);
		return;
	}

	DIR* dr = opendir(&path[0]);
	struct dirent* de;
	
	while((de = readdir(dr))){
		
		if(strcmp(de->d_name, ".")==0 || strcmp(de->d_name, "..")==0 ) continue;

		string new_frm = path + "/" + de->d_name;
		
		if(is_dir(&new_frm[0])){
			delete_dir(new_frm);
		}
		else{
			delete_file(&new_frm[0]);
		}
	}
	rmdir(&path[0]);
	closedir(dr);
}

void goto_(string path){
	if(is_dir(&path[0])){
		if(!hasPermission(&path[0], X_OK)) return;
		isGoto = true;
		
		back_st.push(string(cwd));
		while(!for_st.empty()) for_st.pop();

		enableCanon();
		strcpy(cwd, &path[0]);
		setListing(&path[0]);
	}
	else{
		write(STDOUT_FILENO, ":Invalid Path\n", 14);
	}
}

void search(string path, string target){

	DIR* dr = opendir(&path[0]);
	struct dirent* de;
	
	if(access(&path[0], X_OK)==-1) return;

	while((de = readdir(dr))){
		
		if(strcmp(de->d_name, ".")==0 || strcmp(de->d_name, "..")==0 ) continue;
		
		if(strcmp(de->d_name, &target[0]) == 0){
			searchResult = true;
			return;
		}

		string new_path = path + "/" + de->d_name;

		if(is_dir(&new_path[0])){
			search(new_path, target);
		}
	}
	closedir(dr);
}