# SIRIUS – PART IT CTF Writeup


## Challenge Overview


The program reads a line with `gets()` into a 64-byte buffer on the stack (`char buf[64]`) and stores an int auth right after it (initialized to 0). If `auth == 0x1337` after input, the program prints the poem and reads/prints flag.txt. The objective is to craft input that overflows buf and overwrites auth with the magic value `0x1337`.

