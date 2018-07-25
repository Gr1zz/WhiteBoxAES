# WhiteBox AES
============

This is a basic implementation of [Chow et al's][1] scheme, following [Muir's][2] tutorial.

[1]: https://www.cs.colorado.edu/~jrblack/class/csci7000/s03/project/oorschot-whitebox.pdf
[2]: https://eprint.iacr.org/2013/104.pdf

How to use
-----------

* **aes.c** and **aes.h** implement the standard aes encryption

* **genTables.cpp**, **RandMat.cpp** and **RandMat.h** contain the functions used to generate the whitebox

* **aes_table.c** and **main.c** implement aes with lookup-tables, protected or not

Compile with `make` to build the whitebox generator:

    $ make tables

    $ ./genTables

Then you can compile the whitebox:

    $ make
    $ ./whitebox <input>

 
To Do
------

* Find a way to pass encodings through the non-linear sbox
