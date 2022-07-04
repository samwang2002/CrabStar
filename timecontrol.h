#ifndef TIMECONTROL_H
#define TIMECONTROL_H

/********Time control variables****************/

// exit from engine flag
extern int quit;

// UCI "movestogo" command moves counter
extern int movestogo;

// UCI "movetime" command time counter
extern int movetime;

// UCI "time" command holder (ms)
extern int mseconds;

// UCI "inc" command's time increment holder
extern int inc;

// UCI "starttime" command time holder
extern int starttime;

// UCI "stoptime" command time holder
extern int stoptime;

// variable to flag time control availability
extern int timeset;

// variable to flag when the time is up
extern int stopped;

/********* functions ***************/

//get time in milliseconds
int get_time_ms();

// function to listen to GUI's input during search. Waits for user input from STDIN
int input_waiting();

// read GUI/user input
void read_input();

// communicate between search and GUI inpout
void communicate();
#endif