import sys

# in
stop_file = open(sys.argv[1])
# out
stop_string = open(sys.argv[2], 'w')

stop_string.write('// Generated via `genstopwords.py data/stop-words.in`.\n')
for word in stop_file:
	word = word.strip()
	stop_string.write('"%s",\n' % word)

stop_file.close()
stop_string.close()