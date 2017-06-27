#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <GL/glut.h>

struct dirent *dirst;
DIR *dp;

char filename[256];
FILE *fp;

//描画系
int x[256][256];
int y[256][256];
int i=0,j=0;
int x_len[256];
int len_lim=0;
int check=0,check2=0,check3=0;
int add_dot_support=0;
int icon=0,icon_x[]={0,0,8,16,24,0,8,16,24},icon_y[]={0,0,0,0,0,-20,-20,-20,-20};
int move=0,move_s=0,flip=1;
int move2_x=0,move2_y=0;

//画面全体系
int mode=0;
int red=1,red_2=-1;
int time_c=750;
int zzz=0;

//アニメーション用
int move_x[] = {0,-2,-2,0,2,0,-2,-2,2,-2,-2,2,0,0,0,2,2,2,2,2,0,-2,0,2,0,2,2,-2,2,-2,0,-2,-2,-2};
int move_flip[] = {1,1,1,-1,-1,1,1,1,-1,1,1,1,-1,1,-1,-1,-1,-1,-1,-1,1,1,-1,-1,1,-1,-1,-1,-1,-1,1,1,1,1};
int move_pause[] = {0,1,0,0,1,1,0,1,1,1,3,1,1,1,1,0,1,0,1,0,0,1,1,0,0,1,3,1,3,1,1,0,1,0};
int niku_point[]={8,8,0,0,0,0,0},eat_motion[]={0,0,3,2,3,2,2},food_motion[]={10,10,10,12,12,14,14};
int tre_time_c[]={1000,1000,1000,1000,1000,1000,500,500,100,100,100,100,100,100,1000},
tre_check[]={30,29,28,27,26,25,3,3,34,36,34,36,34,36,3},
tre_check2[]={24,24,24,24,24,24,31,31,35,37,35,37,35,37,32},
tre_check3[]={0,0,0,0,0,0,32,32,0,0,0,0,0,0,0},
tre_move[]={0,0,0,0,0,0,16,16,0,0,0,0,0,0,16},
tre_move2_x[]={0,0,0,0,0,0,0,-8,0,0,0,0,0,0,0};
int result_check[]={7,8,7,8,7,8,7,8,7,2,6,2,6,2,6,2,6,2};

void timer(int value){
	if(red > 0){
		if(move_s == 33) move_s = 0;
		move_s += 1;
		move += move_x[move_s];
		flip = move_flip[move_s];
		check = move_pause[move_s];
		glutPostRedisplay();
		glutTimerFunc(600, timer, 0);
	}
}

void timer_2(int value){
	if(red_2 > 0){
		if(mode > 0){
			switch (icon){
				case 2:
					time_c = 600;
					if(move_s == 6){
						mode = 1;
						check = 9;
						check2 = 16;
						len_lim = x_len[check2];
						red_2 = -1;
						move_s = 0;
						if(add_dot_support > 0) move2_y = -8;
					}
					else{
						move_s += 1;
						move2_y = niku_point[move_s];
						check = eat_motion[move_s];
						check2 = food_motion[move_s]+add_dot_support;
						len_lim = x_len[check2];
						if((add_dot_support > 0)&&(move_s >= 5)) len_lim = 0;
					}
				break;
				case 3:
					move_s++;
					if(move_s < 15){
						time_c = tre_time_c[move_s];
						check = tre_check[move_s];
						check2 = tre_check2[move_s];
						check3 = tre_check3[move_s];
						move = tre_move[move_s];
						move2_x = tre_move2_x[move_s];
						if((move_s == 14) && (add_dot_support > 0)) check2 = 33;
						if(move_s > 5) len_lim = x_len[check2];
						if((move_s > 5) && (move_s < 8) && (add_dot_support == 0)) len_lim = x_len[check2] / 2;
					}
					else if(move_s < 24){
						if(move_s == 16) time_c = 600;
						move = 0;
						check2 = 0;
						check = result_check[move_s-15+(9*add_dot_support)];
					}
					else if(move_s == 24){
						mode = 0;
						move_s = 0;
						check = 0;
						red = 1;
						red_2 = -1;
						glutTimerFunc(600, timer, 0);
					}
				break;
				case 5:
					time_c = 10;
					move--;
					move2_x--;
					if(move2_x < -31){
						mode = 0;
						move = 0;
						move2_x = 0;
						check = 0;
						check2 = 0;
						red = 1;
						red_2 = -1;
						glutTimerFunc(600, timer, 0);
					}
				break;
				case 7:
					time_c = 600;
					move_s++;
					if((move_s != 1) && (move_s != 8)) flip *= -1;
					if(move_s == 8){
						mode = 0;
						move_s = 0;
						check = 0;
						red = 1;
						red_2 = -1;
						glutTimerFunc(600, timer, 0);
					}
				break;
				default:
				break;
			}
		}
		glutPostRedisplay();
		glutTimerFunc(time_c, timer_2, 0);
	}
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	if(icon > 0){
		glBegin(GL_TRIANGLE_FAN);
		glVertex2d(-11+icon_x[icon],17+icon_y[icon]);
		glVertex2d(-11+icon_x[icon],19+icon_y[icon]);
		glVertex2d(-13+icon_x[icon],19+icon_y[icon]);
		glVertex2d(-13+icon_x[icon],17+icon_y[icon]);
		glEnd();
	}
	//基礎描画
	glPushMatrix();
	glTranslated(move, 0.0, 0.0);
	glColor3d(0.0, 0.0, 0.0);
	for(j=0;j<x_len[check];j++){
		glBegin(GL_TRIANGLE_FAN);
		glVertex2d(x[check][j]*flip, y[check][j]);
		glVertex2d((x[check][j]+1)*flip, y[check][j]);
		glVertex2d((x[check][j]+1)*flip, y[check][j]+1);
		glVertex2d(x[check][j]*flip, y[check][j]+1);
		glEnd();
	}
	glPopMatrix();
	//追加ドット用
	if(check2 > 0){
		glPushMatrix();
		glTranslated(move2_x, move2_y, 0.0);
		for(j=0;j<len_lim;j++){
			glBegin(GL_TRIANGLE_FAN);
			glVertex2d(x[check2][j], y[check2][j]);
			glVertex2d(x[check2][j]+1, y[check2][j]);
			glVertex2d(x[check2][j]+1, y[check2][j]+1);
			glVertex2d(x[check2][j], y[check2][j]+1);
			glEnd();
		}
		glPopMatrix();
	}
	//追加ドット用
	if(check3 > 0){
		glPushMatrix();
		for(j=0;j<x_len[check3];j++){
			glBegin(GL_TRIANGLE_FAN);
			glVertex2d(x[check3][j], y[check3][j]);
			glVertex2d(x[check3][j]+1, y[check3][j]);
			glVertex2d(x[check3][j]+1, y[check3][j]+1);
			glVertex2d(x[check3][j], y[check3][j]+1);
			glEnd();
		}
		glPopMatrix();
	}
	glFlush();
}

void resize(int w, int h){
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-16.0, 16.0, -4.0, 20.0, -1.0, 1.0);
	glutReshapeWindow(320, 240);
}

void keyboard(unsigned char key, int x, int y){
	switch (key){
		case 'i'://Aボタン
			if(mode == 0){
				if (icon == 7) icon = 0;
				else icon += 1;
			}
			else{
				mode++;
				switch (icon){
					case 1:
						if(mode > 6) mode = 1;
						check = 16 + mode;
					break;
					case 2:
						if((mode == 2)){
							mode = 1;
							if(move2_y != 0) add_dot_support = 0;
							else add_dot_support = 1;
							move2_y = -8 * add_dot_support;
						}
					break;
					case 3:
						if((mode == 2)&&(move_s < 6)){
							mode = 1;
							if(len_lim+2 <= x_len[24]) len_lim += 2;
							if(len_lim == x_len[24]) add_dot_support = 1;
						}
					break;
					case 6:
						if((mode == 2)){
							mode = 1;
							if(move2_y != 0) add_dot_support = 0;
							else add_dot_support = 1;
							move2_y = -8 * add_dot_support;
						}
					break;
					default:
					break;
				}
			}
			glutPostRedisplay();
		break;
		case 'k'://Bボタン
			if(icon > 0){
				if(mode == 0){
					red = -1;
					move_s = 0;
					flip = 1;
					if(icon != 5) move = 0;
				}
				mode++;
				switch (icon){
					case 1:
						if(mode > 6) mode = 1;
						check = 16 + mode;
						if(zzz == 1) check2 = 0;
					break;
					case 2:
						if(zzz == 0){
							if((mode == 1)||(mode > 2)){
								red_2 = -1;
								move_s = 0;
								mode = 1;
								check = 9;
								check2 = 16;
								move2_y = 0;
								if(add_dot_support > 0) move2_y = -8;
								len_lim = x_len[check2];
							}
							else if(mode == 2){
								red_2 = 1;
								glutTimerFunc(0, timer_2, 0);
							}
						}
						else mode = 0;
					break;
					case 3:
						if(zzz == 0){
							if(mode == 1){
								check = 30;
								check2 = 24;
								len_lim = x_len[check2] - 28;
								red = -1;
								red_2 = 1;
								glutTimerFunc(1000, timer_2, 0);
							}
							else if((mode == 2)&&(move_s < 6)){
								mode = 1;
								if(len_lim+2 <= x_len[24]) len_lim += 2;
								if(len_lim == x_len[24]) add_dot_support = 1;
							}
						}
						else mode = 0;
					break;
					case 4:
						if(zzz == 0){
							mode = 1;
							check = 38;
							check2 = 39;
							len_lim = x_len[check2];
						}
						else mode = 0;
					break;
					case 5:
						if(zzz == 0){
							if((mode == 1)&&(red_2 < 0)){
								red = -1;
								red_2 = 1;
								check2 = 23;
								len_lim = x_len[check2];
								move2_x = 6;
								glutTimerFunc(0, timer_2, 0);
							}
						}
						else mode = 0;
					break;
					case 6:
						if(mode == 1){
							check = 15;
							check2 = 16;
							move2_y = 0;
							if(add_dot_support > 0) move2_y = -8;
							len_lim = x_len[check2];
						}
						else if(mode == 2){
							mode = 0;
							if(add_dot_support > 0){
								check = 40;
								check2 = 41;
								len_lim = x_len[check2];
								move2_y = 0;
								add_dot_support = 0;
								zzz = 1;
							}
							else{
								check = 0;
								check2 = 0;
								red = 1;
								zzz = 0;
								glutTimerFunc(600, timer, 0);
							}
						}
					break;
					case 7:
						if(zzz == 0){
							if(mode == 1){
								red_2 = 1;
								check = 4;
								glutTimerFunc(600, timer_2, 0);
							}
						}
						else mode = 0;
					break;
					default:
					break;
				}
				glutPostRedisplay();
			}
		break;
		case 'm'://Cボタン
			if(mode > 0){
				if(zzz == 0){
					mode = 0;
					move = 0;
					move_s = 0;
					check = 0;
					check2 = 0;
					check3 = 0;
					flip = 1;
					move2_x=0;
					move2_y=0;
					red = 1;
					red_2 = -1;
					add_dot_support = 0;
					glutTimerFunc(600, timer, 0);
				}
				else{
					mode = 0;
					check = 40;
					check2 = 41;
					len_lim = x_len[check2];
					move2_y = 0;
				}
			}
			else if(icon > 0) icon = 0;
			glutPostRedisplay();
		break;
		case 'q':
		case 'Q':
		case '\033':
			exit(0);
		default:
		break;
	}
}

void init(void){
	//ディレクトリの読み込みと一覧表示
	if((dp = opendir("material")) == NULL){
		printf("not dir\n");
		exit(1);
	}
	while((dirst = readdir(dp)) != NULL){
		strcpy(filename, "material/");
		if(dirst->d_name[0] != '.'){
			printf("[datafile_%d]\n",i);
			strcat(filename,dirst->d_name);
			printf("%s\n", filename);
			if((fp = fopen(filename, "r")) == NULL){
				printf("not file\n");
				exit(1);
			}
			j=0;
			while((fscanf(fp,"%d %d",&x[i][j],&y[i][j])) != EOF) j++;
			x_len[i] = j;
			fclose(fp);
			i++;
		}
	}
	closedir(dp);

	glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char *argv[]){
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 240);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow("digi");
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutIgnoreKeyRepeat(GL_TRUE);
	glutTimerFunc(600, timer, 0);
	init();
	glutMainLoop();
	return 0;
}
