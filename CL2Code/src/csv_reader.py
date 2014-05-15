import constants
import csv
from datetime import datetime
from scipy.stats.stats import pearsonr

class CsvReader:
    def __init__(self, file_path, is_testing_data=False):
        self._data = list()
        valid_data_file = open('../dataset/training.csv', 'w')
        with open(file_path, 'rU') as csv_file:
            file_reader = csv.reader(csv_file, dialect=csv.excel_tab, delimiter=',')
            next(file_reader, None)     # Ignore the header row
            total_invalid_rows = 0
            total_rows = 0
            for row in file_reader:
                row_data = list()
                is_valid_row = True
                num_of_cols = constants.NUMBER_OF_COLS_IN_TRAINING_DATA
                if is_testing_data:
                    num_of_cols -= 1
                for col in range(0, num_of_cols):
                    if col == constants.TIMESTAMP_COL and not self.is_valid_timestamp(row[col]):
                        is_valid_row = False
                        break
                    if col == constants.STATUS_COL:
                        # row_data.append(row[col].lower())
                        #row_data.append("\"" + row[col].lower().replace("\"", " ").replace("\'", " ") + "\"")
                        row_data.append(row[col].lower().replace("\"", " ").replace("\'", " "))
		    #if col == constants.GENDER_COL and row[col] == " ":
		    #   is_valid_row = False
		    #   break
                    else:
                        row_data.append(row[col])
                if is_valid_row:
                    self._data.append(row_data)
                    valid_data_file.write(",".join(row_data) + '\n')
                else:
                    total_invalid_rows += 1
                total_rows += 1
            print "total invalid rows: ", total_invalid_rows, " out of ", total_rows

    def convert_to_arff(self, dest_file, feature_name_types, author_ids, all_features, neu_labels, is_testing_data):
	f = open(dest_file, 'w')
        f.write('@RELATION neu_predicate\n\n')
        for feature_name_type in feature_name_types:
            f.write('@ATTRIBUTE ' + feature_name_type[0] + ' ' + feature_name_type[1] + '\n')

        # if not is_testing_data:
        f.write('@ATTRIBUTE class {0, 1}\n')

        f.write('\n@DATA\n')
	feature_vectors = {} 
        labels = []
	cesd_scores = {}
	if(constants.REGRESS_BY_CESD == 'TRUE'):
        	user_cesd_file = open(constants.CESD_SCORES)
                for line in user_cesd_file.readlines():
                	(user_id, cesd_score) = line.split()
			cesd_scores[user_id] = cesd_score
	for author_id in author_ids:
            feature_values = all_features[author_id]
            for idx in range(0, len(feature_values)):
                if idx != 0:
                    f.write(',')
                f.write(str(feature_values[idx]))
		if not is_testing_data:
			if not feature_vectors.has_key(idx):
				feature_vectors[idx] = []
			feature_vectors[idx].append(feature_values[idx])
            if not is_testing_data:
		if(constants.REGRESS_BY_CESD == 'TRUE'):
			f.write(',' + str(cesd_scores[author_id]))
		else:
			f.write(',' + str(neu_labels[author_id]))
		labels.append(neu_labels[author_id])
            else:
                f.write(',1')
            f.write('\n')
	f.close()
	if not is_testing_data:
		file_feature_vectors = open(constants.FEATURE_VECTORS_FILE, 'w')
		for idx in feature_vectors.keys():
			p = pearsonr(feature_vectors[idx], labels)
			if(constants.WRITE_TO_FEATURE_VECTORS_FILE == 'TRUE'):
				file_feature_vectors.write(feature_name_types[idx][0] + ',' + ','.join(str(x) for x in feature_vectors[idx]) + '\n')

    def is_valid_author_id(self, author_id):
        return len(author_id) == constants.AUTHOR_ID_LEN

    def is_valid_timestamp(self, timestamp):
        try:
            datetime.strptime(timestamp, "%Y-%m-%d %H:%M:%S")
        except ValueError:
            return False
        return True

    def get_columns(self, cols):
        if not self._data:
            raise Exception("Training data is not initialized.")

        result = list()
        for row in self._data:
            row_data = list()
            for col in cols:
                row_data.append(row[col])
            result.append(row_data)
        return result[constants.DATA_START_ROW:]

    def get_author_ids(self):
        """
        Returns a list of all the author ids in the order they appear in the file.
        Only to be called on the test dataset.
        """
        author_ids = list()
        for row in self.get_columns([constants.AUTHOR_ID_COL]):
            if len(author_ids) == 0 or row[0] != author_ids[-1]:
                author_ids.append(row[0])

        return author_ids

    def get_author_ids_and_neu_labels(self):
        """
        Returns a list of all the unique author ids and corresponding neu labels in the order they appear in the file.
        Only to be called on the training dataset.
        """
        author_ids = list()
        neus = list()

        for row in self.get_columns([constants.AUTHOR_ID_COL, constants.NEU_COL]):
            if len(author_ids) == 0 or row[0] != author_ids[-1]:
                author_ids.append(row[0])
                if row[1] == constants.NEU_SYMBOL:
                    neus.append(1)
                else:
                    neus.append(0)

        return author_ids, neus

    def init_status_group_by_author(self):
        raw_data = self.get_columns([constants.AUTHOR_ID_COL, constants.STATUS_COL])
        author_statuses = list()
        prev_author_id = 0
        statuses = list()
        for row in raw_data:
            author_id = row[0]
            status = row[1].lower()
            if author_id != prev_author_id:     # start of a new author
                if prev_author_id != 0:
                    author_statuses.append(statuses)    # store result of previous author
                statuses = [status]
                prev_author_id = author_id
            else:
                statuses.append(status)

        author_statuses.append(statuses)    # store the last result
        return author_statuses

    def unset(self):
        """
        Removes the dataset from memory.
        """
        self._data = None


def main():
    training_data = CsvReader('../dataset/dummy_train.csv')
    for row in training_data.get_columns([constants.AUTHOR_ID_COL, constants.TIMESTAMP_COL]):
            print row


if __name__ == "__main__":
    main()
