TRAINING_DATA_FILE = '../training.csv'
TEST_DATA_FILE = '../testdata.csv'
STOP_WORDS_FILE = '../dataset/stop_words.txt'
SWEAR_WORDS_FILE = '../dataset/swear_words.txt'
AFINN_FILE = '../dataset/AFINN-111.txt'
UNIGRAM_FILE = '../unigrams.txt'
BIGRAM_FILE = '../bigrams.txt'
TOPIC_MODEL_FEATURES_FILE = '../mallet/100/output_doc_topics.txt'

#TIME_SLOTS = [(0, 6), (6, 11), (11, 16), (16, 21)]
TIME_SLOTS = [(0, 2), (2, 4), (4, 6), (6, 8), (8, 10), (10, 12), (12, 14), (14, 16), (16, 18), (18, 20), (20, 22), (22, 0)]

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
