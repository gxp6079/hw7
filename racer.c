
#define _DEFAULT_SOURCE

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include "racer.h"

static long DELAY_VALUE = 0;
int FINISH_LINE = 0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

void init_racers( long milliseconds, int length){
    FINISH_LINE = (length - MAX_CAR_LEN -2);
    DELAY_VALUE = milliseconds;
}

Racer * make_racer( char *name, int row ){
    Racer * racer = (Racer*) malloc(sizeof(Racer*));
    char graph[MAX_CAR_LEN];
    graph[0] = '~';
    graph[1] = 'O';
    graph[2] = '=';
    int i = 0;
    while(i < MAX_NAME_LEN + 1){
	if(i < strlen(name)){
	    graph[i + 3] = name[i];
	}	
	else{
	    graph[i + 3] = '-';
	}
        i++;
    }
    graph[MAX_CAR_LEN - 2] = 'o';
    graph[MAX_CAR_LEN - 1] = '>';
    racer->graphic = (char *) malloc(sizeof(graph));
    strcpy(racer->graphic, graph);
    racer->row = row;
    racer->distance = 0;
    return racer;
}

void destroy_racer( Racer *racer ){
    free(racer->graphic);	
    free(racer);
}

void *run( void *racer ){	
    Racer * r = (Racer *) racer;
    while(r->graphic[1] != 'X' && r->distance != FINISH_LINE ){
	pthread_mutex_lock(&mymutex);    
        useconds_t delay = rand() % (DELAY_VALUE + 1);
        usleep(delay);
        if(delay <= 3){
            r->graphic[1] = 'X';
            mvprintw(r->row, r->distance, r->graphic);
        }
        else{
            mvprintw(r->row, r->distance," ");
	    mvprintw(r->row, r->distance + 1, r->graphic);
            r->distance ++;
        }
        refresh();
	pthread_mutex_unlock(&mymutex);
    }
}
