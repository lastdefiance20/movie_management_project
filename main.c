#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//영화 관리 프로그램 by 20201809 정재윤

//년 월 일 struct 구조
typedef struct {
	int year;
	int month;
	int day;
}YMD;

//이름, 개봉일, 상영시간, 장르, 배급사
typedef struct {
	char name[100];
	YMD ymd;
	int run_time;
	char genre[100];
	char film_dist[100];
}MV;

//연결리스트
typedef struct _node {
	MV mv;
	struct _node* next;
}Node;

//MV 출력 함수
void printMVinfo(const MV* mv) {
	printf("영화 이름 : %s\n", mv->name);
	printf("영화 개봉일 : %d년 %d월 %d일\n", mv->ymd.year, mv->ymd.month, mv->ymd.day);
	printf("영화 상영 시간 : %d\n", mv->run_time);
	printf("영화 장르 : %s\n", mv->genre);
	printf("영화 배급사 : %s\n", mv->film_dist);
	printf("--------------------\n");
}

//연결리스트를 따라가며 영화 출력 함수 (MV 출력 함수 호출)
void printList(Node* head, void(*print)(const MV*)) {
	//while (head->next != NULL) {
	Node* cur = head->next;
	printf("\n------영화 목록------\n");
	while (cur != NULL) {
		print(&cur->mv);
		cur = cur->next;
	}
}

//버퍼비우기 함수 (scanf가 남은 버퍼로 인하여 넘어가는것 방지)
void clearInputBuffer()
{
	while (getchar() != '\n');
}

//노드 추가 함수
void addNode(Node* head_ptr, MV mv) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	//find last node
	Node* last = head_ptr;
	while (last->next != NULL)
		last = last->next;

	last->next = newNode;

	newNode->mv = mv;
	newNode->next = NULL;
}

//영화 추가 함수 (노드 추가 함수 호출)
void addMV(Node* head_ptr, void(*add)(Node* head, MV mv)) {
	MV mv = {0, };
	printf("지시에 따라 정보를 입력해 주세요\n");

	//scanf 연속사용으로 버퍼비우기 필요
	printf("영화 이름 : ");
	//공백포함 X  -> 수정함, 이제 이름에는 공백포함 가능
	scanf("%s", &mv.name); 옛날버전
	clearInputBuffer();

	printf("영화 개봉일 (년 월 일) : ");
	scanf("%d %d %d", &mv.ymd.year, &mv.ymd.month, &mv.ymd.day);
	clearInputBuffer();

	printf("영화 상영 시간 : ");
	scanf("%d", &mv.run_time);
	clearInputBuffer();

	printf("영화 장르 : ");
	scanf("%s", &mv.genre);
	clearInputBuffer();

	printf("영화 배급사 : ");
	scanf("%s", &mv.film_dist);
	clearInputBuffer();

	add(head_ptr, mv);
}

//노드 삭제 함수 - 이름으로
void deleteNode(Node* head_ptr, char* name) {
	Node* target_ptr = head_ptr->next;
	Node* prev = head_ptr; //previous Node pointer

	while (target_ptr != NULL) {
		if (strcmp(target_ptr->mv.name, name) == 0) break;
		else {
			prev = target_ptr;
			target_ptr = target_ptr->next;
		}
	}

	//check Null pointer or 못찾은경우
	if (target_ptr == NULL) return;

	prev->next = target_ptr->next;

	//메모리 해제(운영체제에게 반납)
	free(target_ptr);
}

//영화 삭제 함수 (노드 삭제 함수 호출(이름))
void delMV(Node* head_ptr, void(*del)(Node* head, char* name)) {
	char name[100]; //동적할당으로 바꿔보기
	printf("지우고 싶은 영화의 이름을 입력하시오 : ");
	scanf("%s", &name);
	del(head_ptr, name);
}

//영화 검색 함수(검색 형식 함수 호출)
void searchMV(Node* head, int (*shouldprint)(const MV*, char* search), void(*print)(const MV*)) {
	Node* cur = head->next;
	char search[100]; //동적할당으로 바꿔보기
	scanf("%s", &search);

	printf("\n------검색 결과------\n");

	while (cur != NULL) {
		if (shouldprint(&cur->mv, search) == 1)
			print(&cur->mv);
		cur = cur->next;
	}
}

//검색 - 이름으로
int searchname(const MV* mv, char* name) {
	if (strcmp(mv->name, name) == 0) return 1;
	else return 0;
}

//검색 - 장르로
int searchgenre(const MV* mv, char* genre) {
	if (strcmp(mv->genre, genre) == 0) return 1;
	else return 0;
}

//검색 - 배급사로
int searchfilm_dist(const MV* mv, char* film_dist) {
	if (strcmp(mv->film_dist, film_dist) == 0) return 1;
	else return 0;
}

//movie.dat 파일을 읽어와 연결리스트를 생성하는 함수 (노드 추가 함수 호출)
void readMV(Node* head_ptr, void(*add)(Node* head, MV mv)) {
	FILE* fp = NULL;

	fp = fopen("movie.dat", "r");
	if (fp == NULL) {
		printf("Cannot open file\n");
		return;
	}

	while (!feof(fp)) {
		MV mv = {0, };

		//fscanf(fp, "%[^\n]s", &mv.name);
		//fscanf(fp, "%d %d %d %d %s %s\n", &mv.ymd.year, &mv.ymd.month, &mv.ymd.day, &mv.run_time, &mv.genre, &mv.film_dist);

		//이름은 띄어쓰기 가능, 이전버전 혹시몰라 남김
		fscanf(fp, "%s %d %d %d %d %s %s\n", &mv.name, &mv.ymd.year, &mv.ymd.month, &mv.ymd.day, &mv.run_time, &mv.genre, &mv.film_dist);
		add(head_ptr, mv);
	}
	fclose(fp);
}

//movie.dat 파일에 연결리스트에 있는 순서대로 뽑아와 저장하는 함수
void saveMV(Node* head) {
	FILE* fp = NULL;

	fp = fopen("movie.dat", "w");

	Node* cur = head->next;
	while (cur != NULL) {
		MV* mv = &cur->mv;
		//fprintf(fp, "%s\n", mv->name);
		//fprintf(fp, "%d %d %d %d %s %s\n", mv->ymd.year, mv->ymd.month, mv->ymd.day, mv->run_time, mv->genre, mv->film_dist);

		//이름은 띄어쓰기 가능, 이전버전 혹시몰라 남김
		fprintf(fp, "%s %d %d %d %d %s %s\n", mv->name, mv->ymd.year, mv->ymd.month, mv->ymd.day, mv->run_time, mv->genre, mv->film_dist);
		cur = cur->next;
	}
	fclose(fp);
}

//메인 함수, 주요 코드 포함 금지 -> 다 함수로 빼라!
int main() {
	//add node
	Node head;
	head.next = NULL;

	int n;

	while (1)
	{
		printf("====================\n");
		printf("영화 관리 프로그램\n");
		printf("1. 영화 목록 출력\n");
		printf("2. 영화 추가\n");
		printf("3. 영화 제거\n");
		printf("4. 영화 검색\n");
		printf("5. 저장된 영화 목록 불러오기\n");
		printf("6. 현재 영화 목록 저장\n");
		printf("7. 나가기\n");
		printf("====================\n");
		printf("원하시는 메뉴 : ");
		scanf("%d", &n);
		clearInputBuffer();

		if (n == 1)
		{
			printList(&head, printMVinfo);
		}

		else if (n == 2)
		{
			addMV(&head, addNode);
		}

		else if (n == 3)
		{
			delMV(&head, deleteNode);
		}

		else if (n == 4)
		{
			printf("====================\n");
			printf("1. 이름 검색\n");
			printf("2. 장르 검색\n");
			printf("3. 배급사 검색\n");
			printf("====================\n");
			scanf("%d", &n);
			clearInputBuffer();

			if (n == 1) {
				printf("영화 이름을 입력하시오 : ");
				searchMV(&head, searchname, printMVinfo);
			}
			else if (n == 2) {
				printf("영화 장르를 입력하시오 : ");
				searchMV(&head, searchgenre, printMVinfo);
			}
			else if (n == 3) {
				printf("영화 배급사를 입력하시오 : ");
				searchMV(&head, searchfilm_dist, printMVinfo);
			}
			else printf("제대로 입력하시오\n");
		}

		else if (n == 5)
		{
			readMV(&head, addNode);
		}

		else if (n == 6)
		{
			saveMV(&head);
		}

		else if (n == 7)
		{
			break;
		}

		else
		{
			printf("제대로 입력하시오\n");
			n = 0;
		}
	}

	return 0;
}
