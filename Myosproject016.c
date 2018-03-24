#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <stdbool.h>

//N  -> need matrix
//M  -> MAx matrix
//A  -> Allocation Matrix
//AV -> Available  Matrix
// W - work martix

pthread_t Ttid[10];

int ps;//process rank
int dc=0;
int ret;
int A[10][10];
int M[10][10];
int N[10][10];
int W[1][10];
int n,m,pid,ch;
int F[10],W[1][10];
int Pseq[10];

//temperay store for thread exec

int amount=100;
int hold[n][m];

int counter=0;
int Tret;
int tA[10][10];
int tM[10][10];
int tN[10][10];
int tW[1][10];
int tn,tm,tpid,tch;
int tF[10],tW[1][10];
pthread_mutex_t lock;

//my way matrix pritn
void Mprint(int x[][10],int n,int m)
{
	int i,j;
	printf("\n");
	for(i=0;i<m;i++)
	{
			printf("R%d\t",i);

	}
	for(i=0;i<n;i++)
	{
		printf("\n");
		for(j=0;j<m;j++)
		{
			printf("%d\t",x[i][j]);
		}
	}	
}

//Resource Request algorithm
void res_request(int A[10][10],int N[10][10],int AV[10][10],int pid,int m)
{
	int reqmat[1][10];
	int i;
	printf("\n Enter additional request from any process :- \n");
	for(i=0;i<m;i++)
	{
		printf(" Request for resource %d : ",i+1);
		scanf("%d",&reqmat[0][i]);
	}
	
	for(i=0;i<m;i++)
	{	
		if(reqmat[0][i] > N[pid][i])
		{
			printf("\n Sorry this will lead to  UnSafe State  \n Error encountered.\n");
			//Since You  enter more than Need
			exit(0);
		}
	}

	for(i=0;i<m;i++)
	{
		if(reqmat[0][i] > AV[0][i])
		{
			printf("\n Sorry this will lead to  UnSafe State  \n Since  Resources unavailable.\n");
			exit(0);
		}
	}
	
	// update the After REquest is APPROVED
	for(i=0;i<m;i++)
	{
		AV[0][i]-=reqmat[0][i];
		A[pid][i]+=reqmat[0][i];
		N[pid][i]-=reqmat[0][i];
	}
}

//Safety algorithm
int safety(int A[][10],int N[][10],int AV[1][10],int n,int m,int a[])
{

	int i,j,k,x=0;
	
	int pflag=0,flag=0;

	// Fininsh array process initially false
	for(i=0;i<n;i++)
	{
		F[i]=0;
	}
       // Work Array process
	for(i=0;i<m;i++)
	{
		W[0][i]=AV[0][i];
	}

	//
	int c=0;
	while(c<n)
	{
		bool found = false;
		for(i=0;i<n;i++)
		{
			if(F[i] == 0)
			{
				int j;
				for(j=0;j<m;j++)
				{
					if(N[i][j] > W[0][j])
					{
						
						break;
					}
				}

				if(j==m)
				{
					for(int k=0;k<m;k++)
					{
						W[0][k]+=A[i][k];
					}

					//storing the Seq
					a[c++]=i;
					F[i]=1;
					found=true;
					
					
				}
			}
		}
		
		if(found==false)
		{
			return 0;
		}

	}
      /*//saving my P Sequence Globally
	for(int e=0;e<n;e++)
	{
		Pseq[e]=a[e];
	}
	*/
	return 1;

}



void accept(int A[][10],int N[][10],int M[10][10],int W[1][10],int *n,int *m)
{
	int i,j;
	printf("\n Enter total no. of processes in Multithread  : ");
	scanf("%d",n);
	printf("\n Enter total no. of resources : ");
	scanf("%d",m);
	for(i=0;i<*n;i++)
	{
		printf("\n Process %d\n",i+1);
		for(j=0;j<*m;j++){
			printf(" Allocation for resource %d : ",j+1);
			scanf("%d",&A[i][j]);
			printf(" Maximum for resource %d : ",j+1);
			scanf("%d",&M[i][j]);
		}
	}
	printf("\n Available resources : \n");
	for(i=0;i<*m;i++)
	{
		printf(" Resource %d : ",i+1);
		scanf("%d",&W[0][i]);
	}

	// CALCULATE NeeD of Process 
	for(i=0;i<*n;i++)
	{
		for(j=0;j<*m;j++)
		{
			N[i][j]=M[i][j]-A[i][j];
		}
	}

	//calling my function to print	
	printf("\n Allocation Matrix");
	Mprint(A,*n,*m);

	printf("\n Maximum Requirement Matrix");
	Mprint(M,*n,*m);

	printf("\n Need Matrix");
	Mprint(N,*n,*m);

}


void Tcopy()
{
	if(0==0)
	{
		int Tret;
		for(int x=0;x<n;x++)
		{
			for(int y;y<m;y++)
			{
				int tA[10][10];
				int tM[10][10];
				int tN[10][10];
			}
		}
		for(int x=0;x<n;x++)
		{
			for(int y;y<m;y++)
			{
				int tA[10][10];
				int tM[10][10];
				int tN[10][10];
			}
		}

	}	
	
}




int banker(int A[][10],int N[][10],int W[1][10],int n,int m)
{
	int j,i,a[10];
	
		
	// Backup Tempary Copy		Tcopy(A,N,W,n,m,a)		
	Tcopy();
	
	// Calling the SAFETY function
	j=safety(A,N,W,n,m,a);

       // j==1 means SAFE And j==0 means Not SAFE

	if(j != 0 )
	{
		printf("\n\n");

		for(i=0;i<n;i++)
		{
		     printf(" P%d  ",a[i]);
			Pseq[i]=a[i];
		}
		printf("\n A safety sequence has been detected.\n");
		return 1;
	}
	else
	{
		printf("\n Deadlock has occured.\n");
		return 0;
	}
}


void* BankerThreadFun()
{

		
	
	





    pthread_t id = pthread_self();

		
    
	dc++;

///////////////////////////////////////		
	pthread_mutex_lock(&lock);
 
	
    	counter += 1;
    	printf("\n Job %d has started\n", ps);
	
	amount=amount+100;
	printf("\n the amount is %d ",amount);
	/*
	for(int i=0;i<n;i++)
	{
		if(ps==i)
		{
			int x=1;
			for(int j;j<m;j++)
			{
				printf("type %d resource  need -> %d  \n ",x,N[i][j]);
				x++;	
			}
		}
	}
       
	printf(" update value of DC = %d",dc); 
    	printf("\n Job %d has finished\n", ps);
    	*/
	pthread_mutex_unlock(&lock);
/////////////////// //////////////////////
    



}


int main()
{
	
	printf("\n DEADLOCK AVOIDANCE USING BANKER'S ALGORITHM\n");
	
	// ACCepting -> to taken the Argument value
	accept(A,N,M,W,&n,&m);

	// use to check the safty
	ret=banker(A,N,W,n,m);
	if(ret !=0 )
	{
		printf("\n Do you want make an additional request ?\n press Keydown '1' for yes and '0' for No");
		scanf("%d",&ch);
		if(ch == 1)
		{
			printf("\n Enter Process ID no. : ");
			scanf("%d",&pid);
			res_request(A,N,W,pid-1,m);
			ret=banker(A,N,W,n,m);
			if(ret == 0 )
			{
				exit(0);
			}
		}
	}
	else
	{
			exit(0);
	}
	printf("\n DECTECTION of SAFE sequence is completed...\n NOW the Game Begin \t ");

	////////////////////////////////////////////////
	
	int error;
 	if (pthread_mutex_init(&lock, NULL) != 0)
    	{
        	printf("\n mutex init has failed\n");
       		return 1;
    	}
 
 
  

	int j;	
	for(j=0;j<n;j++)
	{
		ps=Pseq[j];
		int need[1][1];
		int max[1][1];
	printf("\n\n Process number = %d",ps); 
    	  //pthread_create(&Ttid[s], NULL, myThreadFun,(void*)need,(void*)max);
	  pthread_create(&Ttid[ps], NULL, &BankerThreadFun,NULL);
	  pthread_join(Ttid[ps],NULL);
	  sleep(1);
	 
	}


	  pthread_mutex_destroy(&lock);
	////////////////////////////////////////////


}
