#include <pthread.h>

#include <semaphore.h>

#include <stdlib.h>

#include <stdio.h>

#include <unistd.h> // usleep() 오류를 위한 코드

#define MaxItems 5 // Maximum items a producer can produce or a consumer can consume

#define BufferSize 5 // Size of the buffer

sem_t empty;

sem_t full;

int in = 0;

int out = 0;

int buffer[BufferSize];

pthread_mutex_t mutex;


void *producer(void *pno)
{   
    int item;

    for(int i = 0; i < MaxItems; i++) {

        item = rand(); // Produce an random item

        sem_wait(&empty); /* 
                             세마포어를 하나 감소시키는 역할
                             세마포어가 0일 경우 1이상이 될 때까지 쓰레드는 대기 상태
                             0이 아닐 경우 대기상태에서 빠져나와 세마포어를 1 감소
                           */                

        pthread_mutex_lock(&mutex); // mutex에 lock 걸기
                                    // 호출한 쓰레드가 이 뮤텍스의 소유자
                                    // pthread_mutex_lock은 즉시 반환
                                    
                                    /* 
                                      다른 쓰레드에 의해 이미 locked 상태이면 
                                      뮤텍스가 다시 unlockedf 상태가 될 때까지
                                      호출한 쓰레드를 중지 시킨다.
                                    */
        buffer[in] = item;

	    usleep(500000); // 500,000ms동안의 딜레이

        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in);

        in = (in+1)%BufferSize;

        pthread_mutex_unlock(&mutex); // 주어진 뮤텍스 unlock

        sem_post(&full); // 세마포어의 값을 1증가 시킨다
    }

}

void *consumer(void *cno)
{   
    for(int i = 0; i < MaxItems; i++) {

        sem_wait(&full);

        pthread_mutex_lock(&mutex);

        int item = buffer[out];

	    usleep(500000);

        printf("Consumer %d: Remove Item %d from %d\n",*((int *)cno),item, out);

        out = (out+1)%BufferSize;

        pthread_mutex_unlock(&mutex);

        sem_post(&empty);

    }

}

int main()

{   
    pthread_t pro[5],con[5];

    pthread_mutex_init(&mutex, NULL); // mutex 객체를 초기화
                                      // 언제나 0을 리턴한다.

    sem_init(&empty,0,BufferSize);  // 세마포어 초기화 함수
    
    
    
                                   /*
                                     첫번째 인자: 세마포어 객체를 초기화 할 세마포어 받기
                                     두번쨰 인자: 세마포어 타입 결정(0을 주지 않을 경우 에러)
                                     세번째 인자: 몇으로 초기화 할지 의미

                                     첫번째 인자로 초기화 하려는 세마포어 변수의 포인터를 넘긴다.
                                     초기화 되는 값은 세번째 인자로 결정
                                   */

    sem_init(&full,0,0);



    int a[5] = {1,2,3,4,5}; //Just used for numbering the producer and consumer



    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);

    }

    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);

    }



    for(int i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL); // 첫번째 인자의 입력된 pthread를 종료될 때까지 기다리다가
    }                               // 특정 pthread가 종료시 자원 해제
                                    // 두번째 인자는 pthread의 return 값
    for(int i = 0; i < 5; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex); // mutex 객체 소멸
                                   // 제거하기 위해서는 unlock 상태여야함
                                   
                                   /*
                                     하지만 LinuxThreads 구현에서는 뮤텍스 객체는 
                                     아무런 자원도 할당받지 않기 때문에 단지 뮤텍스가 
                                     locked 상태인지 검사하는 용도
                                   */

    sem_destroy(&empty); // 세마포어와 관련된 리소스들 소멸(객체 소멸)

    sem_destroy(&full);   


    return 0;

}
