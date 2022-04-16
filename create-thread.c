#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // 쓰레드 생성을 위한 헤더

void *print_message_function( void *ptr );

int main()

{
    pthread_t thread1, thread2;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2;

    // 쓰레드 생성
     iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1); // 첫번째 인자: 생성된 쓰레드의 ID를 저장할 변수의 포인터

     iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2); // 두번째 인자: 쓰레드의 특성을 설정할 때 사용. 주로 NULL
                                                                                        
                                                                                        // 세번째 인자: 쓰레드가 생성되고 나서 실행될 함수가 온다.

                                                                                        // 네번쨰 인자: 세번쨰 인자에서 호출되는 함수에 전달하고자 하는 인자 값

     pthread_join( thread1, NULL); // 쓰레드는 메인 함수가 종료되면 자동으로 종료

     pthread_join( thread2, NULL); // 따라서 메인에서 쓰레드가 종료될 때까지 메인의 종료를 기다려야한다

                                   // 그때 pthread_join() 사용

                                   // 첫번째 인자: 쓰레드 ID. 두번째 인자: 쓰레드 종료시 반환하는 값

     printf("Thread 1 returns: %d\n",iret1);

     printf("Thread 2 returns: %d\n",iret2);

     exit(0);

}


void *print_message_function( void *ptr )  // 출력 메소드

{
     char *message;

     message = (char *) ptr;

     printf("%s \n", message);
}
