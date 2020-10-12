#include "header.h"

void normalMode(){	

	setCursor("1", to_string(37+uname_len+gname_len));
	
	while(true){
		signal(SIGWINCH, winsz_handler);
		
		char inp[3];
		read(STDIN_FILENO, &inp, 3);
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_dim);
		
		if(inp[0] == 27 && inp[2] == 65){
			//UP
			if(curptr>0 && curptr%(term_dim.ws_row-1) != 0){
				write(STDOUT_FILENO, "\033[1A", 4);
				/****************************************************************/
				/*this if condition facilitates up scrolling without using 'k' key*/
				/* remove the second condition in the previous 'if' */
				/****************************************************************/
				/*if(curptr == startptr){
					startptr--;
					clrscreen
					printListing(startptr, min(endptr, term_dim.ws_row-1+startptr));

					setCursor("1", to_string(37+uname_len+gname_len));
				}*/
				curptr--;
			}
		}
		else if(inp[0] == 27 && inp[2] == 66){
			//DOWN
			if(curptr<endptr-1 && (curptr+1)%(term_dim.ws_row-1) != 0){
				/****************************************************************/
				/* this if condition facilitates scrolling without using 'l' key */
				/* remove the second condition in the previous 'if' */
				/****************************************************************/
				/*if(curptr-startptr>=term_dim.ws_row-2){
					startptr++;
					clrscreen
					printListing(startptr, min(endptr, term_dim.ws_row-1+startptr));

					setCursor(to_string(term_dim.ws_row-2), to_string(37+uname_len+gname_len));
				}*/
				write(STDOUT_FILENO, "\033[1B", 4);
				curptr++;
			}		
		}		
		else if(inp[0] == 27 && inp[2] == 67){
			//RIGHT
			if(!for_st.empty()){
				string for_dir = for_st.top();
				for_st.pop();

				back_st.push(string(cwd));
				
				strcpy(cwd, &for_dir[0]);
				enableCanon();
				setListing(&for_dir[0]);
				break;
			}
		}
		else if(inp[0] == 27 && inp[2] == 68){
			//LEFT
			if(!back_st.empty()){
				string back_dir = back_st.top();
				back_st.pop();

				for_st.push(string(cwd));

				strcpy(cwd, &back_dir[0]);
				enableCanon();
				setListing(&back_dir[0]);
				break;
			}
		}
		else if(inp[0] == 'l' || inp[0] == 'L'){
			if(startptr + term_dim.ws_row-1 < endptr){
				startptr = startptr+term_dim.ws_row-1;
				curptr = startptr;
				clrscreen
				printListing(startptr, min(endptr, term_dim.ws_row-1+startptr));

				setCursor("1", to_string(37+uname_len+gname_len));
			}
		}
		else if(inp[0] == 'k' || inp[0] == 'K'){
			if(startptr > 1){
				startptr = startptr - (term_dim.ws_row-1);
				curptr = startptr;
				clrscreen
				printListing(startptr, min(endptr, term_dim.ws_row-1+startptr));

				setCursor("1", to_string(37+uname_len+gname_len));
			}
		}
		else if(inp[0] == 13){
			//Enter
			bool isdir = (curDirListing[curptr][0][0] == 'd')? true : false;
			if(isdir){
				if(access(&curPaths[curptr][0], X_OK)==-1){
					ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_dim);

					setCursor(to_string(curptr-startptr+1), to_string(term_dim.ws_col-20));
					write(STDOUT_FILENO, "Permission denied\n", 19);

					setCursor(to_string(curptr-startptr+1), to_string(37+uname_len+gname_len));

					continue;
				}
				//push to backward stack only if '.' is not selected
				if(curPaths[curptr][strlen(curPaths[curptr])-1] != '.'
					|| curPaths[curptr][strlen(curPaths[curptr])-2] != '/') back_st.push(string(cwd));

				//ignore '..' if current directory = root directory
				if(curPaths[curptr][strlen(curPaths[curptr])-1] == '.'
					&& curPaths[curptr][strlen(curPaths[curptr])-2] == '.') {
						if(strcmp(cwd,pwd)==0){
							continue;
						}
				}

				while(!for_st.empty()) for_st.pop();

				strcpy(cwd, curPaths[curptr]);
				enableCanon();
				setListing(curPaths[curptr]);
				break;
			}
			else{
				if(fork()==0){
					execlp("/usr/bin/vi","vi",curPaths[curptr],NULL);
				} 
				else{
					wait(NULL);
				}
			}
		}
		else if(inp[0]=='h'|| inp[0] == 'H'){
			while(!for_st.empty()) for_st.pop();
			while(!back_st.empty()) back_st.pop();

			enableCanon();
			strcpy(cwd, pwd);
			setListing(pwd);
			break;
		}
		else if(inp[0]=='q' || inp[0] == 'Q'){
			cout << "\033[2J\033[1;1H";
			fflush(0);
			exit(0);
		}
		else if(inp[0] == 127){
			//backspace
			if(strcmp(cwd, pwd)!=0){
				string curdir = cwd;
				int last = curdir.find_last_of("/");
				curdir = curdir.substr(0, last);
				char* tmp = &curdir[0];

				back_st.push(string(cwd));
				while(!for_st.empty()) for_st.pop();

				enableCanon();
				strcpy(cwd, tmp);
				setListing(tmp);
				break;
			}
		}
		else if(inp[0] == 58){
			//cmd mode switch
			isNormalMode = false;
			cmdMode();
			break;
		}
	}
}