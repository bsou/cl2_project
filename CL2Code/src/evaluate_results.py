import sys

if __name__ == "__main__":
	filenames = sys.argv[1:]
	results = {}
	for filename in filenames:
		f = open(filename, 'r')
		ids = f.read()
		results[filename] = ids.split(" ")

	global_common = []
	for i in range(len(filenames)):
		for j in range(i + 1, len(filenames)):
			# print filenames[i]
			print "Size: " + str(len(results[filenames[i]]))
			# print filenames[j]
			print "Size: " + str(len(results[filenames[j]]))
			# print "Common: "
			common = list(set(results[filenames[i]]).intersection(results[filenames[j]]))
			print "Size: " + str(len(common))
			#print ' '.join(common)
			if(len(global_common) == 0):
				global_common = common
			global_common = list(set(common).intersection(global_common))

	# print "Global common: "
	# print ' '.join(global_common)
	# print "Size: " + str(len(global_common))