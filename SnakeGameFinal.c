nclude<stdio.h>
#include<windows.h>
#include<conio.h>
#include<assert.h>
#include<stdlib.h>
#include<time.h>
#pragma warning(disable:4996)
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define PAUSE 112
#define ESC 27
#define STATUS 115
#define MAP_ADJ_X 3// 실행창의 (0,0)로부터 떨어진 거리 
#define MAP_ADJ_Y 2
#define MAP_X 30
#define MAP_Y 20

int x[100], y[100]; //x,y 좌표값을 저장 총 100개 
int food_x, food_y; //food의 좌표값을 저장 
int swamp_x, swamp_y; //늪의 좌표값 저장
int length; //몸길이를 기억 
int speed; //게임 속도 
int score; //점수 저장  --reset함수에 의해 초기화됨
int best_score = 0; //최고 점수 저장 --reset함수에 의해 초기화 되지 않음 
int last_score = 0; //마지막 점수 저장  --reset함수에 의해 초기화 되지 않음
int dir; //이동방향 저장 
int key; //입력받은 키 저장 
int status_on = 0; // 개발자용 status 표시활성화 변수.. 게임중에 S키를 누르면 활성 

void gotoxy(int x, int y, const char* s) { //x값을 2x로 변경, 좌표값에 바로 문자열을 입력할 수 있도록 printf함수 삽입  

   COORD pos = { SHORT(2 * x), SHORT(y) };
      SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
         printf("%s", s);
	 }

	 void title(void); //게임 시작화면 
	 void start_game(void); //게임을 초기화 
	 void draw_inside_blank(void); // 맵 테두리 안쪽을 빈칸으로 채움
	 void draw_snake(void);//뱀 그리기
	 void draw_map_boundary(void); // 게임판 테두리를 그림 
	 void move(int dir); //게임 규칙(food에 닿았을때, 자신의 몸통에 닿았을때)
	 void pause(void); //일시정지 
	 void game_over(void); //게임 오버를 확인 
	 void food(void); // 음식 생성 
	 void swamp(void); //늪 생성
	 void status(void); // 개발자용 status표시  
	 void detect_collision(void);//food에 충돌, 자신의 몸과 충돌을 했는지 판단하는 함수
	 void wallcrash(void); //벽면에 닿을시 함수
	 void print_manual(void); //게임을 어떻게 하는지 가르쳐주는 메뉴얼
	 void check_overlap(void); //먹이랑 함정이랑 같은 좌표에서 출력될 경우 assert 함수를 통해 디버그오류를 냄


	 //방향키와 p=일시정지, ESC=종료, s=관리자용 정보
	 int main() {
	    title();//타이틀 화면 출력

	       while (1) {
	             if (kbhit()) {
		              do { key = getch(); } while (key == 224);
			            }
				          Sleep(speed);

					        switch (key) { //입력받은 키를 파악하고 실행  
						      case LEFT:
						            case RIGHT:
							          case UP:
								        case DOWN:
									         if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) ||
										             (dir == UP && key != DOWN) || (dir == DOWN && key != UP)) {//180회전이동을 방지하기 위해 필요. 
											                 dir = key;
													             key = 0; // 키값을 저장하는 함수를 reset 
														                 break;
																          }
																	        case PAUSE: // P키를 누르면 일시정지 
																		         pause();
																			          break;
																				        case STATUS://S키를 누르면 개발자용 status를 활성화 시킴  
																					         if (status_on == 0) {
																						             status_on = 1;
																							              }
																								               else {
																									                   status_on = 0;
																											            }
																												             key = 0;
																													              break;

																														            case ESC: //ESC키를 누르면 프로그램 종료 
																															             exit(0);
																																           default:
																																	            break;
																																		          }
																																			        move(dir);

																																				      if (status_on == 1) {
																																				               status(); // status표시 
																																					             }
																																						        }
																																							}


																																							void title(void) {
																																							   draw_map_boundary();//맵 테두리를 그림
																																							      while (kbhit()) getch(); //버퍼에 있는 키값을 버림 

																																							         void draw_inside_blank(); // 맵 테두리 안쪽을 빈칸으로 채움

																																								    gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 5, "+--------------------------+");
																																								       gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 6, "|        S N A K E         |");
																																								          gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 7, "+--------------------------+");

																																									     gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 9, " < PRESS ANY KEY TO START > ");

																																									        gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 11, "   ◇ ←,→,↑,↓ : Move    ");
																																										   gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 12, "   ◇ P : Pause             ");
																																										      gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 13, "   ◇ ESC : Quit              ");



																																										         while (1) {
																																											       if (kbhit()) { //키입력받음 
																																											                key = getch();
																																													         if (key == ESC) {
																																														             exit(0); // ESC키면 종료 
																																															              }
																																																               else {
																																																	                   break; //아니면 그냥 계속 진행 
																																																			            }
																																																				          }
																																																					        gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 9, " < PRESS ANY KEY TO START > ");
																																																						      Sleep(400);
																																																						            gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 9, "                            ");
																																																							          Sleep(400);

																																																								     }
																																																								        while (_kbhit()) getch();
																																																									   print_manual(); //메뉴얼 호출
																																																									   }

																																																									   //최초설정 속도 :100, 몸통길이 :4 스코어 :0
																																																									   void start_game(void) { //게임을 초기화
																																																									      system("cls"); //화면을 지움 
																																																									         draw_map_boundary(); //맵 테두리를 그림 
																																																										    while (kbhit()) getch(); //버퍼에 있는 키값을 버림 

																																																										       dir = LEFT; // 방향 초기화  
																																																										          speed = 100; // 속도 초기화 
																																																											     length = 5; //뱀 길이 초기화 
																																																											        score = 0; //점수 초기화 
																																																												   draw_snake();
																																																												      food(); // food 생성 
																																																												         swamp(); //늪 생성
																																																													    check_overlap(); //food의 좌표와 swamp좌표가 같은곳에 생성하는것 방지
																																																													    }
																																																													    void draw_inside_blank(void) { // 맵 테두리 안쪽을 빈칸으로 채움
																																																													       int i;
																																																													          int j;
																																																														     for (i = MAP_ADJ_Y + 1; i < MAP_ADJ_Y + MAP_Y - 1; i++) {
																																																														           for (j = MAP_ADJ_X + 1; j < MAP_ADJ_X + MAP_X - 1; j++) {
																																																															            gotoxy(j, i, "  ");
																																																																          }
																																																																	     }
																																																																	     }
																																																																	     void draw_snake(void) { //뱀 그리기
																																																																	        int i;
																																																																		   for (i = 0; i < length; i++) { //뱀 몸통 그리기 
																																																																		         x[i] = MAP_X / 2 + i;
																																																																			       y[i] = MAP_Y / 2;
																																																																			             gotoxy(MAP_ADJ_X + x[i], MAP_ADJ_Y + y[i], "○");
																																																																				        }
																																																																					   gotoxy(MAP_ADJ_X + x[0], MAP_ADJ_Y + y[0], "♠"); //뱀 머리 그리기
																																																																					   }

																																																																					   void draw_map_boundary(void) { //맵 테두리 그리는 함수
																																																																					      int i;
																																																																					         for (i = 0; i < MAP_X; i++) {
																																																																						       gotoxy(MAP_ADJ_X + i, MAP_ADJ_Y, "■");
																																																																						          }
																																																																							     for (i = MAP_ADJ_Y + 1; i < MAP_ADJ_Y + MAP_Y - 1; i++) {
																																																																							           gotoxy(MAP_ADJ_X, i, "■");
																																																																								         gotoxy(MAP_ADJ_X + MAP_X - 1, i, "■");
																																																																									    }
																																																																									       for (i = 0; i < MAP_X; i++) {
																																																																									             gotoxy(MAP_ADJ_X + i, MAP_ADJ_Y + MAP_Y - 1, "■");
																																																																										        }
																																																																											}


																																																																											// food와 닿았을때, 자기 몸과 닿았을때, 몸이 움직일때
																																																																											// 삭제 : 벽에 닿을때 게임종료
																																																																											// 변경 : 벽에 닿았을때 반대쪽 벽면으로 이동(wallcrash()문)
																																																																											void move(int dir) {
																																																																											   int i;
																																																																											      detect_collision();
																																																																											         wallcrash();
																																																																												    gotoxy(MAP_ADJ_X + x[length - 1], MAP_ADJ_Y + y[length - 1], "  "); //몸통 마지막을 지움
																																																																												       for (i = length - 1; i > 0; i--) { //몸통좌표를 한칸씩 옮김
																																																																												             x[i] = x[i - 1];
																																																																													           y[i] = y[i - 1];
																																																																														      }
																																																																														         gotoxy(MAP_ADJ_X + x[0], MAP_ADJ_Y + y[0], "○"); //머리가 있던곳을 몸통으로 고침
																																																																															    if (dir == LEFT) {
																																																																															          --x[0];
																																																																																     }
																																																																																        if (dir == RIGHT) {
																																																																																	      ++x[0];
																																																																																	         }
																																																																																		    if (dir == UP) {
																																																																																		          --y[0];
																																																																																			     }
																																																																																			        if (dir == DOWN) {
																																																																																				      ++y[0];
																																																																																				         }
																																																																																					    gotoxy(MAP_ADJ_X + x[i], MAP_ADJ_Y + y[i], "♠"); //새로운 머리좌표값에 머리를 그림
																																																																																					    }
																																																																																					    void detect_collision(void) {
																																																																																					       int i;
																																																																																					          //뱀이 음식에 닿았을 경우 
																																																																																						     if (x[0] == food_x && y[0] == food_y)
																																																																																						        {
																																																																																							      score += 10;
																																																																																							            food();
																																																																																								          length++;
																																																																																									        x[length - 1] = x[length - 2];
																																																																																										      y[length - 1] = y[length - 2];
																																																																																										         }
																																																																																											    //뱀의 머리가 몸통에 닿았을 경우
																																																																																											       for (i = 1; i < length; i++) {
																																																																																											             if (x[0] == x[i] && y[0] == y[i])
																																																																																												           {
																																																																																													            game_over();
																																																																																														          }
																																																																																															     }
																																																																																															        //뱀이 늪에 닿았을 경우
																																																																																																   if (swamp_x == x[0] && swamp_y == y[0])
																																																																																																      {
																																																																																																            game_over();
																																																																																																	       }
																																																																																																	       }

																																																																																																	       void wallcrash(void) {//벽을 지나칠경우 반대쪽벽에서 나오게 출력해주는 함수
																																																																																																	          if (x[0] == 0)
																																																																																																		     {

																																																																																																		           x[0] = MAP_X - 2;
																																																																																																			         gotoxy(MAP_ADJ_X, y[0] + 2, "■");
																																																																																																				    }
																																																																																																				       else if (x[0] == MAP_X - 1)
																																																																																																				          {
																																																																																																					        x[0] = 1;
																																																																																																						      gotoxy(MAP_ADJ_X + 29, MAP_ADJ_Y + y[0], "■");
																																																																																																						         }
																																																																																																							    else if (y[0] == 0)
																																																																																																							       {

																																																																																																							             y[0] = MAP_Y - 2;
																																																																																																								           gotoxy(x[0] + MAP_ADJ_X, MAP_ADJ_Y, "■");
																																																																																																									      }
																																																																																																									         else if (y[0] == MAP_Y - 1)
																																																																																																										    {
																																																																																																										          y[0] = 1;
																																																																																																											        gotoxy(MAP_ADJ_X + x[0], MAP_ADJ_Y + MAP_Y - 1, "■");
																																																																																																												   }
																																																																																																												   }

																																																																																																												   void pause(void) { // p키를 눌렀을 경우 게임을 일시 정지 
																																																																																																												      while (1) {
																																																																																																												            if (key == PAUSE) {
																																																																																																													             gotoxy(MAP_ADJ_X + (MAP_X / 2) - 9, MAP_ADJ_Y, "< PAUSE : PRESS ANY KEY TO RESUME > ");
																																																																																																														              Sleep(400);
																																																																																																															               gotoxy(MAP_ADJ_X + (MAP_X / 2) - 9, MAP_ADJ_Y, "                                    ");
																																																																																																																                Sleep(400);
																																																																																																																		      }
																																																																																																																		            else {
																																																																																																																			             draw_map_boundary(); // 게임판 테두리를 그림
																																																																																																																				              return;
																																																																																																																					            }
																																																																																																																						          if (kbhit()) {

																																																																																																																							           do {
																																																																																																																								               key = getch();
																																																																																																																									                } while (key == 224);
																																																																																																																											      }

																																																																																																																											         }
																																																																																																																												 }

																																																																																																																												 void game_over(void) { //게임종료 함수 
																																																																																																																												    gotoxy(MAP_ADJ_X + (MAP_X / 2) - 6, MAP_ADJ_Y + 5, "+----------------------+");
																																																																																																																												       gotoxy(MAP_ADJ_X + (MAP_X / 2) - 6, MAP_ADJ_Y + 6, "|      GAME OVER..     |");
																																																																																																																												          gotoxy(MAP_ADJ_X + (MAP_X / 2) - 6, MAP_ADJ_Y + 7, "+----------------------+");
																																																																																																																													     gotoxy(MAP_ADJ_X + (MAP_X / 2) - 6, MAP_ADJ_Y + 8, " YOUR SCORE : ");
																																																																																																																													        printf("%d", last_score = score);

																																																																																																																														   gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 12, " Press any keys to restart.. ");

																																																																																																																														      if (score > best_score) { //베스트 스코어인지 아닌지 판단하고 맞으면 그 값을 저정하고 축하해주는 조건문
																																																																																																																														            best_score = score;
																																																																																																																															          gotoxy(MAP_ADJ_X + (MAP_X / 2) - 4, MAP_ADJ_Y + 10, "☆ BEST SCORE ☆");
																																																																																																																																     }
																																																																																																																																        Sleep(500);
																																																																																																																																	   while (kbhit()) getch();
																																																																																																																																	      key = getch();
																																																																																																																																	         title();
																																																																																																																																		 }

																																																																																																																																		 void food(void) { // 음식 생성
																																																																																																																																		    int i;

																																																																																																																																		       int food_crush_on;//food가 뱀 몸통좌표에 생기는지 안 생기는지 판단하는 변수 
																																																																																																																																		          int r = 0; //난수 생성에 사동되는 변수 
																																																																																																																																			     gotoxy(MAP_ADJ_X, MAP_ADJ_Y + MAP_Y, " YOUR SCORE: ");//점수표시 
																																																																																																																																			        printf("%3d, LAST SCORE: %3d, BEST SCORE: %3d", score, last_score, best_score);

																																																																																																																																				   while (1) {
																																																																																																																																				         food_crush_on = 0;
																																																																																																																																					       srand((unsigned)time(NULL) + r); //난수표생성 
																																																																																																																																					             food_x = (rand() % (MAP_X - 2)) + 1;    //난수를 좌표값에 넣음 
																																																																																																																																						           food_y = (rand() % (MAP_Y - 2)) + 1;


																																																																																																																																							         for (i = 0; i < length; i++) { //food가 뱀 몸통과 겹치는지 확인  
																																																																																																																																								          if (food_x == x[i] && food_y == y[i]) {
																																																																																																																																									              food_crush_on = 1; //겹치면 food_crush_on 를 on 
																																																																																																																																										                  r++;
																																																																																																																																												              break;
																																																																																																																																													               }
																																																																																																																																														             }

																																																																																																																																															           if (food_crush_on == 1) {
																																																																																																																																																            continue; //겹쳤을 경우 while문을 다시 시작 
																																																																																																																																																	          }
																																																																																																																																																		        gotoxy(MAP_ADJ_X + food_x, MAP_ADJ_Y + food_y, "♪"); //안겹쳤을 경우 좌표값에 food를 찍고 
																																																																																																																																																			      speed -= 3; //속도 증가 
																																																																																																																																																			            break;

																																																																																																																																																				       }
																																																																																																																																																				       }

																																																																																																																																																				       void swamp(void) { //늪을 만들어 뱀의 위치와 food의 위치를 제외한 나머지칸에 생성
																																																																																																																																																				          int swamp_crush_on = 0;
																																																																																																																																																					     int r = 1;
																																																																																																																																																					        srand((unsigned)time(NULL) + r);
																																																																																																																																																						   swamp_x = (rand() % (MAP_X - 2)) + 1;
																																																																																																																																																						      swamp_y = (rand() % (MAP_Y - 2)) + 1;
																																																																																																																																																						         gotoxy(MAP_ADJ_X + swamp_x, MAP_ADJ_Y + swamp_y, "★");
																																																																																																																																																							 }

																																																																																																																																																							 void check_overlap(void) { //음식과 늪의 좌표가 겹칠경우 디버그 해주는 함수
																																																																																																																																																							    assert(food_x != swamp_x && food_y != swamp_y);
																																																																																																																																																							    }


																																																																																																																																																							    void status(void) { //각종 스텟을 볼수 있는 함수 
																																																																																																																																																							       gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y, "head= ");
																																																																																																																																																							          printf("%2d,%2d", x[0], y[0]);
																																																																																																																																																								     gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 1, "food= ");
																																																																																																																																																								        printf("%2d,%2d", food_x, food_y);
																																																																																																																																																									   gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 2, "leng= ");
																																																																																																																																																									      printf("%2d", length);
																																																																																																																																																									         gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 3, "key= ");
																																																																																																																																																										    printf("%3d", key);
																																																																																																																																																										       gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 4, "spd= ");
																																																																																																																																																										          printf("%3d", speed);
																																																																																																																																																											     gotoxy(MAP_ADJ_X + MAP_X + 1, MAP_ADJ_Y + 6, "score= ");
																																																																																																																																																											        printf("%3d", score);
																																																																																																																																																												}

																																																																																																																																																												void print_manual(void) { //겜에 대한 설명을 출력해주는 함수

																																																																																																																																																												   system("cls");
																																																																																																																																																												      draw_map_boundary();
																																																																																																																																																												         gotoxy(MAP_ADJ_X + 12, MAP_ADJ_Y + 1, "■규칙 설명■");
																																																																																																																																																													    gotoxy(MAP_ADJ_X + 1, MAP_ADJ_Y + 3, "뱀은 상하좌우로 움직이며 ♪(먹이)를 먹는다.");
																																																																																																																																																													       gotoxy(MAP_ADJ_X + 1, MAP_ADJ_Y + 5, "♪를 먹을 시 꼬리가 한칸 씩 늘어나며 점수가 10점 오르며");
																																																																																																																																																													          gotoxy(MAP_ADJ_X + 1, MAP_ADJ_Y + 6, "속도가 빨라진다.");
																																																																																																																																																														     gotoxy(MAP_ADJ_X + 1, MAP_ADJ_Y + 8, "움직이며 자신의 몸에 닿거나 ★(늪)에 닿을 시");
																																																																																																																																																														        gotoxy(MAP_ADJ_X + 1, MAP_ADJ_Y + 9, "즉시 종료되며 점수가 저장된다.");
																																																																																																																																																															   gotoxy(MAP_ADJ_X + 1, MAP_ADJ_Y + 11, "게임 실해중에 가장 높은 점수는 best score에 저장되어");
																																																																																																																																																															      gotoxy(MAP_ADJ_X + 1, MAP_ADJ_Y + 12, "게임종료 시 화면상 표시된다.");
																																																																																																																																																															         gotoxy(MAP_ADJ_X + 1, MAP_ADJ_Y + 14, "벽면에 닿을 시 닿은 벽면에 대칭되는 곳으로 이동되며 ");
																																																																																																																																																																    gotoxy(MAP_ADJ_X + 1, MAP_ADJ_Y + 15, "게임은 계속 진행된다.");
																																																																																																																																																																       while (1) {
																																																																																																																																																																             if (kbhit()) {
																																																																																																																																																																	              key = getch();
																																																																																																																																																																		               if (key == ESC) exit(0);
																																																																																																																																																																			                else break;
																																																																																																																																																																					      }
																																																																																																																																																																					            gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 18, " < PRESS ANY KEY TO START > ");
																																																																																																																																																																						          Sleep(400);
																																																																																																																																																																							        gotoxy(MAP_ADJ_X + (MAP_X / 2) - 7, MAP_ADJ_Y + 18, "                            ");
																																																																																																																																																																								      Sleep(400);

																																																																																																																																																																								         }
																																																																																																																																																																									    while (_kbhit()) getch();
																																																																																																																																																																									       start_game();
																																																																																																																																																																									       }
