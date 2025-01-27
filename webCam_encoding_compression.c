#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include<unistd.h>
// Function prototypes
void *encodeVideo(void *arg);
void *compressVideo(void *arg);

// Global semaphore
sem_t semaphore;

int main() 
{
    // Initialize semaphore
    sem_init(&semaphore, 0, 0);

    // Create threads for encoding and compressing
    pthread_t encodeThread, compressThread;

    // Launch encoding thread
    if (pthread_create(&encodeThread, NULL, encodeVideo, NULL) != 0) 
    {
        fprintf(stderr, "Error creating encoding thread\n");
        exit(EXIT_FAILURE);
    }

    // Launch compressing thread
    if (pthread_create(&compressThread, NULL, compressVideo, NULL) != 0) 
    {
        fprintf(stderr, "Error creating compressing thread\n");
        exit(EXIT_FAILURE);
    }

    // Wait for both threads to finish
    pthread_join(encodeThread, NULL);
    pthread_join(compressThread, NULL);

   printf("Compression and encoding completed.\n");
    return 0;
}

void *encodeVideo(void *arg) 
{
   
    system("ffmpeg -i webCam_ResolutionOutput.mp4 -c:v libx264 -crf 23 -c:a aac -strict experimental -b:a 128k webCam_encoded.mp4");

    printf("Webcam frames are encoding\n");
    sleep(2);
    system("mplayer webCam_encoded.mp4");
    sem_post(&semaphore);

    pthread_exit(NULL);
}

void *compressVideo(void *arg) 
{
    // Wait for the encoding process to finish
     sem_wait(&semaphore);
     system("ffmpeg -i webCam_encoded.mp4 -c:v libx265 -crf 28 -c:a aac -strict experimental -b:a 128k webCam_compressed.mp4");
     printf("Webcam encoded frames are compressed\n");
     sleep(2);
     system("mplayer webCam_compressed.mp4");
     pthread_exit(NULL);
} 


