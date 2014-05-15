import numpy
import constants

from datetime import datetime
from collections import defaultdict
from sklearn import svm
from sklearn.metrics import precision_recall_curve
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
import pylab as pl


class TimeFeature:
    def __init__(self, training_data):
        self._training_data = training_data

    def get_frequency_of_posts(self, (start_hour, end_hour)):
        """
        Returns a vector representing the number of posts of each user in the given time slot.
        """

        counts = defaultdict(int)
        for row in self._training_data.get_columns([constants.AUTHOR_ID_COL, constants.TIMESTAMP_COL]):
            author_id = row[0]

            try:
                timestamp = datetime.strptime(row[1], "%Y-%m-%d %H:%M:%S")
            except ValueError:
                # print "Error parsing timestamp: ", row[1]
                continue

            if timestamp.hour >= start_hour and timestamp.hour < end_hour:
                counts[author_id] += 1

        return counts


# def train_model(k_fold):
#     """
#     Returns a classifier built using SVM
#     """
#     num_users = len(neu_labels.keys())
#     num_features = len(all_features.values()[0])

#     X = numpy.zeros((1, num_features))          # Feature matrix
#     Y = numpy.array([0])                         # Label vector
#     for user_id in neu_labels.keys():
#         X = numpy.append(X, [all_features[user_id]], axis=0)
#         Y = numpy.append(Y, [neu_labels[user_id]])

#     print X
#     print Y

#     model = svm.SVC()
#     model.fit(X[:len(X)*k_fold/10], Y[:len(X)*k_fold/10])
#     Y_proba = model.predict(X[len(X)*k_fold/10:])

#     precision = precision_score(Y[len(X)*k_fold/10:], Y_proba, [1, 0])
#     recall = recall_score(Y[len(X)*k_fold/10:], Y_proba, [1, 0])

#     print "Precision: " + str(precision)
#     print "Recall: " + str(recall)

#     precision, recall, thresholds = precision_recall_curve(Y[len(X)*k_fold/10:], Y_proba)
#     pl.clf()
#     pl.plot(recall, precision, label='Precision-Recall curve')
#     pl.xlabel('Recall')
#     pl.ylabel('Precision')
#     pl.show()
#     return model


def main():
    return


if __name__ == "__main__":
    main()
