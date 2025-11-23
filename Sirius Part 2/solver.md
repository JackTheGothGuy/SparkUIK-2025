# SIRIUS – PART IT CTF Writeup


<img width="150" height="20" alt="image" src="https://github.com/user-attachments/assets/4a8c0e62-bda6-49d1-875d-502672d5b050" />


## Challenge Overview


The program reads a line with `gets()` into a 64-byte buffer on the stack (`char buf[64]`) and stores an int auth right after it (initialized to 0). If `auth == 0x1337` after input, the program prints the poem and reads/prints flag.txt. The objective is to craft input that overflows buf and overwrites auth with the magic value `0x1337`.

##Vulnerability



Use of `gets()` — `gets()` never bounds checks input. It allows arbitrary-length writes into `buf`. This is the direct source of the buffer overflow.

Stack-local `auth` variable adjacent to `buf` — `auth` (an `int`) is stored on the stack immediately after `buf` and can be overwritten by overflowing `buf`.

No explicit stack protections assumed in challenge — the challenge likely expects exploitation through overwriting `auth`. Depending on compilation flags and platform, other hardening may be present (stack canaries, ASLR, PIE, NX). The simplest route is a straight overwrite of `auth`, not return-address control.











buf is 64 bytes. Writing 64 bytes fills buf. The next 4 bytes on the stack are auth (an int), so write additional 4 bytes with the little-endian representation of 0x1337 (b'\x37\x13\x00\x00') to overwrite auth.

Send the total payload: 64 bytes filler + 4 bytes p32(0x1337). The program will then check auth and, if equal, run sirius_poem() and print_flag().

Important note about printf("%s", buf): the program prints buf after the gets(). If your 4 bytes for auth include \x00 (they do), the printed string will be truncated at the first \x00. That does not affect the overwrite; gets() wrote the bytes into memory, and auth will be overwritten. The only consequence is less visible echo output.
