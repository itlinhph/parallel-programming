#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define  m       100
#define  n       100
#define  T       3
#define  dt      0.01
#define  dx      0.1
#define  D       0.1
#define epsilon  1e-6
#define nuy      2.0   
float diff=0;
//=========================
void DisplayMatrix(float *A, int row,  int col)
{
  int i,j;
  for(i=0;i<row;i++){
    for(j=0;j<col;j++) printf("  %f",*(A+i*col+j));
    printf("\n");
  }
}
//=========================
void Write2File(float *C)
{
FILE *result=fopen("result2.txt", "a");
int i,j;

for(i=0;i<m;i++) 
{
for(j=0;j<n;j++) 
{ 
fprintf(result, "%lf\t", *(C+i*n+j));
}
fprintf(result, "\n");
}

fclose(result);
}
//=========================
void KhoiTao(float *C)
{
  int i,j;
for (  i = 0 ; i < m ; i++ )
  for ( j = 0 ; j < n ; j++ ){
      *(C+i*n+j) = 1.0;
  }
  int i1=m/2;
  int j1=n/2;
  *(C+i1*n+j1) = 0.0;
}

//=========================
void copyMattrix(float *C1,float *C2){
    for(int i=0;i<m;i++){
        for(int j=0;j<m;j++){
            *(C1+i*n+j)=*(C2+i*n+j);
        }
    }
}
//=========================
float FD(float *C)
{
int i, j;
float diff=0;
float c,u,d,l,r;
for (  i = 0 ; i < m ; i++ )
  for ( j = 0 ; j < n ; j++ )
    {
      c = *(C+i*n+j);
      u = (i==0)   ? 1 : *(C+(i-1)*n+j);
      d = (i==m-1) ? 1 : *(C+(i+1)*n+j);
      l = (j==0)   ? 1 : *(C+i*n+j-1);
      r = (j==n-1) ? 1 : *(C+i*n+j+1);
      if(c!=0){
        *(C+i*n+j) = (1.0/4)*(u+d+l+r);
        if(fabs(*(C+i*n+j)-c)>diff){
          diff = fabs(*(C+i*n+j)-c);
        }
      }
    }
    return diff;
}
//=========================
void updateUCV(float *C,int i,int j){
    float u1,u2,u3,u4;
    u1 = (i==0)   ? 1 : *(C+(i-1)*n+j);
    u2 = (i==m-1) ? 1 : *(C+(i+1)*n+j);
    u3 = (j==0)   ? 1 : *(C+i*n+j-1);
    u4 = (j==n-1) ? 1 : *(C+i*n+j+1);
    float sum=pow(u1,nuy)+pow(u2,nuy)+pow(u3,nuy)+pow(u4,nuy);
    for(int i1=i-1;i1<=i+1;i1+=2){
        for(int j1=j-1;j1<=j+1;j1+=2){
            if(i1>=0 && i1<=m && j1>=0 && j1<=n){
                float proportion=pow(*(C+i1*n+j1),nuy);
                float random=(double)rand() / (double)RAND_MAX ;
                if(proportion>random){
                    *(C+i1*n+j1)=0;
                }
            }
        }
    }
}
//=========================
void updateStepByStep(float *C){
    float *Ctemp = (float *) malloc ((m*n)*sizeof(float));
    copyMattrix(Ctemp,C);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            float c=*(C+i*n+j);
            if(c==0){
                updateUCV(Ctemp,i,j);
            }
        }
    }
    copyMattrix(C,Ctemp);
}
//=========================
int main()
{
    remove("result2.txt");    
    int i,j, count;
    float t;
    time_t t1,t2;
    t=0;
    float *C,*dC;
    C = (float *) malloc ((m*n)*sizeof(float));
    dC = (float *) malloc ((m*n)*sizeof(float));
    KhoiTao(C);
    Write2File(C);

    printf("Gia tri khoi tao:\n");
    DisplayMatrix(C, m, n);
    count=0;
    for(float t=0;t<T;t+=dt){
        do{
            diff=FD(C);
        }while (diff>epsilon);
        count=count+1;      
        updateStepByStep(C);  
        if (count%5==0) Write2File(C);
    }
    t2=time(NULL);
    DisplayMatrix(C, m, n);
return 0;
}

