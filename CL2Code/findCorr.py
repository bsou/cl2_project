from scipy.stats.stats import pearsonr
import string
import math

qarray = [[0 for j in range(700)] for i in range(20)]
with open('qfeature.txt') as fileqf:
	qno = 0
	for line in fileqf:
		user = 0
		for e in line.split(","):
			if user >= 1:
				#usr = str(user)
				#qn = str(qno)
				#print e.strip() + " " + usr + " " + qn
				qarray[qno][user-1] = float(e.strip())
			user += 1
		qno += 1

'''for i in range(len(qarray)):
	linestr = str(i+1)
	for j in range(len(qarray[i])):
  		linestr += "," + str(qarray[i][j])
	print linestr'''
	
        
farray = [[0 for j in range(700)] for i in range(172)]
frnames = [] # list for containing feature names
with open('feature_vectors.csv') as filefv:
	fno = 0
	for line in filefv:
		fterm = 0
		for e in line.split(","):
			if fterm >= 1:
				#ftr = str(fterm)
				#fn = str(fno)
				#print e.strip() + " " + ftr + " " + fn
				farray[fno][fterm-1] = float(e.strip())
			else:
				frnames.append(e)
			fterm += 1
		fno += 1

for i in range(len(farray)):
	linestr = frnames[i]
	for j in range(len(farray[i])):
  		linestr += "," + str(farray[i][j])
	print linestr'''

'''qlist = [4, 9, 10, 14, 17]
for i in range(len(farray)):
	linestr = frnames[i] + ","
	totqs = 0
	val = 0.0
	for j in range(len(qlist)):
		qid = qlist[j]-1
		res = pearsonr(farray[i], qarray[qid])
		if res[1] <= 0.1 and abs(res[0]) >= 0.05:
			totqs += 1
			val += abs(res[0])
			#pcc = '%.3f'%(res[0])
			#conf = '%.3f'%(res[1])
			#linestr += "(" + str(pcc) + ", " + str(conf) + ") "
			#linestr += "(" + str(pcc) + ") "
	if totqs > 0:
		mean = val/totqs
	else:
		mean = 0
	linestr += str(mean)
	print linestr


'''for i in range(len(farray)):
	linestr = ""
	for j in range(len(qarray)):
		res = pearsonr(farray[i], qarray[j])
		pcc = '%.3f'%(res[0])
		conf = '%.3f'%(res[1])
		#linestr += "(" + str(pcc) + ", " + str(conf) + ") "
		linestr += "(" + str(pcc) + ") "
	print linestr'''
