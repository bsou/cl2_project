import sys
import constants

from csv_reader import CsvReader

if __name__ == "__main__":
    weka_output = sys.argv[1]
    neu_indices = list()

    for line in open(weka_output, 'r'):
        row = line.split()
        row = [r.strip() for r in row]
        if float(row[2]) >= 28:
            neu_indices.append(int(row[0]) - 1)       # Weka indices start from 1

    print neu_indices
    test_data = CsvReader(constants.TEST_DATA_FILE, True)
    author_ids_test = test_data.get_author_ids()

    print len(neu_indices)
    print "class,predictions"
    print "+, ",
    for i in range(0, len(neu_indices)):
        print author_ids_test[neu_indices[i]],

