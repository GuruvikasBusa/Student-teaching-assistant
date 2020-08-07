/******************************************************************************
 				PROJECT CODE

*******************************************************************************/

 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define Student_max 50
void wait(int seconds);
void *sleepingta(void *);
void *student(void *sid);
sem_t waitingRoom;
sem_t TARoom;
sem_t tasleep;
sem_t taChair;
int status = 0;
int main(int argc, char *argv[])
{
    pthread_t assistantId;
    pthread_t tid[Student_max];
    int i, count, noOfStudents, numChairs; int param[Student_max];
    printf("Enter number of students and chairs \n");
    scanf("%d,%d",&noOfStudents,&numChairs);
    if (noOfStudents > Student_max) {
       printf("The No of students are more than max number of students");
       return 0;
    }

    for (i = 0; i < Student_max; i++) {
        param[i] = i;
    }
    sem_init(&waitingRoom, 0, numChairs);
    sem_init(&TARoom, 0, 1);
    sem_init(&tasleep, 0, 0);
    sem_init(&taChair, 0, 0);
    pthread_create(&assistantId, NULL, sleepingta, NULL);
     for (i = 0; i < noOfStudents; i++) {
        pthread_create(&tid[i], NULL,student, (void *)&param[i]);
    }
     for (i = 0; i < noOfStudents; i++) {
	      pthread_join(tid[i],NULL);
    }
    status = 1;
    sem_post(&tasleep);
    pthread_join(assistantId,NULL);
    return 0;
}
void *sleepingta(void *Input)
{
  printf("[TA THREAD] TA is in the office.\n");
  while (!status) {
    printf("[TA THREAD] TA is free.\n");
    sem_wait(&tasleep);
     if (!status)
    {
    printf("[TA THREAD] TA is helping a student... \n");
     sleep(2);
     printf("[TA THREAD] TA is done with the student!\n");
      sem_post(&taChair);
    }
    else {
         printf("[SEMAPHORE] TA releases the lock!  \n");
          }
   }
}
void *student(void *sid) {
 int param = *(int *)sid;
     printf("[STUDENT THREAD] Student %d is coming!\n", param);
     sleep(5);
     printf("Student %d is checking the status of waiting room \n", param);
     sem_wait(&waitingRoom);
     printf("[STUDENT THREAD] Student %d is seating on the waiting chair \n", param);
     sem_wait(&TARoom);
     sem_post(&waitingRoom);
     printf("[SEMAPHORE] Student %d gets the lock! \n", param);
     sem_post(&tasleep);
     sem_wait(&taChair);
     sem_post(&TARoom);
     printf("[SEMAPHORE] Student %d releases the lock! \n", param);
}




