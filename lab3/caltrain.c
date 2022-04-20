#include <pthread.h>
#include "caltrain.h"

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
	station->empty=count;
	pthread_mutex_lock(&station->lock);
	while(station->waiting>0 && station->empty>0){
		pthread_cond_broadcast(&station->arrived);
		pthread_cond_wait(&station->full,&station->lock);
	}
	pthread_mutex_unlock(&station->lock);
	station->empty = 0;
}

void
station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&station->lock);
	station->waiting++;
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
	pthread_mutex_lock(&station->lock);
	station->boarding--;
	station->empty--;
	if(station->empty==0 || (station->boarding == 0 && station->waiting == 0)){
		pthread_cond_signal(&station->full);
	}
	pthread_mutex_unlock(&station->lock);
}
