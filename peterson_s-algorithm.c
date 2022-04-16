#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

 
int cnt;

int flag[2] = {0,0};

int turn = 0;

 

void *func0() {

    for (int i = 0; i < 100000; i++) {

        flag[0]=1; // 0번째 프로세스가 임계영역을 사용하겠다고 알림

        turn=1; // 어떤 프로세스를 실행시킬건지 가르키는 변수

        while(flag[1] == 1 && turn ==1){} // busy waits
                                          /*
                                          turn == 1일 경우 내 차례가 아닌데 자원까지 사용하려고 한다면
                                          spinlock에서 머물어야 함.

                                          또는 내 차례이거나 자원을 사용하려고 하지 않는다면 
                                          spinlock을 빠져나와 진입할 수 있다.

                                          이런 방식으로 turn과 flag변수를 이용해서 동시 접근 방지
                                          */

            cnt++; // 카운트 증가

            printf("cnt1 :: %d\n", cnt);  //출력

            flag[0]=0; // 사용종료. false 로 교체
    }

}

void *func1() {

    for (int i = 0; i < 100000; i++) {

        flag[1]=1;

        turn=0;

        while(flag[0] == 1 && turn ==0){}
            
            cnt++;

            printf("cnt2 :: %d\n", cnt);

            flag[1]=0;       

    }

}


int main()

{

     pthread_t thread1, thread2;

     int  iret1, iret2;

     iret1 = pthread_create( &thread1, NULL, func0, NULL);

     iret2 = pthread_create( &thread2, NULL, func1, NULL);


     pthread_join( thread1, NULL);

     pthread_join( thread2, NULL); 


     printf("Thread error: %d\n", 200000-cnt);

     exit(0);

}

