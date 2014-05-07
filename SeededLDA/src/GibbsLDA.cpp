#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

#include "ext/cokus.cpp"

#include "CLLDAConfig.cpp"
#include "IO_CLLDA.cpp"
#include "utils/utils.cpp"
#include "stat/GammaFunc.cpp"

using namespace std;

class GibbsLDA{

	bool *bSrcLang;
	vector<string> Words;

	int *z , *order, *ztot, *wp, *dp, *docLength;
	int *w, *d, *wTest, *dTest;
	int W, D, T, iNoTokens, iNoTestTokens;
	double dAlpha , dBeta, dKAlpha, dWBeta;
	double *dUnigramProbs;
	int *iUnigramFreq;

	CLLDAConfig ldaConfig;
	int *wFoldIn, *dFoldIn, *zFoldIn, *dpFoldIn, *docLengthFoldIn, *orderFoldIn;
	int iNoFoldInTokens, iFoldInD, iMinFoldInD;

	void Init(){
		bSrcLang = NULL;
		z = NULL; order=NULL; ztot=NULL; wp=NULL; dp=NULL; docLength = NULL;
		w = NULL; d = NULL; wTest = NULL; dTest = NULL;
		W = 0; D = 0; T = 0; iNoTokens = 0; iNoTestTokens = 0;
		SEED = 3; OUTPUT = 2; iNoIter = 0;
		dAlpha = 0; dBeta = 0; dKAlpha = 0; dWBeta = 0; 
		iUnigramFreq = NULL;

		wFoldIn = NULL; dFoldIn = NULL; dpFoldIn = NULL; zFoldIn = NULL; docLengthFoldIn = NULL; orderFoldIn = NULL;
		iFoldInD = -1; iMinFoldInD = (int)INFINITY;

		dTemp = 0;
	}

	public: 
	double dTemp;
	int SEED, OUTPUT, iNoIter;

	GibbsLDA(){ Init(); }
	GibbsLDA(string sConfigPath) {
		Init();
		LoadData(sConfigPath);
	}

	void PreInitialize(int , char *argv[]);
	void LoadData(string sConfigPath);
	void RandomInitialize();
	void RandomOrder();
	void Iterate(bool bOnTestSet=true,int iNoSamples=0, const int *iOrder=NULL, int iNoExtraIter=0);

	double GetTopicDocProb(int iTopic, int iDocId);
	double GetWordTopicProb(int iWordId, int iTopic);
	double GetWordDocProb(int iWordId, int iDocId);

	bool CheckTopicDistribution(int iTopic);
	bool CheckDocDistribution(int iDocId);
	bool CheckConsistency();
	
	double Perplexity(int iNoTokens,const int *w,const int *d);
	void PauseToEvaluate(int iter, bool bOnTestSet=true, int iNoSamples=0, const int *iOrder=NULL);
	void UpdateHyperParams();

	void PrintTopicalWords(int aNoWords=10);
	void PrintTextTopics();
	void TopicStats();
	void PrintDocTopicDist(string);
	void PrintTopicWordDist(int idx=-1);
	void PrintTokenAssignments(int idx=-1); 

	void LoadFoldInData();
	void FoldIn();
	bool ConsistencyFoldInDocs();
	void PauseWhileFolding(int iter);
	void PrintDocTopicDistFoldIn();
	void EntropyOfDistributions();

	void SaveObject(string sFilePath);
	void LoadObject(string sFilePath);
	void SaveTopicAssignments(string sFilePath,int idx=-1);
	void LoadTopicAssignments(string sFilePath);
	void InitializeFrmPrevCounts();
	void PrintBestTopic(string);
	void EvaluateWordAssociation(string sWordAssocPath);
};

void GibbsLDA::SaveTopicAssignments(string sFilePath, int idx){
	fprintf(stderr, "Saving to %s\n", sFilePath.c_str());
	if(sFilePath.empty()){
		fprintf(stderr, "Input non-empty filepath to save object\n");
		exit(1234);
	}

	if(idx != -1){
		std::stringstream ss;
		ss << sFilePath << "." << idx;
		sFilePath = ss.str();
	}

	ofstream meta(sFilePath.c_str(), ios::binary);
	meta.write((const char *)&W, sizeof(int));
	meta.write((const char *)&D, sizeof(int));
	meta.write((const char *)&T, sizeof(int));
	meta.write((const char *)&dAlpha, sizeof(double));
	meta.write((const char *)&dBeta, sizeof(double));

	SaveIntArr(meta, z, iNoTokens);
	meta.close();
	fprintf(stderr, "Saving to %s done\n", sFilePath.c_str());
}

void GibbsLDA::LoadTopicAssignments(string sFilePath){
	fprintf(stderr, "Loading from %s\n", sFilePath.c_str());
	if(sFilePath.empty()){
		fprintf(stderr, "The filepath is empty\n");
		exit(1234);
	}

	ifstream meta(sFilePath.c_str(), ios::binary);
	if(meta.fail()){
		fprintf(stderr, "Failed to open file %s\n", sFilePath.c_str());
		exit(1234);
	}
	meta.read((char *)&W, sizeof(int));
	meta.read((char *)&D, sizeof(int));
	meta.read((char *)&T, sizeof(int));
	meta.read((char *)&dAlpha, sizeof(double));
	meta.read((char *)&dBeta, sizeof(double));

	dKAlpha = (double) T*dAlpha;
	dWBeta = (double) W*dBeta;
	printf("W:%d D:%d T:%d\n", W, D, T);
	printf("Alpha:%e Beta:%e dAlpha:%e dWBeta:%e\n", dAlpha, dBeta, dKAlpha, dWBeta);

	z = LoadIntArr(meta, iNoTokens);
	meta.close();
	fprintf(stderr, "Loading from %s done\n", sFilePath.c_str());
}

void GibbsLDA::InitializeFrmPrevCounts(){
	int i, topic, wi, di, widx, iAllowTill, wioffset, idxoffset;
	for (i=0; i<iNoTokens; i++)
	{
		wi = w[ i ];
		di = d[ i ];

		topic = z[i];
		wp[wi*T+topic]++;
		ztot[topic]++;
		dp[di*T+topic]++;
	}
	if( CheckConsistency() == false){
		fprintf(stderr, "Initial topic assignments are not consistent\n");
		exit(1234);
	}
}

void GibbsLDA::SaveObject(string sFilePath){

	fprintf(stderr, "Saving to %s\n", sFilePath.c_str());
	if(sFilePath.empty()){
		fprintf(stderr, "Input non-empty filepath to save object\n");
		exit(1234);
	}

	ofstream meta(sFilePath.c_str(), ios::binary);
	meta.write((const char *)&W, sizeof(int));
	meta.write((const char *)&D, sizeof(int));
	meta.write((const char *)&T, sizeof(int));
	meta.write((const char *)&dAlpha, sizeof(double));
	meta.write((const char *)&dBeta, sizeof(double));

	SaveIntArr(meta, wp, T*W);
	SaveIntArr(meta, ztot, T);
	SaveIntArr(meta, dp, T*D);
	SaveIntArr(meta, docLength, D);
	meta.close();
	fprintf(stderr, "Saving to %s done\n", sFilePath.c_str());
}

void GibbsLDA::LoadObject(string sFilePath){
	fprintf(stderr, "Loading from %s\n", sFilePath.c_str());
	if(sFilePath.empty()){
		fprintf(stderr, "The filepath is empty\n");
		exit(1234);
	}

	ifstream meta(sFilePath.c_str(), ios::binary);
	if(meta.fail()){
		fprintf(stderr, "Failed to open file %s\n", sFilePath.c_str());
		exit(1234);
	}
	meta.read((char *)&W, sizeof(int));
	meta.read((char *)&D, sizeof(int));
	meta.read((char *)&T, sizeof(int));
	meta.read((char *)&dAlpha, sizeof(double));
	meta.read((char *)&dBeta, sizeof(double));

	dKAlpha = (double) T*dAlpha;
	dWBeta = (double) W*dBeta;
	printf("W:%d D:%d T:%d\n", W, D, T);
	printf("Alpha:%e Beta:%e dAlpha:%e dWBeta:%e\n", dAlpha, dBeta, dKAlpha, dWBeta);

	wp = LoadIntArr(meta, T*W);
	ztot = LoadIntArr(meta, T);
	dp = LoadIntArr(meta, T*D);
	docLength = LoadIntArr(meta, D);
	meta.close();
	fprintf(stderr, "Loading from %s done\n", sFilePath.c_str());

}

void GibbsLDA::PreInitialize(int argc, char *argv[]){
	// Preinitilize certain parameters and doesn't allow them to be overwritten
	// May be useful for repeated experiments
	if(argc > 3){
		//T = atoi(argv[2]);
		SEED = atoi(argv[3]);
		printf("Choose %d as seed\n", SEED);
	}
}

void GibbsLDA::LoadData(string sConfigPath){
	cout << "Reading from " << sConfigPath << endl;
	ldaConfig.LoadConfig(sConfigPath);
	ldaConfig.PrintConfig();

	T = ldaConfig.m_iNoTopics;
	iNoIter = ldaConfig.m_iNoIterations;
	dAlpha = ldaConfig.m_dAlpha;
	dBeta = ldaConfig.m_dBeta;

	int iNoSrcWordIndices=0,iNoTgtWordIndices=0,iNoSrcDocIndices=0,iNoTgtDocIndices=0,iNoSrcWords=0,iNoTgtWords=0;
	int *SrcWordIndices = NULL, *SrcDocIndices = NULL, *TgtWordIndices = NULL, *TgtDocIndices = NULL;
	vector<string> SrcWords, TgtWords;
	if(dTemp != 0){
		fprintf(stderr, "****** Using the preinitialized alpha %lf instead of %lf\n", dTemp, dBeta);
		dBeta = dTemp;
	}

	SrcWordIndices = LoadIndices(ldaConfig.m_sSrcWordIndicesPath,&iNoSrcWordIndices);
	SrcDocIndices = LoadIndices(ldaConfig.m_sSrcDocIndicesPath,&iNoSrcDocIndices);
	SrcWords = LoadWords(ldaConfig.m_sSrcWordsPath);
	if(iNoSrcWordIndices != iNoSrcDocIndices)
		cerr << "Mismatch of Src indices " << endl;
	iNoSrcWords = SrcWords.size();

	TgtWordIndices = LoadIndices(ldaConfig.m_sTgtWordIndicesPath,&iNoTgtWordIndices);
	TgtDocIndices = LoadIndices(ldaConfig.m_sTgtDocIndicesPath,&iNoTgtDocIndices);
	TgtWords = LoadWords(ldaConfig.m_sTgtWordsPath);
	if(iNoTgtWordIndices != iNoTgtDocIndices)
		cerr << "Mismatch of Tgt indices" << endl;
	iNoTgtWords = TgtWords.size();

	int *SrcWordTestIndices = NULL, *TgtWordTestIndices = NULL, *SrcDocTestIndices = NULL, *TgtDocTestIndices = NULL;
    int iNoSrcWordTestTokens=0,iNoTgtWordTestTokens=0,iNoSrcDocTestTokens=0,iNoTgtDocTestTokens=0,iNoTestWordTokens=0,iNoTestDocTokens=0;
    if(! ldaConfig.m_sSrcWordTestIndicesPath.empty()){
        SrcWordTestIndices = LoadIndices(ldaConfig.m_sSrcWordTestIndicesPath,&iNoSrcWordTestTokens);
        SrcDocTestIndices = LoadIndices(ldaConfig.m_sSrcDocTestIndicesPath,&iNoSrcDocTestTokens);
        if(iNoSrcWordTestTokens != iNoSrcDocTestTokens){
            fprintf(stderr, "No src word test tokens (%d) != No src doc test tokens (%d)\n", iNoSrcWordTestTokens, iNoSrcDocTestTokens);
            exit(1234);
        }
    }

    if(! ldaConfig.m_sTgtWordTestIndicesPath.empty()){
        TgtWordTestIndices = LoadIndices(ldaConfig.m_sTgtWordTestIndicesPath,&iNoTgtWordTestTokens);
        TgtDocTestIndices = LoadIndices(ldaConfig.m_sTgtDocTestIndicesPath,&iNoTgtDocTestTokens);
        if(iNoTgtWordTestTokens != iNoTgtDocTestTokens){
            fprintf(stderr, "No tgt word test tokens (%d) != No tgt doc test tokens (%d)\n", iNoTgtWordTestTokens, iNoTgtDocTestTokens);
            exit(1234);
        }
    }

    int iTgtWordOffset = MaxIdx(SrcWordIndices, iNoSrcWordIndices, SrcWordTestIndices, iNoSrcWordTestTokens);
    int iTgtDocOffset = MaxIdx(SrcDocIndices, iNoSrcDocIndices, SrcDocTestIndices, iNoSrcDocTestTokens);
	fprintf(stderr, "iTgtWordOffset:%d & iTgtDocOffset:%d\n", iTgtWordOffset, iTgtDocOffset);

    int iNoWordTokens = 0, iNoDocTokens = 0 , iNoWords = 0;
    w = ExtendIndices(SrcWordIndices,iNoSrcWordIndices,TgtWordIndices,iNoTgtWordIndices,iTgtWordOffset, &iNoWordTokens);
    d = ExtendIndices(SrcDocIndices,iNoSrcDocIndices,TgtDocIndices,iNoTgtDocIndices,iTgtDocOffset, &iNoDocTokens);
    wTest = ExtendIndices(SrcWordTestIndices, iNoSrcWordTestTokens, TgtWordTestIndices, iNoTgtWordTestTokens, iTgtWordOffset, &iNoTestWordTokens);
    dTest = ExtendIndices(SrcDocTestIndices, iNoSrcDocTestTokens, TgtDocTestIndices, iNoTgtDocTestTokens, iTgtDocOffset, &iNoTestDocTokens);
    if(iNoWordTokens != iNoDocTokens)
        cerr << "No. of Word tokens didn't match with No. of Doc tokens" << endl;
	iNoTokens = iNoWordTokens;
    if(iNoTestWordTokens != iNoTestDocTokens){
        fprintf(stderr, "No test word tokens didn't match with no test doc tokens\n");
        exit(1234);
    }
	iNoTestTokens = iNoTestWordTokens;

	bool *bSrcLang = new bool[SrcWords.size() + TgtWords.size()];
	int idx=0;
	for(int i=0;i<SrcWords.size();i++)
		bSrcLang[idx++] = true;
	for(int i=0;i<TgtWords.size();i++)
		bSrcLang[idx++] = false;

	Words = ExtendWords(SrcWords,TgtWords);

	if(SrcWordIndices != NULL)
		free(SrcWordIndices);
	if(SrcDocIndices != NULL)
		free(SrcDocIndices);
	if(TgtWordIndices != NULL)
		free(TgtWordIndices);
	if(TgtDocIndices != NULL)
		free(TgtDocIndices);
	if(SrcWordTestIndices != NULL)
		free(SrcWordTestIndices);
	if(SrcDocTestIndices != NULL)
		free(SrcDocTestIndices);
	if(TgtWordTestIndices != NULL)
		free(TgtWordTestIndices);
	if(TgtDocTestIndices != NULL)
		free(TgtDocTestIndices);

	// seeding
	seedMT( 1 + SEED * 2 ); // seeding only works on uneven numbers

	/* allocate memory */
	z  = (int *) calloc( iNoTokens , sizeof( int ));
	order  = (int *) calloc( iNoTokens , sizeof( int ));  
	ztot  = (int *) calloc( T, sizeof( int ));

	// copy over the word and document indices into internal format
	for (int i=0; i<iNoWordTokens; i++) {
		w[ i ] = (int) w[ i ] - 1;
		d[ i ] = (int) d[ i ] - 1;
	}
	for(int i=0;i<iNoTestWordTokens;i++){
        wTest[i] = wTest[i]-1;
        dTest[i] = dTest[i]-1;
    }

	W = 0;
	D = 0;
	for (int i=0; i<iNoWordTokens; i++) {
		if (w[ i ] > W) W = w[ i ];
		if (d[ i ] > D) D = d[ i ];
	}
	for(int i=0;i<iNoTestWordTokens;i++){
        if(wTest[i] > W) W = wTest[i];
        if(dTest[i] > D) D = dTest[i];
    }
	W = W + 1;
	D = D + 1;

	if(W != Words.size()){
		fprintf(stderr, "Mismatch in the number of words from indices (%d) and actual words (%d)\n", W, Words.size());
		fprintf(stderr, "Be aware of this error\n");
		W = Words.size();
	}
	iUnigramFreq = UnigramFreqs(w, iNoTokens, W);
	dUnigramProbs = UnigramProbs(w, iNoTokens, W);
	docLength = DocLengths(d,D,iNoTokens);
	dWBeta = (double) (W*dBeta);
	dKAlpha = (double) (T*dAlpha);

	wp  = (int *) calloc( T*W , sizeof( int ));
	dp  = (int *) calloc( T*D , sizeof( int ));

	if (OUTPUT==2) {
		printf( "Running LDA Gibbs Sampler Version 1.0\n" );
		printf( "Arguments:\n" );
		printf( "\tNumber of words      W = %d\n"    , W );
		printf( "\tNumber of docs       D = %d\n"    , D );
		printf( "\tNumber of topics     T = %d\n"    , T);
		printf( "\tNumber of iterations N = %d\n"    , iNoIter );
		printf( "\tHyperparameter   ALPHA = %4.4f\n" , dAlpha );
		printf( "\tHyperparameter    BETA = %4.4f\n" , dBeta );
		printf( "\tSeed number            = %d\n"    , SEED );
		printf( "\tNumber of tokens       = %d\n"    , iNoTokens);
		//printf( "Checking: sizeof(int)=%d sizeof(long)=%d sizeof(double)=%d\n" , sizeof(int) , sizeof(long) , sizeof(double));
	}
}

void GibbsLDA::RandomInitialize(){
	int i, topic, wi, di, widx;
	for (i=0; i<iNoTokens; i++)
	{
		wi = w[ i ];
		di = d[ i ];
		// pick a random topic 0..T-1
		topic = (int) ( (double) randomMT() * (double) T / (double) (4294967296.0 + 1.0) );
		z[ i ] = topic; // assign this word token to this topic
		wp[ wi*T + topic ]++; // increment wp count matrix
		dp[ di*T + topic ]++; // increment dp count matrix
		ztot[ topic ]++; // increment ztot matrix
	}
}

void GibbsLDA::RandomOrder(){
    printf( "Determining random order update sequence\n" );
    int rp, temp;
    for (int i=0; i<iNoTokens; i++) order[i]=i; // fill with increasing series
    for (int i=0; i<(iNoTokens-1); i++) {
        // pick a random integer between i and nw
        rp = i + (int) ((double) (iNoTokens-i) * (double) randomMT() / (double) (4294967296.0 + 1.0));

        // switch contents on position i and position rp
        temp = order[rp];
        order[rp]=order[i];
        order[i]=temp;
    }
}

double GibbsLDA::GetWordTopicProb(int iWordId, int iTopic){
	return (((double) wp[iWordId*T+iTopic] + dBeta)/((double)ztot[iTopic]+dWBeta));
}

double GibbsLDA::GetTopicDocProb(int iTopic, int iDocId){
	return (((double) dp[iDocId*T+iTopic] + dAlpha)/((double)docLength[iDocId]+dKAlpha));
}

double GibbsLDA::GetWordDocProb(int iWordId, int iDocId){
    double dWordDocProb = 0;
    for(int j=0;j<T;j++){
        double dWordTopicProb = GetWordTopicProb(iWordId,j);
        double dTopicDocProb = GetTopicDocProb(j,iDocId);
        dWordDocProb += (dWordTopicProb * dTopicDocProb);
    }
    return dWordDocProb;
}

bool GibbsLDA::CheckDocDistribution(int iDocId){
    int dioffset = iDocId*T, iTemp=0;
	double dTempProb = 0;
    for(int i=dioffset;i<dioffset+T;i++){
        iTemp += dp[i];
    }
	for(int k=0;k<T;k++)
		dTempProb += GetTopicDocProb(k, iDocId);
    if(iTemp != docLength[iDocId]){
        cerr << "Tring Tring .... the total number of class assignments " << iTemp << " didn't match with doc length " << docLength[iDocId] << " for doc " << iDocId << endl;
        return false;
    }
	if( !isfinite(dTempProb) || fabs(dTempProb-1) > 1e-8){
		fprintf(stderr, "Strange ... Though integers counted, sum p(.|d=%d) = %e didn't match\n", iDocId, dTempProb);
		return false;
	}
    return true;
}

bool GibbsLDA::CheckTopicDistribution(int iTopic){
	int iTemp = 0;
	double dTempProb = 0;
	for(int w=0;w<W;w++){
		iTemp += wp[w*T+iTopic];
		dTempProb += GetWordTopicProb(w,iTopic);
	}
	if(iTemp != ztot[iTopic]){
		fprintf(stderr, "Ooops ... No dict entries assigned to p(.|k=%d) didn't match\n", iTopic);
		return false;
	}
	if( ! isfinite(dTempProb) || fabs(dTempProb-1) > 1e-8){
		fprintf(stderr, "Strange ... Though integers counted, sum p(.|k=%d) = %e didn't match\n", iTopic, dTempProb);
		return false;
	}
    return true;
}

void GibbsLDA::EntropyOfDistributions(){
	double dAvgEntropy = 0, dTmp;
	for(int di=0;di<D;di++){
		for(int k=0;k<T;k++){
			dTmp = GetTopicDocProb(k, di);
			dAvgEntropy += dTmp * log(dTmp);
		}
	}
	dAvgEntropy *= -1;
	dAvgEntropy /= (double)D;
	printf(" Entropy (k|d): %lf ", dAvgEntropy);
	dAvgEntropy = 0;
	for(int k=0;k<T;k++){
		for(int wi=0;wi<W;wi++){
			dTmp = GetWordTopicProb(wi, k);
			dAvgEntropy += dTmp * log(dTmp);
		}
	}
	dAvgEntropy *= -1;
	dAvgEntropy /= (double)T;
	printf("(w|k): %lf", dAvgEntropy);
}

bool GibbsLDA::CheckConsistency(){
    bool bReturn = true;

    for(int iDocId=0;iDocId < D; iDocId++){
        if( CheckDocDistribution(iDocId) == false)
            bReturn = false;
    }
    for(int topic=0;topic<T;topic++){
        if( CheckTopicDistribution(topic) == false)
            bReturn = false;
    }

	/*
    for(int iDocId=0;iDocId < D; iDocId++){
		double dTotProb = 0;
		for(int iWordId=0;iWordId<W;iWordId++)
			dTotProb += GetWordDocProb(iWordId, iDocId);
        if(fabs(dTotProb-1) > 1e-8){
            cerr << "Ooops ... Sum of word probabilities in doc " << iDocId << " is " << dTotProb << " != 1" << endl;
            return false;
        }
    }
	*/
    return bReturn;
}

double GibbsLDA::Perplexity(int n,const int *w,const int *d){
	int i,j,wi,di,wioffset,dioffset;
	double dWordDocProb,dprob,dPerp;
	int iSummedOver = 0;
	dPerp = 0;
	for(i=0;i<n;i++){
		wi = w[i];
		di = d[i];
		//if(dUnigramProbs[wi] > 4)
		//	continue;

		dWordDocProb = GetWordDocProb(wi, di);
		dPerp += log(dWordDocProb);
		iSummedOver+=1;
	}
	dPerp *= -1;
	dPerp /= (double)iSummedOver;
	return exp(dPerp);
}

void GibbsLDA::Iterate(bool bOnTestSet, int iNoSamples, const int *iOrder, int iNoExtraIter){
	int wi,di,i,ii,j,topic,iter, wioffset, dioffset, widx, iAllowTill;
	double totprob, r, max, dTemp;
	double *probs = (double *)calloc(T, sizeof(double));

	if( CheckConsistency() == false){
		fprintf(stderr, "Initial topic assignments are not consistent\n");
		exit(1234);
	}
	PauseToEvaluate(0, bOnTestSet, iNoSamples, iOrder);
	int iRunFor = (iNoExtraIter == 0 ? iNoIter : iNoExtraIter);
	for (iter=1; iter<=iRunFor; iter++) {
		//dTemp = sqrt(iter);
		for (ii = 0; ii < iNoTokens; ii++) {
			i = order[ ii ]; // current word token to assess

			wi  = w[i]; // current word index
			di  = d[i]; // current document index  
			topic = z[i]; // current topic assignment to word token
			ztot[topic]--;  // substract this from counts

			wioffset = wi*T;
			dioffset = di*T;

			wp[wioffset+topic]--;
			dp[dioffset+topic]--;

			//printf( "(1) Working on ii=%d i=%d wi=%d di=%d topic=%d wp=%d dp=%d\n" , ii , i , wi , di , topic , wp[wi+topic*W] , dp[wi+topic*D] );

			totprob = (double) 0;
			for (j = 0; j < T; j++) {
				probs[j] = ((double) wp[ wioffset+j ] + (double) dBeta)/( (double) ztot[j]+ (double) dWBeta)*( (double) dp[ dioffset+ j ] + (double) dAlpha);
				//probs[j] = pow(probs[j], dTemp);
				totprob += probs[j];
			}

			// sample a topic from the distribution
			r = (double) totprob * (double) randomMT() / (double) 4294967296.0;
			max = probs[0];
			topic = 0;
			while (r>max) {
				topic++;
				max += probs[topic];
			}

			if(topic < 0 || topic >= T){
				printf("%lf %lf\n", r, totprob);
				fprintf(stderr, "iteration %d: sampled incorrect topic %d\n", iter, topic);
				exit(1234);
			}

			z[i] = topic; // assign current word token i to topic j
			wp[wioffset + topic ]++; // and update counts
			dp[dioffset + topic ]++;
			ztot[topic]++;

		}
		if ((iter % 100)==0){
			PauseToEvaluate(iter, bOnTestSet, iNoSamples, iOrder);
			//PrintBestTopic(iter);
			//UpdateHyperParams();
		}
	}

	if(iRunFor % 100 != 0)
		PauseToEvaluate(iter, bOnTestSet, iNoSamples, iOrder);
}

void GibbsLDA::PauseToEvaluate(int iter, bool bOnTestSet, int iNoSamples, const int *iOrder){
	double dPerp, dELLikelihood;
	if(CheckConsistency() == false){
		cerr << "Inconsistency in the probabilities " << endl;
		exit(1);
	}
	fprintf(stderr, "\tIter: %d -- consistent\n", iter);
	return;
	printf( "\tIter:");
	if(iter % 20 == 0)
		printf("20x:");
	if(iter % 50 == 0)
		printf("50x");
	if(iter % 100 == 0)
		printf("100x");
	printf( " %d /%d, perp..." , iter , iNoIter);
	dPerp = Perplexity(iNoTokens,w,d);
	printf(" (train) %lf",dPerp);
	if(bOnTestSet && iNoTestTokens != 0){   // If there is separate testing data
		dPerp = Perplexity(iNoTestTokens,wTest,dTest);
		printf(" (test) %lf",dPerp);

		/*
		if(iNoSamples != 0){
			double *dWordTopicLangProbs = GetWordTopicLangProbs();
			if(testWordDocFreq.size() > 0)
				dELLikelihood = EmpiricalLikelihood(dWordTopicLangProbs, testWordDocFreq, iNoSamples, dTopicDocDistributions);
			else
				dELLikelihood = EmpiricalLikelihood(dWordTopicLangProbs, iNoTestTokens, wTest, dTest, iNoSamples, dTopicDocDistributions);
			printf(" EL(m=%d) %lf", iNoSamples, dELLikelihood);
			if(dWordTopicLangProbs != NULL);
			free(dWordTopicLangProbs);
		}
		*/
	}
	//printf(" beta:%e", dBeta);
	EntropyOfDistributions();
	printf("\n");
}

void GibbsLDA::UpdateHyperParams(){
	// Update alpha
	double dNum, dDenom, dTotAlpha, dTotBeta;
	double dDGAlpha = D*GammaFunc::digamma(dAlpha), dDGKAlpha = D*GammaFunc::digamma(dKAlpha);
	dDenom = 0;
	for(int di=0;di<D;di++)
		dDenom += GammaFunc::digamma(docLength[di]+dKAlpha);
	dDenom -= dDGKAlpha;
	
	dTotAlpha = 0;
	for(int k=0;k<T;k++){
		dNum = 0;
		for(int di=0;di<D;di++)
			dNum += GammaFunc::digamma(dp[di*T+k] + dAlpha);
		dNum -= dDGAlpha;
		//dAlpha_k = dNum/dDenom;
		dTotAlpha += dNum;
	}
	dAlpha *= dTotAlpha / ((double)T*dDenom);	// Average of the dAlpha_k vec
	assert(dAlpha >= 0);
	dKAlpha = T*dAlpha;

	// Beta Update
	double dDGBeta = T*GammaFunc::digamma(dBeta), dDGWBeta = T*GammaFunc::digamma(dWBeta);
	dDenom = 0;
	for(int k=0;k<T;k++){
		dDenom += GammaFunc::digamma(ztot[k]+dWBeta);
	}
	dDenom -= dDGWBeta;

	dTotBeta = 0;
	for(int wi=0;wi<W;wi++){
		int wioffset = wi*T;
		dNum = 0;
		for(int k=0;k<T;k++)
			dNum += GammaFunc::digamma(wp[wioffset+k] + dBeta);
		dNum -= dDGBeta;
		dTotBeta += dNum;
	}
	dBeta *= dTotBeta / ((double)W*dDenom);
	assert(dBeta > 0);
	dWBeta = (double)W*dBeta;

	printf("Alpha:%lf Beta:%lf\n", dAlpha, dBeta);
}

void GibbsLDA::TopicStats(){
	vector<Pair<int,int> *> DocWordFreq = WordDocFreq(d, w, iNoTokens);
	map<int, map<int,int> > TopicAssignments;
	int iCurDoc = -1;
	int iNoDocsToScan = 2, iNoDocsScanned = 0, iNoIncosistentTokens = 0, iNoInconsistentTypes = 0;
	double dAvgFraction = 0, dMostFrequent = 0;
	for(int i=0;i<iNoTokens;i++){
		if(iCurDoc != d[i]){
			if(! TopicAssignments.empty()){
				Pair<int,int> *wordFreq = DocWordFreq[iCurDoc];
				for(map<int, map<int,int> >::iterator iter=TopicAssignments.begin();iter!=TopicAssignments.end();++iter)
					if((*iter).second.size() > 1){
						//printf("word[%d]:%s ", (*iter).first, Words[(*iter).first].c_str());
						int iFreq = 0, iwid = (*iter).first, iMostFrequent = 0;
						for(map<int,int>::iterator iter1=(*iter).second.begin();iter1 != (*iter).second.end();++iter1){
							//printf("%d:%d ", (*iter1).first, (*iter1).second);
							iNoIncosistentTokens += (*iter1).second;
							iFreq += (*iter1).second;
							if(iMostFrequent == -1 || iMostFrequent < (*iter1).second)
								iMostFrequent = (*iter1).second;
						}
						for(Pair<int,int> *p=wordFreq;p!=NULL;p=p->Next){
							if(p->First == iwid){
								if(p->Second != iFreq){
									fprintf(stderr, "Number of incositent tokens for this word (%d) is %d != freq %d\n", iwid, iFreq, p->Second);
									exit(1234);
								}
								dMostFrequent += (double)iMostFrequent/p->Second;
								break;
							}
						}
						//printf("\n");
						iNoInconsistentTypes += 1;
					}
			}
			if(iCurDoc != -1){
				iNoDocsScanned++;
				//printf("Doc %d of length: %d -- with %d inconsistent tokens\n", iCurDoc, docLength[iCurDoc], iNoIncosistentTokens);
				dAvgFraction += ((double)iNoIncosistentTokens/docLength[iCurDoc]);
			}
			//if(iNoDocsScanned == iNoDocsToScan)
			//	break;
			TopicAssignments.clear();
			iCurDoc = d[i];
			iNoIncosistentTokens = 0;
		}
		if(TopicAssignments.find(w[i]) == TopicAssignments.end()){
			TopicAssignments[w[i]] = map<int,int>();
		}
		if(TopicAssignments[w[i]].find(z[i]) == TopicAssignments[w[i]].end())
			TopicAssignments[w[i]][z[i]] = 0;
		TopicAssignments[w[i]][z[i]] += 1;
	}

	if(! TopicAssignments.empty()){
		Pair<int,int> *wordFreq = DocWordFreq[iCurDoc];
		for(map<int, map<int,int> >::iterator iter=TopicAssignments.begin();iter!=TopicAssignments.end();++iter)
			if((*iter).second.size() > 1){
				//printf("word[%d]:%s ", (*iter).first, Words[(*iter).first].c_str());
				int iFreq = 0, iwid = (*iter).first, iMostFrequent = -1;
				for(map<int,int>::iterator iter1=(*iter).second.begin();iter1 != (*iter).second.end();++iter1){
					//printf("%d:%d ", (*iter1).first, (*iter1).second);
					iNoIncosistentTokens += (*iter1).second;
					iFreq += (*iter1).second;
					if(iMostFrequent == -1 || iMostFrequent < (*iter1).second)
						iMostFrequent = (*iter1).second;
				}
				for(Pair<int,int> *p=wordFreq;p!=NULL;p=p->Next){
					if(p->First == iwid){
						if(p->Second != iFreq){
							fprintf(stderr, "Number of incositent tokens for this word (%d) is %d != freq %d\n", iwid, iFreq, p->Second);
							exit(1234);
						}
						dMostFrequent += (double)iMostFrequent/p->Second;
						break;
					}
				}
				//printf("\n");
				iNoInconsistentTypes += 1;
			}
		//printf("Doc %d of length: %d -- with %d inconsistent tokens\n", iCurDoc, docLength[iCurDoc], iNoIncosistentTokens);
		dAvgFraction += ((double)iNoIncosistentTokens/docLength[iCurDoc]);
	}
	dAvgFraction /= D;
	printf("\t\tToken assignments with different topics, avg fraction: %lf most common: %lf beta: %e\n", dAvgFraction, dMostFrequent/iNoInconsistentTypes, dBeta);
	/*/
	Pair<int,int>::Compare pDecreasingOrder(true);
	priority_queue<Pair<int,int>, vector<Pair<int,int> >, Pair<int,int>::Compare> pqTop(pDecreasingOrder);
	float fFrac = 0.9, fTopWordsInTop = 0;
	for(int k=0;k<T;k++){
		int iTot = 0, iTopFrac;
		for(int wid=0;wid<W;wid++){
			pqTop.push(Pair<int,int>(wid, wp[wid*T+k]));
			iTot += wp[wid*T+k];
		}
		if(iTot != ztot[k]){
			fprintf(stderr, "The total number of topic assignments for topic : %d didn't match (%d != %d)\n", iTot, ztot[k]);
			exit(1234);
		}
		iTopFrac = (int)(fFrac*(float)ztot[k]);
		iTot = 0;
		while(! pqTop.empty()){
			iTot += pqTop.top().Second;
			fTopWordsInTop++;
			if(iTot > iTopFrac)
				break;
			pqTop.pop();
		}
		while(! pqTop.empty())
			pqTop.pop();
	}
	printf("%f fraction of words compose of top %f of assignments\n", (float)fTopWordsInTop/(T*W), fFrac);  
	float fTopTopicsInTop = 0;
	for(int did=0;did<D;did++){
		int iTot = 0, iTopFrac;
		for(int k=0;k<T;k++){
			pqTop.push(Pair<int,int>(k, dp[did*T+k]));
			iTot += dp[did*T+k];
		}
		if(iTot != docLength[did]){
			fprintf(stderr, "The total number of topic assignments (%d) for doc %d didn't match the document length (%d)\n", iTot, did, docLength[did]);
			exit(1234);
		}
		iTopFrac = (int)(fFrac*(float)docLength[did]);
		iTot = 0;
		while(! pqTop.empty()){
			iTot += pqTop.top().Second;
			fTopTopicsInTop++;
			if(iTot > iTopFrac)
				break;
			pqTop.pop();
		}
		while(! pqTop.empty())
			pqTop.pop();
	}
	printf("%f fraction of topics compose of top %f of assignments\n", (float)fTopTopicsInTop/(T*D), fFrac);  
	*/
}

void GibbsLDA::PrintTextTopics(){
	double topicTrans[T+1][T];
	int iCurDoc, iPrevTopic = T, iFreqPrev, iFreqAfter, iFreq=-1, iConsistent = 0;
	if(iNoTokens > 0)
		iCurDoc = d[0];
	else
		return;
	for(int j=0;j<T+1;j++)
		for(int k=0;k<T;k++)
			topicTrans[j][k] = 0;
	for(int i=0;i<iNoTokens;i++){
		if(iCurDoc != d[i]){
			iPrevTopic = T;
			// Find the frequent transition
			for(int j=0;j<T+1;j++)
				for(int k=0;k<T;k++)
					if(iFreq < topicTrans[j][k]){
						iFreq = (int)topicTrans[j][k];
						iFreqPrev = j;
						iFreqAfter = k;
					}
			cout << "Freq transition in doc "<< iCurDoc << " is topic["<<iFreqPrev<<"]["<<iFreqAfter<<"]:"<<topicTrans[iFreqPrev][iFreqAfter]<<" / "<<docLength[iCurDoc]<<endl;
			// Initialize them over
			for(int j=0;j<T+1;j++)
				for(int k=0;k<T;k++)
					topicTrans[j][k] = 0;
			iCurDoc = d[i];
			iFreq = -1;
		}
		topicTrans[iPrevTopic][z[i]]++;
		if(iPrevTopic == z[i])
			iConsistent++;
		iPrevTopic = z[i];
	}
	printf("Same bigram topic assignments: %d / %d\n", iConsistent, iNoTokens-1);
}

void GibbsLDA::PrintTopicalWords(int aNoWords){
	double dTotalProb=0, dTempProb=0;
	Pair<int,double>::Compare pDecreasingOrder(true);
	priority_queue<Pair<int,double>, vector<Pair<int,double> >, Pair<int,double>::Compare> pqTopicalWords(pDecreasingOrder);
	for(int k=0;k<T;k++){
		printf("Topic:%d\n", k);
		dTotalProb = 0;
		for(int w=0;w<W;w++){
			//if(dUnigramProbs[w] >= 4)
			//  continue;
			dTempProb = GetWordTopicProb(w, k);
			pqTopicalWords.push(Pair<int,double>(w, dTempProb));
			dTotalProb += dTempProb;
		}
		if(!isfinite(dTotalProb) || fabs(dTotalProb-1) > 1e-8){
			fprintf(stderr, "What ?? ... sum p(w|k=%d):%e != 1\n", k, dTotalProb);
			exit(1234);
		}
		for(int i=0;i<aNoWords && ! pqTopicalWords.empty();i++){
			Pair<int, double> p = pqTopicalWords.top();
			printf("%s\t-->\t%e\n", Words[p.First].c_str(), p.Second);
			pqTopicalWords.pop();
		}
		while(! pqTopicalWords.empty())
			pqTopicalWords.pop();
		printf("\n\n");
	}
}

void GibbsLDA::PrintBestTopic(string sFilePath){
	std::stringstream ss;
	//ss << "withIter/docCluster.txt.lda.5." << idx;
	ss << sFilePath;
	ofstream out(ss.str().c_str());
	int imax;
	double dmaxprob, dprob;
	for(int di=0;di<D;di++){
		imax = -1;
		for(int k=0;k<T;k++){
			dprob = GetTopicDocProb(k, di);
			if(imax == -1 || dmaxprob < dprob){
				dmaxprob = dprob;
				imax = k;
			}
		}
		out << imax << endl;
	}
}

void GibbsLDA::PrintDocTopicDist(string sFilePath){
	std::stringstream ss;
	ss << sFilePath;
	
	ofstream out(ss.str().c_str());
	out << "# " << D << " " << T << " " << D*T << endl;
	for(int di=0;di<D;di++){
		//out << di+1 ;
		for(int k=0;k<T;k++){
			//if(dp[di*T+k])
				//out << " " << k+1 << ":" << dp[di*T+k];
			out << GetTopicDocProb(k,di) << " ";
		}
		out << endl;
	}
}

void GibbsLDA::PrintTopicWordDist(int idx){
	std::stringstream ss;
	if(idx != -1)
		ss << "topicWords.lda.txt." << idx;
	else
		ss << "topicWords.lda.txt";
	
	ofstream out(ss.str().c_str());
	out << "# " << T << " " << W << " " << T*W << endl;
	for(int k=0;k<T;k++){
		for(int wi=0;wi<W;wi++){
			out << GetWordTopicProb(wi, k) << " ";
		}
		out << endl;
	}
}

void GibbsLDA::PrintDocTopicDistFoldIn(){
	ofstream out("docTopics.foldin.lda.txt");
	out << "# " << iFoldInD << " " << T << " " << iFoldInD*T << endl;
	double dTemp;
	for(int di=0;di<iFoldInD;di++){
		out << di+1+iMinFoldInD ;
		for(int k=0;k<T;k++){
			if(dpFoldIn[di*T+k] > 0){
				dTemp = (((double) dpFoldIn[di*T+k] + dAlpha)/((double)docLengthFoldIn[di]+dKAlpha));
				out << " " << k+1 << ":" << dTemp; 
			}
		}
		out << endl;
	}
}

void GibbsLDA::PauseWhileFolding(int iter){
	if(ConsistencyFoldInDocs() == false || CheckConsistency() == false){
		cerr << "Inconsistency in the probabilities " << endl;
		exit(1);
	}
	printf( "\tIter:");
	if(iter % 20 == 0)
		printf("20x:");
	if(iter % 50 == 0)
		printf("50x");
	if(iter % 100 == 0)
		printf("100x");
	printf( " %d /%d, perp..." , iter , iNoIter);

	double dPerp = 0, dTemp;
	int wi,di,iSummedOver=0;
	for(int i=0;i<iNoFoldInTokens;i++){
		wi = wFoldIn[i];
		di = dFoldIn[i];

		double dWordDocProb = 0;
		for(int k=0;k<T;k++){
			dTemp = (((double) dpFoldIn[di*T+k] + dAlpha)/((double)docLengthFoldIn[di]+dKAlpha));
			dTemp *= GetWordTopicProb(wi,k);
			dWordDocProb += dTemp;
		}
		dPerp += log(dWordDocProb);
		iSummedOver+=1;
	}
	dPerp *= -1;
	dPerp /= (double)iSummedOver;
	dPerp = exp(dPerp);
	printf(" (fold in) %lf\n",dPerp);
}

bool GibbsLDA::ConsistencyFoldInDocs(){
	int iTot, dioffset;
	for(int i=0;i<iFoldInD;i++){
		iTot = 0;
		dioffset = i*T;
		for(int k=0;k<T;k++)
			iTot += dpFoldIn[dioffset+k];
		if(iTot != docLengthFoldIn[i]){
			fprintf(stderr, "The number of topic assignments %d in foldin doc %d didn't match its length %d\n", iTot, i, docLengthFoldIn[i]);
			return false;
		}
	}
	return true;
}

void GibbsLDA::FoldIn(){
	int wi,di,i,ii,j,topic,iter, wioffset, dioffset;
	double totprob, r, max;
	double *probs = (double *)calloc(T, sizeof(double));

	if( ConsistencyFoldInDocs() == false){
		fprintf(stderr, "Initial topic assignments for new documents are not consistent\n");
		exit(1234);
	}
	PauseWhileFolding(0);
	for (iter=1; iter<=iNoIter; iter++) {
		for (ii = 0; ii < iNoFoldInTokens; ii++) {
			i = orderFoldIn[ii]; // current word token to assess

			wi  = wFoldIn[i]; // current word index
			di  = dFoldIn[i]; // current document index  
			topic = zFoldIn[i]; // current topic assignment to word token

			wioffset = wi*T;
			dioffset = di*T;

			dpFoldIn[dioffset+topic]--;

			//printf( "(1) Working on ii=%d i=%d wi=%d di=%d topic=%d wp=%d dp=%d\n" , ii , i , wi , di , topic , wp[wi+topic*W] , dp[wi+topic*D] );

			totprob = (double) 0;
			for (j = 0; j < T; j++) {
				probs[j] = ((double) wp[ wioffset+j ] + (double) dBeta)/( (double) ztot[j]+ (double) dWBeta)*( (double) dpFoldIn[ dioffset+ j ] + (double) dAlpha);
				totprob += probs[j];
			}

			// sample a topic from the distribution
			r = (double) totprob * (double) randomMT() / (double) 4294967296.0;
			max = probs[0];
			topic = 0;
			while (r>max) {
				topic++;
				max += probs[topic];
			}

			zFoldIn[i] = topic; // assign current word token i to topic j
			dpFoldIn[dioffset+topic]++;
		}
		if(iter % 100 == 0)
			PauseWhileFolding(iter);
	}
	if(iNoIter % 100 == 0)
		PauseWhileFolding(iter);
}

void GibbsLDA::LoadFoldInData(){
	int iNoFoldInSrcWordIndices, iNoFoldInSrcDocIndices;
	wFoldIn = LoadIndices(ldaConfig.m_sSrcFoldInWordIndicesPath,&iNoFoldInSrcWordIndices);
	dFoldIn = LoadIndices(ldaConfig.m_sSrcFoldInDocIndicesPath,&iNoFoldInSrcDocIndices);

	if(iNoFoldInSrcWordIndices != iNoFoldInSrcDocIndices){
		fprintf(stderr, "The number of fold in doc and word indices didn't match\n");
		exit(1234);
	}
	iNoFoldInTokens = iNoFoldInSrcDocIndices;
	if(iNoFoldInTokens == 0)
		return;
	int di, topic;
	for(int i=0;i<iNoFoldInTokens;i++){
		di = dFoldIn[i]-1;
		wFoldIn[i] = wFoldIn[i]-1;
		dFoldIn[i] = di;
		if(iFoldInD < di)
			iFoldInD = di;
		if(iMinFoldInD > di)
			iMinFoldInD = di;
	}
	iFoldInD = iFoldInD+1;
	if(iMinFoldInD < 0){
		fprintf(stderr, "The min doc idx became %d\n", iMinFoldInD);
		exit(1234);
	}
	if(iMinFoldInD != 0 && iMinFoldInD != D){
		fprintf(stderr, "The min doc idx (%d) should be either 0 or %d\n", iMinFoldInD, D);
		exit(1234);
	}
	if(iMinFoldInD == D){
		iFoldInD -= iMinFoldInD;
		for(int i=0;i<iNoFoldInTokens;i++)
			dFoldIn[i] -= iMinFoldInD;
	}

	orderFoldIn = (int *)calloc(iNoFoldInTokens, sizeof(int));
	zFoldIn = (int *)calloc(iNoFoldInTokens, sizeof(int));
	dpFoldIn = (int *)calloc(iFoldInD*T, sizeof(int));
	docLengthFoldIn = DocLengths(dFoldIn, iFoldInD, iNoFoldInTokens);

	// Random Initialize the topics for fold in tokens
	for (int i=0; i<iNoFoldInTokens; i++)
	{
		di = dFoldIn[i];
		// pick a random topic 0..T-1
		topic = (int) ( (double) randomMT() * (double) T / (double) (4294967296.0 + 1.0) );
		zFoldIn[ i ] = topic; // assign this word token to this topic
		dpFoldIn[ di*T + topic ]++; // increment dp count matrix
	}

	
    printf( "Determining random order update sequence of foldin docs\n" );
    int rp, temp;
    for (int i=0; i<iNoFoldInTokens; i++) orderFoldIn[i]=i; // fill with increasing series
    for (int i=0; i<(iNoFoldInTokens-1); i++) {
        // pick a random integer between i and nw
        rp = i + (int) ((double) (iNoFoldInTokens-i) * (double) randomMT() / (double) (4294967296.0 + 1.0));

        // switch contents on position i and position rp
        temp = orderFoldIn[rp];
        orderFoldIn[rp]=orderFoldIn[i];
        orderFoldIn[i]=temp;
    }
	if( ConsistencyFoldInDocs() == false){
		fprintf(stderr, "Random topic assignments for new documents are not consistent\n");
		exit(1234);
	}
}

void GibbsLDA::EvaluateWordAssociation(string sWordAssocPath){

	cerr << "Word associationss ... " << endl;
	map<string,int> Word2Id;
	int wi, wioffset;
	for(wi=0;wi<W;wi++)
		Word2Id[Words[wi]] = wi;

	double dProb, dPairAssoc;
	int iNoElements, i, k, wiTemp, wiTempOffset, aNoWords = 20;
	double *dTopicCue = (double *)calloc(T, sizeof(double));
	double *dWordTopic = (double *)calloc(T*W, sizeof(double));
	for(wi=0;wi<W;wi++){
		wioffset = wi*T;
		for(k=0;k<T;k++)
			dWordTopic[wioffset+k] = GetWordTopicProb(wi, k);
	}

	fstream fin(sWordAssocPath.c_str(),ios_base::in);
	if(fin.fail()){
		cerr << "Unable to open file "<< sWordAssocPath <<" to load vocab" << endl;
		exit(1234);
		return;
	}

	Pair<int,double>::Compare pDecreasingOrder(true);
    priority_queue<Pair<int,double>, vector<Pair<int,double> >, Pair<int,double>::Compare> pqWords(pDecreasingOrder);

	vector<int> vRanks;
	vector<string> orderTgt;
	string sCue, sTgt, sNow, sFirst;
	int iFirstWordId, iTotalRank = 0, iRank, iTotal = 0;
	double dMRR = 0;
	while(! fin.eof()){
		fin >> sCue >> sTgt >> dProb; 

		if(sNow.empty() || sNow == sCue){
			orderTgt.push_back(sTgt);
			sNow = sCue;
		}else{
			cout << sNow << "\t";
			iNoElements = orderTgt.size();
			for(i=0;i<iNoElements;i++)
				cout << orderTgt[i] << " ";
			cout << endl;
			sFirst = orderTgt[0];
			iFirstWordId = Word2Id[sFirst];

			wi = Word2Id[sNow]; 
			for(k=0;k<T;k++)
				dTopicCue[k] = ( dWordTopic[wi*T+k] * (double)ztot[k] ); //  the division by a constant w.r.t ranking   / (double) iUnigramFreq[wi];
			for(wiTemp=0;wiTemp<W;wiTemp++){
				dPairAssoc = 0;
				wiTempOffset = wiTemp*T;
				for(k=0;k<T;k++)
					dPairAssoc += (dTopicCue[k] * dWordTopic[wiTempOffset+k]);
				pqWords.push(Pair<int,double>(wiTemp, dPairAssoc));
			}

			iRank = -1;
			for(int i=1;! pqWords.empty();i++){
				Pair<int, double> p = pqWords.top();
				if(p.First == iFirstWordId){
					iRank = i;
					printf("%s (%d) \t-->\t%e\n", Words[p.First].c_str(), iRank, p.Second);
					break;
				}
				//printf("%s\t-->\t%e\n", Words[p.First].c_str(), p.Second);
				pqWords.pop();
			}
			while(! pqWords.empty())
				pqWords.pop();
			//printf("\n\n");
			if(iRank > 0){
				iTotalRank += iRank;
				dMRR += (double)1/(double)iRank;
				iTotal += 1;
				vRanks.push_back(iRank);
			}else{
				fprintf(stderr, "Rank became %d\n", iRank);
				exit(1234);
			}

			orderTgt.clear();
			sNow = sCue;
			orderTgt.push_back(sTgt);
		}
	}
	if(! sNow.empty()){
		cout << sNow << "\t";
		iNoElements = orderTgt.size();
		for(i=0;i<iNoElements;i++)
			cout << orderTgt[i] << " ";
		cout << endl;
		orderTgt.clear();
		sNow = sCue;
		orderTgt.push_back(sTgt);
	}
	utils<int> u;
	int iMedRank = u.median(vRanks);
	fprintf(stderr, "MRR:%lf/%d = %lf AvgRank:%d/%d = %lf Median Rank:%d\n", dMRR, iTotal, (double)dMRR/(double)iTotal, iTotalRank, iTotal, (double)iTotalRank/(double)iTotal, iMedRank);
}

void GibbsLDA::PrintTokenAssignments(int idx){ 
		std::stringstream ss;
		if(idx != -1)
				ss << "topicAssignments.lda.txt." << idx;
		else
				ss << "topicAssignments.lda.txt";

		ofstream out(ss.str().c_str());
		int prevdi = d[0]; 
		for(int i=0;i<iNoTokens;i++){
			if(d[i] != prevdi){
				for(prevdi;prevdi<d[i];prevdi++)
					out << endl;
			}
			out << Words[w[i]] << "_z" << z[i] << " "; 
			prevdi = d[i];
		}
		out.close();
}

int main(int argc,char *argv[])
{
	int iTopicalWords = 30;
    string sConfigPath, sAssignPath, sJob = "save";
    string sBestTopicFilePath = "LDA_bestTopic.txt";
    string sDocTopicDistFilePath = "LDA_docTopicDist.txt";
    if(argc < 2){
        printf("Usage: ./a.out config_file_path model_file_path [load/save]\n");
        return 0;
    }else if(argc == 2){
        sConfigPath = argv[1];
        printf("Running: %s %s\n", argv[0], sConfigPath.c_str());
    }else if(argc == 3){
        sConfigPath = argv[1];
        sAssignPath = argv[2];
        printf("Running: %s %s %s\n", argv[0], sConfigPath.c_str(), sAssignPath.c_str());
    }else if(argc == 4){
        sConfigPath = argv[1];
        sAssignPath = argv[2];
        sJob = argv[3];
        printf("Running: %s %s %s %s\n", argv[0], sConfigPath.c_str(), sAssignPath.c_str(), sJob.c_str());
    }

    GibbsLDA glda;
    glda.LoadData(sConfigPath);

    if(sJob == "save"){
        glda.RandomInitialize();
        glda.RandomOrder();
        glda.Iterate(true); //, 0, NULL, 100);
        glda.SaveTopicAssignments(sAssignPath);
    }else if(sJob == "load"){
        glda.LoadTopicAssignments(sAssignPath);
        glda.InitializeFrmPrevCounts();
    }
    //*/
    glda.PrintTopicalWords(iTopicalWords);
    glda.PrintBestTopic(sBestTopicFilePath);
    glda.PrintDocTopicDist(sDocTopicDistFilePath);
}
