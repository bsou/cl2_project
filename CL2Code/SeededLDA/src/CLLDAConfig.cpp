#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <fstream>

using namespace std;

#ifndef CLLDA_CONFIG_H_ 
#define CLLDA_CONFIG_H_

class CLLDAConfig{

	void init();

	bool m_bDefaultAlpha;

	public:

	string m_sSrcWordIndicesPath;
	string m_sTgtWordIndicesPath;
	string m_sSrcDocIndicesPath;
	string m_sTgtDocIndicesPath;
	string m_sSrcWordsPath;
	string m_sTgtWordsPath;
	string m_sSrcWordTestIndicesPath;
	string m_sTgtWordTestIndicesPath;
	string m_sSrcDocTestIndicesPath;
	string m_sTgtDocTestIndicesPath;
	string m_sSrcFoldInWordIndicesPath;
	string m_sTgtFoldInWordIndicesPath;
	string m_sSrcFoldInDocIndicesPath;
	string m_sTgtFoldInDocIndicesPath;
	bool m_bFoundTestData;

	int m_iNoIterations;
	int m_iNoTopics;
	double m_dAlpha;
	double m_dBeta;
	double m_dDelta;

	CLLDAConfig();
	CLLDAConfig(string sFilePath);
	void LoadConfig(string sFilePath);
	void Parse(string s);
	void PrintConfig();
};

void CLLDAConfig::init(){
	m_sSrcWordIndicesPath = string();
	m_sTgtWordIndicesPath = string();
	m_sSrcDocIndicesPath = string();
	m_sTgtDocIndicesPath = string();
	m_sSrcWordsPath = string();
	m_sTgtWordsPath = string();
	m_sSrcWordTestIndicesPath = string();
	m_sTgtWordTestIndicesPath = string();
	m_sSrcDocTestIndicesPath = string();
	m_sTgtDocTestIndicesPath = string();

	m_bFoundTestData = false;

	m_iNoIterations = 50;
	m_iNoTopics = 10;
	m_dAlpha = (double)50/m_iNoTopics;
	m_bDefaultAlpha = true;
	m_dBeta = 0.01;
	m_dDelta = 1.0;
}

CLLDAConfig::CLLDAConfig(){
	init();
}

CLLDAConfig::CLLDAConfig(string sFilePath){
	init();
	LoadConfig(sFilePath);
}

void CLLDAConfig::LoadConfig(string sFilePath){
	if(! sFilePath.empty()){
		ifstream fin(sFilePath.c_str(),ios_base::in);
		if(fin.fail())
			cerr << "Unable to open file " << sFilePath << " to load configuration" << endl;	
		else{
			while(! fin.eof()){
				string s;
				getline(fin,s);

				if(s.empty())
					continue;

				int iPos = s.find("//");
				if(iPos != string::npos)
					s = s.substr(0,iPos);
				Parse(s);
			}
		}
	}
}

void CLLDAConfig::Parse(string s){
	
	istringstream istream;
	if(s.empty())
		return;
	
	string sFir, sSec;
	istream.str(s);
	istream >> sFir >> sSec ;
	if(sSec.empty())
		return;
	const char *cFirstPart = sFir.c_str(), *cSecondPart = sSec.c_str();

	if(strcmp(cFirstPart,"sSrcWordIndices") == 0){
		m_sSrcWordIndicesPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sTgtWordIndices") == 0){
		m_sTgtWordIndicesPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sSrcDocIndices") == 0){
		m_sSrcDocIndicesPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sTgtDocIndices") == 0){
		m_sTgtDocIndicesPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sSrcFoldInWordIndices") == 0){
		m_sSrcFoldInWordIndicesPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sTgtFoldInWordIndices") == 0){
		m_sTgtFoldInWordIndicesPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sSrcFoldInDocIndices") == 0){
		m_sSrcFoldInDocIndicesPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sTgtFoldInDocIndices") == 0){
		m_sTgtFoldInDocIndicesPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sSrcWords") == 0){
		m_sSrcWordsPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sTgtWords") == 0){
		m_sTgtWordsPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sSrcWordTestIndices") == 0){
		m_sSrcWordTestIndicesPath = string(cSecondPart);
		m_bFoundTestData = true;
	}else if(strcmp(cFirstPart,"sTgtWordTestIndices") == 0){
		m_sTgtWordTestIndicesPath = string(cSecondPart);
		m_bFoundTestData = true;
	}else if(strcmp(cFirstPart,"sSrcDocTestIndices") == 0){
		m_sSrcDocTestIndicesPath = string(cSecondPart);
		m_bFoundTestData = true;
	}else if(strcmp(cFirstPart,"sTgtDocTestIndices") == 0){
		m_sTgtDocTestIndicesPath = string(cSecondPart);
		m_bFoundTestData = true;
	}else if(strcmp(cFirstPart,"iNoIterations") == 0){
		sscanf(s.c_str(),"%s %d",cFirstPart,&m_iNoIterations);
	}else if(strcmp(cFirstPart,"iNoTopics") == 0){
		sscanf(s.c_str(),"%s %d",cFirstPart,&m_iNoTopics);
		if(m_bDefaultAlpha){
			m_dAlpha = (double)50/m_iNoTopics;
			m_bDefaultAlpha = false;
		}
	}else if(strcmp(cFirstPart,"dAlpha") == 0){
		sscanf(s.c_str(),"%s %lf",cFirstPart,&m_dAlpha);
	}else if(strcmp(cFirstPart,"dBeta") == 0){
		sscanf(s.c_str(),"%s %lf",cFirstPart,&m_dBeta);
	}else if(strcmp(cFirstPart,"dDelta") == 0)
		sscanf(s.c_str(),"%s %lf", cFirstPart, &m_dDelta);
}

void CLLDAConfig::PrintConfig(){
	printf("sSrcWordIndices: %s \n\tsTgtWordIndices: %s\n",m_sSrcWordIndicesPath.c_str(),m_sTgtWordIndicesPath.c_str());
	printf("sSrcDocIndices: %s \n\tsTgtDocIndices: %s\n",m_sSrcDocIndicesPath.c_str(),m_sTgtDocIndicesPath.c_str());
	printf("sSrcWords: %s \n\tsTgtWords: %s\n",m_sSrcWordsPath.c_str(),m_sTgtWordsPath.c_str());
	if(m_bFoundTestData){
		printf("sSrcWordTestIndices: %s\n\tsTgtWordTestIndices: %s\n", m_sSrcWordTestIndicesPath.c_str(), m_sTgtWordTestIndicesPath.c_str());
		printf("sSrcDocTestIndices: %s\n\tsTgtDocTestIndices: %s\n", m_sSrcDocTestIndicesPath.c_str(), m_sTgtDocTestIndicesPath.c_str());
	}
	printf("iNoTopics: %d in iNoIterations %d\n",m_iNoTopics,m_iNoIterations);
	printf("dAlpha: %lf dBeta: %lf dDelta:%lf\n",m_dAlpha,m_dBeta,m_dDelta);
}

#endif

#ifndef AugCLLDA_CONFIG_H_ 
#define AugCLLDA_CONFIG_H_

class AugCLLDAConfig : public CLLDAConfig{

	void init();

	public:

	double m_dMu0;
	double m_dTau0;

	AugCLLDAConfig() : CLLDAConfig() {};
	AugCLLDAConfig(string sFilePath);
	void LoadConfig(string sFilePath);
	void Parse(string s);
	void PrintConfig();
};

AugCLLDAConfig::AugCLLDAConfig(string sFilePath) {
	LoadConfig(sFilePath);
}

void AugCLLDAConfig::LoadConfig(string sFilePath){
	CLLDAConfig::LoadConfig(sFilePath);

	if(! sFilePath.empty()){
		ifstream fin(sFilePath.c_str(),ios_base::in);
		if(fin.fail())
			cerr << "Unable to open file " << sFilePath << " to load configuration" << endl;	
		else{
			while(! fin.eof()){
				string s;
				getline(fin,s);

				if(s.empty())
					continue;

				int iPos = s.find("//");
				if(iPos != string::npos)
					s = s.substr(0,iPos);
				Parse(s);
			}
		}
	}
}

void AugCLLDAConfig::Parse(string s){
	
	if(s.empty())
		return;
	
	char *cFirstPart = new char[s.length()],*cSecondPart=new char[s.length()];
	sscanf(s.c_str(),"%s %s",cFirstPart,cSecondPart);

	if(strcmp(cFirstPart,"dMu0") == 0){
		sscanf(s.c_str(),"%s %lf",cFirstPart,&m_dMu0);
	}else if(strcmp(cFirstPart,"dTau0") == 0){
		sscanf(s.c_str(),"%s %lf",cFirstPart,&m_dTau0);
	}
}

void AugCLLDAConfig::PrintConfig(){
	CLLDAConfig::PrintConfig();
	printf("dMu0: %lf dTau0: %lf\n",m_dMu0,m_dTau0);
}

#endif

#ifndef GuidedLDA_CONFIG_H_ 
#define GuidedLDA_CONFIG_H_

class SeededLDAConfig : public CLLDAConfig{

	void init();

	public:

	string sSeedTopicalWordsPath;
	double dMu, dTau;
	bool bAllowSeedWords;

	SeededLDAConfig() : CLLDAConfig() {};
	SeededLDAConfig(string sFilePath);
	void LoadConfig(string sFilePath);
	void Parse(string s);
	void PrintConfig();
};

SeededLDAConfig::SeededLDAConfig(string sFilePath) {
	bAllowSeedWords = false;
	LoadConfig(sFilePath);
}

void SeededLDAConfig::LoadConfig(string sFilePath){
	CLLDAConfig::LoadConfig(sFilePath);

	if(! sFilePath.empty()){
		ifstream fin(sFilePath.c_str(),ios_base::in);
		if(fin.fail())
			cerr << "Unable to open file " << sFilePath << " to load configuration" << endl;	
		else{
			while(! fin.eof()){
				string s;
				getline(fin,s);

				if(s.empty())
					continue;

				int iPos = s.find("//");
				if(iPos != string::npos)
					s = s.substr(0,iPos);
				Parse(s);
			}
		}
	}
}

void SeededLDAConfig::Parse(string s){
	
	if(s.empty())
		return;
	
	istringstream istream;
	string sFir, sSec;
	istream.str(s);
	istream >> sFir >> sSec ;
	if(sSec.empty())
		return;

	if(sFir == "sSeedTopicalWordsPath"){
	//if(strcmp(sFir,"sSeedTopicWordsPath") == 0){
		sSeedTopicalWordsPath = sSec;
	}else if(sFir == "dMu0"){
		sscanf(sSec.c_str(), "%lf", &dMu);
	}else if(sFir == "dTau0"){
		sscanf(sSec.c_str(), "%lf", &dTau);
	}else if(sFir == "bAllowSeedWords"){
		int iTemp;
		sscanf(sSec.c_str(), "%d", &iTemp);
		if(iTemp)
			bAllowSeedWords = true;
		else
			bAllowSeedWords = false;
	}
}

void SeededLDAConfig::PrintConfig(){
	CLLDAConfig::PrintConfig();
	printf("sSeedTopicalWordsPath: %s\n",sSeedTopicalWordsPath.c_str());
	printf("dMu: %lf\t dTau: %lf \n", dMu, dTau);
	printf("AllowSeedWords: %d\n", bAllowSeedWords);
}

#endif
