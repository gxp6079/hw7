// racer.h - a thread controlling a figure that races across a terminal window
// 
// @author cs @ rit.edu
// @date Wed Oct 31 21:46:50 EDT 2018
// 
// This is the interface for car racing threads.

#ifndef _RACER_H
#define _RACER_H

#define MAX_CAR_LEN 12    ///< length does not include the trailing NUL byte
#define MAX_NAME_LEN 6    ///< length of name the fits 'inside' a car

#define DEFAULT_WAIT 200  ///< default used if not given, or given incorrectly

/// Racer_S structure represents a racer's row, position and display graphic.

typedef struct Racer_S {

    int row;       ///< vertical row or "racing lane" of a racer

    int distance;  ///< column of rear of car, marking its position in race

    char *graphic; ///< graphic is the drawable text of the racer figure

} Racer;

/// init_racers does setup work for all racers before the start of the race.
///
/// @param milliseconds the length of pause between steps in animation 
/// @param length  total length of the race determined by the terminal width
///
/// This must define the FINISH_LINE as (length - MAX_CAR_LEN - 2) because
/// the left end of the racer graphic has to cross the line and 
/// the value 2 is the margin of the racetrack.

void init_racers( long milliseconds, int length );

/// make_racer - Create a new racer.
///
/// @param name the string name to show on the display for this racer
/// @param row the row in which to race in the terminal
/// @return Racer pointer a dynamically allocated Racer object
/// @pre strlen( name ) <= MAX_NAME_LEN, for display reasons.

Racer * make_racer( char *name, int row );

/// destroy_racer - Destroy all dynamically allocated storage for a racer.
///
/// @param racer the object to be de-allocated

void destroy_racer( Racer *racer );

/// This function is the 'main method' for a thread instance.
///
/// run function runs one racer in a race. here are the steps run takes:
///
/// Initialize the display of the racer:
///     Display the racer's graphic (a string) at its lane in the terminal,
///     and show the racer in its lane, with the racer's rear end in column 0.
///
/// Repeat actions below until the racer's distance equals the FINISH_LINE:
///
///     Calculate a random sleep value between 0 and the delay value 
///         given to init_racers at the start of the race.
///     If sleep value is less than or equal to 3, the racer gets a flat and
///         can't finish the race. Display an 'X' in the second character of
///         the graphic to show a flat tire.
///         Display the car with the flat tire and the function ends.
///     Otherwise, sleep for that length of time to indicate speed/slowness.
///
///     Change the display of this racer by moving 1 column to the right.
///         Erase the racer graphic from the display.
///         Increase the racer's distance by 1.
///         Re-display the racer's graphic at the new location.
///         Refresh the terminal.
///
///     Note: run must keep updates of the display by one racer "atomic".
///     That means when one racer is advancing on the screen, no other
///     racers can change the state of the terminal screen.
///
/// @param racer Racer object declared as void* for pthread operability
/// @return void pointer to status. A NULL represents success.
/// @pre racer cannot be NULL.

void *run( void *racer );

#endif
