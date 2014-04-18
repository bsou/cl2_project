unigram_capacity = 20
bigram_capacity = 20

import math
import constants
import numpy
import pickle
import pprint

from csv_reader import CsvReader
from word_feature import WordFeature
from time_feature import TimeFeature
from topic_model_feature import TopicModelFeature
from train_model import TrainModel
from scipy.stats.stats import spearmanr


def extract_gender(dataset, features):
    gender = {}
    for row in dataset.get_columns([constants.AUTHOR_ID_COL, constants.GENDER_COL]):
        if row[0] not in gender:
            gender[row[0]] = int(row[1])

    for one_author in features.keys():
        features[one_author].append(gender[one_author])
    return


def extract_word_features(wf, features, author_ids, unigrams=list(), bigrams=list()):
    """
    Extracts word/status related features into the dictionary of features.
    """

    # Average length of all statuses of a person
    avg_lens = wf.get_author_avg_lens()

    # Average AFINN scores of all statuses of a person
    avg_afinns = wf.get_feature_by_afinn(constants.AFINN_FILE)

    print "Average length and average afinn scores computed."

    unigram_features = dict()
    for unigram in unigrams:
        unigram_features[unigram] = wf.get_feauture_by_unigram(unigram)

    bigram_features = dict()
    for bigram in bigrams:
        bigram_features[bigram] = wf.get_feauture_by_bigram(bigram)

    for idx in range(0, len(author_ids)):
        features[author_ids[idx]].append(avg_lens[idx])
        features[author_ids[idx]].append(avg_afinns[idx])

        for unigram in unigrams:
            features[author_ids[idx]].append(unigram_features[unigram][idx])
        for bigram in bigrams:
            features[author_ids[idx]].append(bigram_features[bigram][idx])


def extract_bag_of_word_features(wf, features, author_ids, unigrams):
    unigram_counts = wf.get_feature_by_all_unigrams(unigrams)
    for idx in range(0, len(author_ids)):
        features[author_ids[idx]].append(unigram_counts[idx])


def read_unigrams_for_feature(capacity):
    unigrams = list()
    for line in open(constants.UNIGRAM_FILE, 'r'):
        unigrams.append(line[:line.index(' ')])
        if len(unigrams) == capacity:
            break
    return unigrams


def read_bigrams_for_feature(capacity):
    bigrams = list()
    for line in open(constants.BIGRAM_FILE, 'r'):
        words = line.split()
        if len(words) >= 2:
            bigrams.append((words[0], words[1]))
        if len(bigrams) == capacity:
            break
    return bigrams


def get_unigrams_for_feature(word_feature, neus, capacity):
    """
    Extract unigram features. The most frequent $capacity number of unigrams will be selected.
    Then these words will be filted based on pearson regression.
    """
    top_unigrams = word_feature.get_top_unigrams(capacity)
    print 'most frequent unigrams: ', top_unigrams
    candicate_map = dict()
    for unigram in top_unigrams:
        w_counts = word_feature.get_feauture_by_unigram(unigram)
        p = spearmanr(w_counts, neus)
        if not math.isnan(p[0]):
            candicate_map[unigram] = p

    selected_unigrams = list()
    for candicate_unigram in candicate_map.keys():
        if len(selected_unigrams) == 0:
            selected_unigrams.append(candicate_unigram)
            continue
        idx = 0
        while idx < len(selected_unigrams):
            if abs(candicate_map[selected_unigrams[idx]][0]) < abs(candicate_map[candicate_unigram][0]):
                break
            idx += 1
        selected_unigrams.insert(idx, candicate_unigram)
        if len(selected_unigrams) > unigram_capacity:
            selected_unigrams = selected_unigrams[:unigram_capacity]

    print '======== selected unigrams for feature ========'
    for selected_unigram in selected_unigrams:
        print selected_unigram, ':', candicate_map.get(selected_unigram)
    return selected_unigrams


def get_bigrams_for_feature(word_feature, neus, capacity):
    """
    Extract unigram features. The most frequent $capacity number of bigrams will be selected.
    Then these bigrams will be filted based on pearson regression.
    """
    top_bigrams = word_feature.get_top_bigrams(capacity)
    print 'most frequent bigrams: ', top_bigrams

    candicate_map = dict()
    for bigram in top_bigrams:
        bg_counts = word_feature.get_feauture_by_bigram(bigram)
        p = spearmanr(bg_counts, neus)
        if not math.isnan(p[0]):
            candicate_map[bigram] = p

    selected_bigrams = list()
    for candicate_bigram in candicate_map.keys():
        idx = 0
        while idx < len(selected_bigrams):
            if abs(candicate_map[selected_bigrams[idx]][0]) < abs(candicate_map[candicate_bigram][0]):
                break
            idx += 1
        selected_bigrams.insert(idx, candicate_bigram)
        if len(selected_bigrams) > bigram_capacity:
            selected_bigrams = selected_bigrams[:bigram_capacity]

    print '======== selected bigrams for feature ========'
    for selected_bigram in selected_bigrams:
        print selected_bigram, ':', candicate_map.get(selected_bigram)
    return selected_bigrams


def extract_time_features(dataset, features, author_ids):
    """
    Extracts time related features into the dictionary.
    """

    tf = TimeFeature(dataset)
    for time_slot in [(0, 6), (6, 11), (11, 16), (16, 21), (21, 00)]:
        counts = tf.get_frequency_of_posts(time_slot)
        for idx in range(0, len(author_ids)):
            if author_ids[idx] in counts:
                features[author_ids[idx]].append(counts[author_ids[idx]])
            else:
                features[author_ids[idx]].append(0)

    print "Frequency of posts computed."
    return


def process_training_data(disk, training_data, test_data, unigrams, bigrams, bag_of_words, topic_model_feature):
    # Initialize an empty feature matrix and empty label vector
    all_features = {}
    neu_labels = {}

    if disk == 0:
        author_ids, neus = training_data.get_author_ids_and_neu_labels()
        print len(neus), numpy.sum(neus)

        for idx in range(0, len(author_ids)):
            all_features[author_ids[idx]] = []
            neu_labels[author_ids[idx]] = neus[idx]

        # wf = WordFeature(training_data)
        # #unigrams = get_unigrams_for_feature(wf, neus, 30)
        # #bigrams = get_bigrams_for_feature(wf, neus, 10)

        # print "Training data word features extracting..."
        # extract_word_features(wf, all_features, author_ids, unigrams, bigrams)
        # extract_bag_of_word_features(wf, all_features, author_ids, bag_of_words)
        # print "Training data word features extracted."

        # print "Training data time features extracting..."
        # extract_time_features(training_data, all_features, author_ids)
        # print "Training time features extracted."

        # extract_gender(training_data, all_features)

        print "Training data topic model features extracting..."
        topic_model_feature.get_topic_model_features(author_ids, all_features)
        print "Training data topic model features extracted"

        pickle.dump(all_features, open("af.p", "wb"))
        pickle.dump(neu_labels, open("nl.p", "wb"))

    else:
        all_features = pickle.load(open("af.p", "rb"))
        neu_labels = pickle.load(open("nl.p", "rb"))

    # pp = pprint.PrettyPrinter(indent=4)
    # pp.pprint(all_features)
    return all_features, neu_labels


if __name__ == "__main__":
    unigrams = read_unigrams_for_feature(5)    # read top unigrams from pre-stored file
    print unigrams
    bigrams = read_bigrams_for_feature(3)       # read top bigrams from pre-stored file
    print bigrams
    bag_of_words = read_unigrams_for_feature(20)

    print "Training data loading..."
    training_data = CsvReader(constants.TRAINING_DATA_FILE)
    print "Training data loaded"

    print "Testing data loading..."
    test_data = CsvReader(constants.TEST_DATA_FILE, True)
    print "Testing data loaded."

    topic_model_feature = TopicModelFeature(training_data, test_data)
    topic_model_feature.train_topic_model()

    # print "Creating the topic model..."
    # topic_model_feature = TopicModelFeature()
    # print "Creation of the topic model done"

    # all_features, neu_labels = process_training_data(0, training_data, test_data, unigrams, bigrams, bag_of_words, topic_model_feature)
    # # print all_features.keys()

    # tm = TrainModel(all_features, neu_labels)
    # print "Model trained on training data."

    # # Running on the test dataset.
    # author_ids_test = test_data.get_author_ids()
    # features_test = {}

    # for idx in range(0, len(author_ids_test)):
    #     features_test[author_ids_test[idx]] = []

    # # wf_test = WordFeature(test_data)
    # # print "Testing data word features extracting..."
    # # extract_word_features(wf_test, features_test, author_ids_test, unigrams, bigrams)
    # # extract_bag_of_word_features(wf_test, features_test, author_ids_test, bag_of_words)
    # # print "Testing data word features extracted."

    # # print "Testing data time features extracting..."
    # # extract_time_features(test_data, features_test, author_ids_test)
    # # print "Testing data time features extracted."

    # # extract_gender(test_data, features_test)

    # print "Test data topic model features extracted"
    # topic_model_feature.get_topic_model_features(author_ids_test, features_test)
    # print "Test data topic model features extracted"

    # results = tm.predict(features_test)
    # print "Total number of users in test_data = ", len(results)
    # print "Total number of neurotic users = ", numpy.sum(results)

    # predictions = "+,"
    # for idx in range(0, len(author_ids_test)):
    #     if results[idx] == 1:
    #         predictions += " " + author_ids_test[idx]
    # print predictions
