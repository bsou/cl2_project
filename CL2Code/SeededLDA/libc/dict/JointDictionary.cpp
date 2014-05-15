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

#ifndef JOINT_DICT_H_ 
#define JOINT_DICT_H_

class JointDictionary{
	Vocab m_srcVocab;
	Vocab m_tgtVocab;
	vector<Tripple<int,int,double> > m_vItems;
	vector<vector<int> > m_srcTrans;
	vector<vector<int> > m_tgtTrans;

	string m_srcVcbPath;
	string m_tgtVcbPath;
	string m_sSrc2TgtDictPath;
	string m_sTgt2SrcDictPath;

	StopWordList m_swlSrc;
	StopWordList m_swlTgt;
	double m_condProbThreshold;
	double m_dSumJointProb;
	Vocab *m_restrictSrcVocab,*m_restrictTgtVocab;

	bool CanInclude(int,int,double);
	void AddEntry(int,int,double,bool);

	vector<int> SrcWordDictEntries(int iSrcId);
	vector<int> TgtWordDictEntries(int iTgtId);
	Pair<int,double> *SrcWordTranslations(int iSrcId);
	Pair<int,double> *TgtWordTranslations(int iTgtId);

	bool TryGetValue(int iSrcId, int iTgtId, double &dProb);

	public:
	void init();
	JointDictionary();
	JointDictionary(string sSrcVcbPath, string sTgtVcbPath, string sSrc2TgtDictFilePath, string sTgt2SrcDictFilePath);

	void RestrictVocab(Vocab *restrictSrcVocab, Vocab *restrictTgtVocab);

	void LoadDictionary();
	void LoadDictionary(double dCondProbThreshold);
	void LoadDictionary(StopWordList swlSrc,StopWordList swlTgt);
	void LoadDictionary(StopWordList swlSrc,StopWordList swlTgt, double dCondProbThreshold);

	bool ForceAddEntry(string sSrcWord, string sTgtWord, double dJointProb, bool bLookForDuplicates);

	double SumProb();
	double Normalize();
	bool CheckConsistency();

	vector<int> SrcWordDictEntries(string sSrcWord);
	vector<int> TgtWordDictEntries(string sTgtWord);
	Pair<string,double> *SrcWordTranslations(string sSrcWord, bool bCondProb);
	Pair<string,double> *TgtWordTranslations(string sTgtWord, bool bCondProb);

	void PrintEntry(int iEntryId);
	void PrintDict();
	double getProb(int iEntryId);
	Tripple<string,string,double> getEntry(int iEntryId);
	string getString(int iEntryId);
	int NoEntries();

	bool TryGetValue(string sSrcWord, string sTgtWord, double &dProb);

	// Specific to some programs
	Pair<string,double> *SrcWordMIs(string sSrcWord );
	Pair<string,double> *TgtWordMIs(string sTgtWord );
};

void JointDictionary::init(){
	m_srcVocab = Vocab(true);
	m_tgtVocab = Vocab(true);

	m_condProbThreshold = 0;
	m_dSumJointProb = 0;

	m_restrictSrcVocab = NULL;
	m_restrictTgtVocab = NULL;
}

JointDictionary::JointDictionary(){
	init();
}

JointDictionary::JointDictionary(string sSrcVcbPath, string sTgtVcbPath, string sSrc2TgtDictFilePath, string sTgt2SrcDictFilePath){
	init();
	m_srcVcbPath = sSrcVcbPath;
	m_tgtVcbPath = sTgtVcbPath;
	m_sSrc2TgtDictPath = sSrc2TgtDictFilePath;
	m_sTgt2SrcDictPath = sTgt2SrcDictFilePath;
}

void JointDictionary::RestrictVocab(Vocab *restrictSrcVocab, Vocab *restrictTgtVocab){
	m_restrictSrcVocab = restrictSrcVocab;
	m_restrictTgtVocab = restrictTgtVocab;
}

void JointDictionary::LoadDictionary(StopWordList swlSrc,StopWordList swlTgt){
	m_swlSrc = swlSrc;
	m_swlTgt = swlTgt;
	LoadDictionary(0);
}

void JointDictionary::LoadDictionary(StopWordList swlSrc,StopWordList swlTgt, double dCondProbThreshold){
	m_swlSrc = swlSrc;
	m_swlTgt = swlTgt;
	LoadDictionary(dCondProbThreshold);
}

void JointDictionary::LoadDictionary(double dCondProbThreshold){
	m_condProbThreshold = dCondProbThreshold;

	m_srcVocab.LoadVocab(m_srcVcbPath);
	cerr << "SrcVcb: " << m_srcVocab.size() << endl; 
	m_tgtVocab.LoadVocab(m_tgtVcbPath);
	cerr << "TgtVcb: " << m_tgtVocab.size() << endl;

	bool bLookForDuplicates = false; 
	if(! m_sSrc2TgtDictPath.empty()){
		ifstream fin(m_sSrc2TgtDictPath.c_str(),ios_base::in);
		if(fin.fail())
			cerr << "Unable to open file " << m_sSrc2TgtDictPath << " to load Src2Tgt Dictionary : Skipping " << endl;
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
					double dJointProb = (dProb * m_srcVocab.Prob(iSrcId));
					//cout << "\t\t" << dJointProb << endl;
					AddEntry(iSrcId,iTgtId,dJointProb,bLookForDuplicates);
					//cout << "\t\t\t\t\tdone" << endl;
				}
			}
			fin.close();
			bLookForDuplicates = true;
		}
	}
	if(! m_sTgt2SrcDictPath.empty()){
		ifstream fin(m_sTgt2SrcDictPath.c_str(),ios_base::in);
		if(fin.fail())
			cerr << "Unable to open file " << m_sTgt2SrcDictPath << " to load Tgt2Src Dictionary : Skipping " << endl;
		else{
			while(! fin.eof()){
				string s;
				getline(fin,s);
				if(s.empty())
					continue;

				int iSrcId,iTgtId;
				double dProb;
				sscanf(s.c_str(),"%d %d %lf",&iTgtId,&iSrcId,&dProb);
				if(CanInclude(iSrcId,iTgtId,dProb)){
					double dJointProb = (dProb * m_tgtVocab.Prob(iTgtId));
					AddEntry(iSrcId,iTgtId,dJointProb,bLookForDuplicates);
				}
			}
		}
		fin.close();
	}
	cout << "Finished reading dictionaries " << endl;
	// double dNormSum = Normalize();
	// cout << "Finished renormalizaing dictionary probabilities to sum " << dNormSum << endl;
}

bool JointDictionary::CanInclude(int iSrcId,int iTgtId,double dProb){
	
	if(dProb < m_condProbThreshold)
		return false;

	string sSrcWord = m_srcVocab[iSrcId],sTgtWord=m_tgtVocab[iTgtId];
	if(m_swlSrc.isStopWord(sSrcWord) || m_swlTgt.isStopWord(sTgtWord))
		return false;

	// printf("%d->%s %d->%s %lf > %lf\n",iSrcId,sSrcWord.c_str(),iTgtId,sTgtWord.c_str(),dProb,m_condProbThreshold);
	if(m_restrictSrcVocab != NULL && m_restrictSrcVocab->Lookup(sSrcWord) == -1)
		return false;
	
	if(m_restrictTgtVocab != NULL && m_restrictTgtVocab->Lookup(sTgtWord) == -1)
		return false;

	return true;
}

void JointDictionary::AddEntry(int iSrcId,int iTgtId,double dProb,bool bLookForDuplicates=false){

	int iItemId = -1;
	dProb /= 2;		// p(w_e,w_s) = [ 0.5 * p(w_e) p(w_s|w_e) + 0.5 * p(w_s) p(w_e|w_s) ]

	if(bLookForDuplicates == true && iSrcId < m_srcTrans.size() && iTgtId < m_tgtTrans.size()){
		if(m_srcTrans[iSrcId].size() <= m_tgtTrans[iTgtId].size()){
			vector<int> thisWordTrans = m_srcTrans[iSrcId];
			for(int i=0;i<thisWordTrans.size();i++)
				if( m_vItems[thisWordTrans[i]].Second == iTgtId)
					iItemId = thisWordTrans[i];
		}else{
			vector<int> thisWordTrans = m_tgtTrans[iTgtId];
			for(int i=0;i<thisWordTrans.size();i++)
				if( m_vItems[thisWordTrans[i]].First == iSrcId)
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

		while(m_srcTrans.size() <= iSrcId)
			m_srcTrans.push_back(vector<int>());
		m_srcTrans[iSrcId].push_back(iItemId);

		while(m_tgtTrans.size() <= iTgtId)
			m_tgtTrans.push_back(vector<int>());
		m_tgtTrans[iTgtId].push_back(iItemId);
	}
}

bool JointDictionary::ForceAddEntry(string sSrcWord, string sTgtWord, double dJointProb, bool bLookForDuplicates=false){
	int iSrcId = m_srcVocab[sSrcWord], iTgtId = m_tgtVocab[sTgtWord];
	if( iSrcId == -1)
		iSrcId = m_srcVocab.AddWord(sSrcWord);
	if(iTgtId == -1)
		iTgtId = m_tgtVocab.AddWord(sTgtWord);
	AddEntry(iSrcId,iTgtId,dJointProb,bLookForDuplicates);
	return true;
}

double JointDictionary::SumProb(){
	return m_dSumJointProb;
}

double JointDictionary::Normalize(){
	double dOldSumJointProb = m_dSumJointProb;
	m_dSumJointProb = 0;

	for(int i=0;i<m_vItems.size();i++){
		m_vItems[i].Third /= dOldSumJointProb;
		m_dSumJointProb += m_vItems[i].Third;
	}
	if(fabs(m_dSumJointProb-1) > 1e-8){
		fprintf(stderr, "JointDictionary::Normalize Ths sum of joint probabilities after normalizing is %e != 1\n", m_dSumJointProb);
		exit(1234);
	}
	return m_dSumJointProb;
}

bool JointDictionary::CheckConsistency(){

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

void JointDictionary::PrintDict(){
	for(int i=0;i<m_vItems.size();i++)
		PrintEntry(i);
}

void JointDictionary::PrintEntry(int iEntryId){
	if(iEntryId >= m_vItems.size())
		cerr << "Entry id " << iEntryId << " exceeds the size" << endl;
	else{
		Tripple<int,int,double> tp = m_vItems[iEntryId];
		cout << tp.First << "->" << m_srcVocab[tp.First] << "\t" << tp.Second << "->" <<m_tgtVocab[tp.Second] << "\t" << tp.Third << endl;
	}
}

double JointDictionary::getProb(int iEntryId){
	if(iEntryId >= m_vItems.size())
		return 0;
	return m_vItems[iEntryId].Third;
}

Tripple<string,string,double> JointDictionary::getEntry(int iEntryId){
	Tripple<string,string,double> tpTemp(string(),string(),-1);
	if(iEntryId >= m_vItems.size())
		return tpTemp;
	
	Tripple<int,int,double> tp = m_vItems[iEntryId];
	tpTemp.First = m_srcVocab[tp.First];
	tpTemp.Second = m_tgtVocab[tp.Second];
	tpTemp.Third = tp.Third;
	return tpTemp;
}

string JointDictionary::getString(int iEntryId){
	Tripple<string,string,double> tpTemp = getEntry(iEntryId);

	if(tpTemp.First.empty() && tpTemp.Second.empty() && tpTemp.Third == -1)
		return string();
	
	ostringstream sStream;
	sStream << tpTemp.First << ":" << tpTemp.Second << "-->" << tpTemp.Third;
	return sStream.str();
}

int JointDictionary::NoEntries(){
	return m_vItems.size();
}

vector<int> JointDictionary::SrcWordDictEntries(string sSrcWord){
	return SrcWordDictEntries(m_srcVocab[sSrcWord]);
}

vector<int> JointDictionary::SrcWordDictEntries(int iSrcId){
	if(iSrcId == -1)	
		return vector<int>();
	else{
		while(m_srcTrans.size() <= iSrcId)
			m_srcTrans.push_back(vector<int>());
		return m_srcTrans[iSrcId];
	}
}

vector<int> JointDictionary::TgtWordDictEntries(string sTgtWord){
	return TgtWordDictEntries(m_tgtVocab[sTgtWord]);
}

vector<int> JointDictionary::TgtWordDictEntries(int iTgtId){
	if(iTgtId == -1)
		return vector<int>();
	else{
		while(m_tgtTrans.size() <= iTgtId)
			m_tgtTrans.push_back(vector<int>());
		return m_tgtTrans[iTgtId];
	}
}

/*
Pair<int,double> *JointDictionary::SrcWordTranslations(int iSrcId){
	if(iSrcId == -1)
		return NULL;
	
	vector<int> vWordTrans = SrcWordDictEntries(iSrcId);
	Pair<int,double> *pStart=NULL;
	for(int i=0;i<vWordTrans.size();i++){
		int iEntryId = vWordTrans[i];
		Pair<int,double> *pTemp = new Pair<int,double>(m_vItems[iEntryId].Second,m_vItems[iEntryId].Third);
		pTemp->Next = pStart;
		pStart = pTemp;
	}
	return pStart;
}

Pair<int,double> *JointDictionary::TgtWordTranslations(int iTgtId){
	if(iTgtId == -1)
		return NULL;

	vector<int> vWordTrans = TgtWordDictEntries(iTgtId);
	Pair<int,double> *pStart=NULL;
	for(int i=0;i<vWordTrans.size();i++){
		int iEntryId = vWordTrans[i];
		Pair<int,double> *pTemp = new Pair<int,double>(m_vItems[iEntryId].First,m_vItems[iEntryId].Third);
		pTemp->Next = pStart;
		pStart = pTemp;
	}
	return pStart;
}
*/

Pair<string,double> *JointDictionary::SrcWordTranslations(string sSrcWord, bool bCondProb){
	// if bCondProb is set, then this will return conditional probabilities instead of joint probabilities
	// if there is no restriction, on the target language vocab then the probabilities will also be normalized
	int iSrcId = m_srcVocab[sSrcWord];
	if(iSrcId == -1)
		return NULL;
	
	vector<int> vWordTrans = SrcWordDictEntries(iSrcId);
	double dUnigramProb = 0, dNormConst = 0;
	if(bCondProb){
		dUnigramProb = m_srcVocab.Prob(iSrcId);
		if(dUnigramProb <= 0){
			cerr << "Oops .. the unigram probability of src word[" << iSrcId << "]="<< sSrcWord << " became " << dUnigramProb << " and hence is not valid "<< endl;
			exit(1234);
		}
	}
	Pair<string,double> *pStart=NULL;
	for(int i=0;i<vWordTrans.size();i++){
		int iEntryId = vWordTrans[i];
		int iTgtId = m_vItems[iEntryId].Second;
		Pair<string,double> *pTemp = new Pair<string,double>(m_tgtVocab[iTgtId],m_vItems[iEntryId].Third);
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

Pair<string,double> *JointDictionary::TgtWordTranslations(string sTgtWord, bool bCondProb){
	int iTgtId = m_tgtVocab[sTgtWord];
	if(iTgtId == -1)
		return NULL;

	vector<int> vWordTrans = TgtWordDictEntries(iTgtId);
	double dUnigramProb = 0, dNormConst = 0;
	if(bCondProb){
		dUnigramProb = m_tgtVocab.Prob(iTgtId);
		if(dUnigramProb <= 0){
			cerr << "Oops .. the unigram probability of tgt word[" << iTgtId << "]="<< sTgtWord << " became " << dUnigramProb << " and hence is not valid "<< endl;
			exit(1234);
		}
	}
	Pair<string,double> *pStart=NULL;
	for(int i=0;i<vWordTrans.size();i++){
		int iEntryId = vWordTrans[i];
		int iSrcId = m_vItems[iEntryId].First;
		Pair<string,double> *pTemp = new Pair<string,double>(m_srcVocab[iSrcId],m_vItems[iEntryId].Third);
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

bool JointDictionary::TryGetValue(int iSrcId, int iTgtId, double &dProb){
	vector<int> srcWordTrans = SrcWordDictEntries(iSrcId), tgtWordTrans = TgtWordDictEntries(iTgtId);
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

bool JointDictionary::TryGetValue(string sSrcWord, string sTgtWord, double &dProb){
	int iSrcId = m_srcVocab[sSrcWord], iTgtId = m_tgtVocab[sTgtWord];
	if(iSrcId == -1 || iTgtId == -1)
		return false;
	return TryGetValue(iSrcId, iTgtId, dProb);
}

// Specifically needed for some programs
Pair<string,double> *JointDictionary::SrcWordMIs(string sSrcWord ){
	// if bCondProb is set, then this will return conditional probabilities instead of joint probabilities
	// if there is no restriction, on the target language vocab then the probabilities will also be normalized
	int iSrcId = m_srcVocab[sSrcWord];
	if(iSrcId == -1)
		return NULL;
	
	vector<int> vWordTrans = SrcWordDictEntries(iSrcId);
	double dUnigramProb = 0 ;
	dUnigramProb = m_srcVocab.Prob(iSrcId);
	if(dUnigramProb <= 0){
		cerr << "Oops .. the unigram probability of src word[" << iSrcId << "]="<< sSrcWord << " became " << dUnigramProb << " and hence is not valid "<< endl;
		exit(1234);
	}
	Pair<string,double> *pStart=NULL;
	for(int i=0;i<vWordTrans.size();i++){
		int iEntryId = vWordTrans[i];
		int iTgtId = m_vItems[iEntryId].Second;
		Pair<string,double> *pTemp = new Pair<string,double>(m_tgtVocab[iTgtId],m_vItems[iEntryId].Third);

		double dTgtUnigramProb = m_tgtVocab.Prob(iTgtId);
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

Pair<string,double> *JointDictionary::TgtWordMIs(string sTgtWord ){
	int iTgtId = m_tgtVocab[sTgtWord];
	if(iTgtId == -1)
		return NULL;

	vector<int> vWordTrans = TgtWordDictEntries(iTgtId);
	double dUnigramProb = 0 ;
	dUnigramProb = m_tgtVocab.Prob(iTgtId);
	if(dUnigramProb <= 0){
		cerr << "Oops .. the unigram probability of tgt word[" << iTgtId << "]="<< sTgtWord << " became " << dUnigramProb << " and hence is not valid "<< endl;
		exit(1234);
	}

	Pair<string,double> *pStart=NULL;
	for(int i=0;i<vWordTrans.size();i++){
		int iEntryId = vWordTrans[i];
		int iSrcId = m_vItems[iEntryId].First;
		Pair<string,double> *pTemp = new Pair<string,double>(m_srcVocab[iSrcId],m_vItems[iEntryId].Third);

		double dSrcUnigramProb = m_srcVocab.Prob(iSrcId);
		if(dSrcUnigramProb <= 0){
			cerr << "Oops .. the unigram probability of src word[" << iSrcId << "]="<< pTemp->First << " became " << dSrcUnigramProb << " and hence is not valid "<< endl;
			exit(1234);
		}

		pTemp->Second = ((double)pTemp->Second / (double) (dUnigramProb * dSrcUnigramProb));

		pTemp->Next = pStart;
		pStart = pTemp;
	}
	return pStart;
}

#endif

