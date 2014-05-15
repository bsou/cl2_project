import pickle

if __name__ == "__main__":
    scores = {}
    content = [line.rstrip(' \n') for line in open('../dataset/nrc_emotion_lexicon.txt')]
    content = [item for item in content if len(item) > 1 and not item.isdigit()]

    content = zip(*(iter(content),) * 11)

    for word_prop in content:
        scores[word_prop[0]] = [int(word_prop[3].split(':')[-1]),
                                int(word_prop[4].split(':')[-1]),
                                int(word_prop[5].split(':')[-1]),
                                int(word_prop[6].split(':')[-1]),
                                int(word_prop[7].split(':')[-1]),
                                int(word_prop[8].split(':')[-1]),
                                int(word_prop[9].split(':')[-1]),
                                int(word_prop[10].split(':')[-1])]

    pickle.dump(scores, open("nrc.p", "wb"))
    print len(scores.keys()), "words stored."
    #for line in open('../dataset/nrc_emotion_lexicon.txt'):
