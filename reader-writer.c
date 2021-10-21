//created by ujjwal on 18/10/2021
//reader-writer-problem-in-c

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t wrt = (void *) 1;
pthread_mutex_t mutex;
int count = 100;
int total_num_readers = 0;

void * writer(void * W) {
    sem_wait(&wrt);
    count += 10;
    printf("%d Writer incremented count to %d\n",*(int *)W,count);
    sem_post(&wrt);
}

void * reader(void * R) {

    //reader acquire the lock before making changes
    pthread_mutex_lock(&mutex);
    total_num_readers++;

    //if this is the first reader, block the writer
    if(total_num_readers == 1) {
        sem_wait(&wrt);
    }
    pthread_mutex_unlock(&mutex);

    //reading section
    printf("%d Reader reads the count value %d\n",*(int *)R,count);

    //reader acquire the lock again
    pthread_mutex_lock(&mutex);
    total_num_readers--;

    //if reached the last reader, writer will be allowed access
    if(total_num_readers == 0) {
        sem_post(&wrt);
    }
    pthread_mutex_unlock(&mutex);
}

int main() {

    pthread_t read[30];
    pthread_t write[20];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt, 0, 1);

    int arr[30] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};

    for(int i = 0; i < 30; i++) {
        pthread_create(&read[i],NULL,(void *) reader, &arr[i]);
    }
    for(int i = 0; i < 20; i++) {
        pthread_create(&write[i], NULL,(void *) writer, &arr[i]);
    }
    for(int i = 0; i < 30; i++)
    {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < 20; i++)
    {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}


