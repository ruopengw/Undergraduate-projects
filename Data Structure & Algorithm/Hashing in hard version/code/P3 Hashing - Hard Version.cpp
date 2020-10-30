#include<stdio.h>
int judge(int m,int *newcell,int *cell,int N)  //Define a judgment function to determine whether the elements in the original sequence are inserted into the new hash at the same location each time.
{
	int k=m;
	k%=N;
	while(newcell[k]>=0)	//When conflict occurs, move backwards.
	{
		k++;
		k%=N;
	}
	if(cell[k]==m)		//If the insertion position in the new hash is the same as that in the old hash, the condition can be satisfied
	{
		return 1;
	}
	else return 0;
}
void insert(int m,int *newcell,int N)	//Define a function that inserts elements into a hash 
{
	int k=m;
	k%=N;
	while(newcell[k]>=0)		//When conflict occurs, move backwards.
	{
		k++;
		k%=N;
	}
	newcell[k]=m;			//After the corresponding position array subscript is found, the element is assigned to the array unit. 
}
int main()
{
	int N;					//define the size of the hash table
	scanf("%d",&N);
	int cell[1001];			//Define an array that accepts a sequence of elements 
	int newcell[1001]; 			//Define a re-inserted hash 
	int result[1001];			//Define the output result array 
	int i,j,min,num=-1,sum=0,k,flag=0;			//Define some cycles, counts, and flags 
	for(i=0;i<N;i++)
	newcell[i]=-1;			//Initialize all elements of the newly created hash array to -1 
	for(i=0;i<N;i++)		//Start accepting element sequences 
	{
		scanf("%d",&cell[i]); 		
		if(cell[i]>=0)
		sum++;					//Sum is used to record the number of elements in a table 
	} 
	for(i=0;i<sum;i++)			//Total cyclic sum times 
	{
		for(j=0;j<N;j++)		//Start at the head of the cell every time. 
		{
			if(cell[j]>=0)		//If the value of the element is less than 0 and is expressed as null, the following loop is not necessary
			{				
				if(flag==0&&judge(cell[j],newcell,cell,N))		//Find the first valid element value 
				{
					min=cell[j];
					flag=1;		
				}
				else if(flag==1&&judge(cell[j],newcell,cell,N))		//Find the values that hold in the subsequent loop and select the smaller values from them 
				{
					if(min>cell[j])
					min=cell[j];
				}
			}
		}
			if(flag!=0)						//When you find the next element, make some changes to the new and old hashes 
			{
				result[++num]=min;
				for(k=0;k<N;k++)	//Find the location of min in the old sequence
				{
					if(cell[k]==min)
					break;
			 	} 
			 	cell[k]=-1;		//Change the element stored in the corresponding location of the old hash to a negative number
				insert(min,newcell,N);	//Insert this element into the new hash 
			}
			flag=0;						//Change the marker variable of whether to find the next element to 0 
	}
	for(i=0;i<=num;i++)					//Circulation of output results 
	{
		if(i==0)		//When the output is the first, there is no space 
		printf("%d",result[i]);
		else
		printf(" %d",result[i]);
	}			
}
