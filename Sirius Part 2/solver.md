<img width="520" height="415" alt="image" src="https://github.com/user-attachments/assets/7fe5839f-78ea-40c5-bdda-841bfd7cd71b" /># SIRIUS – PART IT CTF Writeup


![f56de5_911f969f057d4a149f8c30d27daa74cb~mv2](https://github.com/user-attachments/assets/ed0b076b-af90-4e28-835d-bde8669d36bd)


## Challenge Overview


The program reads a line with `gets()` into a 64-byte buffer on the stack (`char buf[64]`) and stores an int auth right after it (initialized to 0). If `auth == 0x1337` after input, the program prints the poem and reads/prints flag.txt. The objective is to craft input that overflows buf and overwrites auth with the magic value `0x1337`.

## Vulnerability



Use of `gets()` — `gets()` never bounds checks input. It allows arbitrary-length writes into `buf`. This is the direct source of the buffer overflow.

Stack-local `auth` variable adjacent to `buf` — `auth` (an `int`) is stored on the stack immediately after `buf` and can be overwritten by overflowing `buf`.

No explicit stack protections assumed in challenge — the challenge likely expects exploitation through overwriting `auth`. Depending on compilation flags and platform, other hardening may be present (stack canaries, ASLR, PIE, NX). The simplest route is a straight overwrite of `auth`, not return-address control.






<img width="166" height="169" alt="image" src="https://github.com/user-attachments/assets/ebd35b20-5fd8-4ab3-a860-140b40598e2a" />



## Exploit Strat

```buf``` is 64 bytes. Writing 64 bytes fills ```buf```. The next 4 bytes on the stack are ```auth``` (an ```int```), so write additional 4 bytes with the little-endian representation of ```0x1337``` ```(b'\x37\x13\x00\x00')``` to overwrite ```auth```.

Send the total payload: ```64 bytes filler + 4 bytes p32(0x1337)```. The program will then check auth and, if equal, run sirius_poem() and print_flag().

```Important note about printf("%s", buf): the program prints buf after the gets(). If your 4 bytes for auth include \x00 (they do), the printed string will be truncated at the first \x00. That does not affect the overwrite; gets() wrote the bytes into memory, and auth will be overwritten. The only consequence is less visible echo output.```






<img width="520" height="415" alt="image" src="https://github.com/user-attachments/assets/8c097a50-2384-469a-b1e2-b8363d675749" />



Netcat Version
```python3 -c "import sys; sys.stdout.buffer.write(b'A'*76 + b'\x37\x13\x00\x00')" | nc 72.146.224.45 6050```

Local Version
```python3 -c "import sys; sys.stdout.buffer.write(b'A'*76 + b'\x37\x13\x00\x00')" | ./sirius2```



<img width="898" height="750" alt="image" src="https://github.com/user-attachments/assets/7a4f4823-939c-49a9-b2b2-850ffc21ac8f" />


and you'll get the flag



<img width="88" height="31" alt="image" src="https://github.com/user-attachments/assets/12c81770-53a9-4a5a-b4ba-7b95e80a79ac" />


