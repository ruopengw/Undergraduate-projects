#include<stdio.h>
#include<time.h>
int MaxSum(int N);
int Array[500][500];
clock_t start,stop;
double duration;
int main()
{
	int N;
	int i,j,Max;
	int K,k;
	printf("Input a number N, using the N row N column of the matrix:\n");
	scanf("%d",&N);/* using N rows and N columns of array Array */
	printf("Input a number K, K is the number of cycles of functions:\n");
	scanf("%d",&K);/* number of iterations */ 
	printf("Matrix initialization:\n");
	for(i=0;i<N;i++)/* initialize the array Array[N][N] */ 
	for(j=0;j<N;j++)
	scanf("%d",&Array[i][j]);
	start=clock();
	for(k=0;k<K;k++)/* iterative for K */
	Max=MaxSum(N);
	stop=clock();
	printf("the maximum submatrix has the sum of %d\n",Max);
	duration=((double)(stop-start))/CLK_TCK;
	printf("The program runs %d times and consumes %lf seconds.\n",K,duration);
	printf("The program runs %d times and consumes %lf milliseconds.\n",K,stop-start);
	return 0;
}

int MaxSum(int N)
{
	int i,j,row_1,row_2,column_1,column_2;/* i, row_1, row_2 are row parameters; j, column_1, column_2 are column parameters */ 
	int Max,ThisSum;
	Max=ThisSum=0;/* initialize the Max and Thissum */
	for(column_1=0;column_1<N;column_1++)/* column_1 starts from the first column, when row_1>=N, column_1 plus one until colunm_1>=N */ 
	for(row_1=0;row_1<N;row_1++)/* row_1 starts from the first row, when row_2>=N, row_1 adds one until row_1>=N */
	for(row_2=row_1;row_2<N;row_2++)/*  row_2 starts from row_1, until row_2>=N */
	for(column_2=column_1;column_2<N;column_2++)/* column_2 starts from column_1, until column_2>=N */ 
	{
		ThisSum=0;/* every time column_2 changes, ThisSum returns to zero */ 
		for(j=column_1;j<=column_2;j++)/* j starts from column_1, until j>column_2 */ 
		for(i=row_1;i<=row_2;i++)/* i starts from row_1, until i>row_2 */
		ThisSum+=Array[i][j];/* sum from Array[row_1][colunm_1] to Array[row_2][colunm_2] */ 
		if(Max<ThisSum)
	    Max=ThisSum;/* update Max */
	}
	if(Max<=0)/* if Max<=0, return 0, otherwise return Max */ 
	return 0;
	return Max;
}
