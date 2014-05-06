import constants
import pickle

from collections import defaultdict
from scipy.stats.stats import pearsonr
from nltk.util import bigrams
from csv_reader import CsvReader

class WordFeature:

    def __init__(self, data_set):
        self._data_set = data_set

        # read stop words and swear words
        self._stop_words = []
        for line in open(constants.STOP_WORDS_FILE, 'r'):
            self._stop_words.append(line.lower().strip())
        self._swear_words = []
        for line in open(constants.SWEAR_WORDS_FILE, 'r'):
            self._swear_words.append(line.lower().strip())

        self._author_ids = data_set.get_author_ids()
        self._author_statuses = self.split_author_statuses(data_set.init_status_group_by_author())

    def split_author_statuses(self, author_statuses):
        splitted_author_statuses = list()
        for statuses in author_statuses:
            splitted_statuses = list()
            for status in statuses:
                splitted_status = self.split_status(status)
                splitted_statuses.append(splitted_status)
            splitted_author_statuses.append(splitted_statuses)
        return splitted_author_statuses

    def split_status(self, status):
        words = list()
        for word in status.split():
            if len(word) > 1 and not word[-1].isalpha() and word[-2:-1].isalpha():
                word = word[:-1]
            words.append(word)
        return words

    def get_swear_words(self):
        return self._swear_words

    def get_author_avg_lens(self):
        avg_lens = list()
        for statuses in self._author_statuses:
            total_len = 0
            for status in statuses:
                total_len += len(status)
            avg_lens.append(1.0 * total_len / len(statuses))
        return avg_lens

    def get_top_unigrams(self, capacity):
        if capacity == 0:
            return list()

        uni_map = defaultdict(int)
        for statuses in self._author_statuses:
            for status in statuses:
                for word in status:
                    uni_map[word] += 1

        result = list()
        for word in uni_map.keys():
            if word in self._stop_words:
                continue
            if len(result) == 0:
                result.append(word)
                continue
            for idx in range(0, len(result)):
                if uni_map[result[idx]] < uni_map[word]:
                    result.insert(idx, word)
                    break
            if len(result) > capacity:
                result = result[:capacity]
        for swear_word in self._swear_words:
            if swear_word not in result:
                result.append(swear_word)
        return result

    def get_top_bigrams(self, capacity):
        if capacity == 0:
            return list()

        bi_map = defaultdict(int)
        for statuses in self._author_statuses:
            for status in statuses:
                for bg in bigrams(status):
                    bi_map[bg] += 1
        result = list()
        for bigram in bi_map.keys():
            if len(result) == 0:
                result.append(bigram)
                continue
            for idx in range(0, len(result)):
                if bi_map[result[idx]] < bi_map[bigram]:
                    result.insert(idx, bigram)
                    break
            if len(result) > capacity:
                result = result[:capacity]
        return result

    def get_feauture_by_unigram(self, word):
        word_counts = list()
        for statuses in self._author_statuses:
            count = 0
            for status in statuses:
                for w in status:
                    if w == word:
                        count += 1
            word_counts.append(count)
        return word_counts

    def get_feature_by_all_unigrams(self, unigrams):
        word_counts = list()
        for statuses in self._author_statuses:
            count = 0
            for status in statuses:
                for w in status:
                    if w in unigrams:
                        count += 1
            word_counts.append(count)
        return word_counts

    def get_feauture_by_bigram(self, bigram):
        bg_counts = list()
        for statuses in self._author_statuses:
            count = 0
            for status in statuses:
                for bg in bigrams(status):
                    if bg == bigram:
                        count += 1
            bg_counts.append(count)
        return bg_counts

    def get_feature_by_all_bigrams(self, bgs):
        bg_counts = list()
        for statuses in self._author_statuses:
            count = 0
            for status in statuses:
                for bg in bigrams(status):
                    if bg in bgs:
                        count += 1
            bg_counts.append(count)
        return bg_counts

    def get_feature_by_afinn(self, afinn_file):
        afinn = dict(map(lambda (k, v): (k, int(v)), [line.split('\t') for line in open(afinn_file)]))
        avg_afinns = list()
        for statuses in self._author_statuses:
            total_affin = 0
            for status in statuses:
                total_affin += sum(map(lambda word: afinn.get(word, 0), status))
            avg_afinns.append(1.0 * total_affin / len(statuses))
        return avg_afinns

    def get_feature_by_nrc_lexicon(self):
        nrc_scores = pickle.load(open("nrc.p", "rb"))
        print len(nrc_scores.keys()), "words read."
        auth_scores = list()
        for statuses in self._author_statuses:
            total = [0, 0, 0, 0, 0, 0, 0, 0]
            cnt = 0
            for status in statuses:
                for word in status:
                    if word in nrc_scores:
                        total = [x + y for x, y in zip(total, nrc_scores[word])]
                        cnt += 1
            # print total
            if cnt == 0:
                auth_scores.append([0, 0, 0, 0, 0, 0, 0, 0])
            else:
                auth_scores.append([int(t * 100.0 / cnt) for t in total])
        return auth_scores

    def get_feature_by_punctuation(self, punc):
        aut_statuses = self._data_set.init_status_group_by_author()
        punc_counts = list()
        for statuses in aut_statuses:
            count = 0
            for status in statuses:
                for c in status:
                    if c == punc:
                        count += 1
            punc_counts.append(count)
        return punc_counts


if __name__ == "__main__":
    training_data = CsvReader(constants.TRAINING_DATA_FILE)
    fr = WordFeature(training_data)
    author_ids, neus = training_data.get_author_ids_and_neu_labels()
    # author_ids = fr.get_author_list()
    # print len(author_ids), ' author ids ', author_ids[0], ' ... ', author_ids[-1], ' loaded: ', author_ids.index('2ab5dc09af49cf5114e60087919f1f3a')
    # neus = fr.get_nue_list()
    # print len(neus), ' neus ', neus[0], ' ... ', neus[-1]
    # author_statuses = fr.read_status_group_by_author()
    # print len(author_statuses), ' author statuses '     # , author_statuses[0], ' ... ', author_statuses[author_ids.index('2ab5dc09af49cf5114e60087919f1f3a')]

    # avg_lens = fr.get_author_avg_lens()
    # print len(avg_lens), ' avg_lens:', avg_lens[0], ' ', avg_lens[1], '...', avg_lens[-1]
    # print 'average length feature: ', pearsonr(avg_lens, neus)

    # avg_afinns = fr.get_feature_by_afinn('../dataset/AFINN-111.txt')
    # print len(avg_afinns), ' avg_afinns:', avg_afinns[0], ' ', avg_afinns[1], '...', avg_afinns[-1]
    # print 'average afinn feature: ', pearsonr(avg_afinns, neus)

    top_unigrams = fr.get_top_unigrams(6000)
    #print top_unigrams
    selected_unigrams = list()
    for unigram in top_unigrams:
        w_counts = fr.get_feauture_by_unigram(unigram)
        p = pearsonr(w_counts, neus)
        #print unigram, ': ', p
        if abs(p[0]) >= 0.1:
            selected_unigrams.append(unigram)
    print len(selected_unigrams), ', ', selected_unigrams
    #print ''

    # top_bigrams = fr.get_top_bigrams(20)
    # #print topBigrams
    # selected_bigrams = list()
    # for bigram in top_bigrams:
    #     bg_counts = fr.get_feauture_by_bigram(bigram)
    #     p = pearsonr(bg_counts, neus)
    #     if abs(p[0]) >= 0.1:
    #         selected_bigrams.append(bigram)
    # print len(selected_bigrams), ', ', selected_bigrams
