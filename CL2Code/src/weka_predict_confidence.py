import sys
import constants

from csv_reader import CsvReader

if __name__ == "__main__":
    weka_output = sys.argv[1]
    regression_values = open(sys.argv[2])
    neu_indices = list()
    confidence = {}
    test_data = CsvReader(constants.TEST_DATA_FILE, True)
    author_ids_test = test_data.get_author_ids()
    for line in open(weka_output, 'r'):
	regress_line = regression_values.readline()
	regression_value = regress_line.split()[2]
        row = line.split()
	id = int(row[0]) - 1
	weka_prediction = int(row[2].split(":")[1])
	prediction = ''
	if weka_prediction == 1:
		prediction = '+'
	else:
		prediction = '-'
	confidence = row[4]
	print str(author_ids_test[id]) + ", " + str(prediction) + ", " + str(confidence) + ", " + str(regression_value)
