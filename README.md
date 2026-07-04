# Music Management System

A console-based music playlist manager written in C, built on two AVL
trees and a doubly linked list.

## Data structures

- **AVL trees** (`song.c`) — one per playlist, keyed by song ID. Standard
  rotations keep insert/delete/search at O(log n) regardless of playlist size.
- **Doubly linked list** (`history.c`) — records every song played or
  searched, in order, viewable forwards or backwards.

## Features

- Add / delete / display songs in either playlist
- Search by ID, title, or artist
- Range search by ID (either playlist)
- Shuffle and sort (by title, artist, or artist+title), for either playlist
- Interactive player: next / previous / toggle repeat / stop
- Play/search history, forward or reverse
- Set operations across the two playlists: union, intersection,
  difference, symmetric difference
- Save/load playlists and history to/from text files
  (`playlist1.txt`, `playlist2.txt`, `history.txt`)

## Build & run

    make
    ./music_manager

## Project layout

    main.c      - menu loop
    song.c/h    - Song struct + AVL tree operations
    history.c/h - play/search history (doubly linked list)
    setops.c/h  - union / intersection / difference / symmetric difference
    player.c/h  - interactive playback (next / prev / repeat)
    fileio.c/h  - save/load playlists and history to disk
    input.c/h   - validated console input helpers
    util.c/h    - shared bounded string-copy helper
