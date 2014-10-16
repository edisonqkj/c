#include <malloc.h>
#include <stdio.h>

#define INTQUEUE_EMPTY -9999

typedef struct IntNode{
	int data;
	struct IntNode* next;
}IntNode,*pIntNode;

typedef struct IntQueue{
	int Num;
	pIntNode front;
	pIntNode rear;
}IntQueue,*pIntQueue;

/// initialize int queue
void IntInit(pIntQueue queue){
	queue->Num=0;
	queue->rear=queue->front=NULL;
}

/// enqueue element into int queue
void IntEnqueue(pIntQueue queue, int element){
	if(queue==NULL)
	{
		printf("Queue is not initialized.\n");
		return;
	}
	pIntNode newNode=(pIntNode)malloc(sizeof(IntNode));
	newNode->data=element;
	newNode->next=NULL;
	if(queue->front==NULL)
	{
		queue->front=newNode;
		queue->rear=newNode;
	}
	else
	{
		queue->rear->next=newNode;
		queue->rear=newNode;
	}
	queue->Num++;
}

/// dequeue element out of int queue
int IntDequeue(pIntQueue queue){
	if(queue==NULL)
	{
		printf("Queue is not initialized.\n");
		return INTQUEUE_EMPTY;
	}
	if(queue->front==NULL)
	{
		printf("Queue is empty.\n");
		return INTQUEUE_EMPTY;
	}
	int head=queue->front->data;
	pIntNode tmp=queue->front;
	queue->front=queue->front->next;
	free(tmp);
	queue->Num--;
	return head;
}

/// get the head element of int queue
int IntHead(pIntQueue queue){
	if(queue==NULL)
	{
		printf("Queue is not initialized.\n");
		return INTQUEUE_EMPTY;
	}
	if(queue->front==NULL){
		printf("Queue is empty.\n");
		return INTQUEUE_EMPTY;
	}
	return queue->front->data;
}

/// destroy int queue
void IntDestroy(pIntQueue queue){
	if(queue==NULL)
	{
		return;
	}
	/// dequeue all elements
	while(queue->Num>0) IntDequeue(queue);
	queue->rear=queue->front=NULL;
}

void tmain(){
	pIntQueue p=(pIntQueue)malloc(sizeof(IntQueue));
	IntInit(p);
	for(int i=0;i<10;i++)
		IntEnqueue(p,i);
	printf("Head: %d\n",IntHead(p));
	for(int i=0;i<10;i++)
	{
		printf("NUm of Left Elements: %d\n",p->Num);
		IntDequeue(p);
	}
	//printf("%d\n",IntHead(p));
	IntDestroy(p);
}
