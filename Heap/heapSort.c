#include <stdio.h>

int arr[] = { 10, 15, 8, 18, 7, 1, 5, 9, 17, 11, 12, 30, 25, 6, 32 };

#define SIZE (sizeof(arr)/sizeof(int))
int hsize = SIZE;

//포인터를 이용한 스왑 함수
void swap(int* a, int* b) { 
	int temp = *a; 
	*a = *b; 
	*b = temp;
}

//최대힙 구성 함수
void maxHeapify(int *arr, int i) {
	int max;

	//왼쪽 자식 노드의 인덱스를 찾는다.
	int left = 3 * i + 1;

	//왼쪽 자식 노드와 현재 인덱스의 노드의 값을 비교해서
	if (left < hsize && arr[left] > arr[i]) max = left; //왼쪽 자식이 더 크면 그걸로 가장 큰 값이라고 지정해둔다.
	else max = i; //그렇지 않으면 현재 인덱스 값으로 지정.

	//가운데 자식 노드의 인덱스를 찾는다.
	int middle = 3 * i + 2;

	//가운데 자식 노드를 지금까지 찾아낸 가장 큰 값의 노드와 비교해서
	if (middle < hsize && arr[middle] > arr[max])
		max = middle; //가운데 자식 노드가 더 크면 가장 큰 값이라고 지정해둔다.

	//오른쪽 자식 노드의 인덱스를 찾는다.
	int right = 3 * i + 3;

	//오른쪽 자식 노드와 현재 인덱스의 노드의 값을 비교해서
	if (right < hsize && arr[right] > arr[max]) //오른쪽 자식이 더 크면 그걸 가장 큰 값이라고 지정해둔다.
		max = right;

	//만약 현 위치의 노드보다 자식노드들의 값들이 더 크다면 
	if (max != i) {
		//만약 가장 큰 노드가 현위치의 노드랑 일치하지 않을 때
		//현 위치와 가장 큰 값을 지닌 노드를 맞바꾼다.
		swap(&arr[i], &arr[max]);

		//이 때 자식노드와 상위노드의 값이 교환됨에 의해 최대힙의 특성이 아니게 될 여지가 생겼으므로, 
		//변경된 노드에 대해서 재귀적으로 최대힙 구성 함수를 다시 호출한다.
		maxHeapify(arr, max);
	}
}

//최대힙 초기화 함수
void init_maxheap(int *arr) {
	int j;
	//삼진힙의 특성을 적용한다.
	for (j = hsize / 3 - 1; j >= 0; --j)
		maxHeapify(arr, j);
}

//삼진힙 정렬 함수
void heapsort(int *arr) {
	init_maxheap(arr); //우선 arr 배열을 인자로 최대힙을 구성한다.

	for (int i = hsize - 1; i > 0; --i) //아래의 과정을 힙사이즈(== 구현한 배열의 길이)가 1이 될 때까지 계속 반복한다.
	{
		//최대힙의 최대값을 수열의 맨뒤로 보내고, 
		swap(&arr[0], &arr[i]);

		// 힙의 크기를 줄인다.
		--hsize;

		//배열의 내용이 변경되었으므로, 변경된 배열에 대해 다시 최대힙을 구성한다.
		maxHeapify(arr, 0);
	}
}

int main() 
{
	// > 정렬 전
	printf("> 삼진힙 정렬 전 배열: ");
	for (int i = 0; i< SIZE; ++i) 
		printf("%d ", arr[i]); //정렬 전 원소 출력

	// > 정렬 후
	printf("\n> 삼진힙 정렬 후 배열: ");
	heapsort(arr); //힙 정렬 수행
	for (int i = 0; i< SIZE; ++i)
		printf("%d ", arr[i]); //정렬 후 원소 출력
	printf("\n");

	return 0;
}
