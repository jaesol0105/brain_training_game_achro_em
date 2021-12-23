#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<string.h>
#include "./fpga_dot_font.h" // fpga dot device
#include "/usr/include/mysql/mysql.h" // MYSQL

#define MAX_BUTTON 9
#define GAME2_MAX_BUTTON 6
#define GAME3_MAX_BUTTON 9

int game1(void); // 게임1
void shuffle(int* arr); // 게임1 숫자 셔플 함수
void game1_user_signal(int sig);

int game2(void); // 게임2
void display(int mode); // 게임2 UI 출력 함수
void game2_user_signal(int sig);

int game3(void); // 게임 3
void game3_user_signal(int sig);

void naming(void); // 랭킹에 등록할 닉네임 설정

// 종료 시그널
unsigned char game1_quit = 0;
unsigned char game2_quit = 0;
unsigned char game3_quit = 0;

// fpga 디바이스
int dev_push_sw; // push switch
int dev_fnd; // fnd
int dev_t_lcd; // text lcd

int game2_on_off[6] = { 0, };
char game2_list[6] = { 0, };
int game2_level = 1;

int num = -1;
int buff_size;
int name_int[3] = { 65,65,65 };

unsigned char push_sw_buff[MAX_BUTTON];
unsigned char data[4];
unsigned char retval;

struct connection_details { // DB 연결 정보 구조체
	char* server;
	char* user;
	char* password;
	char* database;
};

// DB 연결 수행
MYSQL* mysql_connection_setup(struct connection_details mysql_details) {

	MYSQL* connection = mysql_init(NULL);

	if (!mysql_real_connect(connection, mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, 0, NULL, 0)) {

		printf("Connection error : %s\n", mysql_error(connection));
		exit(1);

	}
	return connection;
}

// DB 쿼리 수행
MYSQL_RES* mysql_perform_query(MYSQL* connection, char* sql_query) {

	if (mysql_query(connection, sql_query)) {

		printf("MYSQL query error : %s\n", mysql_error(connection));
		exit(1);

	}
	return mysql_use_result(connection);
}

void game1_user_signal(int sig) {
	game1_quit = 1;
}

void game2_user_signal(int sig)
{
	game2_quit = 1;
}

void game3_user_signal(int sig)
{
	game3_quit = 1;
}

void naming(void) {
	system("clear");
	printf("랭킹에 등록할 닉네임을 입력해주세요.\n\n");

	int checker = 0;
	char name_char[3] = "AAA";

	unsigned char push_sw_buff[9];
	buff_size = sizeof(push_sw_buff);

	unsigned char string[32];
	memset(string, 0, sizeof(string));
	strcat(string, "AAA                             ");

	write(dev_t_lcd, string, 32);

	// 9개의 push switch 로부터 닉네임을 설정할수있다. 알파벳 3글자 닉네임.
	// [↑] [↑] [↑]
	// [ ] [결정] [ ]    <---->   [ A B C ] 
	// [↓] [↓] [↓]

	while (checker == 0) {
		usleep(200000);
		read(dev_push_sw, &push_sw_buff, buff_size);
		for (int i = 0; i < buff_size; i++) {
			if (push_sw_buff[i] == 1) {
				if (i == 0) {
					if (name_int[0] == 65)
					{
						name_int[0] = 90;
					}
					else
					{
						name_int[0] = name_int[0] - 1;
					}
				}
				else if (i == 1) {
					if (name_int[1] == 65)
					{
						name_int[1] = 90;
					}
					else
					{
						name_int[1] = name_int[1] - 1;
					}
				}
				else if (i == 2) {
					if (name_int[2] == 65)
					{
						name_int[2] = 90;
					}
					else
					{
						name_int[2] = name_int[2] - 1;
					}
				}
				if (i == 6) {
					if (name_int[0] == 90)
					{
						name_int[0] = 65;
					}
					else
					{
						name_int[0] = name_int[0] + 1;
					}
				}
				else if (i == 7) {
					if (name_int[1] == 90)
					{
						name_int[1] = 65;
					}
					else
					{
						name_int[1] = name_int[1] + 1;
					}
				}
				else if (i == 8) {
					if (name_int[2] == 90)
					{
						name_int[2] = 65;
					}
					else
					{
						name_int[2] = name_int[2] + 1;
					}
				}
				else if (i == 4) {
					checker = 1;
				}

				for (int w = 0; w < 3; w++) {
					string[w] = (char)(name_int[w]);
				}
			}
		}
		write(dev_t_lcd, string, 32);
	}

	close(dev_push_sw);
}

void shuffle(int* arr) {

	srand(time(NULL));

	int random;
	int temp;

	for (int i = 0; i < 9; i++) {
		random = rand() % 9;
		temp = arr[i];
		arr[i] = arr[random];
		arr[random] = temp;
	}
}

int game1(void) {

	int numbers[9] = { 1,2,3,4,5,6,7,8,9 };

	int round = 1;
	int score = 1;
	int flag = 0;
	int count = 0;

	// fnd 디바이스 오픈
	dev_fnd = open("/dev/fpga_fnd", O_RDWR);

	memset(data, 0, sizeof(data));

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;

	retval = write(dev_fnd, &data, 4);
	memset(data, 0, sizeof(data));
	sleep(1);
	retval = read(dev_fnd, &data, 4);

	if (dev_push_sw < 0) {
		printf("Device 1 Open Error\n");
		close(dev_push_sw);
		return -1;
	}

	if (dev_fnd < 0) {
		printf("Device 2 Open Error\n");
		close(dev_fnd);
		return -1;
	}

	(void)signal(SIGINT, game1_user_signal);

	buff_size = sizeof(push_sw_buff);

	while (flag == 0) {
		system("clear");

		for (int i = 0; i < 9; i++) {
			numbers[i] = 0;
		}

		printf("--Round %d--\n", score);

		for (int i = 0; i < round; i++) {
			numbers[i] = i + 1;
		}

		// 숫자 배열 순서 섞기
		shuffle(numbers);

		// 숫자 UI 출력
		for (int i = 0; i < 9; i++) {
			if (numbers[i] != 0) {
				if (i == 2 || i == 5 || i == 8) {
					printf("[%d]\n", numbers[i]);
				}
				else {
					printf("[%d] ", numbers[i]);
				}
			}
			else {
				if (i == 2 || i == 5 || i == 8) {
					printf("[ ]\n");
				}
				else {
					printf("[ ] ");
				}
			}
		}
		printf("------------\n");

		// 빈칸 UI 출력 (숫자 가리기)
		sleep(2);
		system("clear");
		printf("--Round %d--\n", score);
		printf("[ ] [ ] [ ]\n");
		printf("[ ] [ ] [ ]\n");
		printf("[ ] [ ] [ ]\n");
		printf("------------\n");

		for (int i = 0; i < round; i++) {
			// switch 버퍼 초기화
			for (int k = 0; k < 9; k++) {
				push_sw_buff[k] = 0;
			}

			num = -1;

			// push switch 입력 받기
			while (num == -1) {
				usleep(200000);
				read(dev_push_sw, &push_sw_buff, buff_size);
				for (int j = 0; j < 9; j++) {
					if (push_sw_buff[j] == 1) {
						num = j;
					}
				}
			}

			// 정답 확인
			if (numbers[num] != i + 1) {
				printf("wrong\n");
				flag = 1;
				break;
			}
			else { printf("correct!\n"); }
		}

		// 9 라운드 이전에서는 라운드 + 1
		if (round < 9) {
			round = round + 1;
		}

		if (score < 10) {
			data[3] = score;
		}
		else if (score < 100) {
			data[3] = (int)(score % 10);
			data[2] = (int)(score / 10);
		}

		retval = write(dev_fnd, &data, 4);
		memset(data, 0, sizeof(data));
		sleep(1);
		retval = read(dev_fnd, &data, 4);

		score = score + 1;
	}

	score = score - 2;

	if (score < 10) {
		data[3] = score;
	}
	else if (score < 100) {
		data[3] = (int)(score % 10);
		data[2] = (int)(score / 10);
	}

	retval = write(dev_fnd, &data, 4);
	memset(data, 0, sizeof(data));
	sleep(1);
	retval = read(dev_fnd, &data, 4);

	printf("[ score = %d ]\n", score);
	close(dev_fnd);
	return score;
}

void display(int mode) {
	printf("[level = %d]\n", game2_level);
	if (mode == 0) printf("아래의 카드를 기억해 두세요.\n");
	printf("┌────────────────────┐\n");
	printf("│  ┌──┐  ┌──┐  ┌──┐  │\n");

	for (int i = 0; i < 2; i++) {
		printf("│  │ ");
		for (int j = 0; j < 3; j++) {

			if (mode == 0) { // 보여주기
				printf("%c│  │ ", game2_list[i * 3 + j]);
			}
			if (mode == 1) { // 게임시작
				if (game2_on_off[i * 3 + j] == 0) {
					printf("?│  │ ");
				}
				else {
					printf("%c│  │ ", game2_list[i * 3 + j]);
				}
			}
		}
		printf("\n");
		printf("│  └──┘  └──┘  └──┘  │\n");
		if (i == 0)
			printf("│  ┌──┐  ┌──┐  ┌──┐  │\n");
	}
	printf("└────────────────────┘\n");
}

// 게임2
int game2(void) {
	int index;
	int flag[6];
	int card_check[91];
	int num;
	int card1, card2;
	int score;

	int state = 1;

	srand(time(NULL));

	int i;
	int buff_size;
	unsigned char push_sw_buff[GAME2_MAX_BUTTON];

	unsigned char string[32];

	int dev_dot;
	int str_size;

	// dot 디바이스 오픈
	dev_dot = open("/dev/fpga_dot", O_RDWR);
	if (dev_push_sw < 0 || dev_t_lcd < 0 || dev_dot < 0) {
		printf("Device Open Error\n");
		return -1;
	}

	(void)signal(SIGINT, game2_user_signal);
	buff_size = sizeof(push_sw_buff);

	while (1) {
		// dot 디바이스 현재 라운드 표시
		str_size = sizeof(fpga_number[game2_level]);
		write(dev_dot, fpga_number[game2_level], str_size);

		// 초기화
		for (int i = 0; i < 6; i++) { // 카드가 뒤집혀 있는지 여부
			game2_on_off[i] = 0;
		}
		for (int i = 0; i < 6; i++) { // 카드 위치 배치용
			flag[i] = 0;
		}
		for (int i = 0; i < 91; i++) { // 알파벳 중복 체크용
			card_check[i] = 0;
		}

		// 카드 무작위 배치 (A-Z 중 3 쌍)
		for (int i = 0; i < 3; i++) {
			while (1) {
				num = rand() % 26 + 65;
				if (card_check[num] == 0)
					break;
			}
			card_check[num] = 1;

			while (1) {
				index = rand() % 6;
				if (flag[index] == 0)
					break;
			}
			game2_list[index] = (char)num;
			flag[index] = 1;

			while (1) {
				index = rand() % 6;
				if (flag[index] == 0)
					break;
			}
			game2_list[index] = (char)num;
			flag[index] = 1;
		}

		//1. 보여주기. (레벨에 따라 시간 감소 - 미구현)
		system("clear");
		display(0);

		sleep(3);
		system("clear");

		display(1);
		score = 0;

		int read_check = 0;

		// 2. push switch 버튼 입력을 받는다
		while (1) {
			memset(string, 0, sizeof(string));
			strcat(string, "Choose a card1  to flip over.    "); // 카드 1
			write(dev_t_lcd, string, 32);
			while (1) {
				usleep(400000);
				read(dev_push_sw, &push_sw_buff, buff_size);
				for (i = 0; i < GAME2_MAX_BUTTON; i++) {
					if (push_sw_buff[i] == 1) {
						card1 = i;
						read_check = 1;
						break;
					}
				}
				if (read_check == 1) {
					read_check = 0;
					break;
				}
			}
			game2_on_off[card1] = 1;

			system("clear");
			display(1);

			memset(string, 0, sizeof(string));
			strcat(string, "Choose a card2  to flip over.    "); // 카드 2
			write(dev_t_lcd, string, 32);

			while (1) {
				usleep(400000);
				read(dev_push_sw, &push_sw_buff, buff_size);
				for (i = 0; i < GAME2_MAX_BUTTON; i++) {
					if (push_sw_buff[i] == 1) {
						card2 = i;
						read_check = 1;
						break;
					}
				}
				if (read_check == 1) {
					read_check = 0;
					break;
				}
			}
			game2_on_off[card2] = 1;

			system("clear");
			display(1);

			sleep(1);

			if (game2_list[card1] == game2_list[card2]) { // 정답 확인
				system("clear");
				display(1);
				printf("정답입니다!\n");
				score = score + 2;
			}
			else {
				system("clear");
				printf("게임 오버\n");
				state = 0;
				break;
			}

			if (score == 6) { // 모든 카드를 뒤집었을경우, 다음 라운드
				game2_level++;
				sleep(1);
				break;
			}
		}
		if (state == 0)
			break;

		// while
		// display
		// 뒤집을 카드1 선택 :
		// 	   카드뒤집기
		// 뒤집을 카드2 선택 :
		// 	   카드뒤집기
		// 정답 -> 정답반영, continue
		// 오답 -> 게임 종료,
		// 만약 모든카드가 다 뒤집혔을경우 : level++ 다음단계

	}
	close(dev_dot);
	return game2_level;
}

int game3(void)
{
	srand(time(NULL));
	int a;
	int ran[9];
	int n, i, j, k;
	int stg = 0;
	int dev_dot;
	int str_size2;

	// dot 디바이스 오픈
	dev_dot = open("/dev/fpga_dot", O_RDWR);

	if (dev_dot < 0) {
		printf("Dot Open Error\n");
		close(dev_dot);
		return -1;
	}

	(void)signal(SIGINT, game3_user_signal);
	buff_size = sizeof(push_sw_buff);

	while (1) {
		stg++;

		// 무작위 숫자 배열
		printf("\n~ Round %d ~\n", stg);
		for (i = 0; i < 9; i++) {
			ran[i] = (rand() % 9) + 1;
			for (j = 0; j < i; j++) {
				if (ran[i] == ran[j]) {
					i--;
					break;
				}
			}
		}

		// 5초간 화면에 출력한다.
		printf("Rember the numbers for 5seconds!!\n");
		printf("------\n");
		for (i = 0; i < 9; i++) {
			printf("%d ", ran[i]);
			if ((i + 1) % 3 == 0) {
				printf("\n");
			}
		}
		printf("------\n");
		sleep(5);
		system("clear");

		n = (rand() % 9) + 1;

		printf("\n");
		printf("Number -->> ");
		printf("%d", ran[n - 1]);
		printf("\n");

		// push switch 입력 받기
		num = -1;
		while (num == -1) {
			usleep(200000);
			read(dev_push_sw, &push_sw_buff, buff_size);
			for (k = 0; k < 9; k++) {
				if (push_sw_buff[k] == 1) {
					num = k;
				}
			}
		}
		a = num + 1;
		printf("Nth place :%d\n ", a);

		// 정답
		if (a == n) {
			printf("Good job^^ --> O\n\n");
			str_size2 = sizeof(fpga_number[a]);
			write(dev_dot, fpga_number[a], str_size2);
			sleep(1);
			system("clear");
		}

		// 오답
		if (a != n) {
			printf("Oh my god!! --> X\n\n");
			str_size2 = sizeof(fpga_number[a]);
			write(dev_dot, fpga_number[a], str_size2);
			printf("%d Round Clear\n", stg);
			sleep(1);
			system("clear");
			printf("\n\n\n");
			printf("Game over!~!\n\n");
			close(dev_dot);
			break;
		}
	}
	return stg;
}

// 메인
int main(void) {
	system("clear");

	int buff_size;
	unsigned char push_sw_buff[3];
	unsigned char string[32];

	int mode = 0;
	int score = 0;
	char name[4] = "AAA";

	// text_lcd, push_sw 디바이스 오픈
	dev_push_sw = open("/dev/fpga_push_switch", O_RDWR);
	dev_t_lcd = open("/dev/fpga_text_lcd", O_WRONLY);
	if (dev_push_sw < 0 || dev_t_lcd < 0) {
		printf("Device Open Error\n");
		return -1;
	}

	buff_size = sizeof(push_sw_buff);

	// text_lcd 메세지 출력
	memset(string, 0, sizeof(string));
	strcat(string, "Choose Game Number              ");
	write(dev_t_lcd, string, 32);

	int GameNum = -1;
	int read_check = 0;
	int i;

	// 사용자의 게임 번호 입력
	while (1) {
		usleep(400000);
		read(dev_push_sw, &push_sw_buff, buff_size);
		for (i = 0; i < buff_size; i++) {
			if (push_sw_buff[i] == 1) {
				GameNum = i;
				read_check = 1;
				break;
			}
		}
		if (read_check == 1) {
			read_check = 0;
			break;
		}
	}

	if (GameNum == 0) { score = game1(); mode = 1; }
	else if (GameNum == 1) { score = game2(); mode = 2; }
	else if (GameNum == 2) { score = game3(); mode = 3; }

	naming(); // 게임 종료 후, 랭킹에 등록할 닉네임 설정함수.

	name[0] = (char)name_int[0];
	name[1] = (char)name_int[1];
	name[2] = (char)name_int[2];

	printf("[게임번호]%d, [내점수]%d, [닉네임]%s 등록완료\n", mode, score, name);

	// DB 연결
	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;

	struct connection_details mysqlD;
	mysqlD.server = "localhost";
	mysqlD.user = "testuser2";
	mysqlD.password = "raspberry";
	mysqlD.database = "test";

	conn = mysql_connection_setup(mysqlD);
	char query[200];

	char mode_[4];
	char score_[4];
	sprintf(mode_, "%d", mode); // string 변환
	sprintf(score_, "%d", score);

	// 랭킹 등록 query 수행.
	sprintf(query, "insert into embsys(mode,name,score) values "
		"('%s', '%s', '%s')",
		mode_, name, score_);
	mysql_perform_query(conn, query);
	mysql_close(conn);

	printf("DB done\n");
	return 0;
}
