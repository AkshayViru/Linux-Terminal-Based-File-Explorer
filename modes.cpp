#include "header.h"

struct termios orig_termios;

/**********************************************/
/* for enabling canonical mode */
/**********************************************/
void enableCanon() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/**********************************************/
/* for disabling canonical mode */
/**********************************************/
void disableCanon(){
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(enableCanon);
	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

	if(!isGoto){
		isNormalMode = true;
		normalMode();
	} 
	else{
		isGoto = false;

		ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_dim);
		
		setCursor(to_string(term_dim.ws_row), "1");
		write(STDOUT_FILENO, "\33[2K\r", 5);
		write(STDOUT_FILENO, ":Command Mode:\n", 15);
	}
}