
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Song {
    int id;
    char title[100];
    char artist[100];
    int height;
    struct Song *left, *right;
} Song;

typedef struct History {
    int id;
    char title[100];
    char artist[100];
    struct History *prev, *next;
} History;

History *historyHead = NULL;
History *historyTail = NULL;

Song *root = NULL;
Song *root2 = NULL;

Song *playArr[500];
int playN = 0;
int currentIndex = -1;
int repeatMode = 0;

int height(Song *n) {
    if (n == NULL) return 0;
    return n->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Song* createSong(int id, char *title, char *artist) {
    Song *node = (Song*)malloc(sizeof(Song));
    node->id = id;
    strcpy(node->title, title);
    strcpy(node->artist, artist);
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int getBalance(Song *n) {
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

Song* rightRotate(Song *y) {
    Song *x = y->left;
    Song *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Song* leftRotate(Song *x) {
    Song *y = x->right;
    Song *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

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

    node->height = 1 + max(height(node->left), height(node->right));

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

Song* minValueNode(Song* node) {
    Song* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Song* deleteSong(Song* root, int id) {
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
            strcpy(root->title, temp->title);
            strcpy(root->artist, temp->artist);
            root->right = deleteSong(root->right, temp->id);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

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

void storeSongs(Song *root, Song **arr, int *n) {
    if (root == NULL) return;

    storeSongs(root->left, arr, n);
    arr[*n] = root;
    (*n)++;
    storeSongs(root->right, arr, n);
}

void displayByID(Song *root) {
    if (root == NULL) return;

    displayByID(root->left);
    printf("%d | %s | %s\n", root->id, root->title, root->artist);
    displayByID(root->right);
}

int cmpTitle(const void *a, const void *b) {
    Song *s1 = *(Song**)a;
    Song *s2 = *(Song**)b;
    return strcmp(s1->title, s2->title);
}

int cmpArtist(const void *a, const void *b) {
    Song *s1 = *(Song**)a;
    Song *s2 = *(Song**)b;
    return strcmp(s1->artist, s2->artist);
}

int cmpArtistTitle(const void *a, const void *b) {
    Song *s1 = *(Song**)a;
    Song *s2 = *(Song**)b;

    int res = strcmp(s1->artist, s2->artist);
    if (res == 0)
        return strcmp(s1->title, s2->title);

    return res;
}

void displaySorted(Song *root, int mode) {
    Song *arr[500];
    int n = 0;

    storeSongs(root, arr, &n);

    if (mode == 1)
        qsort(arr, n, sizeof(Song*), cmpTitle);
    else if (mode == 2)
        qsort(arr, n, sizeof(Song*), cmpArtist);
    else
        qsort(arr, n, sizeof(Song*), cmpArtistTitle);

    for (int i = 0; i < n; i++)
        printf("%d | %s | %s\n", arr[i]->id, arr[i]->title, arr[i]->artist);
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

void addHistory(Song *s) {
    History *node = (History*)malloc(sizeof(History));

    node->id = s->id;
    strcpy(node->title, s->title);
    strcpy(node->artist, s->artist);

    node->next = NULL;
    node->prev = historyTail;

    if (historyHead == NULL)
        historyHead = node;
    else
        historyTail->next = node;

    historyTail = node;
}

void showHistory() {
    History *t = historyHead;

    while (t != NULL) {
        printf("%d | %s | %s\n", t->id, t->title, t->artist);
        t = t->next;
    }
}

void buildPlayArray(Song *root) {
    playN = 0;
    storeSongs(root, playArr, &playN);
    currentIndex = -1;
}

void playNext() {
    if (playN == 0) return;

    if (currentIndex < playN - 1)
        currentIndex++;
    else {
        if (repeatMode == 1)
            currentIndex = 0;
        else
            return;
    }

    printf("Playing: %s\n", playArr[currentIndex]->title);
    addHistory(playArr[currentIndex]);
}

void shuffle(Song *root) {
    Song *arr[500];
    int n = 0;

    storeSongs(root, arr, &n);

    srand(time(0));

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Song *temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    for (int i = 0; i < n; i++)
        printf("%d | %s | %s\n", arr[i]->id, arr[i]->title, arr[i]->artist);
}
void showHistoryReverse() {
    History *t = historyTail;

    while (t != NULL) {
        printf("%d | %s | %s\n", t->id, t->title, t->artist);
        t = t->prev;
    }
}

void playPrev() {
    if (playN == 0) return;

    if (currentIndex > 0)
        currentIndex--;
    else {
        if (repeatMode == 1)
            currentIndex = playN - 1;
        else
            return;
    }

    printf("Playing: %s\n", playArr[currentIndex]->title);
    addHistory(playArr[currentIndex]);
}

void unionPlaylists(Song *r1, Song *r2) {
    Song *arr[800];
    int n = 0;

    storeSongs(r1, arr, &n);

    Song *arr2[400];
    int n2 = 0;
    storeSongs(r2, arr2, &n2);

    for (int i = 0; i < n2; i++) {
        int exists = 0;

        for (int j = 0; j < n; j++) {
            if (arr[j]->id == arr2[i]->id) {
                exists = 1;
                break;
            }
        }

        if (exists == 0) {
            arr[n] = arr2[i];
            n++;
        }
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i]->id > arr[j]->id) {
                Song *temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    printf("Union Playlist:\n");

    for (int i = 0; i < n; i++) {
        printf("%d | %s | %s\n", arr[i]->id, arr[i]->title, arr[i]->artist);
    }
}

void intersection(Song *r1, Song *r2) {
    Song *arr[500];
    int n = 0;

    storeSongs(r1, arr, &n);

    for (int i = 0; i < n; i++) {
        Song *f = searchByID(r2, arr[i]->id);

        if (f != NULL)
            printf("%d | %s | %s\n", arr[i]->id, arr[i]->title, arr[i]->artist);
    }
}

void difference(Song *r1, Song *r2) {
    Song *arr[500];
    int n = 0;

    storeSongs(r1, arr, &n);

    for (int i = 0; i < n; i++) {
        Song *f = searchByID(r2, arr[i]->id);

        if (f == NULL)
            printf("%d | %s | %s\n", arr[i]->id, arr[i]->title, arr[i]->artist);
    }
}

void symmetricDiff(Song *r1, Song *r2) {
    difference(r1, r2);
    difference(r2, r1);
}

void savePlaylistToFile(Song *root, FILE *fp) {
    if (root == NULL) return;

    savePlaylistToFile(root->left, fp);
    fprintf(fp, "%d|%s|%s\n", root->id, root->title, root->artist);
    savePlaylistToFile(root->right, fp);
}

void saveHistoryToFile(FILE *fp) {
    History *temp = historyHead;

    while (temp != NULL) {
        fprintf(fp, "%d|%s|%s\n", temp->id, temp->title, temp->artist);
        temp = temp->next;
    }
}

void addHistoryFromFile(int id, char *title, char *artist) {
    History *node = (History*)malloc(sizeof(History));

    node->id = id;
    strcpy(node->title, title);
    strcpy(node->artist, artist);

    node->next = NULL;
    node->prev = historyTail;

    if (historyHead == NULL)
        historyHead = node;
    else
        historyTail->next = node;

    historyTail = node;
}

void loadHistoryFromFile() {
    FILE *fp = fopen("history.txt", "r");

    if (fp == NULL)
        return;

    char line[300];
    int id;
    char title[100], artist[100];

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, "|");
        if (token == NULL) continue;
        id = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(title, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(artist, token);

        addHistoryFromFile(id, title, artist);
    }

    fclose(fp);
}
void loadPlaylistFromFile(char *filename, Song **root) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
        return;

    char line[300];
    int id;
    char title[100], artist[100];

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, "|");
        if (token == NULL) continue;
        id = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(title, token);

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(artist, token);

        *root = insert(*root, id, title, artist);
    }

    fclose(fp);
}

void saveAllData() {
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

void loadAllData() {
    loadPlaylistFromFile("playlist1.txt", &root);
    loadPlaylistFromFile("playlist2.txt", &root2);
    loadHistoryFromFile();
}

void readLine(char *str) {
    fgets(str, 100, stdin);
    str[strcspn(str, "\n")] = 0;
}

int main() {
    int ch, id;
    char title[100], artist[100];

    loadAllData();

    while (1) {
       // printf("\n1.Add(P1)\n2.Add(P2)\n3.Delete\n4.Display\n5.Search\n6.Range\n7.Shuffle\n8.Sort\n9.Play\n10.History\n11.Union\n12.Intersection\n13.Difference\n14.SymmetricDiff\n15.Save\n16.Load\n17.Exit\n");
        printf("\n");
printf("1.Add(P1)\t\t10.History\n");
printf("2.Add(P2)\t\t11.Union\n");
printf("3.Delete\t\t12.Intersection\n");
printf("4.Display\t\t13.Difference\n");
printf("5.Search\t\t14.SymmetricDiff\n");
printf("6.Range\t\t\t15.Save\n");
printf("7.Shuffle\t\t16.Load\n");
printf("8.Sort\t\t\t17.Exit\n");
printf("9.Play\n");
        scanf("%d", &ch);
        getchar();

        if (ch == 1 || ch == 2) {
    printf("ID: ");
    scanf("%d", &id);
    getchar();

    printf("Title: ");
    readLine(title);

    printf("Artist: ");
    readLine(artist);

    if (ch == 1) {
        Song *existingSong = searchByID(root2, id);

        if (existingSong != NULL) {
            if (strcmp(existingSong->title, title) != 0 ||
                strcmp(existingSong->artist, artist) != 0) {

                printf("Error: Same ID already exists in Playlist 2 with different song details.\n");
                printf("Playlist 1 can only use the same ID if title and artist are also identical.\n");
            }
            else {
                root = insert(root, id, title, artist);
            }
        }
        else {
            root = insert(root, id, title, artist);
        }
    }
    else {
        Song *existingSong = searchByID(root, id);

        if (existingSong != NULL) {
            if (strcmp(existingSong->title, title) != 0 ||
                strcmp(existingSong->artist, artist) != 0) {

                printf("Error: Same ID already exists in Playlist 1 with different song details.\n");
                printf("Playlist 2 can only use the same ID if title and artist are also identical.\n");
            }
            else {
                root2 = insert(root2, id, title, artist);
            }
        }
        else {
            root2 = insert(root2, id, title, artist);
        }
    }
}
        else if (ch == 3) {
    int op;

    printf("1.Delete from Playlist 1\n2.Delete from Playlist 2\n");
    scanf("%d", &op);

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    if (op == 1)
        root = deleteSong(root, id);
    else if (op == 2)
        root2 = deleteSong(root2, id);
}
        else if (ch == 4) {
    int op;

    printf("1.Display Playlist 1\n2.Display Playlist 2\n");
    scanf("%d", &op);

    if (op == 1) {
        printf("\nPlaylist 1:\n");
        displayByID(root);
    }
    else if (op == 2) {
        printf("\nPlaylist 2:\n");
        displayByID(root2);
    }
}
      else if (ch == 5) {
    int playlistChoice;
    Song *selectedRoot;

    printf("1.Search in Playlist 1\n2.Search in Playlist 2\n");
    scanf("%d", &playlistChoice);
    getchar();

    if (playlistChoice == 1)
        selectedRoot = root;
    else
        selectedRoot = root2;

    printf("1.ID 2.Title 3.Artist\n");
    int op;
    scanf("%d", &op);
    getchar();

    if (op == 1) {
        printf("Enter ID: ");
        scanf("%d", &id);

        Song *s = searchByID(selectedRoot, id);

        if (s != NULL) {
            printf("Found: %s\n", s->title);
            addHistory(s);
        }else {
            printf("Song not found\n");
        }
    }
    else if (op == 2) {
        printf("Enter Title: ");
        readLine(title);
        searchByTitle(selectedRoot, title);
    }
    else if (op == 3) {
        printf("Enter Artist: ");
        readLine(artist);
        searchByArtist(selectedRoot, artist);
    }
}
        else if (ch == 6) {
            int s1, s2;
            printf("Enter range: ");
            scanf("%d %d", &s1, &s2);
            rangeSearch(root, s1, s2);
        }
        else if (ch == 7) {
    int playlistChoice;

    printf("1.Shuffle Playlist 1\n2.Shuffle Playlist 2\n");
    scanf("%d", &playlistChoice);

    if (playlistChoice == 1)
        shuffle(root);
    else if (playlistChoice == 2)
        shuffle(root2);
    else
        printf("Invalid choice\n");
}
        else if (ch == 8) {
            int op;
            printf("1.Title 2.Artist 3.Artist+Title\n");
            scanf("%d", &op);
            displaySorted(root, op);
        }
        else if (ch == 9) {
            buildPlayArray(root);
            playNext();
            playNext();
            playPrev();
        }
        else if (ch == 10) {
            int op;
            printf("1.Normal 2.Reverse\n");
            scanf("%d", &op);

            if (op == 1)
                showHistory();
            else
                showHistoryReverse();
        }
        else if (ch == 11) {
            unionPlaylists(root, root2);
        }
        else if (ch == 12) {
            intersection(root, root2);
        }
        else if (ch == 13) {
            difference(root, root2);
        }
        else if (ch == 14) {
            symmetricDiff(root, root2);
        }
        else if (ch == 15) {
            saveAllData();
        }
        else if (ch == 16) {
            root = NULL;
            root2 = NULL;
            loadAllData();
            printf("Data loaded successfully.\n");
        }
        else if (ch == 17) {
            saveAllData();
            break;
        }
    }

    return 0;
}