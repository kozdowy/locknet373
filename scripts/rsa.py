import sys

def MonMult(A, B, n):
	P = [0]*65
	q = [0]*64
	assert(A < n)
	assert(B < n)
	for i in range(0, 64):
		q[i] = ((P[i] & 0b1) ^ (((A >> i) & 0b1) & (B & 0b1)))
		P[i + 1] = (P[i] + (((A >> i) & 0b1) * B) + (q[i] * n)) / 2
	if P[64] >= n:
		result = P[64] - n
	else:
		result = P[64]
	return result

def ModExp(C, d, n):
	Z = [0]*65
	result = [0]*65
	residue = (2 ** (64*2)) % n
	result[0] = MonMult(residue, 1, n)
	Z[0] = MonMult(residue, C, n)
	for i in range(0, 64):
		result_temp = MonMult(result[i], Z[i], n)
		Z[i+1] = MonMult(Z[i], Z[i], n)
		if ((d >> i) & 0b1):
			result[i + 1] = result_temp
		else:
			result[i + 1] = result[i]
	final = MonMult(1, result[64], n)
	return final

def main():
	message = 0xdcbaabcdabcddcba
	pub = 0x10001
	private = 0x44de026cabdb311
	modulus = 0xeda515ef24029417

	correct = (message ** pub) % modulus
	cypher = ModExp(message, pub, modulus)
	decypher = ModExp(correct, private, modulus)
	print(hex(message))
	print(hex(cypher))
	print(hex(decypher))
	print(hex(correct))

if __name__ == '__main__':
	main()
