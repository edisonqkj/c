#include <malloc.h>
#include <stdio.h>
#include "ds.c"

#define TRUE 1
#define FALSE 0
typedef int bool;

/// global variables
int width=10,height=10;
#include "data"/// image[10][10]
int move[2]={-1,1};

/// check valid
bool IsValid(int x, int y){
	if(x>=0 && x<width && y>=0 && y<height)
		return TRUE;
	return FALSE;
}

void Iteration(pIntStack stack, int ocolor, int ncolor){
	if(stack->Num>0){
		int y=IntPop(stack),
		    x=IntPop(stack);
		//printf("(%d,%d)\n",x,y);
		if(IsValid(x,y)){
			image[y][x]=ncolor;
			if(IsValid(x,y-1) && image[y-1][x]==ocolor){
				IntPush(stack,x);
				IntPush(stack,y-1);
			}
			if(IsValid(x,y+1) && image[y+1][x]==ocolor){
				IntPush(stack,x);
				IntPush(stack,y+1);
			}
			if(IsValid(x-1,y) && image[y][x-1]==ocolor){
				IntPush(stack,x-1);
				IntPush(stack,y);
			}
			if(IsValid(x+1,y) && image[y][x+1]==ocolor){
				IntPush(stack,x+1);
				IntPush(stack,y);
			}
			Iteration(stack,ocolor,ncolor);
		}
	}
}

/// paint
void Paint(int x, int y, int color){
	printf("Position: (%d,%d)\n",x,y);
	printf("Repaint color %d to %d\n",image[y-1][x-1],color);
	pIntStack stack=(pIntStack)malloc(sizeof(IntStack));
	IntInit(stack);
	/// push start point
	IntPush(stack,x-1);
	IntPush(stack,y-1);
	Iteration(stack,image[y-1][x-1],color);
}

/// print image
void Print(){
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			printf("%d ",image[i][j]);
		}
		printf("\n");
	}
}

void main(){
	printf("Original image:\n");
	Print();
	Paint(4,4,0);
	printf("Repainted image:\n");
	Print();
}
