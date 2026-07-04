#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "history.h"
#include "input.h"

static Song **playArr = NULL;
static int playN = 0;
static int currentIndex = -1;
static int repeatMode = 0;

static void buildPlayArray(Song *root) {
    free(playArr);
    playArr = storeSongsArray(root, &playN);
    currentIndex = -1;
}

static void toggleRepeat(void) {
    repeatMode = !repeatMode;
    printf("Repeat mode is now %s.\n", repeatMode ? "ON" : "OFF");
}

static void playNext(void) {
    if (playN == 0) {
        printf("Playlist is empty.\n");
        return;
    }

    if (currentIndex < playN - 1)
        currentIndex++;
    else if (repeatMode)
        currentIndex = 0;
    else {
        printf("End of playlist.\n");
        return;
    }

    printf("Playing: %s\n", playArr[currentIndex]->title);
    addHistory(playArr[currentIndex]);
}

static void playPrev(void) {
    if (playN == 0) {
        printf("Playlist is empty.\n");
        return;
    }

    if (currentIndex > 0)
        currentIndex--;
    else if (repeatMode)
        currentIndex = playN - 1;
    else {
        printf("Start of playlist.\n");
        return;
    }

    printf("Playing: %s\n", playArr[currentIndex]->title);
    addHistory(playArr[currentIndex]);
}

void playMenu(Song *root) {
    buildPlayArray(root);

    if (playN == 0) {
        printf("Playlist is empty.\n");
        return;
    }

    int choice;
    do {
        printf("\n--- Player (Repeat: %s) ---\n", repeatMode ? "ON" : "OFF");
        printf("1.Play Next\t2.Play Previous\t3.Toggle Repeat\t4.Stop\n");
        choice = readInt("Choice: ");

        switch (choice) {
            case 1: playNext(); break;
            case 2: playPrev(); break;
            case 3: toggleRepeat(); break;
            case 4: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);
}
