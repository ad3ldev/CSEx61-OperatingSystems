#include <pthread.h>

// Station has 2 conditions:
// 1- arrived if the train have arrived
// 2- full if the train is full and ready to go
// Station has 3 counters for each phase of the passengers
// waiting, in process of boarding and empty seats
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