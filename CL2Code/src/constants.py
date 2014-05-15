TRAINING_DATA_FILE = '../training.csv'
TEST_DATA_FILE = '../testdata.csv'
STOP_WORDS_FILE = '../dataset/stop_words.txt'
SWEAR_WORDS_FILE = '../dataset/swear_words.txt'
AFINN_FILE = '../dataset/AFINN-111.txt'
UNIGRAM_FILE = '../unigrams.txt'
BIGRAM_FILE = '../bigrams.txt'
TOPICS = 50
SEEDED_TOPICS = 15
TOPIC_MODEL_FEATURES_FILE = '../mallet/50_withunlabeled/output_doc_topics.txt'
SEEDED_LDA_FEATURES_FILE_TRAIN = '../SeededLDA/output/SeededLDA_train_DSM.txt'
SEEDED_LDA_FEATURES_FILE_TEST = '../SeededLDA/output/SeededLDA_test_DSM.txt'
SEEDED_LDA_FEATURES_FILE_TEST_IDMAP= '../SeededLDA/output/id-map.txt'
CESD_SCORES = '../cesd_scores.txt'
FEATURE_VECTORS_FILE='../feature_vectors.csv'
#WRITE_TO_FEATURE_VECTORS_FILE = 'TRUE'
WRITE_TO_FEATURE_VECTORS_FILE = 'FALSE'
FILTER_FEATURE = 'FALSE'
REGRESS_BY_CESD = 'FALSE'

TIME_SLOTS = [(0, 6), (6, 11), (11, 16), (16, 21), (21, 0)]
#TIME_SLOTS = [(0, 2), (2, 4), (4, 6), (6, 8), (8, 10), (10, 12), (12, 14), (14, 16), (16, 18), (18, 20), (20, 22), (22, 0)]

NUMBER_OF_COLS_IN_TRAINING_DATA = 6

# The order of fields in the training dataset
AUTHOR_ID_COL = 0
STATUS_COL = 1
TIMESTAMP_COL = 2
NUMBER_OF_STATUS_COL = 3
GENDER_COL = 4
NEU_COL = 5

NEU_SYMBOL = '+'

DATA_START_ROW = 0

AUTHOR_ID_LEN = 32

