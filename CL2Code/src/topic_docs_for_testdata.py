import sys, os
import csv 

def main(argv):
	testdata_path = argv[0]
	topic_model_doc_dir = argv[1]
	userid_status = {}
	with open(testdata_path, 'rU') as csv_file:
            file_reader = csv.reader(csv_file, dialect=csv.excel_tab, delimiter=',')
            for row in file_reader:
		userid = row[0]
		status = row[1]
		if userid in userid_status.keys():
			userid_status[userid] += status + "\n"
		else:
			userid_status[userid] = status + "\n"
	for userid in userid_status.keys():
		test_document = open(os.path.join(topic_model_doc_dir, userid + ".txt"), 'w')
		test_document.write(userid_status[userid])

if __name__ == "__main__":
	main(sys.argv[1:])
