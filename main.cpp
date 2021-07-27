#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

struct termios orig_termios;
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  //raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

using namespace std;
char oldmovekey = 'w';
bool gameOver = false;
const int height = 25;
const int width = 25;
int snakePos[2] = {height/2,width/2};
int fruitPos[2] = {4,14};
int tailX[100];
int tailY[100];

int score = 0;

string multiplyText(int x,string y){
	string tmp;
	for(int i=0;i < x;i++){
		tmp.append(y);
	}	
	return tmp;
}
void renderMap(){
	system("clear");
	for(int i=0;i<height;i++){
		for(int j=0;j < width;j++){
			if(i == 0||i == height -1){
				cout << "#";
			}
			else{
				if(j == 0 || j == width -1){
					cout << "#";
					continue;
				}
				else{
					if(snakePos[0] == i && snakePos[1] == j){
						cout << "S";
					}
					else{
						bool istail = false;
						for(int l=0;l < 100;l++){
							if(tailX[l] == j && tailY[l] == i){
								cout << "s";
								istail = true;
							}
						}
						if(istail == false){
							if(fruitPos[0] == i && fruitPos[1] == j){
								cout << "F";
							}
							else{
								cout << " ";
							}
						}
					}
				}

			}
		}
		cout << "\n";
		//cout << snakePos[0] << "-" << snakePos[1] << endl;
	}
	system("sleep 0.1");
}
void snakeControl(){
	char c;
	read(STDIN_FILENO,&c, 1);
	if(c != '\0'){
		oldmovekey = c;
	}
      	//printf("%d ('%c')\r\n", c, c);
	switch(oldmovekey){
		case 'w':
			snakePos[0]--;
			break;
		case 's':
			snakePos[0]++;
			break;
		case 'a':
			snakePos[1]--;
			break;
		case 'd':
			snakePos[1]++;
			break;
		case 'q':
			gameOver = true;
	}
}
void gameLogic(){
	int oldxy[2];
	//rand() % 10 + 1
	if(snakePos[0] < 1 || snakePos[0] >= height -1){
		gameOver = true;
	}
	if(snakePos[1] < 1 || snakePos[1] >= width -1){
		gameOver = true;
	}
	if(snakePos[0] == fruitPos[0] && snakePos[1] == fruitPos[1]){
		score++;
		bool validSpawn = false;
		bool isValid = false;
		while(isValid == false){
			bool tempvalid = true;
			fruitPos[0] = rand() % (height -1) + 1;
			fruitPos[1] = rand() % (width -1) + 1;
			if(fruitPos[0] == snakePos[0] && fruitPos[1] == snakePos[1]){
				tempvalid = false;
			}

			for(int i=0;i < 10;i++){
				if(tailY[i] == fruitPos[0] && tailX[i] == fruitPos[1]){
					tempvalid = false;
				}
			}
			if(tempvalid == true){
				isValid = true;
			}
			else{
				fruitPos[0] = rand() % (height -1) + 1;
				fruitPos[1] = rand() % (width -1) + 1;
			}
		}

	}
	for(int i=0;i < 100;i++){
		if(snakePos[0] == tailY[i] && snakePos[1] == tailX[i]){
			gameOver = true;
		}
	}
	for(int i=0;i < score;i++){
		if(i == 0){
			oldxy[1] = tailX[i];
			oldxy[0] = tailY[i];
			tailX[i] = snakePos[1];
			tailY[i] = snakePos[0];
		}
		else{
			int tmpxy[2] = {tailY[i],tailX[i]};
			tailX[i] = oldxy[1];
			tailY[i] = oldxy[0];
			oldxy[0] = tmpxy[0];
			oldxy[1] = tmpxy[1];
		}
	}

}
int main(){

	enableRawMode();
	while(gameOver == false){
		renderMap();
		gameLogic();
		//cout << "BOX";
		snakeControl();
	}
}
