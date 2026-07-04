#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>

/* Reads one integer, reprompting on invalid input and discarding the
 * rest of the line so leftover characters can't desync later reads. */
int readInt(const char *prompt);

/* Reads two space-separated integers on one line, reprompting on
 * invalid input. */
void readTwoInts(const char *prompt, int *a, int *b);

/* Reads a line of text into str (size-bounded), stripping the newline. */
void readLine(char *str, size_t size);

#endif
