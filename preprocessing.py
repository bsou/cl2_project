# preprocessing
# input - folderpath
# output - variable docs

from spellchecker import *

def savedocs(folderpath):

	from nltk.tokenize import word_tokenize,sent_tokenize
	import re
	import os

	docs = list()

	# getting stopwords
#	stopwords = list()
#	f = open('stopwordlist','rb')
#	for r in f:
#		r = r.splitlines()
#		stopwords.append(r[0])
#	f.close()

	stopwords = words(file('stopwordlist').read())
	print 'saved stopwords'
	vocab = words(file('words').read())
	print 'saved vocabulary'

	# saving acronym dictionary
	acronyms = dict()
	f = open('acronym_dict','rb')
	for r in f:
		r = r.splitlines()
		r = r[0].split(',')
		acronyms[r[0]] = r[1]
	print 'built acronym dictionary'

	if folderpath:
		for filename in os.listdir(folderpath):
			filename = folderpath + '/' + filename
			f = open(filename,'rb')
			singledocs = list()
			counter = 0
			for r in f:
				counter += 1
				if counter < 5: continue
				r = r.splitlines()
				r = r[0].lower()
				tokens = [word for sent in sent_tokenize(r) for word in word_tokenize(sent)]
				singledocs.extend(tokens)
			docs.append(singledocs)
			f.close()
		print 'saved corpus'

	else:
		print "enter folderpath"
		exit()

	# regex for mapping words like hahha, hehehe to laugh
	p = re.compile('(a|e)*(h+(a|e)+)+h*')

	# regex for handling repeated characters in words like 'homeee'
	q = re.compile(r"[a-z]*(\w)\1{2,}[a-z]*")

	# deleting stop words and tokens containing only punctuation
	for i in range(0,len(docs)):

		print 'processing file ',i
		temp_doci = list()

		for j in range(0,len(docs[i])):

			# need to use a pre-built vocabulary and keep only those words which are in the vocabulary
			# currently just building the vocabulary from the corpus itself
			if docs[i][j] in stopwords: continue
			elif re.search('[a-zA-Z]',docs[i][j]) == None: continue
			elif docs[i][j] in acronyms:
				tempstr = acronyms[docs[i][j]]
				tempstr = word_tokenize(tempstr)
				temp_doci.extend(tempstr)
			elif p.match(docs[i][j]): temp_doci.append('laugh')
			elif q.match(docs[i][j]):
				tempword = docs[i][j][0] + docs[i][j][1]
				for k in range(2,len(docs[i][j])):
					if not(docs[i][j][k] == docs[i][j][k-1] and docs[i][j][k] == docs[i][j][k-2]):
						tempword += docs[i][j][k]
				if tempword not in vocab: 
					print tempword
					tempword = correct(tempword)
					print tempword
				temp_doci.append(tempword)
			elif docs[i][j] not in vocab:
				tempword = correct(docs[i][j])
				print docs[i][j],tempword
				temp_doci.append(tempword)
			else: temp_doci.append(docs[i][j])

		temp_dock = list()
		for k in range(0,len(temp_doci)):

			if temp_doci[k] in stopwords: continue
			else: temp_dock.append(temp_doci[k])

		docs[i] = temp_dock



	print docs
	return docs

def main():

	from optparse import OptionParser
	parser = OptionParser()
	parser.add_option("--folderpath",dest="folderpath",help="corpus folder path - where each document has a seperate file for it")
	(options,args) = parser.parse_args()
	
	docs = savedocs(options.folderpath)

if __name__ == "__main__":
	main()
