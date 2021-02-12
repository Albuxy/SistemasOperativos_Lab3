#ifndef MYUTILS
#define MYUTILS

#include <sys/time.h>
#include <unistd.h>     // unix-like system calls read and write
#include <fcntl.h>      // unix-like file handling : open
#include <stdio.h>      // standard C lib input output basic functions compatible with Windows
#include <string.h>     // also from standard C lib : basic string functions like strlen


void startTimer();   // start timer
long endTimer();     // returns millis since startTimer

int readSplit( int fin, char* buff, char* s, int maxlen );
// It reads from "fin" writing the content in "buff"
// until: character "s" or new-line "\n" or maxlength 
// if "\n" was found instead of "s", it is returned in "s"

// Inputs:
// -descriptor 0 (stdin) / file descriptor / socket
// -buff where to write the bytes that will be read
// -input separator character, but if '\n' is found it is also returned
// -max length to read: usually the same than the buff size
// 
// Returns:  
// -it returns the number of characters that it was able to read
// -buff is filled with the content it was able to read
// -s is filled with '\n' if it is found






#endif