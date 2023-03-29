#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)  //initialize with null and zero
{
    pthread_mutex_init(&station->lock_mutex,NULL);
    pthread_cond_init(&station->trainleft,NULL);
    pthread_cond_init(&station->trainarrive,NULL);
    station->boardpass=0;
    station->waitpass=0;
}

void
station_load_train(struct station *station, int count)   // a train arrives in the station and has opened its doors,  
{ 							 // count indicates how many seats are available(empty) on the train
    pthread_mutex_lock(&station->lock_mutex); // lock to enter the ctitical section	 
    station->empty=count; // update the value of empty seats with the count of them
    while(station->empty>0 && station->waitpass>0){ // loop as long as there are empty seats and there are waiting passengers
        pthread_cond_broadcast(&station->trainarrive); // signal waiting threads that a train has just arrived
        pthread_cond_wait(&station->trainleft,&station->lock_mutex); // wait for the condition: trainleft, unlock
    } // there is no empty seats or there is no waiting passengers
    station->empty=0;
    pthread_mutex_unlock(&station->lock_mutex);
}                                                       

void
station_wait_for_train(struct station *station) // passenger is arriving in the station and is waiting for a train
{
    pthread_mutex_lock(&station->lock_mutex); // lock to enter the ctitical section
    station->waitpass++; // increment the number of waiting passengers by 1
    while(station->boardpass==station->empty){ 
	pthread_cond_wait(&station->trainarrive,&station->lock_mutex); // wait for the condition: trainarrive, unlock
    }
    station->waitpass--; 
    station->boardpass++; 
    pthread_mutex_unlock(&station->lock_mutex);
}

void
station_on_board(struct station *station) // a passenger is seated
{
    pthread_mutex_lock(&station->lock_mutex); // lock to enter the ctitical section
    station->empty--; // decrement the number of empty seats by 1
    station->boardpass--; 
    if((station->waitpass==0 &&station->boardpass==0)||station->empty==0){
        station->boardpass=0;
        pthread_cond_signal(&station->trainleft);
    }
    pthread_mutex_unlock(&station->lock_mutex);
}
