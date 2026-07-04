#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"
#include "history.h"
#include "util.h"

static void savePlaylistToFile(Song *root, FILE *fp) {
    if (root == NULL) return;

    savePlaylistToFile(root->left, fp);
    fprintf(fp, "%d|%s|%s\n", root->id, root->title, root->artist);
    savePlaylistToFile(root->right, fp);
}

static void loadPlaylistFromFile(const char *filename, Song **root) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return;

    char line[300];

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, "|");
        if (token == NULL) continue;
        int id = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        char title[100];
        safeCopy(title, token, sizeof(title));

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        char artist[100];
        safeCopy(artist, token, sizeof(artist));

        *root = insert(*root, id, title, artist);
    }

    fclose(fp);
}

void saveAllData(Song *root, Song *root2) {
    FILE *fp1 = fopen("playlist1.txt", "w");
    FILE *fp2 = fopen("playlist2.txt", "w");
    FILE *fp3 = fopen("history.txt", "w");

    if (fp1 != NULL) {
        savePlaylistToFile(root, fp1);
        fclose(fp1);
    }

    if (fp2 != NULL) {
        savePlaylistToFile(root2, fp2);
        fclose(fp2);
    }

    if (fp3 != NULL) {
        saveHistoryToFile(fp3);
        fclose(fp3);
    }

    printf("Data saved successfully.\n");
}

void loadAllData(Song **root, Song **root2) {
    loadPlaylistFromFile("playlist1.txt", root);
    loadPlaylistFromFile("playlist2.txt", root2);
    loadHistoryFromFile();
}
