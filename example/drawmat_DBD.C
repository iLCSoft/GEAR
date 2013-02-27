 void drawmat_DBD(char* FILEN) {

   //char* title = "Material budget for ILD_O1_v05" ;


   double M_PI = 3.14159265358979312e+00 ;
   //   180./pi = 57.295779 ;
   
   //ntuple variables
   double itheta, nrvxd, nrsit, nrtpc, nrtpc_i, nrset, nrecal ;

   
   // stuff for histograms
   const int nhist = 7;
   TString htitle[nhist];
   int idx = 0 ;
   htitle[idx++] = "ECAL layer 1";
   htitle[idx++] = "SET";
   htitle[idx++] = "outside TPC";
   htitle[idx++] = "TPC";
   htitle[idx++] = "SIT + FTD";
   htitle[idx++] = "VXT";
   htitle[idx++] = "dummy";
   TString hname[nhist];
   idx = 0 ;
   hname[idx++] = "ecal";
   hname[idx++] = "set";
   hname[idx++] = "tpc_o";
   hname[idx++] = "tpc_i";
   hname[idx++] = "sit";
   hname[idx++] = "vtx";
   hname[idx++] = "dummy";

   int nbin = 90 ; // 720

   float xmin = -90;
   float xmax = -0;
   int color[nhist] = { 14, 8, 38, 2, 1, 9};
   TString option[nhist];
      
  TLegend *leg = new TLegend (0.2, 0.7, 0.45, 0.9);
  leg->SetFillStyle(0);
  
   // stuff for histograms
   TH1F* hmat[nhist];
   for (int ihist = 0; ihist < nhist; ihist++) {
     hmat[ihist] = new TH1F(hname[ihist],"",nbin,xmin,xmax);
     //// ensure proper errors
     // hmat[ihist]->Sumw2();
     hmat[ihist]->SetLineColor(color[ihist]);
     hmat[ihist]->SetLineWidth(3);
     hmat[ihist]->SetTitle(";#theta / degrees;X_{0}");
   }
   
   
  // read tree
  TFile* file = new TFile ( FILEN ) ;
  TTree* tree = (TTree *) file->Get("ntuple");
  if (!tree) {
    cout << "ERROR: Couldn't open tree ntuple on file: "<< FILEN << endl;
    return 1; 
  }
  
  tree->SetBranchAddress("itheta",  &itheta);
  tree->SetBranchAddress("nrecal",   &nrecal);
  tree->SetBranchAddress("nrset",   &nrset);
  tree->SetBranchAddress("nrtpc",   &nrtpc);
  tree->SetBranchAddress("nrtpc_i", &nrtpc_i);
  tree->SetBranchAddress("nrsit",   &nrsit);
  tree->SetBranchAddress("nrvxd",   &nrvxd);
  int entries = tree->GetEntries();
    cout << "tree has " << entries << " entries" << endl;
  
  // event loop
  for (int ievt = 0; ievt < entries; ++ievt) {

     tree->GetEntry(ievt);
     if (itheta < 4.45) continue;


     hmat[0]->Fill(-itheta,nrecal);
     hmat[1]->Fill(-itheta,nrset);
     hmat[2]->Fill(-itheta,nrtpc);
     if( itheta > 8. ) 
     hmat[3]->Fill(-itheta,nrtpc_i);

     hmat[4]->Fill(-itheta,nrsit);
     hmat[5]->Fill(-itheta,nrvxd);
     hmat[6]->Fill(-itheta,1);
  }
  
//   for (int ibin = 0; ibin < nbin+1; ++ibin) {
//     cout << "Bin " << ibin << " has " << hmat[4]->GetBinContent(ibin) << " entries" << endl;
//   }

  c1 = new TCanvas("c1","c1",600,600);
  

  bool first = true ;

#define _include_Ecal 0

#if _include_Ecal 
  for (int ihist = 0; ihist < nhist-1; ihist++) {
#else
  for (int ihist = 1; ihist < nhist-1; ihist++) {
#endif

    // average double counts 
    hmat[ihist]->Divide( hmat[nhist-1] );

    if( first ) { 

      hmat[ihist]->Draw( ""  );

      // --------- change axis labels from minus to plus --------
      c1->Update() ;
      TF1 *f1 = new TF1("f1","-x",0,90);
      hmat[ihist]->GetXaxis()->SetLabelOffset(99);
      Double_t x_min = c1->GetUxmin();
      Double_t x_max = c1->GetUxmax();
      Double_t y_min = c1->GetUymin();
      //std::cout << "x_min " << x_min << " x_max " << x_max << " y_min " << y_min << std::endl ;
      TGaxis *axis = new TGaxis( x_min, y_min, x_max, y_min, "f1", 5);    
      axis->SetLabelSize( 0.06 );
      axis->SetLabelFont( 42 ); 
      axis->Draw(); 
      // -----end: change axis labels from minus to plus --------

      first = false ;


    } else {
      hmat[ihist]->Draw( "same"  );
    }

    leg->AddEntry(hmat[ihist],htitle[ihist],"L");

  }
  leg->Draw();
  

  std::string pdfFile( std::string( FILEN ) + std::string( ".pdf" ) ) ;
  c1->Print( pdfFile.c_str() ) ;
  
}
