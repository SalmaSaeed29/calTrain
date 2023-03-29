#include <pthread.h>

struct station {
	// FILL ME IN
	int waitpass;
    	int boardpass;
   	int empty;
   	pthread_mutex_t lock_mutex;
  	pthread_cond_t trainleft;
  	pthread_cond_t trainarrive;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
