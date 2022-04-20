#include <pthread.h>
#include "caltrain.h"


// init the station
void
station_init(struct station *station)
{
	pthread_mutex_init(&station->lock, NULL);
	pthread_cond_init(&station->arrived, NULL);
	pthread_cond_init(&station->full, NULL);
	station->waiting=0;
	station->boarding=0;
}

void
station_load_train(struct station *station, int count)
{
	// empty seats is count
	// lock the station
	pthread_mutex_lock(&station->lock);
	station->empty=count;
	// while the there is people waiting or seats are empty then broadcast all waiting pthreads
	while(station->waiting>0 && station->empty>0){
		pthread_cond_broadcast(&station->arrived);
		pthread_cond_wait(&station->full,&station->lock);
	}
	station->empty = 0;
	pthread_mutex_unlock(&station->lock);
}

void
station_wait_for_train(struct station *station)
{
	// each passengers adds a waiting
	pthread_mutex_lock(&station->lock);
	station->waiting++;
	// if boarding = 0 and empty = 0 then the thread waits, and if boarding = empty = n, then the thread also waits
	// since any other waiting can't go inside the train
	while(station->boarding==station->empty){
		pthread_cond_wait(&station->arrived,&station->lock);
	}
	station->boarding++;
	station->waiting--;
	pthread_mutex_unlock(&station->lock);
}

void
station_on_board(struct station *station)
{	
	// changing from boarding to actually on board
	pthread_mutex_lock(&station->lock);
	station->boarding--;
	station->empty--;
	if(station->empty==0 || (station->boarding == 0 && station->waiting == 0)){
		pthread_cond_signal(&station->full);
	}
	pthread_mutex_unlock(&station->lock);
}
