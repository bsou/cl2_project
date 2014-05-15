import numpy

from sklearn import svm
from sklearn import preprocessing
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn import cross_validation


class TrainModel:

    def __init__(self, all_features, neu_labels):
        """
        Trains a classifier using SVM
        """
        self._num_features = len(all_features.values()[0])

        self._X = numpy.zeros((1, self._num_features))          # Feature matrix
        self._Y = numpy.array([0])                        # Label vector
        for user_id in neu_labels.keys():
            self._X = numpy.append(self._X, [all_features[user_id]], axis=0)
            self._Y = numpy.append(self._Y, [neu_labels[user_id]])
        self._X = numpy.delete(self._X, 0, 0)           # Delete the first row (contains all 0s)
        self._Y = numpy.delete(self._Y, 0)

        # print X
        # print Y
        self._X = self._X.astype(float)
        self._X = preprocessing.scale(self._X, axis=0)

        print "Using SVM"
        self._model = svm.SVC(cache_size=1000, kernel='linear', class_weight={1: 10})
        print "f1 score: "
        print cross_validation.cross_val_score(self._model, self._X, self._Y, cv=10, scoring='f1')

        # print "Precision: "
        # print cross_validation.cross_val_score(self._model, self._X, self._Y, cv=10, scoring='precision')
        # print "Recall: "
        # print cross_validation.cross_val_score(self._model, self._X, self._Y, cv=10, scoring='recall')
        self._model.fit(self._X, self._Y)
        print self._model.coef_

    def predict(self, features):
        A = numpy.zeros((1, self._num_features))
        for user_id in features.keys():
            A = numpy.append(A, [features[user_id]], axis=0)
        A = numpy.delete(A, 0, 0)
        A = A.astype(float)
        A = preprocessing.scale(A, axis=0)
        return self._model.predict(A)

    def evaluate(self, k_fold):
        kf = cross_validation.KFold(n=len(self._X), n_folds=k_fold)
        precision = []
        recall = []
        for train_index, test_index in kf:
            model = svm.SVC(cache_size=1000)
            model.fit(self._X[train_index], self._Y[train_index])
            Y_proba = model.predict(self._X[test_index])
            precision.append(precision_score(self._Y[test_index], Y_proba, [1, 0]))
            recall.append(recall_score(self._Y[test_index], Y_proba, [1, 0]))

        avg_precision = sum(precision)/len(precision)
        avg_recall = sum(recall)/len(recall)
        print "Precision: " + str(avg_precision)
        print "Recall: " + str(avg_recall)
