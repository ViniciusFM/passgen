Password Generator
==================

This is a simple GPL password generator with a dumb pseudorandom integer generator. This source was made during a hobby programming moment and may or may not be used as a serious program to generate secure passwords.

You can compile this source using GCC by running:
```
$ mkdir bin
$ gcc --std=c11 -o bin/passgen passgen.c
```

The software usage is simples as follows:

```
Usage:

        > passgen [flags] [password_length]

Flags are: 

        l -> lowercase [a-z]
        L -> uppercase [A-Z]
        n -> number [0-9]
        s -> symbol characters [!@#$*()_,.;:?+-=]

Password length restriction [default 16]: 

        password_length must be greater than 0 and lesser than 4294967295.

Example of use:

* All flags activated and default 16 characters length:

        > passgen
        9LwVZ06JF/)G-8H*

* Letters and numbers flags activated and 8 character length:

        > passgen lLn 8
        a18bR69x
```