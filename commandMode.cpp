#include "header.h"

void cmdMode(){
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_dim);

	setCursor(to_string(term_dim.ws_row), "1");
	write(STDOUT_FILENO, "\33[2K\r", 5);
	write(STDOUT_FILENO, ":Command Mode:\n", 15);

	while(true){
		signal(SIGWINCH, winsz_handler);
		
		setCursor(to_string(term_dim.ws_row), "1");
		write(STDOUT_FILENO, "\33[2K\r", 5);
		write(STDOUT_FILENO, ":", 1);

		int cmdModCurPtr = 2;
		string cmd_str;
		char cptr[3];
		bool isEscaped = false;

		while(read(0, cptr, 3)){
			if(cptr[0]==13){
				write(STDOUT_FILENO, "\r\n", 2);

				ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_dim);
				setCursor(to_string(term_dim.ws_row), "1");
				
				if( cmdModCurPtr > 2) getCMD(cmd_str);
				break;
			}
			if(cptr[0]==27){
				if(cptr[1] == '[' && (cptr[2] == 'A' || cptr[2] == 'B'
					 || cptr[2] == 'C' || cptr[2] == 'D' )){
						 cptr[1] = 0;
						 cptr[2] = 0; 
						 break;
					 }
				enableCanon();
				isEscaped = true;
				setListing(cwd);
				break;
			}
			if(cptr[0]==127){
				//delete last input char
				if(cmdModCurPtr > 2){
					cmdModCurPtr--;

					setCursor(to_string(term_dim.ws_row), to_string(cmdModCurPtr));

					write(STDOUT_FILENO, "\033[0K", 4);
					cmd_str.pop_back();
				}
			}
			else{
				//take command input
				ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_dim);

				setCursor(to_string(term_dim.ws_row), to_string(cmdModCurPtr));

				write(STDOUT_FILENO, cptr, 1);

				cmdModCurPtr++;
				cmd_str += cptr[0];
			}
			
		}
		if(isEscaped){
			break;
		} 
	}
}