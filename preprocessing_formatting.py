# preprocessing
# input - folderpath
# output - variable docs

from spellchecker import *

def savedocs(folderpath,start,end,outputfolder):

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

		
		filename1 = outputfolder
		fout = open(filename1,'wb')

		for filename in os.listdir(folderpath):

			statuscount = 0

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

				if counter == 1:
					r = r.splitlines()
					userid = r[0]

				if counter == 4:
					r = r.splitlines()
					neuro = r[0]

				if counter == 5:
					r = r.splitlines()
					gender = r[0]

				if counter < 6: continue

				statuscount += 1
				r = r.splitlines()
				r = r[0].lower()
				tokens = [word for sent in sent_tokenize(r) for word in word_tokenize(sent)]
				singledocs.append(tokens)
		

			for i in range(0,len(singledocs)):

				temp_doci = list()
				
				date = singledocs[i][0]
				time = singledocs[i][1]

				for j in range(2,len(singledocs[i])):

					if singledocs[i][j] in stopwords:
						#print 'stopword -',singledocs[j]
						continue
					elif re.search('[a-zA-Z]',singledocs[i][j]) == None:
						#print 'not alphabets -',singledocs[j]
						continue
					elif singledocs[i][j] in acronyms:
						tempstr = acronyms[singledocs[i][j]]
						tempstr = word_tokenize(tempstr)
						temp_doci.extend(tempstr)
						#print 'acronym',singledocs[j],'-',tempstr
					elif p.match(singledocs[i][j]):
						temp_doci.append("laugh")
						#print 'laugh -',singledocs[j]
					elif q.match(singledocs[i][j]):
						tempword = singledocs[i][j][0] + singledocs[i][j][1]
						for k in range(2,len(singledocs[i][j])):
							if not(singledocs[i][j][k] == singledocs[i][j][k-1] and singledocs[i][j][k] == singledocs[i][j][k-2]):
								tempword += singledocs[i][j][k]
						if tempword not in vocab: tempword = correct(tempword)
						if tempword in vocab: temp_doci.append(tempword)
						#print 'repeated chars -',singledocs[j],'-',tempword
					elif singledocs[i][j] not in vocab:
						tempword = correct(singledocs[i][j])
						if tempword in vocab: temp_doci.append(tempword)
						#print 'not in vocab -',singledocs[j],'-',tempword
					else:
						temp_doci.append(singledocs[i][j])
						#print 'in vocab -',singledocs[j]

				temp_dock = "\""
				check = 0
				for k in range(0,len(temp_doci)):
					if temp_doci[k] in stopwords: continue
					else:
						if check == 0:
							temp_dock += temp_doci[k]
							check = 1
						else: temp_dock += " " + temp_doci[k]
				
				temp_dock += "\""
				tempstr = str(userid) + "," + temp_dock + "," + date + " " + time + "," + str(statuscount) + "," + str(gender) + "," + neuro
				fout.write("%s\n" % tempstr)

			#print temp_doci

			#filename1 = outputfolder + filename
			#fout = open(filename1,'wb')
			#for item in temp_doci:
  			#	fout.write("%s\n" % item)
			#fout.close()

			f.close()
		fout.close()

	else:
		print "enter folderpath"
		exit()


def main():

	from optparse import OptionParser
	parser = OptionParser()
	parser.add_option("--folderpath",dest="folderpath",help="corpus folder path - where each document has a seperate file for it")
	parser.add_option("--start",dest="start",type="int",help="start range of files")
	parser.add_option("--end",dest="end",type="int",help="end range of files")
	parser.add_option("--outputfolder",dest="outputfolder",help="output folder path")
	(options,args) = parser.parse_args()
	
	savedocs(options.folderpath,options.start,options.end,options.outputfolder)

if __name__ == "__main__":
	main()
