# snake

Clone of a popular snake game.

Created for _Programming I_ and _Programming II_ courses at Silesian University
of Technology.

## Building

1. Clone the repository.
2. Download SFML using `install_sfml` script:

```
$ ./install_sfml
```

3. Build the project using `make`:

```
$ make
```

## Running

To run the game:

```
$ ./snake
```

A cool feature is that you can modify snake's speed and grid size by passing
arguments:

```
$ ./snake --width 64 --height 64 --interval 100 # snake moves every 100 ms
```

To see help:

```
$ snake --help
```

## Contributing

Feel free to raise Issues and create Pull Requests.
