#include <stdio.h>
#include <stdlib.h> //malloc, free 함수 사용
#include <string.h> //메뉴 입력값을 받을 때 atoi 함수 사용
#pragma warning(disable : 4996)

//1. 할일 입력(시작시간, 소요시간, 주제)
//2. 할일 지우기
//3. 할 일 검색 (시작시간을 받아서 그에 해당하는 할 일 모두 출력

//이 값이 참이면 메시지를 영문으로 출력
int IsEng = 0; 

//각 노드는 할 일의 주제(문자열), 시작 시간(정수), 소요 시간(정수)의 집합인 구조체와 다음 노드를 가리키는 링크로 구성
typedef struct {
	char topic[20];
	int start_t;
	int estimated_t;
}element;
typedef struct Todo {
	element data;
	struct Todo *link;
}Todo;

//동적으로 노드 생성
Todo *create_todo(element data, Todo *link) 
{
	Todo *new_todo;
	new_todo = (Todo *)malloc(sizeof(Todo));
	if (new_todo == NULL) return;
	new_todo->data = data;
	new_todo->link = link;
	return new_todo;
}

//검색과 출력 모두 담당하는 알고리즘. 시작시간을 입력받으면 일치하는 모든 할 일을 출력만 하고 반환값은 없음.
void *search_print(Todo *phead, int start_time)
{
	Todo *p; 
	p = phead;
	//입력한 시간이 -1일 때 모든 작업을 출력
	if (start_time == -1){
			while (p != NULL) {
				//언어별로 출력을 다르게 함
				if (IsEng == 0) 
					printf("|ㅡ ● 시작시간: %d, 소요시간: %d, 주제: %s\n", p->data.start_t, p->data.estimated_t, p->data.topic);
				else 
					printf("|ㅡ ● From: %d, During: %d, Topic: %s\n", p->data.start_t, p->data.estimated_t, p->data.topic);
				
				p = p->link; //모든 노드를 순차적으로 방문
			}
			printf("\n");
		}

	//그 이외의 경우. 입력한 값과 일치하는 작업(==노드)들만 출력
	else {
		while (p != NULL) {
			if (p->data.start_t == start_time){
				//언어별로 출력을 다르게 함
				if(IsEng ==0) 
					printf("|ㅡ ● 시작시간: %d, 소요시간: %d, 주제: %s\n", p->data.start_t, p->data.estimated_t, p->data.topic);
				else
					printf("|ㅡ ● From: %d, During: %d, Topic: %s\n", p->data.start_t, p->data.estimated_t, p->data.topic);
			}
			p = p->link; //모든 노드를 방문하되, 조건에 맞는 값들만 출력한다.
		}
		printf("\n");
	}
}

//작업(노드) 삽입
void insert_todo(Todo **phead, Todo *new_node)
{
	// 1. 리스트가 비어있거나 
	// 2. 추가하려는 노드의 시작시간이 첫노드보다 작을 때(이를 때),
	// 3. 또는 추가하려는 노드의 시작시간이 첫노드와 동일하면서, 소요시간이 더 짧을 때 
	// (위 조건 중 하나라도 빠뜨릴 경우, 오름차순 정렬에 반드시 문제가 생긴다)

	if (*phead == NULL || (*phead)->data.start_t > new_node->data.start_t 
		|| (((*phead)->data.start_t == new_node->data.start_t) 
			&& ((*phead)->data.estimated_t >= new_node->data.estimated_t)))
	{	
		new_node->link = *phead;
		*phead = new_node;
	}

	//위의 경우 이외의, 모든 일반적인 경우 노드 삽입을 오름차순으로 수행하는 알고리즘. 
	else{
		Todo *current = *phead; //헤드포인터에서 시작하는 임시변수.
		// 현재 인덱스의 다음 노드가 추가하려는 노드의 시작시간보다 작을 때(이를 때) 위치탐색 수행
		while (current->link != NULL && current->link->data.start_t < new_node->data.start_t) 	{
				current = current->link; // 시작 시간이 같아질 때까지 인덱스를 다음 노드로 옮긴다.
				if (current->link == NULL || current->link->data.start_t == new_node->data.start_t) break;	// 같은 시작 시간을 만나면 현재 루프를 탈출하고 다음의 조건부 루프로 이동.
		}
		// 인덱스와 추가하는 노드의 시작시간이 같으면서
		// 인덱스의 다음 노드의 소요시간이 추가하는 노드의 소요시간보다 작거나 같을 때 위치탐색 수행.	
		while (current->link != NULL && current->link->data.start_t == new_node->data.start_t 
			&& current->link->data.estimated_t <= new_node->data.estimated_t) 
			current = current->link;

		new_node->link = current->link; //탐색을 멈추었을 때 그 위치에 노드를 삽입한다.
		current->link = new_node;
	}
}

//작업(노드) 삭제: 지울 노드의 위치를 탐색하기 위해 헤드포인터와 키 값을 인자로 받음.
//헤드의 값이 나중에 바뀔 수도 있기 때문에 새로운 헤드포인터의 값을 리턴함.
Todo *delete_todo(Todo *head, int start_time)
{
	if (head == NULL) 
		return; //리스트가 비어있을 땐 함수를 종료.
	else {
		// -1을 사용자가 입력했을 때 모든 노드를 삭제하는 알고리즘.
		if (start_time == -1){
			struct Todo *temp = head;
			// 노드의 링크가 NULL을 만날 때까지 모든 노드를 하나씩 방문하며 순차적으로 지워나간다.
			while (head != NULL){
				temp = head;
				head = head->link;
				free(temp);
			}
			if (IsEng == 0)
				printf("\n  ㅡ       Message :: 모두 삭제되었습니다!       ㅡ \n\n\n");
			else
				printf("\n  ㅡ     Message :: Successfully deleted ALL tasks!     ㅡ \n\n\n");		
		}
		else {
			// 삭제하는 노드가 리스트의 바로 첫번째에 있을 경우
			while (head && head->data.start_t == start_time)
			{
				Todo *tmp = head;
				head = head->link;
				free(tmp);
			}
			// 삭제하려는 노드가 처음이 아닌 다른 모든 위치에 있을 때의 알고리즘.
			// 인덱스 포인터의 역할을 하는 current가 Head부터 시작하여 증가하면서 
			// 인덱스가 리스트의 끝에 다다르지 않았으면서 사용자가 지정한 작업들만 선택적으로 지워나간다.
			for (Todo *current = head; current != NULL; current = current->link){
				while (current->link != NULL && current->link->data.start_t == start_time){
					Todo *tmp = current->link; //임시 변수에 저장한다.
					current->link = tmp->link;
					free(tmp);
				}
			}
			//언어에 따라 메시지 다르게 출력
			if (IsEng == 0)
				printf("\n  ㅡ  Message :: 선택한 작업이 삭제되었습니다!  ㅡ  \n\n\n");
			else
				printf("\n  ㅡ   Message :: Successfully deleted selected tasks!   ㅡ \n\n\n");
		}
	}	
	return head; //리턴을 안 해주면 리스트를 편집하거나 작업할 때 엉뚱한 값을 가리킬 수 있음. 
}

//메인 함수: 한글과 영어로 나누어져 있다.
//사용자가 1 or 2 or 3 이외의 키를 눌러서 종료시킬 때까지 무한루프로 동작.
int main() 
{
	Todo *Base = NULL;
	Todo *p;
	element user_input; //사용자가 입력한 데이터 구조체를 저장하는 임시 변수. 노드 추가에 이용됨.
	char selection[2];
	char lang = '0' ;
	
	printf("\n\n\n  ㅡ Select your language  /  언어를 선택해 주십시오  ㅡ  \n"); //언어 설정 페이지.
	printf("  ㅡ    한국어를 사용하시려면 숫자 0을 입력하세요.    ㅡ\n");
	printf("  ㅡ     If you want to choose English, press 1.      ㅡ\n");
	printf("  ㅡ  Press other key to exit / 다른 키를 누르면 종료 ㅡ\n");
	printf("\n>> 언어 입력: ");
	scanf("%c", &lang);

	if (lang == '0') { // 한국어
		IsEng = 0;
		printf("\n>> 언어가 한국어로 설정되었습니다!\n");
		printf(">> 언어를 변경하시려면 프로그램을 다시 시작해야 합니다.\n\n\n");
		do{
			//메인메뉴
			printf("\n\n===============    MY TODO :: Main    ===============\n");
			printf("=                                                   =\n");
			printf("=   ☞  1: 할 일 입력                               =\n");
			printf("=   ☞  2: 할 일 삭제                               =\n");
			printf("=   ☞  3: 할 일 검색                               =\n");
			printf("=   ☞  그 이외의 키: 나가기                        =\n");
			printf("=                                                   =\n");
			printf(">> 명령 입력: ");
			scanf("%s", &selection);

			//할 일 입력
			if (atoi(selection) == 1) {
				printf("\n\n\n  ■ 추가할 작업의 이름/주제: ");
				scanf("%s", &user_input.topic);
				printf("  ■ 추가할 작업의 시작 시간: ");
				scanf("%d", &user_input.start_t);
				printf("  ■ 추가할 작업의 소요 시간: ");
				scanf("%d", &user_input.estimated_t);
				insert_todo(&Base, create_todo(user_input, NULL));
				printf("\n>> (i) 작업이 추가되었습니다!\n\n\n\n");
			}
			//할 일 삭제
			else if (atoi(selection) == 2) {
				if (Base == NULL){
					printf("\n    ㅡ       (!) 저장된 작업이 없습니다.         ㅡ");
					printf("\n    ㅡ        초기화면으로 되돌아갑니다.         ㅡ\n\n\n");
					continue; //리스트가 비어있으면 다시 메인메뉴로 돌아감
				}
				int keyword;
				char tf[2];
				printf("\n\n\n\n============== {{ MY TODO >> Delete }} ==============\n");
				printf("=                                                   =\n");
				printf("=   ☞  삭제할 작업의 시작 시간 입력(-1: 모두 삭제): \n>> ");
				scanf("%d", &keyword);
				printf("\n");
				search_print(Base, keyword); // 삭제할 작업들을 출력하기 위해 호출.
				Base = delete_todo(Base, keyword); // 노드 삭제 함수에서 리턴받은 새로운 헤드포인터로 초기화하여 세그민테이션 폴트를 방지.
				printf("\n\n");
				continue;
			}
			//할 일 검색
			else if (atoi(selection) == 3) {
				if (Base == NULL){
					printf("\n    ㅡ       (!) 저장된 작업이 없습니다.         ㅡ");
					printf("\n    ㅡ        초기화면으로 되돌아갑니다.         ㅡ\n\n\n");
					continue; //리스트가 비어있으면 다시 메인메뉴로 돌아감
				}
				int keyword;
				printf("=                                                   =\n\n\n");
				printf(">> 검색할 작업의 시작 시간 입력(-1: 모두 출력) : ");
				scanf("%d", &keyword);
				printf("\n<<  검  색  결  과 >> \n\n|\n");
				search_print(Base, keyword);
				printf("\n\n");
				continue;
			}
			else
				break; // 1~3 이외의 키를 누르면 루프를 탈출하여 프로그램을 종료시킨다.
		} while (atoi(selection) >= 1 && atoi(selection) <= 3); //1, 2, 3 이외의 모든 키를 누르면 탈출하는 무한루프로 구성
	}

	// 언어를 영어로 설정했을 때. 출력하는 메시지의 언어만 다르고 코드는 위의 한국어 메뉴와 동일함.
	else if (lang == '1') {
		IsEng = 1;
		printf("\n>> Message: Your language was set to English.\n");
		printf(">> You should restart the app if you want to change language.\n\n\n");
		//메인메뉴
		do{
			printf("\n\n===============    MY TODO :: Main    ===============\n");
			printf("=                                                   =\n");
			printf("=   ☞  1: Add a task                               =\n");
			printf("=   ☞  2: Delete a task                            =\n");
			printf("=   ☞  3: Search your tasks                        =\n");
			printf("=   ☞  (Other Keys): Exit this program             =\n");
			printf("=                                                   =\n");
			printf(">> Type: ");
			scanf("%s", &selection);

			//할 일 입력
			if (atoi(selection) == 1) {
				printf("\n\n\n  ■ Name of new task: ");
				scanf("%s", &user_input.topic);
				printf("  ■ Starting from: ");
				scanf("%d", &user_input.start_t);
				printf("  ■ Estimated time: ");
				scanf("%d", &user_input.estimated_t);
				insert_todo(&Base, create_todo(user_input, NULL));
				printf("\n>> Message: Your task is added to your ToDo list.\n\n\n\n");
			}
			//할 일 삭제
			else if (atoi(selection) == 2) {
				if (Base == NULL)
				{
					printf("\n    ㅡ               (!) NO TASKS.                 ㅡ");
					printf("\n    ㅡ        Heading back to main menu...         ㅡ\n\n\n");
					continue; //리스트가 비어있으면 다시 메인메뉴로 돌아감
				}
				int keyword;
				char tf[2];
				printf("\n\n\n\n============== {{ MY TODO >> Delete }} ==============\n");
				printf("=                                                   =\n");
				printf(">>  Type the time of the task(s) you want to delete (-1: Delete ALL): \n>>  ");
				scanf("%d", &keyword);
				search_print(Base, keyword); // 삭제할 작업들을 출력하기 위해 호출.
				Base = delete_todo(Base, keyword); // 노드 삭제 함수에서 리턴받은 새로운 헤드포인터로 초기화하여 세그민테이션 폴트를 방지.
				printf("\n\n");
				continue;
			}
			//할 일 검색
			else if (atoi(selection) == 3) {
				if (Base == NULL){
					printf("\n    ㅡ               (!) NO TASKS.                 ㅡ");
					printf("\n    ㅡ        Heading back to main menu...         ㅡ\n\n\n");
					continue; //리스트가 비어있으면 다시 메인메뉴로 돌아감
				}
				int keyword;
				printf("=                                                   =\n\n\n");
				printf(">>  Type the time of the task(s) you want to search.\n");
				printf(">>  (-1: Print ALL) : \n>> ");
				scanf("%d", &keyword);
				search_print(Base, keyword);
				printf("\n\n");
				continue; //함수를 종료시킬 때 컨티뉴를 쓰지 않는다면 Variable Corrupted 오류가 발생한다.
			}
			else // 1~3 이외의 키를 누르면 루프를 탈출하여 프로그램을 종료시킨다.
				break;
		} while (atoi(selection) >= 1 && atoi(selection) <= 3); //1, 2, 3 이외의 모든 키를 누르면 탈출하는 무한루프로 구성
	}
	printf("\n\n\n ㅡ  <<  MY TODO. Developed by Somniuw.  >>  ㅡ \n\n\n"); 
	return 0;
}
