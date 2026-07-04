#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "song.h"
#include "util.h"

static int height(Song *n) {
    return n ? n->height : 0;
}

static int maxInt(int a, int b) {
    return a > b ? a : b;
}

static int getBalance(Song *n) {
    return n ? height(n->left) - height(n->right) : 0;
}

static Song* createSong(int id, char *title, char *artist) {
    Song *node = malloc(sizeof(Song));
    node->id = id;
    safeCopy(node->title, title, sizeof(node->title));
    safeCopy(node->artist, artist, sizeof(node->artist));
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static Song* rightRotate(Song *y) {
    Song *x = y->left;
    Song *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = maxInt(height(y->left), height(y->right)) + 1;
    x->height = maxInt(height(x->left), height(x->right)) + 1;

    return x;
}

static Song* leftRotate(Song *x) {
    Song *y = x->right;
    Song *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = maxInt(height(x->left), height(x->right)) + 1;
    y->height = maxInt(height(y->left), height(y->right)) + 1;

    return y;
}

Song* insert(Song *node, int id, char *title, char *artist) {
    if (node == NULL)
        return createSong(id, title, artist);

    if (id < node->id)
        node->left = insert(node->left, id, title, artist);
    else if (id > node->id)
        node->right = insert(node->right, id, title, artist);
    else
        return node;

    node->height = 1 + maxInt(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && id < node->left->id)
        return rightRotate(node);

    if (balance < -1 && id > node->right->id)
        return leftRotate(node);

    if (balance > 1 && id > node->left->id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && id < node->right->id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

static Song* minValueNode(Song *node) {
    Song *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Song* deleteSong(Song *root, int id) {
    if (root == NULL)
        return root;

    if (id < root->id)
        root->left = deleteSong(root->left, id);
    else if (id > root->id)
        root->right = deleteSong(root->right, id);
    else {
        if (root->left == NULL || root->right == NULL) {
            Song *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }

            free(temp);
        } else {
            Song *temp = minValueNode(root->right);
            root->id = temp->id;
            safeCopy(root->title, temp->title, sizeof(root->title));
            safeCopy(root->artist, temp->artist, sizeof(root->artist));
            root->right = deleteSong(root->right, temp->id);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + maxInt(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Song* searchByID(Song *root, int id) {
    if (root == NULL) return NULL;
    if (root->id == id) return root;
    if (id < root->id) return searchByID(root->left, id);
    return searchByID(root->right, id);
}

void searchByTitle(Song *root, char *title) {
    if (root == NULL) return;

    searchByTitle(root->left, title);

    if (strcmp(root->title, title) == 0)
        printf("%d | %s | %s\n", root->id, root->title, root->artist);

    searchByTitle(root->right, title);
}

void searchByArtist(Song *root, char *artist) {
    if (root == NULL) return;

    searchByArtist(root->left, artist);

    if (strcmp(root->artist, artist) == 0)
        printf("%d | %s | %s\n", root->id, root->title, root->artist);

    searchByArtist(root->right, artist);
}

void displayByID(Song *root) {
    if (root == NULL) return;

    displayByID(root->left);
    printf("%d | %s | %s\n", root->id, root->title, root->artist);
    displayByID(root->right);
}

void rangeSearch(Song *root, int s1, int s2) {
    if (root == NULL) return;

    if (s1 < root->id)
        rangeSearch(root->left, s1, s2);

    if (root->id >= s1 && root->id <= s2)
        printf("%d | %s | %s\n", root->id, root->title, root->artist);

    if (s2 > root->id)
        rangeSearch(root->right, s1, s2);
}

int countSongs(Song *root) {
    if (root == NULL) return 0;
    return 1 + countSongs(root->left) + countSongs(root->right);
}

static void storeSongsHelper(Song *root, Song **arr, int *idx) {
    if (root == NULL) return;

    storeSongsHelper(root->left, arr, idx);
    arr[*idx] = root;
    (*idx)++;
    storeSongsHelper(root->right, arr, idx);
}

Song** storeSongsArray(Song *root, int *n) {
    *n = countSongs(root);
    if (*n == 0) return NULL;

    Song **arr = malloc((*n) * sizeof(Song*));
    int idx = 0;
    storeSongsHelper(root, arr, &idx);
    return arr;
}

static int cmpTitle(const void *a, const void *b) {
    Song *s1 = *(Song**)a;
    Song *s2 = *(Song**)b;
    return strcmp(s1->title, s2->title);
}

static int cmpArtist(const void *a, const void *b) {
    Song *s1 = *(Song**)a;
    Song *s2 = *(Song**)b;
    return strcmp(s1->artist, s2->artist);
}

static int cmpArtistTitle(const void *a, const void *b) {
    Song *s1 = *(Song**)a;
    Song *s2 = *(Song**)b;

    int res = strcmp(s1->artist, s2->artist);
    if (res == 0)
        return strcmp(s1->title, s2->title);

    return res;
}

void displaySorted(Song *root, int mode) {
    int n;
    Song **arr = storeSongsArray(root, &n);

    if (n == 0) {
        printf("Playlist is empty.\n");
        return;
    }

    if (mode == 1)
        qsort(arr, n, sizeof(Song*), cmpTitle);
    else if (mode == 2)
        qsort(arr, n, sizeof(Song*), cmpArtist);
    else
        qsort(arr, n, sizeof(Song*), cmpArtistTitle);

    for (int i = 0; i < n; i++)
        printf("%d | %s | %s\n", arr[i]->id, arr[i]->title, arr[i]->artist);

    free(arr);
}

void shuffle(Song *root) {
    int n;
    Song **arr = storeSongsArray(root, &n);

    if (n == 0) {
        printf("Playlist is empty.\n");
        return;
    }

    srand(time(0));

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Song *temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    for (int i = 0; i < n; i++)
        printf("%d | %s | %s\n", arr[i]->id, arr[i]->title, arr[i]->artist);

    free(arr);
}
