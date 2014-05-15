#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using namespace std;

#ifndef DICT_CONFIG_H_ 
#define DICT_CONFIG_H_

class DictConfig{

	void init();

	public:

	string m_sSrcVcbPath;
	string m_sTgtVcbPath;
	string m_sSrc2TgtDictPath;
	string m_sTgt2SrcDictPath;

	int m_iDebug;
	double m_dThreshold;
	string m_sSrcSwlPath; 
	string m_sTgtSwlPath;

	DictConfig();
	DictConfig(string sFilePath);
	void LoadConfig(string sFilePath);
	void Parse(string s);
	void PrintConfig();
};

void DictConfig::init(){
	m_iDebug = 0;
	m_dThreshold = 0;
	m_sSrcSwlPath = string();
	m_sTgtSwlPath = string();
}

DictConfig::DictConfig(){
	init();
}

DictConfig::DictConfig(string sFilePath){
	init();
	LoadConfig(sFilePath);
}

void DictConfig::LoadConfig(string sFilePath){
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

void DictConfig::Parse(string s){
	
	if(s.empty())
		return;
	
	char *cFirstPart = new char[s.length()],*cSecondPart=new char[s.length()];
	sscanf(s.c_str(),"%s %s",cFirstPart,cSecondPart);

	if(strcmp(cFirstPart,"sSrcVcbPath") == 0){
		m_sSrcVcbPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sTgtVcbPath") == 0){
		m_sTgtVcbPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sSrc2TgtDictPath") == 0){
		m_sSrc2TgtDictPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sTgt2SrcDictPath") == 0){
		m_sTgt2SrcDictPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"dThreshold") == 0){
		sscanf(s.c_str(),"%s %lf",cFirstPart,&m_dThreshold);
	}else if(strcmp(cFirstPart,"sSrcSwlPath") == 0){
		m_sSrcSwlPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sTgtSwlPath") == 0){
		m_sTgtSwlPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"DEBUG") == 0){
		sscanf(s.c_str(),"%s %d",cFirstPart,&m_iDebug);
	}
}

void DictConfig::PrintConfig(){
	printf("SrcVcb: %s TgtVcb: %s\n",m_sSrcVcbPath.c_str(),m_sTgtVcbPath.c_str());
	printf("Src2TgtDict: %s\n",m_sSrc2TgtDictPath.c_str());
	printf("Tgt2SrcDict: %s\n",m_sTgt2SrcDictPath.c_str());
	printf("Dictionary Threshold: %e\n",m_dThreshold);
	printf("SrcSwlPath: %s\n",m_sSrcSwlPath.c_str());
	printf("TgtSwlPath: %s\n",m_sTgtSwlPath.c_str());
}

#endif

#ifndef MONODICT_CONFIG_H_ 
#define MONODICT_CONFIG_H_

class MonoDictConfig{

	void init();

	public:

	string m_sVcbPath;
	string m_sDictPath;

	int m_iDebug;
	double m_dThreshold;
	string m_sSwlPath; 

	MonoDictConfig();
	MonoDictConfig(string sFilePath);
	void LoadConfig(string sFilePath);
	void Parse(string s);
	void PrintConfig();
};

void MonoDictConfig::init(){
	m_iDebug = 0;
	m_dThreshold = 0;
	m_sSwlPath = string();
	m_sVcbPath = string();
	m_sDictPath = string();
}

MonoDictConfig::MonoDictConfig(){
	init();
}

MonoDictConfig::MonoDictConfig(string sFilePath){
	init();
	LoadConfig(sFilePath);
}

void MonoDictConfig::LoadConfig(string sFilePath){
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

void MonoDictConfig::Parse(string s){
	
	if(s.empty())
		return;
	
	char *cFirstPart = new char[s.length()],*cSecondPart=new char[s.length()];
	sscanf(s.c_str(),"%s %s",cFirstPart,cSecondPart);

	if(strcmp(cFirstPart,"sVcbPath") == 0){
		m_sVcbPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"sDictPath") == 0){
		m_sDictPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"dThreshold") == 0){
		sscanf(s.c_str(),"%s %lf",cFirstPart,&m_dThreshold);
	}else if(strcmp(cFirstPart,"sSwlPath") == 0){
		m_sSwlPath = string(cSecondPart);
	}else if(strcmp(cFirstPart,"DEBUG") == 0){
		sscanf(s.c_str(),"%s %d",cFirstPart,&m_iDebug);
	}
}

void MonoDictConfig::PrintConfig(){
	printf("Vcb: %s \n",m_sVcbPath.c_str());
	printf("Dict: %s\n",m_sDictPath.c_str());
	printf("Dictionary Threshold: %lf\n",m_dThreshold);
	printf("SwlPath: %s\n",m_sSwlPath.c_str());
}

#endif
