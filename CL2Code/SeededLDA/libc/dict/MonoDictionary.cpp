#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <sstream>

#include "text/StopWordList.cpp"
#include "text/Vocab.cpp"
#include "utils/Pair.cpp"
#include "utils/Tripple.cpp"

using namespace std;

#ifndef MONO_DICT_H_ 
#define MONO_DICT_H_

class MonoDictionary{
	Vocab m_Vocab;
	vector<Tripple<int,int,double> > m_vItems;
	vector<vector<int> > m_Trans;

	string m_VcbPath;
	string m_sDictPath;

	StopWordList m_swl;
	double m_condProbThreshold;
	double m_dSumJointProb;
	Vocab m_restrictVocab;

	bool CanInclude(int,int,double);
	void AddEntry(int,int,double,bool);

	vector<int> WordDictEntries(int iSrcId);
	Pair<int,double> *WordTranslations(int iSrcId);

	bool TryGetValue(int iSrcId, int iTgtId, double &dProb);
	bool SameEntry(int iEntryId, int iSrcId, int iTgtId);

	public:
	void init();
	MonoDictionary();
	MonoDictionary(string sVcbPath, string sDictFilePath);

	void RestrictVocab(Vocab &restrictVocab);
	int RestrictVocabSize(){
		return m_restrictVocab.size();
	}

	void LoadDictionary();
	void LoadDictionary(double dCondProbThreshold);
	void LoadDictionary(StopWordList swl);
	void LoadDictionary(StopWordList swl,double dCondProbThreshold);

	bool AddEntry(string sSrcWord, string sTgtWord, double dJointProb, bool bLookForDuplicates);
	bool ForceAddEntry(string sSrcWord, string sTgtWord, double dJointProb, bool bLookForDuplicates);

	double SumProb();
	double Normalize();
	bool CheckConsistency();

	vector<int> WordDictEntries(string sWord);
	Pair<string,double> *WordTranslations(string sWord, bool bCondProb);

	void PrintEntry(int iEntryId);
	void PrintDict();
	double getProb(int iEntryId);
	Tripple<string,string,double> getEntry(int iEntryId);
	string getString(int iEntryId);
	int NoEntries();

	bool TryGetValue(string sSrcWord, string sTgtWord, double &dProb);

	// Specific to some programs
	Pair<string,double> *WordMIs(string sWord );
};

void MonoDictionary::init(){
	m_Vocab = Vocab(true);

	m_condProbThreshold = 0;
	m_dSumJointProb = 0;

	//m_restrictVocab = NULL;

	m_VcbPath = string();
	m_sDictPath = string();
}

MonoDictionary::MonoDictionary(){
	init();
}

MonoDictionary::MonoDictionary(string sVcbPath, string sDictFilePath){
	init();
	m_VcbPath = sVcbPath;
	m_sDictPath = sDictFilePath;
}

void MonoDictionary::RestrictVocab(Vocab &restrictVocab){
	m_restrictVocab = restrictVocab;
}

void MonoDictionary::LoadDictionary(StopWordList swl){
	m_swl= swl;
	LoadDictionary(0);
}

void MonoDictionary::LoadDictionary(StopWordList swl,double dCondProbThreshold){
	m_swl= swl;
	LoadDictionary(dCondProbThreshold);
}

void MonoDictionary::LoadDictionary(double dCondProbThreshold){
	m_condProbThreshold = dCondProbThreshold;

	if(! m_VcbPath.empty())
		m_Vocab.LoadVocab(m_VcbPath);
	cerr << "Vcb: " << m_Vocab.size() << endl; 

	bool bLookForDuplicates = true; 
	if(! m_sDictPath.empty()){
		ifstream fin(m_sDictPath.c_str(),ios_base::in);
		if(fin.fail())
			cerr << "Unable to open file " << m_sDictPath << " to load Src2Tgt Dictionary : Skipping " << endl;
		else{
			while(! fin.eof()){
				string s;
				getline(fin,s);
				//cout << s << endl;
				if(s.empty())
					continue;

				int iSrcId,iTgtId;
				double dProb;
				sscanf(s.c_str(),"%d %d %lf",&iSrcId,&iTgtId,&dProb);
				if(CanInclude(iSrcId,iTgtId,dProb)){
					double dJointProb = (dProb * m_Vocab.Prob(iSrcId));
					//cout << "\t\t" << dJointProb << endl;
					AddEntry(iSrcId,iTgtId,dJointProb,bLookForDuplicates);
					//cout << "\t\t\t\t\tdone" << endl;
				}
			}
			fin.close();
		}
	}
	// double dNormSum = Normalize();
	// cout << "Finished renormalizaing dictionary probabilities to sum " << dNormSum << endl;
	for(int i=0;i<m_Vocab.size();i++){
		vector<int> thisWordTrans = WordDictEntries(i);
	}
	cout << "Finished reading dictionaries " << endl;
}

bool MonoDictionary::CanInclude(int iSrcId,int iTgtId,double dProb){
	
	if(dProb < m_condProbThreshold)
		return false;

	string sSrcWord = m_Vocab[iSrcId],sTgtWord=m_Vocab[iTgtId];
	if(m_swl.isStopWord(sSrcWord) || m_swl.isStopWord(sTgtWord))
		return false;

	// printf("%d->%s %d->%s %lf > %lf\n",iSrcId,sSrcWord.c_str(),iTgtId,sTgtWord.c_str(),dProb,m_condProbThreshold);
	if(m_restrictVocab.size() > 0  && (m_restrictVocab.Lookup(sSrcWord) == -1 || m_restrictVocab.Lookup(sTgtWord) == -1))
		return false;
	
	return true;
}

void MonoDictionary::AddEntry(int iSrcId,int iTgtId,double dProb,bool bLookForDuplicates=true){

	int iItemId = -1;
	dProb /= 2;		// p(w_e,w_s) = [ 0.5 * p(w_e) p(w_s|w_e) + 0.5 * p(w_s) p(w_e|w_s) ]

	if(bLookForDuplicates == true && iSrcId < m_Trans.size() && iTgtId < m_Trans.size()){
		if(m_Trans[iSrcId].size() <= m_Trans[iTgtId].size()){
			vector<int> thisWordTrans = m_Trans[iSrcId];
			for(int i=0;i<thisWordTrans.size();i++)
				if( SameEntry(thisWordTrans[i], iSrcId, iTgtId))
					iItemId = thisWordTrans[i];
		}else{
			vector<int> thisWordTrans = m_Trans[iTgtId];
			for(int i=0;i<thisWordTrans.size();i++)
				if( SameEntry(thisWordTrans[i], iSrcId, iTgtId))
					iItemId = thisWordTrans[i];
		}
		if(iItemId != -1){
			m_vItems[iItemId].Third += dProb;
			m_dSumJointProb += dProb;
			/*
			double dExistingProb = m_vItems[iItemId].Third;
			m_dSumJointProb -= dExistingProb;

			dProb = 0.5 * (dProb + dExistingProb);
			m_dSumJointProb += dProb;
			*/
		}
	}

	if(iItemId == -1){
		Tripple<int,int,double> tp(iSrcId,iTgtId,dProb);
		iItemId = m_vItems.size();
		m_vItems.push_back(tp);

		m_dSumJointProb += dProb;

		while(m_Trans.size() <= iSrcId)
			m_Trans.push_back(vector<int>());
		m_Trans[iSrcId].push_back(iItemId);

		while(m_Trans.size() <= iTgtId)
			m_Trans.push_back(vector<int>());
		m_Trans[iTgtId].push_back(iItemId);
	}
}

bool MonoDictionary::AddEntry(string sSrcWord, string sTgtWord, double dJointProb, bool bLookForDuplicates=true){

	if(m_restrictVocab.size() > 0 && (m_restrictVocab.Lookup(sSrcWord) == -1 || m_restrictVocab.Lookup(sTgtWord) == -1))
		return false;
	
	int iSrcId = m_Vocab[sSrcWord], iTgtId = m_Vocab[sTgtWord];
	if( iSrcId == -1)
		iSrcId = m_Vocab.AddWord(sSrcWord);
	if(iTgtId == -1)
		iTgtId = m_Vocab.AddWord(sTgtWord);
	
	if(CanInclude(iSrcId,iTgtId,dJointProb)){
		AddEntry(iSrcId,iTgtId,dJointProb,bLookForDuplicates);
		return true;
	}else
		return false;
}

bool MonoDictionary::ForceAddEntry(string sSrcWord, string sTgtWord, double dJointProb, bool bLookForDuplicates=true){
	int iSrcId = m_Vocab[sSrcWord], iTgtId = m_Vocab[sTgtWord];
	if( iSrcId == -1)
		iSrcId = m_Vocab.AddWord(sSrcWord);
	if(iTgtId == -1)
		iTgtId = m_Vocab.AddWord(sTgtWord);
	AddEntry(iSrcId,iTgtId,dJointProb,bLookForDuplicates);
	return true;
}

double MonoDictionary::SumProb(){
	return m_dSumJointProb;
}

double MonoDictionary::Normalize(){
	double dOldSumJointProb = m_dSumJointProb;
	m_dSumJointProb = 0;

	for(int i=0;i<m_vItems.size();i++){
		m_vItems[i].Third /= dOldSumJointProb;
		m_dSumJointProb += m_vItems[i].Third;
	}
	if(fabs(m_dSumJointProb-1) > 1e-8){
		fprintf(stderr, "MonoDictionary::Normalize Ths sum of joint probabilities after normalizing is %e != 1\n", m_dSumJointProb);
		exit(1234);
	}
	return m_dSumJointProb;
}

bool MonoDictionary::CheckConsistency(){

	double dTempSumJointProb = 0;

	for(int i=0;i<m_vItems.size();i++){
		dTempSumJointProb += m_vItems[i].Third;
	}

	if(dTempSumJointProb != m_dSumJointProb){
		cerr << "Sum of prob " << dTempSumJointProb << " doesn't seem to be consisten with existing sum " << m_dSumJointProb << endl;
		return false;
	}

	if(fabs(m_dSumJointProb-1.0) >= 1e-8){
		cerr << "Sum of probabilities " << m_dSumJointProb << "!= 1" << endl;
		return false;
	}
	return true;
}

void MonoDictionary::PrintDict(){
	for(int i=0;i<m_vItems.size();i++)
		PrintEntry(i);
}

void MonoDictionary::PrintEntry(int iEntryId){
	if(iEntryId >= m_vItems.size())
		cerr << "Entry id " << iEntryId << " exceeds the size" << endl;
	else{
		Tripple<int,int,double> tp = m_vItems[iEntryId];
		cout << tp.First << "->" << m_Vocab[tp.First] << "\t" << tp.Second << "->" <<m_Vocab[tp.Second] << "\t" << tp.Third << endl;
	}
}

double MonoDictionary::getProb(int iEntryId){
	if(iEntryId >= m_vItems.size())
		return 0;
	return m_vItems[iEntryId].Third;
}

Tripple<string,string,double> MonoDictionary::getEntry(int iEntryId){
	Tripple<string,string,double> tpTemp(string(),string(),-1);
	if(iEntryId >= m_vItems.size())
		return tpTemp;
	
	Tripple<int,int,double> tp = m_vItems[iEntryId];
	tpTemp.First = m_Vocab[tp.First];
	tpTemp.Second = m_Vocab[tp.Second];
	tpTemp.Third = tp.Third;
	return tpTemp;
}

string MonoDictionary::getString(int iEntryId){
	Tripple<string,string,double> tpTemp = getEntry(iEntryId);

	if(tpTemp.First.empty() && tpTemp.Second.empty() && tpTemp.Third == -1)
		return string();
	
	ostringstream sStream;
	sStream << tpTemp.First << ":" << tpTemp.Second << "-->" << tpTemp.Third;
	return sStream.str();
}

int MonoDictionary::NoEntries(){
	return m_vItems.size();
}

vector<int> MonoDictionary::WordDictEntries(string sWord){
	return WordDictEntries(m_Vocab[sWord]);
}

vector<int> MonoDictionary::WordDictEntries(int iSrcId){
	if(iSrcId == -1)	
		return vector<int>();
	else{
		while(m_Trans.size() <= iSrcId)
			m_Trans.push_back(vector<int>());
		return m_Trans[iSrcId];
	}
}

Pair<string,double> *MonoDictionary::WordTranslations(string sWord, bool bCondProb){
	// if bCondProb is set, then this will return conditional probabilities instead of joint probabilities
	// if there is no restriction, on the target language vocab then the probabilities will also be normalized
	int iSrcId = m_Vocab[sWord];
	if(iSrcId == -1)
		return NULL;
	
	vector<int> vWordTrans = WordDictEntries(iSrcId);
	double dUnigramProb = 0, dNormConst = 0;
	if(bCondProb){
		dUnigramProb = m_Vocab.Prob(iSrcId);
		if(dUnigramProb <= 0){
			cerr << "Oops .. the unigram probability of src word[" << iSrcId << "]="<< sWord << " became " << dUnigramProb << " and hence is not valid "<< endl;
			exit(1234);
		}
	}
	Pair<string,double> *pStart=NULL;
	for(int i=0;i<vWordTrans.size();i++){
		int iEntryId = vWordTrans[i];
		int iTgtId = m_vItems[iEntryId].Second;
		Pair<string,double> *pTemp = new Pair<string,double>(m_Vocab[iTgtId],m_vItems[iEntryId].Third);
		if(bCondProb){
			pTemp->Second /= dUnigramProb;
			dNormConst += pTemp->Second;
		}
		pTemp->Next = pStart;
		pStart = pTemp;
	}
	if(dNormConst > 0)
		for(Pair<string,double> *pTemp=pStart;pTemp!=NULL;pTemp=pTemp->Next)
			pTemp->Second /= dNormConst;
	return pStart;
}

bool MonoDictionary::SameEntry(int iEntry, int iSrcId, int iTgtId){
	if(m_vItems[iEntry].First == iSrcId && m_vItems[iEntry].Second == iTgtId)
		return true;
	else if(m_vItems[iEntry].First == iTgtId && m_vItems[iEntry].Second == iSrcId)
		return true;
	return false;
}

bool MonoDictionary::TryGetValue(int iSrcId, int iTgtId, double &dProb){
	vector<int> srcWordTrans = WordDictEntries(iSrcId), tgtWordTrans = WordDictEntries(iTgtId);
	if(srcWordTrans.size() < tgtWordTrans.size()){
		for(int i=0;i<srcWordTrans.size();i++){
			int iDictEntry = srcWordTrans[i];
			if(iTgtId == m_vItems[iDictEntry].Second){
				dProb = m_vItems[iDictEntry].Third;
				return true;
			}
		}
	}else{
		for(int i=0;i<tgtWordTrans.size();i++){
			int iDictEntry = tgtWordTrans[i];
			if(iSrcId == m_vItems[iDictEntry].First){
				dProb = m_vItems[iDictEntry].Third;
				return true;
			}
		}
	}
	return false;
}

bool MonoDictionary::TryGetValue(string sSrcWord, string sTgtWord, double &dProb){
	int iSrcId = m_Vocab[sSrcWord], iTgtId = m_Vocab[sTgtWord];
	if(iSrcId == -1 || iTgtId == -1)
		return false;
	return TryGetValue(iSrcId, iTgtId, dProb);
}

// Specifically needed for some programs
Pair<string,double> *MonoDictionary::WordMIs(string sWord ){
	// if bCondProb is set, then this will return conditional probabilities instead of joint probabilities
	// if there is no restriction, on the target language vocab then the probabilities will also be normalized
	int iSrcId = m_Vocab[sWord];
	if(iSrcId == -1)
		return NULL;
	
	vector<int> vWordTrans = WordDictEntries(iSrcId);
	double dUnigramProb = 0 ;
	dUnigramProb = m_Vocab.Prob(iSrcId);
	if(dUnigramProb <= 0){
		cerr << "Oops .. the unigram probability of src word[" << iSrcId << "]="<< sWord << " became " << dUnigramProb << " and hence is not valid "<< endl;
		exit(1234);
	}
	Pair<string,double> *pStart=NULL;
	for(int i=0;i<vWordTrans.size();i++){
		int iEntryId = vWordTrans[i];
		int iTgtId = m_vItems[iEntryId].Second;
		Pair<string,double> *pTemp = new Pair<string,double>(m_Vocab[iTgtId],m_vItems[iEntryId].Third);

		double dTgtUnigramProb = m_Vocab.Prob(iTgtId);
		if(dTgtUnigramProb <= 0){
			cerr << "Oops .. the unigram probability of tgt word[" << iTgtId << "]="<< pTemp->First << " became " << dTgtUnigramProb << " and hence is not valid "<< endl;
			exit(1234);
		}

		pTemp->Second = ((double)pTemp->Second / (double) (dUnigramProb * dTgtUnigramProb));

		pTemp->Next = pStart;
		pStart = pTemp;
	}
	return pStart;
}

#endif

