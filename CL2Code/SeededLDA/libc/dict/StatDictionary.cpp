#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>

#include "text/StopWordList.cpp"
#include "utils/Tripple.cpp"

using namespace std;

#ifndef STAT_DICT_H_ 
#define STAT_DICT_H_

class StatDictionary{
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
	double m_dictThreshold;

	bool CanInclude(int,int,double);
	void AddEntry(int,int,double);
	void PrintEntry(int iEntryId);

	public:
	void init();
	StatDictionary();
	StatDictionary(string sSrcVcbPath, string sTgtVcbPath, string sSrc2TgtDictFilePath, string sTgt2SrcDictFilePath);
	void LoadDictionary();
	void LoadDictionary(double dThreshold);
	void LoadDictionary(StopWordList swlSrc,StopWordList swlTgt);
	void LoadDictionary(StopWordList swlSrc,StopWordList swlTgt, double dThreshold);
};

void StatDictionary::init(){
}

StatDictionary::StatDictionary(){
	init();
}

StatDictionary::StatDictionary(string sSrcVcbPath, string sTgtVcbPath, string sSrc2TgtDictFilePath, string sTgt2SrcDictFilePath){
	m_srcVcbPath = sSrcVcbPath;
	m_tgtVcbPath = sTgtVcbPath;
	m_sSrc2TgtDictPath = sSrc2TgtDictFilePath;
	m_sTgt2SrcDictPath = sTgt2SrcDictFilePath;
}

void StatDictionary::LoadDictionary(double dThreshold){
	m_dictThreshold = dThreshold;

	m_srcVocab.LoadVocab(m_srcVcbPath);
	cout << "SrcVcb: " << m_srcVocab.size() << endl; 
	m_tgtVocab.LoadVocab(m_tgtVcbPath);
	cout << "TgtVcb: " << m_tgtVocab.size() << endl;

	if(m_sSrc2TgtDictPath.empty()){
		ifstream fin(m_sSrc2TgtDictPath.c_str(),ios_base::in);
		while(! fin.eof()){
			string s;
			getline(fin,s);
			if(s.empty())
				continue;

			int iSrcId,iTgtId;
			double dProb;
			sscanf(s.c_str(),"%d %d %lf",&iSrcId,&iTgtId,&dProb);
			if(CanInclude(iSrcId,iTgtId,dProb))
				AddEntry(iSrcId,iTgtId,dProb);
		}
		fin.close();
	}
}

bool StatDictionary::CanInclude(int iSrcId,int iTgtId,double dProb){
	
	if(dProb < m_dictThreshold)
		return false;

	return true;
}

void StatDictionary::AddEntry(int iSrcId,int iTgtId,double dProb){
	Tripple<int,int,double> tp(iSrcId,iTgtId,dProb);
	int iItemId = m_vItems.size();
	m_vItems.push_back(tp);

	m_srcTrans[iSrcId].push_back(iItemId);
	m_tgtTrans[iTgtId].push_back(iItemId);
}

void StatDictionary::PrintEntry(int iEntryId){
	if(iEntryId >= m_vItems.size())
		cerr << "Entry id " << iEntryId << " exceeds the size" << endl;

	Tripple<int,int,double> tp = m_vItems[iEntryId];
	cout << m_srcVocab[tp.First] << "\t" << m_srcVocab[tp.Second] << "\t" << tp.Third << endl;
}

#endif
