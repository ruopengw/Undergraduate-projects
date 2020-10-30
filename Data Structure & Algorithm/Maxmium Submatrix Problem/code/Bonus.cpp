#include<stdio.h>
#include<time.h>
int MaxSum(int N);
int Array_1[1000][1000];
int Array_2[1000];
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
            scanf("%d",&Array_1[i][j]);
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
	int row_1,row_2,i;/* row_1, row_2 are row parameters of array Array_1, i is column parameter of array Array_2 */
	int Max,ThisSum;
	Max=ThisSum=0;/* initialize the Max and Thissum */
	for(row_1=0;row_1<N;row_1++)
	{
		for(i=0;i<N;i++)/* every time row_1 plus one, array Array_2 returns to zero */
		Array_2[i]=0;
		for(row_2=row_1;row_2<N;row_2++)
		{
			for(i=0;i<N;i++)/* enumerate all subsequences of each column */
			Array_2[i]+=Array_1[row_2][i];
			ThisSum=0;
			for(i=0;i<N;i++)/* find the largest subsequence of array Array_2 */
			{
				ThisSum+=Array_2[i];
				if(ThisSum<0)
				ThisSum=0;
				else
				if(Max<ThisSum)
				Max=ThisSum;
			}
		}
	}
	if(Max<=0)/* if Max<=0, return 0, otherwise return Max */ 
	return 0;
	return Max;
}
