#include <stdio.h>

#include <pthread.h> // 쓰레드 생성을 위한 헤더



#define NTHREADS 10 // 쓰레드 10개를 생성해 기다린다는 의미

void *thread_function(void *);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // mutex 정보를 저장하기 위한 타입 선언 및 초기화

int  counter = 0;



main()

{
   pthread_t thread_id[NTHREADS]; // pthread의 자료형을 의미

   int i, j;

   for(i=0; i < NTHREADS; i++) {
    pthread_create( &thread_id[i], NULL, thread_function, NULL );
   }

   for(j=0; j < NTHREADS; j++){
    pthread_join( thread_id[j], NULL);  
   }

  

   /* Now that all threads are complete I can print the final result.     */

   /* Without the join I could be printing a value before all the threads */

   /* have been completed.                                                */


   printf("Final counter value: %d\n", counter);

}



void *thread_function(void *dummyPtr)

{
   printf("Thread number %ld\n", pthread_self());

   pthread_mutex_lock( &mutex1 ); // mutex 잠금

   counter++; // counter값 1증가

   pthread_mutex_unlock( &mutex1 ); // mutex 잠금해제
}
