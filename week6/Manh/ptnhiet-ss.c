#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#define M 20
#define Time 10
#define dt 0.01
#define dx 0.1
#define D 0.1
int NT= 4;
void khoiTao(float *T, float *dT){
  int i=0;
  
  
  for (i = 0; i<M; i++)
  *(T+i) = 25;
  for (i = 0; i<M ;i++)
  *(dT+ i) = 0;
}

void khongGian(float *T, float *dT ){
  int i;
  float c,l,r;
  for ( i = 0 ; i < M ; i++ ){
    c = *(T+i);
    l = (i==0) ? 100 : *(T+i-1);
    r = (i==M-1) ? 25 : *(T+i+1);
    *(dT+i) = D*(l-2*c+r)/(dx*dx);
  } 
}

void thoiGian(float *T, float *dT){
  int i=0, t=0, Mc, start, end, id; 
  int NTime = Time/dt;
  for (t=0;  t<= NTime;t++)
  {
  #pragma omp parallel private(start, end, i,id)
  {
    id = omp_get_thread_num(); 
    Mc= M/NT;
    start =  id*Mc;
    end = start +Mc;
  
    khongGian(T, dT);
    #pragma omp barrier
    for ( i = start ; i < end ; i++ )
    *(T+i) = *(T+i) + *(dT+i)*dt;
    #pragma omp barrier
  }
  }
}
void in(float *T, float *dT){
  int i=0;
  for (i=0; i<M; i++)
   printf("%f ",*(T+i));
  printf("\n");
  //for (i=0;i<M;i++)
   //printf("%f ",*(dT+i));
}
int main(){
  omp_set_num_threads(4);
  float *T;
  float *dT;
  T = (float*)malloc(M*sizeof(float));
  dT = (float*)malloc(M*sizeof(float));
  khoiTao(T, dT);
  thoiGian(T, dT);
  in(T, dT);
  return(1);
}
