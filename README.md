# snake

Clone of a popular snake game.

Created for _Programming I_ and _Programming II_ courses at Silesian University
of Technology.

## Building

### With CMake

Open the project in CLion, or alternatively from command line:

```console
git clone git@github.com:bartekpacia/snake.git
mkdir snake-build
cd snake-build
cmake ../snake
make
```

Then `cd bin/` and run `./snake`.

### With Make

1. Clone the repository.
2. Download SFML using `install_sfml` script:

    ```console
    $ ./install_sfml
    ```

3. Build the project using `make`:

    ```console
    $ make
    ```

## Running

To run the game:

```console
$ ./snake
```

A cool feature is that you can modify snake's speed and grid size by specifying
arguments:

```console
$ ./snake --width 64 --height 64 --interval 100 # snake moves every 100 ms
```

To see help:

```console
$ snake --help
```

## Contributing

Feel free to raise Issues and create Pull Requests.
