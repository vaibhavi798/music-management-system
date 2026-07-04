#ifndef SETOPS_H
#define SETOPS_H

#include "song.h"

void unionPlaylists(Song *r1, Song *r2);
void intersection(Song *r1, Song *r2);
void difference(Song *r1, Song *r2);
void symmetricDiff(Song *r1, Song *r2);

#endif
