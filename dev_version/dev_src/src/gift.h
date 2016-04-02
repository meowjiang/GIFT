// The head file for the method of gift.

// GIFT is used to predicte the interactions between compounds
// substructures and protein domains from the known drug-protein
// interactions. See the details:
// http://bioinformatics.oxfordjournals.org/content/31/15/2523.abstract

// Note: in this version, we don't support
//   1. the association method.
//   2. the cross-validation method and draw the AUC curve.
//   3. check for drugs/protein without fingerprints.
//   4. check for fingerpints involved in few drugs/proteins.

// Author: Songpeng Zu
// Email: zusongpeng@gmail.com
// Date: 2016-03-06

#ifndef __GIFT_H__
#define __GIFT_H__

// C/C++ system library
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
// Boost library
#include<boost/thread/thread.hpp>
#include<boost/bind.hpp>
//#include<boost/thread/thread.hpp>
namespace gift {
  // gift information.
  const std::string author("Songpeng Zu");
  const std::string email("zusongpeng@gmail.com");
  const std::string version("gift-2.0");
  const std::string updateTime("2016-03-06");

  // gift namespace variables
  const int recLogLeastNum = 5;
  typedef std::vector<int> IntList;
  typedef std::vector<std::vector<int> > IntArrayList;
  typedef std::vector<std::vector<double> > numericMatrix;

  IntArrayList drug2proteinList;
  IntArrayList drug2subList;
  IntArrayList sub2drugList;
  IntArrayList protein2domainList;
  IntArrayList domain2proteinList;
  numericMatrix drugSub2proteinSubMatrix;
  numericMatrix observedDrug2ProteinMatrix;
  numericMatrix vardrugSub2proteinSubMatrix;
  std::vector<double> loglikelyArray;

  int Matrix2Fingerpints(const std::string, IntArrayList&, std::string delims="\t,");
  int writeMatrix(const std::string, numericMatrix&, std::string delims="\t,");
  int readMatrix(const std::string, numericMatrix&, std::string delims="\t,");

  class rowCol;
  int rowColFile(const std::string, rowCol&, std::string delims="\t,");
  int helpGift();

  class parameters;
  class EM;
  int outRecord(parameters&, EM&);

  // put template or inline function in one file.
  template <typename func>
  int functionThread(func useFun,int thread, EM * point) {
    boost::thread * y;
    boost::thread_group * x = new boost::thread_group;
    for(int i=0;i<thread;++i){
      y = new boost::thread(useFun,point,i);
      x->add_thread(y);
    } // end of loop i
    x->join_all();
    delete x;
    return 0;
  } // end of function.

  // classes
  class rowCol {
  public:
    rowCol(int row, int col): rowNum(row), colNum(col) {}
    rowCol(): rowNum(1), colNum(1) {}
    int rowNum;
    int colNum;
  };

  class parameters {
  // Further design recommmendation:
  // parameters should be the SINGLETON, a specific desing pattern.
  // And the outside in gift  data should be stored inside parameters
  //  with shared ptr.
  // Note: some people did not agree with traditional singleton, since it
  // is just like global variables. And it is not easy to keep thread safe
  // since the use of static variables.
  public:
    //parameters ();
    // Init with config file.
    parameters (const std::string);

    inline int setDrugNum (int number) { drugNum = number; return 0; }
    inline int setSubNum (int number) { subNum = number; return 0; }
    inline int setProteinNum (int number) {proteinNum = number; return 0; }
    inline int setDomainNum (int number) { domainNum = number; return 0; }

    int InitDrugSub2ProteinSub();

    // DATA MEMBERS
    // input data file name
    std::string drug2proteinFileName;
    std::string drug2subFileName;
    std::string protein2subFileName;
    std::string drugSub2proteinSubFileName;
    // input name list file name
    std::string drugNameListFile;
    std::string drugSubNameListFile;
    std::string proteinNameListFile;
    std::string proteinSubNameListFile;
    // input parameters for EM
    bool loglikelyRecord;
    double alphaEB; // Emprical Bayesian estimated parameter.
    double betaEB; // Emprical Bayesian estimated parameter.
    double fn;
    double fp;
    int thread;
    int iterNum;
    int drugNum;
    int subNum;
    int domainNum;
    int proteinNum;
    std::string inputDelims;
    std::string outputDelims;
    std::string task;
    // input file version information.
    std::string chemfpRec;
    std::string proteinfpRec;
    std::string CPIsRec;
    // input file names for prediction.
    // If both files are given, we only predict the interactions between them.
    // It means we now support only one-time prediction.
    std::string predictDrugsFileName;
    std::string predictProteinsFileName;
    // output file name
    std::string outRecordFileName;
    std::string outPredictCPIsFileName;
    std::string outDrugSub2ProteinSubFileName;
    std::string outVarDrugSub2proteinSubFileName;
  }; // end of class parameters

  class EM {
    // In fact, usually only one EM objact is allowed.
  public:
    // Inition with parameters
    EM(parameters& param)
      : loglikelyRecord(param.loglikelyRecord)
      , fn(param.fn)
      , fp(param.fp)
      , thread(param.thread)
      , iterNum(param.iterNum)
      , drugNum(param.drugNum)
      , proteinNum(param.proteinNum)
      , task(param.task)
      , drug2sub(nullptr)
      , sub2drug(nullptr)
      , protein2sub(nullptr)
      , sub2protein(nullptr)
      , drug2protein(nullptr)
      , drugSub2proteinSub(nullptr)
      , observedDrug2Protein(nullptr)
      , vardrugSub2proteinSub(nullptr)
      , loglikely(nullptr)
    { } // end of constuctor.

    ~EM() { } // end of default destruction.
    // Set the pointers to several matrix.
    inline int setPointerDrug2Sub(IntArrayList & d2s) {
      drug2sub = &d2s;
      return 0;
    } // end of func
    inline int setPointerProtein2Sub(IntArrayList & p2s) {
      protein2sub = &p2s;
      return 0;
    } // end of func
    inline int setPointerDrug2Protein(IntArrayList & d2p) {
      drug2protein = &d2p;
      return 0;
    } // end of func
    inline int setPointerDrugSub2ProteinSub(numericMatrix & ds2ps) {
      drugSub2proteinSub = &ds2ps;
      return 0;
    } // end of func

    // two init EM, i.e., the drugSub2proteinSub matrix.
    // when the task is train, we use int initEM() to init the matrix
    // when the task is predict, we directoly read the trained matrix.
    //    int initEM();
    // Core of EM.
    double iterdrugSub2ProteinSub(int drugIndex, int proteinIndex);
    void EStepThread(int threadNth);
    int EStep();
    void MStepThread(int threadNth);
    int MStep();
    double recLoglikely();
    inline int setLoglikely(double logscore) {
      (*loglikely).push_back(logscore);
      return 0;
    } // end of function
    int trainEM();
    int predictEMByDrug(IntList &, numericMatrix &);
    int predictEMByProtein(IntList &, numericMatrix &);
    int predictEMByBoth(IntList & drugs, IntList & proteins, numericMatrix &);
    int varEM();
    int outTrainResult(std::string);
    int outTrainVariance(std::string);
    int outPredict(std::string);
  private:
    bool loglikelyRecord;
    double fn;
    double fp;
    int thread;
    int iterNum;
    int drugNum;
    int subNum;
    int domainNum;
    int proteinNum;
    std::string task;
    // pointer to outside data.
    IntArrayList * drug2sub;
    IntArrayList * sub2drug;
    IntArrayList * protein2sub;
    IntArrayList * sub2protein;
    IntArrayList * drug2protein;
    numericMatrix * drugSub2proteinSub;
    numericMatrix * observedDrug2Protein;
    numericMatrix * vardrugSub2proteinSub;
    std::vector<double> * loglikely;
  };// end of class EM

} // end of namepsace gift

#endif // end of GIFT_H
