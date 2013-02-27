#include "Riostream.h"
#include <string>



void treeFromAscii(const char* filen) {

//  Read data from an ascii file and create a root file with a tree

  std::string rFile( filen ) ;
  rFile += ".root" ;
  TFile *f = new TFile( rFile.c_str() ,"RECREATE");

  TTree*  ntuple = new TTree("ntuple","") ;
  ntuple->ReadFile( filen ) ;
  
  f->Write() ;
  f->Close() ;
}

// void treeFromAscii(const char* filen) {

// //  Read data from an ascii file and create a root file with a tree

//   ifstream in;
//   in.open( filen );

//   //  Float_t thedeg,theta,eta,nrvxd,nrsit,nrtpc,nrset ;
//   Float_t thedeg,theta,eta,nlset,nlecal,nlhcal,nlcoil,nlyoke ; 

//   Int_t nlines = 0;

//   std::string rFile( filen ) ;

//   rFile += ".root" ;

//   TFile *f = new TFile( rFile.c_str() ,"RECREATE");


//   //  TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","thedeg:theta:eta:nrvxd:nrsit:nrtpc:nrset" );
//   TNtuple *ntuple = new TNtuple("ntuple","data from ascii file","thedeg:theta:eta:nlset:nlecal:nlhcal:nlcoil:nlyoke" );
 

//   while (1) {

//     //    in >> thedeg >> theta >> eta >> nrvxd >> nrsit >> nrtpc >> nrset;
//     in >> thedeg>>theta>>eta>>nlset>>nlecal>>nlhcal>>nlcoil>>nlyoke ;  

//     printf(" thetadeg : %f \n",  thedeg ) ;

//     if (!in.good()) { 
//       //      printf(" problem reading file ") ;
//       break;
//     }

//     //    ntuple->Fill(  thedeg,theta,eta,nrvxd,nrsit,nrtpc,nrset );
//     ntuple->Fill(  thedeg,theta,eta,nlset,nlecal,nlhcal,nlcoil,nlyoke ) ;

//     nlines++;

//   }
//   printf(" found %d points\n",nlines);
  
//   in.close();
  
//   f->Write();
// }
