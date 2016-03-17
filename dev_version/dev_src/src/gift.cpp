// Libraries.
#include<boost/algorihtm/string.hpp>
#include "gift.h"

namespace gift{

  int Matrix2Fingerpints(const std::string inputFile,IntArrayList & getFp,
                         std::string delims="\t,"){
    std::ifstream input;
    std::string line;
    std::vector<string> array;
    std::vector<int> tempRec;

    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      input.open(inputFile, std::ifstream::in);
      if (input.peek() == std::ifstream::traits_type::eof()){
        std::cerr <<inputFile <<" is empty. " <<std::endl;
      } // end of if
      while(std::getline(input,line)){
        boost::algorithm::split(array,line,boost::is_any_of(delims));
        int arraylen = array.size();
        for(int i=0;i<arraylen;++i){
          if(array[i].compare("1") == 0){
            tempRec.push_back(i);
          }// end of if
        } // end of for
        getFp.push_back(tempRec);
        tempRec.clear();
      } // end of while
    } catch(std::ifstream::failure e) {
      std::cerr <<"Exceptions open/read file "<<inputFile<<std::endl;
      return 1;
    } // end of catch
    return 0;
  } // end of function.

  int readMatrix(const std::string inputFile,numericMatirx& getMat,
                 std::string delims="\t,"){
    std::ifstream input;
    std::string line;
    std::vector<string> array;
    std::vector<double> tempRec;

    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      input.open(inputFile, std::ifstream::in);
      if (input.peek() == std::ifstream::traits_type::eof()){
        std::cerr <<inputFile <<" is empty. " <<std::endl;
      } // end of if
      while(std::getline(input,line)){
        boost::algorithm::split(array,line,boost::is_any_of(delims));
        int arraylen = array.size();
        for(int i=0;i<arraylen;++i){
          tempRec.push_back(stod(array[i],std::string::size_type* idx=0));
        } // end of for
        getMat.push_back(tempRec);
        tempRec.clear();
      } // end of while
    } catch(std::ifstream::failure e) {
      std::cerr <<"Exceptions open/read file "<<inputFile<<std::endl;
      return 1;
    } // end of catch
    return 0;
  }// end of function.

  int rowColFile(const std::string inputFile, rowCol& matrixRec,
                 std::string delims="\t,"){
    std::ifstream input;
    std::string line;
    int count = 0;
    std::vector<string> array;

    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      input.open(inputFile,std::ifstream::in);
      if (input.peek() == std::ifstream::traits_type::eof()){
        std::cerr <<inputFile<<" is empty."<<std::endl;
        return 1;
      }

      std::getline(input,line);
      ++count;
      boost::algorithm::split(array, line, boost::is_any_of(delims));
      matrixRec.colNum = array.size();
      // string getline func over istream.
      while(std::getline(input,line)){
        // QUESTION: how about empty line?
        ++count;
      } // end of while
      input.close();

      matrixRec.rowNum = count;
      matrixRec.colNum = array.size();
    } catch (std::ifstream::failure e) {
      std::cerr << "Exceptions open/read file "<<inputFile<<std::endl;
      return 1;
    } // end of catch
    return 0;
  } // end of function.

  int writeMatrix(const std::string outFileName,)

}
