import sys

# def train_topic_model(self, num_topics):
# 	author_statuses = self._training_data_set.init_status_group_by_author() + self._test_data_set.init_status_group_by_author()
# 	author_ids = self._training_data_set.get_author_ids_and_neu_labels()[0] + self._test_data_set.get_author_ids()
# 	if not os.path.exists(constants.DOCUMENTS_DIR):
# 		os.makedirs(constants.DOCUMENTS_DIR)
# 		author_statuses = self._training_data_set.init_status_group_by_author()
# 		author_ids = self._training_data_set.get_author_ids_and_neu_labels()[0]
# 		idx = 0
# 		for author_status in author_statuses:
# 			f = open(os.path.join(constants.DOCUMENTS_DIR, author_ids[idx]) + '.txt', 'a')
# 			f.write(' '.join(author_status))
# 			f.close()
# 			idx += 1

if __name__ == "__main__":
	filename = sys.argv[1]
	f = open(filename, 'r')
	features = {}
	for line in f.readlines():
			#print line
			words = line.split("\t")
			author_id = words[1].split("/")[-1][:-4]
			print author_id
			words = words[2:]
			topic_proportions = {}
			it = iter(words)
			for i in range(0, len(words)-1, 2):
				topic = words[i]
				score = words[i + 1]
				topic_proportions[topic] = score
			topics = topic_proportions.keys()
			topics.sort()
			for topic in topics:
				if(features.has_key(author_id)):
					features[author_id].append(topic_proportions[topic])
				else:
					features[author_id] = [topic_proportions[topic]]
	print features




