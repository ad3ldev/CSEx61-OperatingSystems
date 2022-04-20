#include <pthread.h>

struct station {
	pthread_mutex_t lock;
	pthread_cond_t arrived;
	pthread_cond_t full;
	int waiting;
	int boarding;
	int empty;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);