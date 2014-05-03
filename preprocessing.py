# preprocessing
# input - folderpath
# output - variable docs

from spellchecker import *

def savedocs(folderpath,start,end):

	from nltk.tokenize import word_tokenize,sent_tokenize
	import re
	import os

	#docs = list()

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

	# regex for mapping words like hahha, hehehe to laugh
	p = re.compile('(a|e)*(h+(a|e)+)+h*')

	# regex for handling repeated characters in words like 'homeee'
	q = re.compile(r"[a-z]*(\w)\1{2,}[a-z]*")

	
	if folderpath:
		for filename in os.listdir(folderpath):

			fmod = filename.split('.')
			fmod = fmod[0].split('-')
			fmod = int(fmod[1])
			if fmod < start or fmod > end: continue
		
			filename_path = folderpath + '/' + filename
			f = open(filename_path,'rb')	
			print 'processing file:',filename

			singledocs = list()
			counter = 0
			for r in f:
				counter += 1
				if counter < 5: continue
				r = r.splitlines()
				r = r[0].lower()
				tokens = [word for sent in sent_tokenize(r) for word in word_tokenize(sent)]
				singledocs.extend(tokens)

			temp_doci = list()

			for j in range(0,len(singledocs)):

				if singledocs[j] in stopwords: continue
				elif re.search('[a-zA-Z]',singledocs[j]) == None: continue
				elif singledocs[j] in acronyms:
					tempstr = acronyms[singledocs[j]]
					tempstr = word_tokenize(tempstr)
					temp_doci.extend(tempstr)
				elif p.match(singledocs[j]): temp_doci.append('laugh')
				elif q.match(singledocs[j]):
					tempword = singledocs[j][0] + singledocs[j][1]
					for k in range(2,len(singledocs[j])):
						if not(singledocs[j][k] == singledocs[j][k-1] and singledocs[j][k] == singledocs[j][k-2]):
							tempword += singledocs[j][k]
					if tempword not in vocab: tempword = correct(tempword)
					temp_doci.append(tempword)
				elif singledocs[j] not in vocab:
					tempword = correct(singledocs[j])
					temp_doci.append(tempword)
				else: temp_doci.append(singledocs[j])

			temp_dock = list()
			for k in range(0,len(temp_doci)):
				if temp_doci[k] in stopwords: continue
				else: temp_dock.append(temp_doci[k])

			filename1 = 'processedstatusfiles/' + filename
			fout = open(filename1,'wb')
			for item in singledocs:
  				fout.write("%s\n" % item)

			f.close()

	else:
		print "enter folderpath"
		exit()


def main():

	from optparse import OptionParser
	parser = OptionParser()
	parser.add_option("--folderpath",dest="folderpath",help="corpus folder path - where each document has a seperate file for it")
	parser.add_option("--start",dest="start",type="int",help="start range of files")
	parser.add_option("--end",dest="end",type="int",help="end range of files")
	(options,args) = parser.parse_args()
	
	savedocs(options.folderpath,options.start,options.end)

if __name__ == "__main__":
	main()
