unigram_capacity = 114
bigram_capacity = 38

use_nrc = True
use_punctuation = True
puncs = ['!', '?']
use_time_slots = True
use_gender = True

import math
import constants
import numpy
import pickle
import pprint
import time

from csv_reader import CsvReader
from word_feature import WordFeature
from time_feature import TimeFeature
from topic_model_feature import TopicModelFeature
from train_model import TrainModel
from train_naive_bayes import TrainNaiveBayes
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
    # avg_lens = wf.get_author_avg_lens()

    # Average AFINN scores of all statuses of a person
    # avg_afinns = wf.get_feature_by_afinn(constants.AFINN_FILE)

    # NRC Lexicon scores
    if use_nrc:
        nrc_scores = wf.get_feature_by_nrc_lexicon()
        print "NRC scores computed."

    punc_features = dict()
    if use_punctuation:
        for punc in puncs:
            punc_features[punc] = wf.get_feature_by_punctuation(punc)
        print 'punc scores computed.'

    unigram_features = dict()
    for unigram in unigrams:
        unigram_features[unigram] = wf.get_feauture_by_unigram(unigram)

    bigram_features = dict()
    for bigram in bigrams:
        bigram_features[bigram] = wf.get_feauture_by_bigram(bigram)

    for idx in range(0, len(author_ids)):
        # features[author_ids[idx]].append(avg_lens[idx])
        # features[author_ids[idx]].append(avg_afinns[idx])

        features[author_ids[idx]] += nrc_scores[idx]

        if use_punctuation:
            for punc in puncs:
                features[author_ids[idx]].append(punc_features[punc][idx])

        for unigram in unigrams:
            features[author_ids[idx]].append(unigram_features[unigram][idx])
        for bigram in bigrams:
            features[author_ids[idx]].append(bigram_features[bigram][idx])


def extract_bag_of_word_feature(wf, features, author_ids, unigrams):
    if len(unigrams) == 0:
        return
    unigram_counts = wf.get_feature_by_all_unigrams(unigrams)
    for idx in range(0, len(author_ids)):
        features[author_ids[idx]].append(unigram_counts[idx])


def extract_bag_of_bigram_feature(wf, features, author_ids, bigrams):
    if len(bigrams) == 0:
        return
    bigram_counts = wf.get_feature_by_all_bigrams(bigrams)
    for idx in range(0, len(author_ids)):
        features[author_ids[idx]].append(bigram_counts[idx])


def read_unigrams_for_feature(capacity):
    unigrams = list()
    for line in open(constants.UNIGRAM_FILE, 'r'):
        if len(unigrams) == capacity:
            break
        unigrams.append(line[:line.index(' ')])
    return unigrams


def read_bigrams_for_feature(capacity):
    bigrams = list()
    for line in open(constants.BIGRAM_FILE, 'r'):
        if len(bigrams) == capacity:
            break
        words = line.split()
        if len(words) >= 2:
            bigrams.append((words[0], words[1]))
    return bigrams


def get_unigrams_for_feature(word_feature, neus, capacity):
    """
    Extract unigram features. The most frequent $capacity number of unigrams will be selected.
    Then these words will be filted based on spearman rank correlation.
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
    for time_slot in constants.TIME_SLOTS:
        counts = tf.get_frequency_of_posts(time_slot)
        for idx in range(0, len(author_ids)):
            if author_ids[idx] in counts:
                features[author_ids[idx]].append(counts[author_ids[idx]])
            else:
                features[author_ids[idx]].append(0)

    print "Frequency of posts computed."
    return


def process_training_data(disk, training_data, unigrams, bigrams, bag_of_words, bag_of_bigrams, topic_model_feature, is_testing_data=False):
    # Initialize an empty feature matrix and empty label vector
    all_features = {}
    neu_labels = {}
    author_ids = list()
    neus = list()
    title = "training"
    if is_testing_data:
        title = 'testing'

    if disk == 0:
        if not is_testing_data:
            author_ids, neus = training_data.get_author_ids_and_neu_labels()
            print len(neus), numpy.sum(neus)
        else:
            author_ids = training_data.get_author_ids()

        for idx in range(0, len(author_ids)):
            all_features[author_ids[idx]] = []
            if not is_testing_data:
                neu_labels[author_ids[idx]] = neus[idx]

        wf = WordFeature(training_data)
        #unigrams = get_unigrams_for_feature(wf, neus, unigram_capacity)
        #bigrams = get_bigrams_for_feature(wf, neus, bigram_capacity)

        print title, "data word features extracting..."
        extract_word_features(wf, all_features, author_ids, unigrams, bigrams)
        print title, "data word features extracted"

        print title, "data bag of words feature extracting..."
        extract_bag_of_word_feature(wf, all_features, author_ids, bag_of_words)
        print title, "data bag of words feature extracted."

        print title, "data bag of bigrams feature extracting..."
        extract_bag_of_bigram_feature(wf, all_features, author_ids, bag_of_bigrams)
        print title, "data bag of bigrams feature extracted."

        if use_time_slots:
            print title, "data time features extracting..."
            extract_time_features(training_data, all_features, author_ids)
            print title, "data time features extracted."

        if use_gender:
            print title, "data gender feature extracting..."
            extract_gender(training_data, all_features)
            print title, "data gender feature extracted"

        print title, "data topic model features extracting..."
        if topic_model_feature is not None:
            topic_model_feature.get_topic_model_features(author_ids, all_features)
        print title, "data topic model features extracted"

        feature_name_types = list()
        if use_nrc:
            for idx in range(0, 8):
                feature_name_types.append(('nrc_' + str(idx), 'numeric'))

        if use_punctuation:
            for idx in range(0, len(puncs)):
                feature_name_types.append(('punc_' + str(idx), 'numeric'))

        for idx in range(0, len(unigrams)):
            feature_name_types.append(('unigram_' + remove_quotation(unigrams[idx]), 'numeric'))

        for idx in range(0, len(bigrams)):
            feature_name_types.append(('bigram_' + remove_quotation(bigrams[idx][0]) + '_' + remove_quotation(bigrams[idx][1]), 'numeric'))

        if len(bag_of_words) > 0:
            feature_name_types.append(('bag_of_words_' + str(len(bag_of_words)), 'numeric'))

        if len(bag_of_bigrams) > 0:
            feature_name_types.append(('bag_of_bigrams_' + str(len(bag_of_bigrams)), 'numeric'))

        if use_time_slots:
            for idx in range(0, len(constants.TIME_SLOTS)):
                feature_name_types.append(('time_slot_' + str(constants.TIME_SLOTS[idx][0]) + '_' + str(constants.TIME_SLOTS[idx][1]), 'numeric'))

        if use_gender:
            feature_name_types.append(('gender', 'numeric'))
        
        if topic_model_feature:
            for idx in range(0, 100):
                feature_name_types.append(('topic_model_' + str(idx), 'numeric'))

        dest_file = '../arff/' + str(int(round(time.time() * 1000))) + '_' + title + '.arff'
        print 'writing', title, 'features and labels to arff file:', dest_file
        training_data.convert_to_arff(dest_file, feature_name_types, author_ids, all_features, neu_labels, is_testing_data)

        pickle.dump(all_features, open("af.p", "wb"))
        pickle.dump(neu_labels, open("nl.p", "wb"))

    else:
        all_features = pickle.load(open("af.p", "rb"))
        neu_labels = pickle.load(open("nl.p", "rb"))

    # pp = pprint.PrettyPrinter(indent=4)
    # pp.pprint(all_features)
    return all_features, neu_labels


def remove_quotation(word):
    return word.replace("\"", "").replace("\'", "")

if __name__ == "__main__":
    use_nrc = True
    use_punctuation = True
    use_time_slots = True
    use_gender = True

    unigrams = read_unigrams_for_feature(unigram_capacity)
    print ">>", len(unigrams), 'unigram features:', unigrams
    bigrams = read_bigrams_for_feature(bigram_capacity)
    print ">>", len(bigrams), 'bigram features:', bigrams

    bag_of_words = read_unigrams_for_feature(0)    # 84
    print ">> a bag of words of size ", len(bag_of_words), ":", bag_of_words
    bag_of_bigrams = read_bigrams_for_feature(0)   # 5
    
    print ">> a bag of bigrams of size ", len(bag_of_bigrams), ":", bag_of_bigrams

    #print "Training data loading..."
    #training_data = CsvReader(constants.TRAINING_DATA_FILE)
    #print "Training data loaded"

    print "Creating the topic model..."
    topic_model_feature = TopicModelFeature()
    #topic_model_feature = None
    print "Creation of the topic model done"

    #all_features, neu_labels = process_training_data(0, training_data, unigrams, bigrams, bag_of_words, bag_of_bigrams, topic_model_feature)

    print "Testing data loading..."
    test_data = CsvReader(constants.TEST_DATA_FILE, True)
    print "Testing data loaded."

    features_test, neu_labels_not_used = process_training_data(0, test_data, unigrams, bigrams, bag_of_words, bag_of_bigrams, topic_model_feature, True)
    #author_ids_test = test_data.get_author_ids()

    # print "Model training on training data..."
    # tm = TrainModel(all_features, neu_labels)
    # print "Model trained on training data."

    # results = tm.predict(features_test)
    # print "Total number of users in test_data = ", len(results)
    # print "Total number of neurotic users = ", numpy.sum(results)

    # predictions = "+,"
    # for idx in range(0, len(author_ids_test)):
    #     if results[idx] == 1:
    #         predictions += " " + author_ids_test[idx]
    # print predictions
