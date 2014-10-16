#include <malloc.h>
#include <stdio.h>

#define STACK_MAXIMUM_SIZE 8
#define INTSTACK_EMPTY -9999

typedef struct IntStack{
	int Num;
	int* top;
	int* base;
}IntStack,*pIntStack;

/// initialize int stack
void IntInit(pIntStack stack){
	stack->Num=0;
	stack->base=(int*)malloc(STACK_MAXIMUM_SIZE*sizeof(int));
	stack->top=stack->base;
}

/// push element into int stack
void IntPush(pIntStack stack, int element){
	if(stack==NULL)
	{
		printf("Stack is not initialized.\n");
		return;
	}
	if(stack->Num>STACK_MAXIMUM_SIZE){
		stack->base=(int*)realloc(stack->base,
			(stack->Num+STACK_MAXIMUM_SIZE)*sizeof(int));
	}
	*(stack->top)=element;
	stack->top++;//one sizeof(int) interval between top and base
	stack->Num++;
}

/// pop element out of int stack
int IntPop(pIntStack stack){
	if(stack==NULL)
	{
		printf("Stack is not initialized.\n");
		return INTSTACK_EMPTY;
	}
	if(stack->top==stack->base){
		return INTSTACK_EMPTY;//none element in int stack
	}
	stack->top--;
	stack->Num--;
	return *(stack->top);
}

/// get the top element of int stack
int IntPeak(pIntStack stack){
	if(stack==NULL)
	{
		printf("Stack is not initialized.\n");
		return INTSTACK_EMPTY;
	}
	if(stack->top==stack->base){
		return INTSTACK_EMPTY;
	}
	return *(stack->top-1);
}

/// destroy int stack
void IntDestroy(pIntStack stack){
	if(stack==NULL)
	{
		return;
	}
	/// pop out all elements
	while(stack->Num>0) IntPop(stack);
	free(stack->base);
	//free(stack->top);
	stack->base=stack->top=NULL;
}

void tmain(){
	pIntStack p=(pIntStack)malloc(sizeof(IntStack));
	IntInit(p);
	for(int i=0;i<10;i++)
		IntPush(p,i);
	printf("%d\n",IntPeak(p));
	for(int i=0;i<10;i++)
		printf("%d,%d\n",p->Num,IntPop(p));
	printf("%d\n",IntPeak(p));
	IntDestroy(p);
	//printf("%d\n",p->base==NULL);
}
