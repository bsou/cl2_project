#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

#ifndef VOCAB_H_
#define VOCAB_H_

class Vocab{
	map<string,int> m_vocab2id;
	vector<string> m_id2vocab;
	string sNull;
	bool m_bLoadFrequency;
	vector<int> m_wordFreq;
	unsigned long int m_totalFreq;

	void init(bool);

	public:
	Vocab(bool);
	Vocab(string ,bool);
	void LoadVocab(string sFilePath);
	void Print();

	int operator[](string);
	int Lookup(string);
	string operator[](int);
	string Lookup(int);

	int size();
	int AddWord(string ,int ifreq);
	int AddWordIdPair(string sWord,int id,int ifreq);
	double Prob(string s);
	double Prob(int id);
	bool VerifyFreq();
};

Vocab::Vocab(bool bLoadFrequency=false){
	init(bLoadFrequency);
}

void Vocab::init(bool bLoadFrequency){
	m_bLoadFrequency = bLoadFrequency;
	m_totalFreq = 0;

	sNull = string("[NULL]");
	m_vocab2id[sNull]= 0;
	m_id2vocab.push_back(sNull);
}

Vocab::Vocab(string sFilePath, bool bLoadFrequency){
	init(bLoadFrequency);
	LoadVocab(sFilePath);
}

void Vocab::LoadVocab(string sFilePath){

	fstream fin(sFilePath.c_str(),ios_base::in);
	if(fin.fail()){
		cerr << "Unable to open file "<< sFilePath <<" to load vocab" << endl;
		exit(1234);
		return;
	}

	while(! fin.eof()){
		string s;
		int id, ifreq;
		getline(fin,s);
		if(s.empty())
			continue;

		int iDigitPos = s.find_first_of("0123456789");
		const char* line = s.c_str()+iDigitPos;

		char* buf = new char[s.length()];
		sscanf(line,"%d %s %d",&id,buf,&ifreq);
		string word = string(buf);
	
		AddWordIdPair(word,id,ifreq);
	}
}

int Vocab::AddWordIdPair(string word,int id,int ifreq=0){

	if(m_vocab2id.find(word) != m_vocab2id.end()){
		cerr << "Key value pair (" << word << "," << m_vocab2id[word] << ") already exists for " << word << endl;
		exit(1234);
	}
	//if(id < m_id2vocab.size()){
	//	cerr << "Key value pair (" << m_id2vocab[id] << "," << id << ") already exists for " << id << endl;
	//	exit(1234);
	//}

	m_vocab2id[word]=id;
	while(m_id2vocab.size() <= id)
		m_id2vocab.push_back(sNull);
	m_id2vocab[id]=word;

	if(m_bLoadFrequency){
		if(ifreq < 0){
			cerr << "The frequency of (" << word << "," << id << ") = "<< ifreq <<"is not valid " << endl;
			exit(1234);
		}
		while(m_wordFreq.size() <= id)
			m_wordFreq.push_back(0);
		m_wordFreq[id] = ifreq;
		m_totalFreq += ifreq;
	}

	return id;
}

int Vocab::AddWord(string word,int ifreq=0){
	return AddWordIdPair(word,m_id2vocab.size(),ifreq);
}

void Vocab::Print(){
	for(map<string,int>::iterator iter = m_vocab2id.begin();iter!=m_vocab2id.end();++iter){
		cout << iter->first << " " << iter->second << endl;
	}
}

int Vocab::operator[](string s){
	return Lookup(s);
}

int Vocab::Lookup(string s){
	map<string,int>::iterator iter = m_vocab2id.find(s);
	if(iter == m_vocab2id.end())
		return -1;
	else
		return m_vocab2id[s];
}

string Vocab::operator[](int id){
	return Lookup(id);
}

string Vocab::Lookup(int id){
	if(id >= 0 && id < m_id2vocab.size())
		return m_id2vocab[id];
	else
		return sNull; 
}

int Vocab::size(){
	return m_id2vocab.size();
}

double Vocab::Prob(string s){
	int id = m_vocab2id[s];
	return Prob(id);
}

double Vocab::Prob(int id){
	if(m_bLoadFrequency)
		return ((double)m_wordFreq[id]/m_totalFreq);
	else{
		cerr << "Vocab::Prob frequencies are not available to compute probability" << endl;
		return 0;
	}
}

bool Vocab::VerifyFreq(){

	unsigned int dTempTotal = 0;
	for(int i=1;i<size();i++)
		dTempTotal += m_wordFreq[i];
	
	if(dTempTotal == m_totalFreq)
		return true;
	else
		return false;
}

#endif
