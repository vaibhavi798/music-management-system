#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"
#include "util.h"

typedef struct History {
    int id;
    char title[100];
    char artist[100];
    struct History *prev, *next;
} History;

static History *historyHead = NULL;
static History *historyTail = NULL;

static History* createHistoryNode(int id, const char *title, const char *artist) {
    History *node = malloc(sizeof(History));

    node->id = id;
    safeCopy(node->title, title, sizeof(node->title));
    safeCopy(node->artist, artist, sizeof(node->artist));
    node->next = NULL;
    node->prev = historyTail;

    return node;
}

static void appendHistory(History *node) {
    if (historyHead == NULL)
        historyHead = node;
    else
        historyTail->next = node;

    historyTail = node;
}

void addHistory(Song *s) {
    appendHistory(createHistoryNode(s->id, s->title, s->artist));
}

void showHistory(void) {
    History *t = historyHead;

    while (t != NULL) {
        printf("%d | %s | %s\n", t->id, t->title, t->artist);
        t = t->next;
    }
}

void showHistoryReverse(void) {
    History *t = historyTail;

    while (t != NULL) {
        printf("%d | %s | %s\n", t->id, t->title, t->artist);
        t = t->prev;
    }
}

void saveHistoryToFile(FILE *fp) {
    History *t = historyHead;

    while (t != NULL) {
        fprintf(fp, "%d|%s|%s\n", t->id, t->title, t->artist);
        t = t->next;
    }
}

void loadHistoryFromFile(void) {
    FILE *fp = fopen("history.txt", "r");
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

        appendHistory(createHistoryNode(id, title, artist));
    }

    fclose(fp);
}
