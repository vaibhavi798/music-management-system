#include <stdio.h>
#include <stdlib.h>
#include "setops.h"

void unionPlaylists(Song *r1, Song *r2) {
    int n1, n2;
    Song **arr1 = storeSongsArray(r1, &n1);
    Song **arr2 = storeSongsArray(r2, &n2);

    int cap = n1 + n2;
    Song **result = cap > 0 ? malloc(cap * sizeof(Song*)) : NULL;
    int n = 0;

    for (int i = 0; i < n1; i++)
        result[n++] = arr1[i];

    for (int i = 0; i < n2; i++) {
        int exists = 0;

        for (int j = 0; j < n1; j++) {
            if (arr1[j]->id == arr2[i]->id) {
                exists = 1;
                break;
            }
        }

        if (!exists)
            result[n++] = arr2[i];
    }

    for (int i = 1; i < n; i++) {
        Song *key = result[i];
        int j = i - 1;
        while (j >= 0 && result[j]->id > key->id) {
            result[j + 1] = result[j];
            j--;
        }
        result[j + 1] = key;
    }

    printf("Union Playlist:\n");
    for (int i = 0; i < n; i++)
        printf("%d | %s | %s\n", result[i]->id, result[i]->title, result[i]->artist);

    free(arr1);
    free(arr2);
    free(result);
}

void intersection(Song *r1, Song *r2) {
    int n;
    Song **arr = storeSongsArray(r1, &n);

    for (int i = 0; i < n; i++) {
        if (searchByID(r2, arr[i]->id) != NULL)
            printf("%d | %s | %s\n", arr[i]->id, arr[i]->title, arr[i]->artist);
    }

    free(arr);
}

void difference(Song *r1, Song *r2) {
    int n;
    Song **arr = storeSongsArray(r1, &n);

    for (int i = 0; i < n; i++) {
        if (searchByID(r2, arr[i]->id) == NULL)
            printf("%d | %s | %s\n", arr[i]->id, arr[i]->title, arr[i]->artist);
    }

    free(arr);
}

void symmetricDiff(Song *r1, Song *r2) {
    difference(r1, r2);
    difference(r2, r1);
}
