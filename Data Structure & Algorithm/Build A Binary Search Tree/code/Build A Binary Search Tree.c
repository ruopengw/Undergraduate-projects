#include <stdio.h>
#include <stdlib.h>

typedef struct Node *Tree;
struct Node{
    int key; /* the key of this node */
    Tree Left; /* point at its left son */
    Tree Right; /* point at its right son */
};

struct Information{
    int left;  /* index of the number of left son */
    int right;  /* index of the number of right son */
};

Tree BuildTree(int N, struct Information *info) /* build a new tree */
{
    Tree T[N]; /* define a tree with N nodes */
    int i;
    for(i=0;i<N;i++)  /* allocate space for the tree */
        T[i] = (Tree)malloc(sizeof(struct Node));
    for(i=0;i<N;i++)
    {
        if(info[i].left==-1)  /* has no left son */
            T[i]->Left = NULL;
        else
            T[i]->Left = T[info[i].left];  /* point at info[i].left */
        if(info[i].right==-1)  /* has no right son */
            T[i]->Right = NULL;
        else
            T[i]->Right = T[info[i].right];  /* point at info[i].right */
    }
    return T[0];  /* return the root */
}

int count = 0;
void InOrder (Tree T, int *num)  /* inorder traversal as well as input keys */
{
    if(T) /* not empty */
    {
        InOrder(T->Left, num);  /* inorder traversal the left subtree */
        T->key = num[count++];  /* input the sorted key into the node */
        InOrder(T->Right, num);  /* inorder traversal the right subtree */
    }
}

void LevelOrderPrint(Tree T)  /* level-order traversal as well as output */
{
    Tree queue[100];  /* create a queue to save all the nodes */
    int rear=0, i=0, temp=0;
    if(!T)  /* empty tree */
        return;
    else
    {
        queue[rear] = T;
        rear++;
        while(temp!=rear)
        {
            if(queue[temp]->Left)
            {
                queue[rear] = queue[temp]->Left;
                rear++;
            }
            if(queue[temp]->Right)
            {
                queue[rear] = queue[temp]->Right;
                rear++;
            }
            temp++;
        }
        for(i=0;i<rear-1;i++)  /* output the level order traversal sequence of the tree */
            printf("%d ",queue[i]->key);
        printf("%d\n",queue[i]->key);
        return;
    }
}

int main()
{
    int N;  /* the total number of nodes in the tree */
    int i,j,temp;
    scanf("%d\n",&N);
    int num[N];  /* save all the keys */
    struct Information info[N]; /* save all the relation information of each node */
    
    for(i=0;i<N;i++) /* input all the information of each node */
    {
        scanf("%d%d\n",&info[i].left,&info[i].right);
    }
    for(i=0;i<N;i++) /* input all the keys into an array */
    {
        scanf("%d",&num[i]);
    }
    for(i=0;i<N;i++)  /* bubble sort the array */
    {
        for(j=0;j<N-1;j++)
        {
            if(num[j]>num[j+1])
            {
                temp = num[j];
                num[j] = num[j+1];
                num[j+1] = temp;
            }
        }
    }
    /* the sequence of sorted array is the same as inorder traversal of BST */
    Tree BST = BuildTree(N, info); /* build a tree according to the information */
    InOrder(BST, num);  /* inorder traversal and input the sorted keys into the tree */
    LevelOrderPrint(BST);  /* print the keys in level order */
    return 0; 
}
