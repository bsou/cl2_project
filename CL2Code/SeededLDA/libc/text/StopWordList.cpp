#include <iostream>
#include <map>
#include <string>

using namespace std;

#ifndef STOP_WORD_LIST_H_ 
#define STOP_WORD_LIST_H_

class StopWordList{
	map<string,bool> m_stopwords;

	void init();

	public:
	StopWordList() {}
	StopWordList(string sFilePath);
	void Add(string sWord);
	bool isStopWord(string sWord);
};

StopWordList::StopWordList(string sFilePath){

	ifstream fin(sFilePath.c_str());
    if(fin.fail()){
        cerr << "Unable to open file "<< sFilePath <<" to load stop words" << endl;
        exit(1234);
        return;
    }

    while(! fin.eof()){
        string s;
        int id, ifreq;
        getline(fin,s);
        if(s.empty())
            continue;
		
		char *buf = new char[s.length()];
		sscanf(s.c_str(),"%s",buf);
		Add(string(buf));
	}
	cout << "StopWordList::Loaded " << m_stopwords.size() << " elements" << endl;
}

void StopWordList::Add(string sWord){
	m_stopwords[sWord] = true;

	map<string,bool>::iterator iter = m_stopwords.find(sWord);
    if(iter == m_stopwords.end())
		m_stopwords[sWord] = true;
}

bool StopWordList::isStopWord(string sWord){
	map<string,bool>::iterator iter = m_stopwords.find(sWord);
	if(iter != m_stopwords.end())
		return true;
	else
		return false;
}

#endif
