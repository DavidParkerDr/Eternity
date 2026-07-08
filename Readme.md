# MAXIMUS

*Son of Brutus and Mark IV*

*Bearer of the Glorious Arrays of Genius*

**Eternity 2 Solver Program**

## Instructions

### Command Line Interface (CLI)

The CLI is the interface to Maximus. It offers the following commands, all of which can also be seen by typing `HELP!` or just `help` into the program:

- `CLS`  
  Simply clears the screen of text.

- `LOAD`  
  Loads a previously saved solver thread. All saved files are stored in the `files/` directory, and the program looks there automatically. This command asks for the file name you wish to load, which should be the name you originally gave the thread. You do not need to include the `files/` part. If the file can be found, Maximus loads it and continues from where it left off.

- `NOVIEW`  
  Cancels the board viewer, if active.

- `QSTART`  
  Quickstart, used mostly for testing. This automatically starts a new thread using the full Eternity 2 dataset, going from corner configuration 0 to corner configuration 23, with the save file name `quickstart.txt`.

- `QUIT`  
  Exits the program. It shuts down and saves all running threads first, including the viewer.

- `RUN`  
  Runs a batch file. Maximus asks for a filename, which is the name of the batch file you want to load. It does not automatically look in the `files/` directory for batch files. See [Batch Files](#batch-files) below for more information.

- `START`  
  Starts a new solver thread from the beginning. You will be asked for:
  - which dataset to use, from the list or by typing your own;
  - which corner configuration to start with;
  - if you did not choose corner 23, which corner config to end with;
  - what name you wish to give the thread, which is used to identify it in `STATUS` and also as its save file name;
  - if you want to use a path. If you type `N` or `n` for no path, it uses the default `VERTICAL`. You can also type `VERTICAL` or `HORIZONTAL`.

- `STATUS`  
  Lists all currently running threads and shows their names, datasets, and corner configurations.

- `STOP`  
  Stops a currently running thread. You will be offered a list to choose from. Stopping a thread also saves it.

- `TIMER`  
  Used for timing how quickly the program runs. It uses a small 4x4 dataset and runs it 1000 times; console output is disabled so you see nothing until it finishes. You cannot run any more commands until it completes. Running other solver threads at the same time will obviously affect the results.

- `VIEW`  
  Views a currently running thread. You will be offered a list to choose from.

Note that you can use the normal command line keys: delete, home, end, and so on. You can press the up arrow to see the last thing you typed. You can also queue commands or parameters; for example, rather than typing `START <enter> 1 <enter> 0 <enter> 24 <enter> y <enter>`, you can just type `START 1 0 24 y <enter>`. You could even type `START 1 0 24 y VIEW 1<enter>` which starts the thread and opens the viewer.

Commands can be typed in both lower and upper case, but not a mixture of both until the STL provides a `toUpper()` function like virtually every other string library in virtually every other language.

## Batch Files

Batch files are external text files that can store commands. The only commands possible are `load` and `start`, and they are case-sensitive in batch files. They can be used as follows:

```text
start <dataset> <start corner config> <end corner config> <name> <pathname>
```

where `pathname` can also be `NONE` in which case it defaults to `VERTICAL`. Any other pathname is treated as a custom path file and is loaded from the `paths/` directory.

```text
load <save file>
```

which is self explanatory. You can also use `/* comments */`.

When running a batch file from within the CLI, the console input can get interrupted because currently running threads interrupt the batch process. For best results, do not load too many threads at once.

## Other Files

There are six types of file used or produced by Maximus.

### Datasets

There are four datasets, all stored in the `datasets/` directory:

- `datasets/Dataset_Eternity2.txt` - the main puzzle
- `datasets/Dataset_clue1.txt` - the small clue
- `datasets/Dataset_clue2.txt` - the big clue
- `datasets/Dataset_online.txt` - the online puzzle

The format is as follows:

```text
<board width> <board height>
<index of tile> <north pattern> <east pattern> <south pattern> <west pattern>
<index of tile> <north pattern> <east pattern> <south pattern> <west pattern>
<index of tile> <north pattern> <east pattern> <south pattern> <west pattern>
...
```

The patterns are single letters from A to W, where A is the grey border.

### Batch files

Described above, stored in the main directory.

### Solution files

When the program finds its first solution for a given board, it produces a solution file, and only one. It is in the same format as the save file except its name is of the form `files/solutions/<savename>-thread<thread ID>_solution.txt`. It also writes a matching grid file at `files/solutions/<savename>-thread<thread ID>_solutionGrid.txt`.

### Save files

Produced by the program and stored in the `files/` directory. The format is mostly self explanatory, but basically:

```text
DATASET:                        <dataset name>
PATHFILE:                       <path name>
START_CORNER_CONFIG:    <obvious>
END_CORNER_CONFIG:      <obvious>
CURRENT_CORNER_CONFIG:  <where it got to>
FURTHEST:                       <furthest path ID>
SOLUTIONS:                      <how many solutions it found>
<tile index> <rotation> <x pos> <y pos> <fixed flag>
```

Tile index is 1 to 256, rotation is in 90 degree clockwise increments, and x and y positions range from 0 to board size minus 1. The flag tells the program whether or not a tile is in a fixed position.

### Output grids

These are another form of output that show the tiles, and their rotations, in approximate board positions. They are for viewing only; they do not have any other purpose. Maximus produces them automatically when saving and stores them in the `files/` directory.

### Destroyed files

When a thread object is destroyed, Maximus writes `files/Destroyed<thread ID>.txt`.
