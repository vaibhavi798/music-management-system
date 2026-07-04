#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include "song.h"

void addHistory(Song *s);
void showHistory(void);
void showHistoryReverse(void);
void saveHistoryToFile(FILE *fp);
void loadHistoryFromFile(void);

#endif
