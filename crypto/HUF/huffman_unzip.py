from collections import defaultdict

def ascii_to_bin(s):
	res = ""
	for ch in s:
		q = bin(ord(ch))[2:]
		while len(q) != 8:
			q = '0' + q
		res += q
	return res

def unzip(name):
	inp = open(name, "r", encoding = "utf-8")
	out = open(name.split('.')[0] + "_unzipped.txt", "w", encoding = "utf-8")

	huff = defaultdict(str)
	for p in inp.readline()[:-2].split('/'):
		huff[p.split(':')[1]] = p.split(':')[0]

	guf = ""
	res = ""
	hui = ascii_to_bin(inp.read())
	print(hui)
	for ch in hui:
		guf += ch
		if huff[guf] != '':
			res += huff[guf]
			guf = ''
	print(res)
	out.write(res)
	out.close()

unzip('file.huff')