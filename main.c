#include <stdio.h>
#include <string.h>
#include "song.h"
#include "history.h"
#include "setops.h"
#include "player.h"
#include "fileio.h"
#include "input.h"

static void printMenu(void) {
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
}

/* addingToFirst selects which playlist number to report in the
 * conflict message (root is playlist 1 when addingToFirst is true). */
static void handleAdd(Song **root, Song **otherRoot, int addingToFirst) {
    int id = readInt("ID: ");
    char title[100], artist[100];

    printf("Title: ");
    readLine(title, sizeof(title));
    printf("Artist: ");
    readLine(artist, sizeof(artist));

    Song *existing = searchByID(*otherRoot, id);

    if (existing != NULL &&
        (strcmp(existing->title, title) != 0 || strcmp(existing->artist, artist) != 0)) {
        printf("Error: Same ID already exists in Playlist %d with different song details.\n",
               addingToFirst ? 2 : 1);
        printf("Playlist %d can only use the same ID if title and artist are also identical.\n",
               addingToFirst ? 1 : 2);
        return;
    }

    *root = insert(*root, id, title, artist);
}

int main(void) {
    Song *root = NULL;
    Song *root2 = NULL;

    loadAllData(&root, &root2);

    int ch;
    do {
        printMenu();
        ch = readInt("Choice: ");

        switch (ch) {
            case 1:
                handleAdd(&root, &root2, 1);
                break;

            case 2:
                handleAdd(&root2, &root, 0);
                break;

            case 3: {
                int op = readInt("1.Delete from Playlist 1\n2.Delete from Playlist 2\nChoice: ");
                int id = readInt("Enter ID to delete: ");
                if (op == 1) root = deleteSong(root, id);
                else if (op == 2) root2 = deleteSong(root2, id);
                break;
            }

            case 4: {
                int op = readInt("1.Display Playlist 1\n2.Display Playlist 2\nChoice: ");
                if (op == 1) {
                    printf("\nPlaylist 1:\n");
                    displayByID(root);
                } else if (op == 2) {
                    printf("\nPlaylist 2:\n");
                    displayByID(root2);
                }
                break;
            }

            case 5: {
                int playlistChoice = readInt("1.Search in Playlist 1\n2.Search in Playlist 2\nChoice: ");
                Song *selectedRoot = (playlistChoice == 1) ? root : root2;

                int op = readInt("1.ID 2.Title 3.Artist\nChoice: ");

                if (op == 1) {
                    int id = readInt("Enter ID: ");
                    Song *s = searchByID(selectedRoot, id);

                    if (s != NULL) {
                        printf("Found: %s\n", s->title);
                        addHistory(s);
                    } else {
                        printf("Song not found\n");
                    }
                } else if (op == 2) {
                    char title[100];
                    printf("Enter Title: ");
                    readLine(title, sizeof(title));
                    searchByTitle(selectedRoot, title);
                } else if (op == 3) {
                    char artist[100];
                    printf("Enter Artist: ");
                    readLine(artist, sizeof(artist));
                    searchByArtist(selectedRoot, artist);
                }
                break;
            }

            case 6: {
                int playlistChoice = readInt("1.Range Search Playlist 1\n2.Range Search Playlist 2\nChoice: ");
                int s1, s2;
                readTwoInts("Enter range (min max): ", &s1, &s2);
                rangeSearch(playlistChoice == 2 ? root2 : root, s1, s2);
                break;
            }

            case 7: {
                int playlistChoice = readInt("1.Shuffle Playlist 1\n2.Shuffle Playlist 2\nChoice: ");
                if (playlistChoice == 1) shuffle(root);
                else if (playlistChoice == 2) shuffle(root2);
                else printf("Invalid choice\n");
                break;
            }

            case 8: {
                int playlistChoice = readInt("1.Sort Playlist 1\n2.Sort Playlist 2\nChoice: ");
                int op = readInt("1.Title 2.Artist 3.Artist+Title\nChoice: ");
                displaySorted(playlistChoice == 2 ? root2 : root, op);
                break;
            }

            case 9: {
                int playlistChoice = readInt("1.Play Playlist 1\n2.Play Playlist 2\nChoice: ");
                playMenu(playlistChoice == 2 ? root2 : root);
                break;
            }

            case 10: {
                int op = readInt("1.Normal 2.Reverse\nChoice: ");
                if (op == 1) showHistory();
                else showHistoryReverse();
                break;
            }

            case 11: unionPlaylists(root, root2); break;
            case 12: intersection(root, root2); break;
            case 13: difference(root, root2); break;
            case 14: symmetricDiff(root, root2); break;
            case 15: saveAllData(root, root2); break;

            case 16:
                root = NULL;
                root2 = NULL;
                loadAllData(&root, &root2);
                printf("Data loaded successfully.\n");
                break;

            case 17:
                saveAllData(root, root2);
                break;

            default:
                printf("Invalid choice.\n");
        }
    } while (ch != 17);

    return 0;
}
