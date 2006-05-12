#include "gearxml/tinyxml.h"
#include "gearxml/MergeXML.h"
#include "gear/GEAR.h"

int main( int argc, char *argv[] ) {
  try{

    if( argc < 4 ) {
      std::cout << "\nMergeXML:   program to join two xml files"
		<< "\nusage:      MergeXML <devot> <dominant> <target>"
		<< "\n"
		<< "\n<devot>     filename for base-file that will be overwritten"
		<< "\n<dominant>  filename for file that overrules devot file"
		<< "\n<target>    filename for output file " 
		<< std::endl ;
      exit(1) ;
    }

    
    std::string fileName1   = argv[1] ;
    std::string fileName2   = argv[2] ;
    std::string fileNameNew = argv[3] ;
    
    gear::MergeXML mergeXML ;
    
    if ( mergeXML.setFile1 (fileName1) ) {
      std::cout << "devot   : ok" << std::endl ;
    }
    else {
      std::cout << "devot   : error reading file " << fileName1 << std::endl ;
      exit (2) ;
    }

    if ( mergeXML.setFile2 (fileName2) ) {
      std::cout << "dominant: ok" << std::endl ;
    }
    else {
      std::cout << "dominant: error reading file " << fileName2 << std::endl ;
      exit (2) ;
    }

    if ( mergeXML.mergeFiles(fileNameNew) ) {
      std::cout << "target  : ok" << std::endl ;
    }
    else {
      std::cout << "target: error merging files in " << fileNameNew << std::endl ;
      exit(1) ;
    }
    
    return 0;

  }
  catch( gear::Exception& e) {
    std::cout << " An Exception occured : " << e.what() << std::endl ;
  }

}
  

