# preprocessing
# input - folderpath
# output - variable docs

def savedocs(folderpath):

	from nltk.tokenize import word_tokenize,sent_tokenize
	import re
	import os

	docs = list()
	voc = list()

	# getting stopwords
	stopwords = list()
	f = open('stopwordlist','rb')
	for r in f:
		r = r.splitlines()
		stopwords.append(r[0])

	if folderpath:
		for filename in os.listdir(folderpath):
			filename = folderpath + '/' + filename
			f = open(filename,'rb')
			singledocs = list()
			for r in f:
				r = r.splitlines()
				r = r[0].lower()
				tokens = [word for sent in sent_tokenize(r) for word in word_tokenize(sent)]
				singledocs.extend(tokens)
			docs.append(singledocs)
			f.close()

	else:
		print "enter folderpath"
		exit()

	# deleting stop words and tokens containing only punctuation
	for i in range(0,len(docs)):
		for j in range(0,len(docs[i])):

			# need to use a pre-built vocabulary and keep only those words which are in the vocabulary
			# currently just building the vocabulary from the corpus itself
			if docs[i][j] in stopwords: docs[i][j]='a'
			if re.search('[a-zA-Z]',docs[i][j]) == None: docs[i][j]='a'
			if docs[i][j]!='a' and docs[i][j] not in voc: voc.append(docs[i][j])

		docs[i] = [x for x in docs[i] if x != 'a']
		assignments[i] = [0 for x in docs[i] if x != 'a']

	return docs,voc

def main():

	from optparse import OptionParser
	parser = OptionParser()
	parser.add_option("--folderpath",dest="folderpath",help="corpus folder path - where each document has a seperate file for it")
	(options,args) = parser.parse_args()
	
	docs = savedocs(options.savedocs)

if __name__ = "__main__":
	main()
