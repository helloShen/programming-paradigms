#!/usr/bin/env python

import random	# for seed, random
import sys    	# for stdout

# Computes the score of the optimal alignment of two DNA strands.
def findOptimalAlignment(strand1, strand2, memory):
	item = strand1 + "\n" + strand2
	if (item in memory.items()):
		return memory[item]

	result = {}
		
	if len(strand1) == 0:
		result['count'] = len(strand2) * -2
		result['strand1'] = list("  ")
		result['strand2'] = list("  " + strand2)
		result['plus'] = list("+ ")
		result['minus'] = list("- ")
		for i in range(0, len(strand2)):
			result['strand1'][2:2] = " "
			result['plus'][2:2] = " "
			result['minus'][2:2] = "2"
		memory[item] = result
		return result

	if len(strand2) == 0:
		result['count'] = len(strand1) * -2
		result['strand1'] = list("  " + strand1)
		result['strand2'] = list("  ")
		result['plus'] = list("+ ")
		result['minus'] = list("- ")
		for i in range(0, len(strand1)):
			result['strand2'][2:2] = " "
			result['plus'][2:2] = " "
			result['minus'][2:2] = "2"
		memory[item] = result
		return result

	both_match = {}
	if strand1[0] == strand2[0]:
		both_match = findOptimalAlignment(strand1[1:], strand2[1:], memory)
		result['count'] = both_match['count'] + 1
		both_match['strand1'][2:2] = strand1[0]
		result['strand1'] = both_match['strand1']
		both_match['strand2'][2:2] = strand2[0]
		result['strand2'] = both_match['strand2']
		both_match['plus'][2:2] = "1"
		result['plus'] = both_match['plus']
		both_match['minus'][2:2] = " "
		result['minus'] = both_match['minus']
		memory[item] = result
		return result

	both_pass = {}
	strand1_space = {}
	strand2_space = {}
	both_pass = findOptimalAlignment(strand1[1:], strand2[1:], memory)
	strand1_space = findOptimalAlignment(strand1, strand2[1:], memory)
	strand2_space = findOptimalAlignment(strand1[1:], strand2, memory)
	max_sub = max(both_pass, strand1_space, strand2_space)
	if (max_sub == both_pass):
		result['count'] = both_pass['count'] - 1
		both_pass['strand1'][2:2] = strand1[0]
		result['strand1'] = both_pass['strand1']
		both_pass['strand2'][2:2] = strand2[0]
		result['strand2'] = both_pass['strand2']
		both_pass['plus'][2:2] = " "
		result['plus'] = both_pass['plus']
		both_pass['minus'][2:2] = "1"
		result['minus'] = both_pass['minus']
	elif (max_sub == strand1_space):
		result['count'] = strand1_space['count'] - 2
		strand1_space['strand1'][2:2] = " "
		result['strand1'] = strand1_space['strand1']
		strand1_space['strand2'][2:2] = strand2[0]
		result['strand2'] = strand1_space['strand2']
		strand1_space['plus'][2:2] = " "
		result['plus'] = strand1_space['plus']
		strand1_space['minus'][2:2] = "2"
		result['minus'] = strand1_space['minus']
	else:
		result['count'] = strand2_space['count'] - 2
		strand2_space['strand1'][2:2] = strand1[0]
		result['strand1'] = strand2_space['strand1']
		strand2_space['strand2'][2:2] = " "
		result['strand2'] = strand2_space['strand2']
		strand2_space['plus'][2:2] = " "
		result['plus'] = strand2_space['plus']
		strand2_space['minus'][2:2] = "2"
		result['minus'] = strand2_space['minus']
	memory[item] = result
	return result

# Utility function that generates a random DNA string of
# a random length drawn from the range [minlength, maxlength]
def generateRandomDNAStrand(minlength, maxlength):
	assert minlength > 0, \
	       "Minimum length passed to generateRandomDNAStrand" \
	       "must be a positive number" # these \'s allow mult-line statements
	assert maxlength >= minlength, \
	       "Maximum length passed to generateRandomDNAStrand must be at " \
	       "as large as the specified minimum length"
	strand = ""
	length = random.choice(range(minlength, maxlength + 1))
	bases = ['A', 'T', 'G', 'C']
	for i in range(0, length):
		strand += random.choice(bases)
	return strand

# Method that just prints out the supplied alignment score.
# This is more of a placeholder for what will ultimately
# print out not only the score but the alignment as well.

def printAlignment(score, out = sys.stdout):	
	out.write("Optimal alignment score is " + str(score['count']) + '\n')
	out.write("".join(score['plus']) + "\n")
	out.write("".join(score['strand1']) + "\n")
	out.write("".join(score['strand2']) + "\n")
	out.write("".join(score['minus']) + "\n")

# Unit test main in place to do little more than
# exercise the above algorithm.  As written, it
# generates two fairly short DNA strands and
# determines the optimal alignment score.
#
# As you change the implementation of findOptimalAlignment
# to use memoization, you should change the 8s to 40s and
# the 10s to 60s and still see everything execute very
# quickly.
 
def main():
	while (True):
		sys.stdout.write("Generate random DNA strands? ")
		answer = sys.stdin.readline()
		if answer == "no\n": break
		strand1 = generateRandomDNAStrand(10, 15)
		strand2 = generateRandomDNAStrand(10, 15)
		sys.stdout.write("Aligning these two strands: " + strand1 + "\n")
		sys.stdout.write("                            " + strand2 + "\n")
		alignment = findOptimalAlignment(strand1, strand2, {})
		printAlignment(alignment)
		
if __name__ == "__main__":
  main()
