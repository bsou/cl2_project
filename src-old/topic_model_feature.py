import constants
import os

class TopicModelFeature:

	def __init__(self):
		filename = constants.TOPIC_MODEL_FEATURES_FILE
		f = open(filename, 'r')
		self._features = {}
		for line in f.readlines():
			words = line.split("\t")
			file_name = words[1].split("/")[-1]
			author_id = ""
			if(len(file_name) > 20):
				author_id = file_name[:-4]
			else:
				author_id = file_name[5:-4]
			words = words[2:]
			topic_proportions = {}
			it = iter(words)
			for i in range(0, len(words)-1, 2):
				topic = words[i]
				score = words[i + 1]
				topic_proportions[topic] = int(100*float(score))
			topics = topic_proportions.keys()
			topics.sort()
			for topic in topics:
				if(self._features.has_key(author_id)):
					self._features[author_id].append(topic_proportions[topic])
				else:
					self._features[author_id] = [topic_proportions[topic]]

	def get_topic_model_features(self, author_ids, features):
		for author_id in author_ids:
			features[author_id] += self._features[author_id]
