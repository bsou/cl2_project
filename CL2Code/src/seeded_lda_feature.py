import constants
import os

class SeededLDAFeature:

	def __init__(self):
		filename = constants.SEEDED_LDA_FEATURES_FILE_TRAIN
		f = open(filename, 'r')
		self._features = {}
		author_id = 1
		for line in f.readlines():
			topic_proportions = line.split()
			for i in range(0, len(topic_proportions)):
				topic_proportions[i] = int(100*float(topic_proportions[i]))
			self._features[str(author_id)] = topic_proportions
			author_id += 1
		filename_test_idmap = constants.SEEDED_LDA_FEATURES_FILE_TEST_IDMAP
		f_test_idmap = open(filename_test_idmap, 'r')
		author_id_map = {}
		for line in f_test_idmap.readlines():
			author_id, no = line.split()
			author_id_map[no] = author_id
		filename_test = constants.SEEDED_LDA_FEATURES_FILE_TEST
		f_test = open(filename_test, 'r')
		no = 1
		for line in f_test.readlines():
			topic_proportions = line.split()
			for i in range(0, len(topic_proportions)):
                                topic_proportions[i] = int(100*float(topic_proportions[i]))
			self._features[str(author_id_map[str(no)])] = topic_proportions
			no += 1
	def get_seeded_lda_features(self, author_ids, features):
		for author_id in author_ids:
			features[author_id] += self._features[author_id]
