# dmake
> easy replacement for make

dmake is a program for compiling any project easily with no makefile required,
dmake will assess the environment it's currently in and compiles the project automatically, additionally,
dmake can also do installations easily with no configuration needed. 

![](header.png)

## Installation

*Compile dmake for the first time*
```sh
make
```

*Install dmake into your system*
```sh
sudo make install
```

***DONE!***


## Usage

##### Compiling a project

- cd into a project's directory
- enter the command `dmake`
- dmake will assess your project and compile it
- all output files will be in the `bin` directory

##### Installing a project

- once a project has been compiled
- enter the command `sudo dmake install`
- dmake will move the binary from `bin` into the `/usr/bin/` directory

## About

Daniel Jones – [McSwaggens](https://github.com/McSwaggens/) – McSwaggens@protonmail.com

Copyright 2016 Daniel Jones

Distributed under the `GNU GPL v3` license. See ``LICENSE`` for more information.
