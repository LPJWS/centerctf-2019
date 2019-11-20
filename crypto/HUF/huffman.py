from heapq import heappush, heappop, heapify
from collections import defaultdict
import binascii

def bin_to_ascii(s):
    while len(s) % 8 != 0:
        s += '0'
    s1 = ""
    counter = 0
    res = ""
    for ch in s:
        s1 += ch
        counter += 1
        if counter == 8:
            res += chr(int(s1, 2))
            counter = 0
            s1 = ""
    return res


def encode(symb2freq):
    heap = [[wt, [sym, ""]] for sym, wt in symb2freq.items()]
    heapify(heap)
    while len(heap) > 1:
        lo = heappop(heap)
        hi = heappop(heap)
        for pair in lo[1:]:
            pair[1] = '0' + pair[1]
        for pair in hi[1:]:
            pair[1] = '1' + pair[1]
        heappush(heap, [lo[0] + hi[0]] + lo[1:] + hi[1:])
    return sorted(heappop(heap)[1:], key=lambda p: (len(p[-1]), p))


def cut_8():
    global temp, zipedtxt, output
    zipedtxt += bin_to_ascii(temp[:8])
    output.write(bin_to_ascii(temp[:8]))
    print(bin_to_ascii(temp[:8]), end = "")
    temp = temp[8:]

def zip(name):
    global temp, zipedtxt, output
    output = open(name + "_zipped.txt", "w", encoding = "utf-8")
    txt = open(name + ".txt", encoding = "utf-8").read()
    symb2freq = defaultdict(int)
    for ch in txt:
        symb2freq[ch] += 1
    huff = encode(symb2freq)

    zipedtxt = ""
    for p in huff:
        #if p[0] == '\n':
            #output.write("/n:"+p[1]+"/")
        #else:
            output.write(p[0]+":"+p[1]+"/")
    output.write("\n")

    temp = ""
    for ch in txt:
        for p in huff:
            if p[0] == ch:
                temp += p[1]
                break
        if len(temp) >= 8:
            cut_8()


temp, zipedtxt, output = '', '', ''
zip('war&piece')