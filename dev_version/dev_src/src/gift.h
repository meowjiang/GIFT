// The head file for the method of gift.

// GIFT is used to predicte the interactions between compounds
// substructures and protein domains from the known drug-protein
// interactions. See the details:
// http://bioinformatics.oxfordjournals.org/content/31/15/2523.abstract

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

namespace gift {
  // gift information.
  const std::string author{"Songpeng Zu"};
  const std::string email{"zusongpeng@gmail.com"};
  const std::string version{"gift-2.0"};
  const std::string updateTime{"2016-03-06"};

  typedef std::vector<std::vector<int> > IntArrayList;
  typedef std::vector<std::vector<double> > numericMatrix;

  int Matrix2Fingerpints(const std::string, IntArrayList&, std::string delims="\t,");
  int writeMatrix(const std::string, numericMatrix&, std::string delims="\t,");
  int readMatrix(const std::string, numericMatrix&, std::string delims="\t,");

  class rowCol;
  int rowColFile(const std::string, rowCol&, std::string delims="\t,");
  int helpGift();

  class parameter;
  class EM;
  int outRecord(parameters&, EM&);

  // classes
  class rowCol {
  public:
    rowCol(int row, int col): rowNum(row), colNum(col) {}
    rowCol(): rowNum(1), colNum(1) {}
    int rowNum;
    int colNum;
  }

  class parameters {
  public:
    //parameters ();
    // Init with config file.
    parameters (const std::string);

    int setDrugNum(int);
    int setSubNum(int);
    int setProteinNum(int);
    int setDomainNum(int);

    // public members
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
    std::string chemfpRec;
    std::string proteinfpRec;
    std::string CPIsRec;
    std::string outFilePrefixCPIs;
    std::string outFilePrefixTrain;
  }; // end of class parameters

  class EM {
  public:
    // Inition with parameters
    EM(parameters&);
    // Default Destruction
    ~EM();
    // Set the pointers to several matrix.
    int setPointerDrug2Sub(IntArrayList&);
    int setPointerProtein2Sub(IntArrayList &);
    int setPointerDrug2Protein(IntArrayList&);
    int setPointerDrugSub2ProteinSub(IntArrayList&);
    // Core of EM.
    int EStep();
    int MStep();
    int initEM();
    int loglikely();
    int trainEM();
    int predictEM();
    int setLoglikely(double);
    int outTrain(std::string&);
    int outPredict(std::string&);
  private:
    double fn;
    double fp;
    int thread;
    int iterationNum;
    int drugNum;
    int subNum;
    int domainNum;
    int proteinNum;
    std::string task;
    // or use static with pointer.
    IntArrayList * drug2sub;
    IntArrayList * proein2sub;
    IntArrayList * drug2protein;
    numericMatrix * drugSub2proteinSub;
    std::vector<double> * loglikely;
  };// end of class EM

} // end of namepsace gift

#endif // end of GIFT_H
