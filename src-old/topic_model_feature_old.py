import constants
import os

class TopicModelFeature:
		
	def __init__(self, training_data, test_data):
		self._training_data_set = training_data
		self._test_data_set = test_data

	def train_topic_model(self):
		author_statuses = self._training_data_set.init_status_group_by_author() + self._test_data_set.init_status_group_by_author()
		author_ids = self._training_data_set.get_author_ids_and_neu_labels()[0] + self._test_data_set.get_author_ids()
		if not os.path.exists(constants.DOCUMENTS_DIR):
			os.makedirs(constants.DOCUMENTS_DIR)
			idx = 0
			for author_status in author_statuses:
				f = open(os.path.join(constants.DOCUMENTS_DIR, author_ids[idx]) + '.txt', 'a')
				f.write(' '.join(author_status))
				f.close()
				idx += 1

	# def __init__(self):
	# 	filename = constants.TOPIC_MODEL_FEATURES
	# 	f = open(filename, 'r')
	# 	self._features = {}
	# 	for line in f.readlines():
	# 		words = line.split("\t")
	# 		author_id = words[1].split("/")[-1][:-4]
	# 		words = words[2:]
	# 		topic_proportions = {}
	# 		it = iter(words)
	# 		for i in range(0, len(words)-1, 2):
	# 			topic = words[i]
	# 			score = words[i + 1]
	# 			topic_proportions[topic] = score
	# 		topics = topic_proportions.keys()
	# 		topics.sort()
	# 		for topic in topics:
	# 			if(self._features.has_key(author_id)):
	# 				self._features[author_id].append(topic_proportions[topic])
	# 			else:
	# 				self._features[author_id] = [topic_proportions[topic]]

	# def get_topic_model_features(self, author_ids, features):
	# 	for author_id in author_ids:
	# 		features[author_id] += self._features[author_id]