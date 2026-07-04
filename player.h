#ifndef PLAYER_H
#define PLAYER_H

#include "song.h"

/* Builds a play queue from root and runs an interactive next/prev/
 * repeat-toggle loop until the user stops. */
void playMenu(Song *root);

#endif
