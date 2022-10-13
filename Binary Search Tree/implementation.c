#include <stdio.h>
#include <stdlib.h>

//이진 탐색 트리 노드의 구조
typedef struct {
	int k;
	struct TreeNode *left, *right;
} TreeNode;

//삽입함수
void insert(TreeNode **root, int k)
{
	TreeNode *p, *t; 
	TreeNode *n;
	t = *root;
	p = NULL;

	//키값을 갖는 노드 t를 탐색, p는 t의 부모노드
	while (t != NULL)
	{
		//선행탐색을 수행해서 추가하고자 하는 노드가 이미 있으면 함수 종료,
		if (k == t->k) return;

		p = t;
		if (k < p->k) t = p->left;
		else t = p->right;
	}

	//없으면 새로 넣어준다.
	n = (TreeNode *)malloc(sizeof(TreeNode));
	if (n == NULL) return;
	n->k = k;
	n->left = n->right = NULL;

	//부모노드와 이어준다.
	if (p != NULL)
		if (k < p->k)
			p->left = n;
		else p->right = n;
	else *root = n;
		
	printf("\n:: %d 이(가) 삽입됨.", k);		
}


//삭제 함수
void delete (TreeNode **root, int k)
{
	TreeNode *p, *child, *succ, *succ_p, *t;

	//키값을 갖는 노드 t를 탐색, p는 t의 부모노드
	p = NULL;
	t = *root;

	//키값을 갖는 노드 t를 탐색
	while (t != NULL && t->k != k)
	{
		p = t;
		t = (k < p->k) ? p->left : p->right;
	}

	//탐색 종료된 시점에 t가 NULL이라면 트리 안에 키값이 없으므로,
	if (t == NULL)
	{
		printf("검색 결과 없음\n"); //메시지 출력하고 함수 종료
		return;
	}

	//Case 1: 단말 노드일 때
	if ((t->left == NULL) && (t->right == NULL))
	{
		if (p != NULL)
		{
			if (p->left == t) 
				p->left = NULL;
			else p->right = NULL;
		}
		else
			*root = NULL;
	}

	//Case 2: 하나의 자식만 가지는 경우
	else if ((t->left == NULL) || (t->right == NULL))
	{
		child = (t->left != NULL) ? t->left : t->right; //좌측이 비어있지 않으면 좌측이, 비어있다면 우측이 자식노드가 된다.
		if (p != NULL) {
			if (p->left == t)
				p->left = child;
			else p->right = child;
		}
		else *root = child;
	}

	//Case 3: 두 개의 자식을 가지는 경우
	else
	{
		//오른쪽 부분 트리에서 후계자 찾기
		succ_p = t;
		succ = t->right;

		//찾아서 계속 왼쪽으로 이동
		while (succ->left != NULL) {
			succ_p = succ;
			succ = succ->left;
		}

		//후속자의 부모와 자식을 연결시킨다
		if (succ_p->left == succ) 
			succ_p->left = succ->right;
		else 
			succ_p->right = succ->right;
		//후속자가 가진 키 값을 현재 노드에 복사
		t->k = succ->k;
		//원래의 후속자 삭제
		t = succ;
	}
	free(t); //제거한 노드는 메모리에서 할당 해제해준다.
	printf("\n:: %d 이(가) 삭제됨.", k);
}

int leaf_count = 0; //단말노드의 갯수를 세어주는 정수형 변수 선언.
void print_leaf(TreeNode* tree) {

	if (tree->left == NULL && tree->right == NULL) {
		//단말노드를 찾았을 때 바로 그 값을 출력하고 카운트 값을 1 증가시킨다.
		printf("%d ", tree->k);
		leaf_count++;
	}
	else {
		//왼쪽 서브트리에 재귀적으로 이 함수를 다시 호출한다.
		if (tree->left != NULL) {
			print_leaf(tree->left);
		}
		//오른쪽 서브트리에 재귀적으로 이 함수를 다시 호출한다.
		if (tree->right != NULL) {
			print_leaf(tree->right);
		}
	}
}

int main(void)
{
	TreeNode *d = NULL; //비어있는 이진 트리 선언.

	//삽입
	insert(&d, 10); 
	insert(&d, 15);
	insert(&d, 8);
	insert(&d, 18);
	insert(&d, 7);
	insert(&d, 1);
	insert(&d, 5);
	insert(&d, 9);
	insert(&d, 17);
	insert(&d, 11);
	insert(&d, 12);
	insert(&d, 30);
	insert(&d, 25);
	insert(&d, 6);
	insert(&d, 32);
	printf("\n");

	//삭제
	delete(&d, 9);
	delete(&d, 7);
	delete(&d, 10);
	
	printf("\n\n> 단말노드: ");
	print_leaf(d); //단말노드 출력함수
	printf("\n> 단말노드의 개수: %d\n\n", leaf_count); //단말노드의 개수 출력
	
	return 0;
}
