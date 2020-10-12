#include "header.h"

bool searchResult;

/**********************************************/
/* For parsing the entered line in CMD mode */
/**********************************************/
void getCMD(string cmd_str){
	stringstream ss(cmd_str);
	vector<string> cmd_tokens;
	string tmp;
	while(ss >> tmp) cmd_tokens.push_back(tmp);

	if(cmd_tokens[0] == "copy"){
		isMove = false;
		copy_move_cmd(cmd_tokens);
	}
	else if(cmd_tokens[0] == "move"){
		isMove = true;
		copy_move_cmd(cmd_tokens);
	}
	else if(cmd_tokens[0] == "rename"){
		rename(cmd_tokens);
	}
	else if(cmd_tokens[0] == "create_file"){
		if(cmd_tokens.size()!=3){
			write(STDOUT_FILENO, ":Invalid Argument Count.\n", 25);
		}
		else{
			create_file(cmd_tokens[1], cmd_tokens[2]);
		}
	}
	else if(cmd_tokens[0] == "create_dir"){
		if(cmd_tokens.size()!=3){
			write(STDOUT_FILENO, ":Invalid Argument Count.\n", 25);
		}
		else{
			
			create_dir(cmd_tokens[1], cmd_tokens[2]);
		}
	}
	else if(cmd_tokens[0] == "delete_file"){
		string path = getAbsPath(cmd_tokens[1]);
		if(cmd_tokens.size()!=2){
			write(STDOUT_FILENO, ":Invalid Argument Count.\n", 25);
		}
		else{
			delete_file(path);
		}
	}
	else if(cmd_tokens[0] == "delete_dir"){
		string path = getAbsPath(cmd_tokens[1]);
		if(cmd_tokens.size() != 2){
			write(STDOUT_FILENO, ":Invalid Argument Count.\n", 25);
		}
		else{
			delete_dir(path);
		}
	}
	else if(cmd_tokens[0] == "goto"){
		if(cmd_tokens.size() != 2){
			write(STDOUT_FILENO, ":Invalid Argument Count.\n", 25);
		}
		else{
			string path = getAbsPath(cmd_tokens[1]);
			goto_(path);			
		}
	}
	else if(cmd_tokens[0] == "search"){
		if(cmd_tokens.size() != 2){
			write(STDOUT_FILENO, ":Invalid Argument Count.\n", 25);
		}
		else{
			search(cwd, cmd_tokens[1]);	
			if(searchResult){
				write(STDOUT_FILENO, ":True\n", 6);
				searchResult = false;
			}	
			else{
				write(STDOUT_FILENO, ":False\n", 7);
			}	
		}
	}
	else{
		write(STDOUT_FILENO, ":Invalid Command\n", 17);
	}
}