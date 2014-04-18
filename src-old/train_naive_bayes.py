import numpy

from sklearn.naive_bayes import MultinomialNB
from sklearn import cross_validation


class TrainNaiveBayes:

    def __init__(self, all_features, neu_labels):
        """
        Trains a classifier using Naive Bayes
        """
        self._num_features = len(all_features.values()[0])

        self._X = numpy.zeros((1, self._num_features))          # Feature matrix
        self._Y = numpy.array([0])                        # Label vector
        for user_id in neu_labels.keys():
            self._X = numpy.append(self._X, [all_features[user_id]], axis=0)
            self._Y = numpy.append(self._Y, [neu_labels[user_id]])
        self._X = numpy.delete(self._X, 0, 0)           # Delete the first row (contains all 0s)
        self._Y = numpy.delete(self._Y, 0)

        print "Using MultinomialNB"
        self._model = MultinomialNB()
        print cross_validation.cross_val_score(self._model, self._X, self._Y, cv=10, scoring='f1')

        self._model.fit(self._X, self._Y)

    def predict(self, features):
        A = numpy.zeros((1, self._num_features))
        for user_id in features.keys():
            A = numpy.append(A, [features[user_id]], axis=0)
        A = numpy.delete(A, 0, 0)
        return self._model.predict(A)
