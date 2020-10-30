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
	int column_1,column_2;
	int i,j;/* i is row parameter; j, column_1, column_2 are column parameters */ 
	int Max,ThisSum;
	Max=ThisSum=0;/* initialize the Max and Thissum */
	for(column_1=0;column_1<N;column_1++)/* column_1 starts from the first column, when column_2>=N, column_1 plus one until colunm_1>=N */ 
	for(column_2=column_1;column_2<N;column_2++)
	{
		ThisSum=0;/* every time column_2 changes, ThisSum returns to zero */ 
		for(i=0;i<N;i++)/* find the largest submatrix between column_1 and column_2 */
		for(j=column_1;j<=column_2;j++)
		{
			ThisSum+=Array[i][j];
			if(j==column_2)
			{
				if(ThisSum<0)
				ThisSum=0;
				else if(ThisSum>Max)
				Max=ThisSum;
			}
		}
	}
	if(Max<0)/* if Max<=0, return 0, otherwise return Max */ 
	return 0;
	return Max;
} 
