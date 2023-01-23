//2-D Sudoku
#include<stdio.h>
#include<conio.h>
#include<alloc.h>

#define N 3

struct node
{
int i , j , data;
int possibilities[N*N];
struct node *nextNode;

struct sameRow *sameRowPtr;
struct sameColumn *sameColumnPtr;
struct sameSquare *sameSquarePtr;
};

struct sameRow
{
struct sameRow *nextSameRow;
struct node *nodeRowPtr;
};

struct sameColumn
{
struct sameColumn *nextSameColumn;
struct node *nodeColumnPtr;
};

struct sameSquare
{
struct sameSquare *nextSameSquare;
struct node *nodeSquarePtr;
};

									 /////////////////////////////////////////////////

void CreateList(int a[N*N][N*N] , struct node **p);
struct node* CreateNode(int a[N*N][N*N] , int i , int j);
void CompilePossibilities(int a[N*N][N*N] , int i , int j , int x[N*N]);

void CreateSameRowList(struct node **p);
void CreateSameColumnList(struct node **p);
void CreateSameSquareList(struct node **p);

void SolveList(int a[N*N][N*N] , struct node **p , struct node *n1);
void EliminatePossibilities(struct node *n1 , int m);
int CheckPossibilities(struct node *n1);
void RecreatePossibilities(struct node *n1 , int m);

void Sudoku(int a[N*N][N*N]);
int CheckSudoku(int a[N*N][N*N] , struct node **p);
void display(int a[N*N][N*N] , struct node **p);

									 /////////////////////////////////////////////////

									 ///////////////////////////////////////////////////////////




void main()
{


int a[N*N][N*N] =
/*
		    { {1,0,0, 3,0,4, 0,6,5} ,
		    {0,0,5, 0,0,0, 2,0,0} ,
		    {0,0,4, 9,0,8, 1,0,0} ,

		    {0,5,0, 4,0,1, 0,8,0} ,
		    {0,0,0, 0,0,0, 0,0,0} ,
		    {0,2,0, 6,0,5, 0,9,0} ,

		    {0,0,8, 7,0,2, 3,0,0} ,
		    {0,0,3, 0,0,0, 6,0,0} ,
		    {5,4,0, 8,0,3, 0,1,7} };
*/




		 {  {8,0,0, 0,0,0, 0,0,0} ,
		    {0,0,3, 6,0,0, 0,0,0} ,
		    {0,7,0, 0,9,0, 2,0,0} ,

		    {0,5,0, 0,0,7, 0,0,0} ,
		    {0,0,0, 0,4,5, 7,0,0} ,
		    {0,0,0, 1,0,0, 0,3,0} ,

		    {0,0,1, 0,0,0, 0,6,8} ,
		    {0,0,8, 5,0,0, 0,1,0} ,
		    {0,9,0, 0,0,0, 4,0,0} };

	 clrscr();
Sudoku(a);
}



void CreateList(int a[N*N][N*N] , struct node **p)
{
int i,j;
struct node *newnode;
struct node *temp;

x1:
for(i=0 ; i<N*N ; i++)
   {
   for(j=0 ; j<N*N ; j++)
      {
	  if(a[i][j]==0)
	    {
		newnode = CreateNode(a,i,j);
		if(newnode==NULL)
		  {
		  *p = NULL;
		  goto x1;
		  }
		else
		  {
		  if(*p==NULL)
		    {
			*p = newnode;
		    temp = newnode;
			}
		  else
		    {
			temp->nextNode = newnode;
			temp = temp->nextNode;
			}
		  }
		}
	  }
   }
}

struct node* CreateNode(int a[N*N][N*N] , int i , int j)
{
int x[N*N] = {0} , m , temp1 , temp2;
struct node *newnode;

CompilePossibilities(a,i,j,x);

for(m=0 , temp1=0 , temp2=0 ; m<N*N ; m++)
   {
   if(x[m]!=0)
     {
	 temp1++;
	 temp2 = x[m];
	 }
   }
if(temp1==0)
  {
  printf("\n a[%d][%d] = -1 \n Wrong Sudoku \n",i,j);
  a[i][j] = -1;
  getch();
  exit(-1);
  return NULL;
  }
if(temp1==1)
  {
  a[i][j] = temp2;
  return NULL;
  }

newnode = (struct node*)malloc(sizeof(struct node));
for(m=0 ; m<N*N ; m++)
   newnode->possibilities[m] = x[m];
newnode->data = 0;
newnode->i = i;
newnode->j = j;
newnode->nextNode = NULL;
newnode->sameRowPtr = NULL;
newnode->sameColumnPtr = NULL;
newnode->sameSquarePtr = NULL;

return newnode;
}

void CompilePossibilities(int a[N*N][N*N] , int i , int j , int x[N*N])
{
int m , l , row[N*N] = {0} , column[N*N] = {0} , square[N*N] = {0};

for(m=0 ; m<N*N ; m++)
   if(a[i][m]!=0)
      row[a[i][m]-1] = a[i][m];

for(m=0 ; m<N*N ; m++)
   if(a[m][j]!=0)
      column[a[m][j]-1] = a[m][j];

for( l = ((int)(i/N))*N ; l <= ((((int)i/N)+1)*N)-1 ; l++)
   {
   for( m = ((int)(j/N))*N ; m <= ((((int)j/N)+1)*N)-1 ; m++)
      {
	  if(a[l][m]!=0)
	     square[a[l][m]-1] = a[l][m];
	  }
   }

for(m=0 ; m<N*N ; m++)
   if(row[m]==0 && column[m]==0 && square[m]==0)
      x[m] = m+1;
}
                                     
									 ///////////////////////////////////////////////////////////
									 
void CreateSameRowList(struct node **p)
{
struct node *temp1 , *temp2;
struct sameRow *newSame , *tempSame;

for(temp1 = *p ; temp1!=NULL ; temp1 = temp1->nextNode)
   {
   for(temp2 = *p ; temp2!=NULL ; temp2 = temp2->nextNode)
      {
	  if(temp2==temp1) continue;
	  else
	    {
		if(temp2->i==temp1->i)
		  {
		  newSame = (struct sameRow*)malloc(sizeof(struct sameRow));
		  newSame->nodeRowPtr = temp2;
		  newSame->nextSameRow = NULL;
		  if(temp1->sameRowPtr==NULL)
		    {
			temp1->sameRowPtr = newSame;
			tempSame = newSame;
			}
		  else
		    {
			tempSame->nextSameRow = newSame;
			tempSame = tempSame->nextSameRow;
			}
		  }
		}
	  }
   }
}

void CreateSameColumnList(struct node **p)
{
struct node *temp1 , *temp2;
struct sameColumn *newSame , *tempSame;

for(temp1 = *p ; temp1!=NULL ; temp1 = temp1->nextNode)
   {
   for(temp2 = *p ; temp2!=NULL ; temp2 = temp2->nextNode)
      {
	  if(temp2==temp1) continue;
	  else
	    {
		if(temp2->j==temp1->j)
		  {
		  newSame = (struct sameColumn*)malloc(sizeof(struct sameColumn));
		  newSame->nodeColumnPtr = temp2;
		  newSame->nextSameColumn = NULL;
		  if(temp1->sameColumnPtr==NULL)
		    {
			temp1->sameColumnPtr = newSame;
			tempSame = newSame;
			}
		  else
		    {
			tempSame->nextSameColumn = newSame;
			tempSame = tempSame->nextSameColumn;
			}
		  }
		}
	  }
   }
}

void CreateSameSquareList(struct node **p)
{
struct node *temp1 , *temp2;
struct sameSquare *newSame , *tempSame;

for(temp1 = *p ; temp1!=NULL ; temp1 = temp1->nextNode)
   {
   for(temp2 = *p ; temp2!=NULL ; temp2 = temp2->nextNode)
      {
	  if(temp2==temp1) continue;
	  else
	    {
	    if( (((int)((temp1->i)/N))*N <= temp2->i) && (temp2->i <= ((((int)((temp1->i)/N))+1)*N)-1) )
		if( (((int)((temp1->j)/N))*N <= temp2->j) && (temp2->j <= ((((int)((temp1->j)/N))+1)*N)-1) )
		  {
		  if(temp2->i!=temp1->i && temp2->j!=temp1->j)
		  {
		  newSame = (struct sameSquare*)malloc(sizeof(struct sameSquare));
		  newSame->nodeSquarePtr = temp2;
		  newSame->nextSameSquare = NULL;
		  if(temp1->sameSquarePtr==NULL)
		    {
			temp1->sameSquarePtr = newSame;
			tempSame = newSame;
			}
		  else
		    {
			tempSame->nextSameSquare = newSame;
			tempSame = tempSame->nextSameSquare;
			}
		  }
		  }
		}
	  }
   }
}
									 
									 ///////////////////////////////////////////////////////////
									 
void SolveList(int a[N*N][N*N] , struct node **p , struct node *n1)
{
int m,t;

for(m=1 ; m<=N*N ; m++)
   {
   if(n1->possibilities[m-1]>0)
     {
     EliminatePossibilities(n1,m);
     t = CheckPossibilities(n1);
     if(t!=-1)
       {
	   n1->data = m;
	   if(n1->nextNode!=NULL)
	     {
	     SolveList(a,p,n1->nextNode);
	     }
	   else
	     {
	     display(a,p);                      /////////////// SUCCESS
	     /* exit(-1);  //For single solution Sudoku */
	     }
	   }
     n1->data = 0;
     RecreatePossibilities(n1,m);
     }
   }
}

void EliminatePossibilities(struct node *n1 , int m)    //m : 1 to N*N
{
struct sameRow *tempSameRow;
struct sameColumn *tempSameColumn;
struct sameSquare *tempSameSquare;

for(tempSameRow = n1->sameRowPtr ; tempSameRow!=NULL ; tempSameRow = tempSameRow->nextSameRow)
   {
   if(tempSameRow->nodeRowPtr->data==0 && tempSameRow->nodeRowPtr->possibilities[m-1]!=0)
     {
	 if(tempSameRow->nodeRowPtr->possibilities[m-1] >0)
	   {
	   tempSameRow->nodeRowPtr->possibilities[m-1] = -1;
	   }
	 else
	   {
	   (tempSameRow->nodeRowPtr->possibilities[m-1])--;
	   }
	 }
   }

for(tempSameColumn = n1->sameColumnPtr ; tempSameColumn!=NULL ; tempSameColumn = tempSameColumn->nextSameColumn)
   {
   if(tempSameColumn->nodeColumnPtr->data==0 && tempSameColumn->nodeColumnPtr->possibilities[m-1]!=0)
     {
	 if(tempSameColumn->nodeColumnPtr->possibilities[m-1] >0)
	   {
	   tempSameColumn->nodeColumnPtr->possibilities[m-1] = -1;
	   }
	 else
	   {
	   (tempSameColumn->nodeColumnPtr->possibilities[m-1])--;
	   }
	 }
   }

for(tempSameSquare = n1->sameSquarePtr ; tempSameSquare!=NULL ; tempSameSquare = tempSameSquare->nextSameSquare)
   {
   if(tempSameSquare->nodeSquarePtr->data==0 && tempSameSquare->nodeSquarePtr->possibilities[m-1]!=0)
     {
	 if(tempSameSquare->nodeSquarePtr->possibilities[m-1] >0)
	   {
	   tempSameSquare->nodeSquarePtr->possibilities[m-1] = -1;
	   }
	 else
	   {
	   (tempSameSquare->nodeSquarePtr->possibilities[m-1])--;
	   }
	 }
   }
}

int CheckPossibilities(struct node *n1)
{
int l , temp1;
struct node *tempNode;
struct sameRow *tempSameRow;
struct sameColumn *tempSameColumn;
struct sameSquare *tempSameSquare;

for(tempSameRow = n1->sameRowPtr ; tempSameRow!=NULL ; tempSameRow = tempSameRow->nextSameRow)
   {
   if(tempSameRow->nodeRowPtr->data==0)
   {
   for(l=0 , temp1=0 , tempNode = tempSameRow->nodeRowPtr ; l<N*N ; l++)
      {
	  if(tempNode->possibilities[l]>0)
	      temp1++;
	  }
   if(temp1==0)
       return -1;
   }
   }

for(tempSameColumn = n1->sameColumnPtr ; tempSameColumn!=NULL ; tempSameColumn = tempSameColumn->nextSameColumn)
   {
   if(tempSameColumn->nodeColumnPtr->data==0)
   {
   for(l=0 , temp1=0 , tempNode = tempSameColumn->nodeColumnPtr ; l<N*N ; l++)
      {
	  if(tempNode->possibilities[l]>0)
	      temp1++;
	  }
   if(temp1==0)
       return -1;
   }
   }

for(tempSameSquare = n1->sameSquarePtr ; tempSameSquare!=NULL ; tempSameSquare = tempSameSquare->nextSameSquare)
   {
   if(tempSameSquare->nodeSquarePtr->data==0)
   {
   for(l=0 , temp1=0 , tempNode = tempSameSquare->nodeSquarePtr ; l<N*N ; l++)
      {
	  if(tempNode->possibilities[l]>0)
	      temp1++;
	  }
   if(temp1==0)
       return -1;
   }
   }

return 1;
}

void RecreatePossibilities(struct node *n1 , int m)    //m : 1 to N*N
{
struct sameRow *tempSameRow;
struct sameColumn *tempSameColumn;
struct sameSquare *tempSameSquare;

for(tempSameRow = n1->sameRowPtr ; tempSameRow!=NULL ; tempSameRow = tempSameRow->nextSameRow)
   {
   if(tempSameRow->nodeRowPtr->data==0 && tempSameRow->nodeRowPtr->possibilities[m-1]<0)
     {
	 if(tempSameRow->nodeRowPtr->possibilities[m-1]==-1)
	   {
	   tempSameRow->nodeRowPtr->possibilities[m-1] = m;
	   }
	 else
	   {
	   (tempSameRow->nodeRowPtr->possibilities[m-1])++;
	   }
	 }
   }

for(tempSameColumn = n1->sameColumnPtr ; tempSameColumn!=NULL ; tempSameColumn = tempSameColumn->nextSameColumn)
   {
   if(tempSameColumn->nodeColumnPtr->data==0 && tempSameColumn->nodeColumnPtr->possibilities[m-1]<0)
     {
	 if(tempSameColumn->nodeColumnPtr->possibilities[m-1]==-1)
	   {
	   tempSameColumn->nodeColumnPtr->possibilities[m-1] = m;
	   }
	 else
	   {
	   (tempSameColumn->nodeColumnPtr->possibilities[m-1])++;
	   }
	 }
   }

for(tempSameSquare = n1->sameSquarePtr ; tempSameSquare!=NULL ; tempSameSquare = tempSameSquare->nextSameSquare)
   {
   if(tempSameSquare->nodeSquarePtr->data==0 && tempSameSquare->nodeSquarePtr->possibilities[m-1]<0)
     {
	 if(tempSameSquare->nodeSquarePtr->possibilities[m-1]==-1)
	   {
	   tempSameSquare->nodeSquarePtr->possibilities[m-1] = m;
	   }
	 else
	   {
	   (tempSameSquare->nodeSquarePtr->possibilities[m-1])++;
	   }
	 }
   }
}
									 
									 ///////////////////////////////////////////////////////////
									 
void Sudoku(int a[N*N][N*N])
{
int m,l;
struct node *head = NULL;
clrscr();

printf("Given :\n");
for(m=0 ; m<N*N ; m++)
   {
   for(l=0 ; l<N*N ; l++)
      {
	  printf("%d  ",a[m][l]);
	  if( l==(((((int)(l/N))+1)*N)-1) ) printf("  ");
	  }
   printf("\n");
   if( m==(((((int)(m/N))+1)*N)-1) ) printf("\n");
   }
getch();

CreateList(a,&head);
if(head==NULL)
  {
  display(a,&head);
  return;
  }

CreateSameRowList(&head);
CreateSameColumnList(&head);
CreateSameSquareList(&head);

SolveList(a,&head,head);
}

int CheckSudoku(int a[N*N][N*N] , struct node **p)
{
struct node *temp;
int m,l,i,j;
int x[N*N] = {0};

for(m=0 ; m<N*N ; m++)
   {
   for(l=0 ; l<N*N ; l++)
      {
	  if(a[m][l]==0)
	    {
		for(temp = *p ; temp!=NULL ; temp = temp->nextNode)
		   {
		   if(temp->i==m && temp->j==l)
		     {
			 x[temp->data-1] = temp->data;
			 break;
			 }
		   }
		}
	  else
	    {
		x[a[m][l]-1] = a[m][l];
		}
	  }
   for(l=0 ; l<N*N ; l++)
      {
	  if(x[l]==0)
	     return -1;
	  }
   }

for(m=0 ; m<N*N ; m++)
   {
   for(l=0 ; l<N*N ; l++)
      {
	  if(a[l][m]==0)
	    {
		for(temp = *p ; temp!=NULL ; temp = temp->nextNode)
		   {
		   if(temp->i==l && temp->j==m)
		     {
			 x[temp->data-1] = temp->data;
			 break;
			 }
		   }
		}
	  else
	    {
		x[a[l][m]-1] = a[l][m];
		}
	  }
   for(l=0 ; l<N*N ; l++)
      {
	  if(x[l]==0)
	     return -1;
	  }
   }

for(i=0 ; i<N*N ; i = i+N)
   {
   for(j=0 ; j<N*N ; j = j+N)
      {
	  
	  for(m = ((int)(i/N))*N ; m <= ((((int)i/N)+1)*N)-1 ; m++)
	     {
		 for(l = ((int)(j/N))*N ; l <= ((((int)j/N)+1)*N)-1 ; l++)
		    {
			if(a[m][l]==0)
			  {
			  for(temp = *p ; temp!=NULL ; temp = temp->nextNode)
			     {
				 if(temp->i==m && temp->j==l)
				   {
				   x[temp->data-1] = temp->data;
				   break;
				   }
				 }
			  }
			else
			  {
			  x[a[m][l]-1] = a[m][l];
			  }
			}
		 }
	  
	  for(m=0 ; m<N*N ; m++)
	     {
		 if(x[m]==0)
		    return -1;
		 }
	  
	  }
   }
return 1;
}

void display(int a[N*N][N*N] , struct node **p)
{
struct node *temp = *p;
int m,l,t;

t = CheckSudoku(a,p);
if(t==1)
  {
  printf("\nVerified :\n");
  }
else
  {
  printf("\nFALSE :\n");
  }

for(m=0 ; m<N*N ; m++)
   {
   for(l=0 ; l<N*N ; l++)
      {
	  if(a[m][l]<=0)
	     {
		 for(temp = *p ; temp!=NULL ; temp = temp->nextNode)
		    {
			if(temp->i==m && temp->j==l)
			   printf("%d  ",temp->data);
			}
		 }
	  else
	    {
		printf("%d  ",a[m][l]);
		}
	  if( l==(((((int)(l/N))+1)*N)-1) ) printf("  ");
	  }
   printf("\n");
   if( m==(((((int)(m/N))+1)*N)-1) ) printf("\n");
   }
getch();

if(t==-1)
   exit(-1);
}
