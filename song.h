#ifndef SONG_H
#define SONG_H

typedef struct Song {
    int id;
    char title[100];
    char artist[100];
    int height;
    struct Song *left, *right;
} Song;

Song* insert(Song *node, int id, char *title, char *artist);
Song* deleteSong(Song *root, int id);
Song* searchByID(Song *root, int id);
void searchByTitle(Song *root, char *title);
void searchByArtist(Song *root, char *artist);
void displayByID(Song *root);
void displaySorted(Song *root, int mode);
void rangeSearch(Song *root, int s1, int s2);
void shuffle(Song *root);

int countSongs(Song *root);

/* In-order snapshot of the tree as an array of node pointers.
 * Sized dynamically (via countSongs) instead of a fixed-size buffer,
 * so it can't silently overflow once a playlist grows past a hardcoded
 * cap. Caller owns the returned array and must free() it (NULL if empty). */
Song** storeSongsArray(Song *root, int *n);

#endif
