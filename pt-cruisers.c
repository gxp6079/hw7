/*
 * File: pt-cruisers.c
 * @author Giovana Puccini
 * @email gxp6079@rit.edu
 */

#include "racer.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[]){
    srand(time(NULL));
    int is_n = 1;    
    for(int i = 0 ; i < strlen(argv[1]) ; i++){
        if(!(isalnum(argv[1][i]) && argv[1][i] != '-' )|| isalpha(argv[1][i])){
	    is_n = 0;
	}
    }
    long mil = 0;
    if(is_n){
        sscanf(argv[1], "%ld", &mil);
	if(mil <= 0){
	    fprintf(stderr, "Error: delay (%ld) is invalid.", mil);
	    return 1;
	}
    }
    else{
        mil = DEFAULT_WAIT;
    }
    if(argc - is_n < 3){
            fprintf(stderr, "Usage: ./pt-cruisers [max-speed-delay] name1 name2 [name3...]");
            return 1;
    }
    initscr();
    cbreak();
    noecho();
    int width = getmaxx(stdscr);
    init_racers(mil, width);
    int num_racers = argc - (1 + is_n);
    pthread_t threads[num_racers];
    Racer * racers[num_racers];
    for(int j = 1 + is_n ; j < argc; j++){
        pthread_t mythread;
        if(strlen(argv[j]) <= MAX_NAME_LEN){
	    Racer * racer = make_racer(argv[j], j);
            racers[j - (1 + is_n)] = racer;	    
            if ( pthread_create( &mythread, NULL, run, (void *) racer) ) {
                fprintf(stderr, "error creating thread\n");
                exit(1);
            }
	    mvprintw(racer->row, racer->distance, racer->graphic);
        }
        else{
            fprintf(stderr, "Error: racer %s exceeds length limit of 6.", argv[j]);
            return 1;
        }
        threads[j - (1 + is_n)] = mythread;
    }
    refresh();
    for(int k = 0 ; k < num_racers ; k++){
        if(pthread_join(threads[k], NULL)){
	    fprintf(stderr, "error joining thread.");
    	    return 1;
	}
    }
    for(int k = 0 ; k < num_racers ; k++){
        destroy_racer(racers[k]);
    }
    mvprintw(0, 0, "The race is over! Press any key to exit the racetrack.");
    refresh();
    char c;
    while((c = fgetc(stdin) == 0x0A)){}
    endwin();
}
