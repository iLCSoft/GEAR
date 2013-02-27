 void drawintlen_DBD(char* FILEN) {

   //char* title = "Material budget for ILD_O1_v05" ;


   double M_PI = 3.14159265358979312e+00 ;
   //   180./pi = 57.295779 ;
   
   //ntuple variables
   //   float theta, nrvxd, nrsit, nrtpc, nrset, thedeg;
   //   float theta, nlyoke,nlcoil,nlhcal,nlecal,nlset
   Float_t thedeg,theta,eta,nlset,nlecal,nlhcal,nlcoil,nlyoke ; 

   // stuff for histograms
   const int nhist = 5;
   TString htitle[nhist];
   htitle[0] = "Ecal (Lcal)";
   htitle[1] = "Hcal (LHcal/bcal)";
   htitle[2] = "Coil";
   htitle[3] = "";
   htitle[4] = "dummy";
   TString hname[nhist];
   hname[0] = "ECAL";
   hname[1] = "HCAL";
   hname[2] = "SIT";
   hname[3] = "VTX";
   hname[4] = "dummy";

   int nbin = 180;
   float xmin = -90;
   float xmax = -0;
   //   int color[nhist] = {9,8,2,38,1};
   int color[nhist] = {8,2,38,1,9};
   TString option[nhist];

   option[0] = "same";
   option[1] = "same";
   option[2] = "";
   option[3] = "";
   option[4] = "";
      
   //  TLegend *leg = new TLegend (0.25, 0.6, 0.5, 0.85);
  TLegend *leg = new TLegend (0.25, 0.3, 0.6, 0.55);
  leg->SetFillStyle(0);
  
  // stuff for histograms
  TH1F* hmat[nhist];

  for (int ihist = 0; ihist < nhist; ihist++) {

    hmat[ihist] = new TH1F(hname[ihist],"",nbin,xmin,xmax);
    //// ensure proper errors
    // hmat[ihist]->Sumw2();
    hmat[ihist]->SetLineColor(color[ihist]);
    hmat[ihist]->SetLineWidth(3);
    hmat[ihist]->SetTitle(";#theta / degrees;Interaction Lengths");
  }
  
  
  // read tree
  TFile* file = new TFile ( FILEN ) ;
  TTree* tree = (TTree *) file->Get("ntuple");
  if (!tree) {
    cout << "ERROR: Couldn't open tree ntuple on file: "<< FILEN << endl;
    return 1; 
  }
  
  tree->SetBranchAddress("theta", &theta);
  tree->SetBranchAddress("thedeg",&thedeg);
  tree->SetBranchAddress("nlset", &nlset);
  tree->SetBranchAddress("nlecal", &nlecal);
  tree->SetBranchAddress("nlhcal", &nlhcal);
  tree->SetBranchAddress("nlcoil", &nlcoil);
  tree->SetBranchAddress("nlyoke", &nlyoke);
  int entries = tree->GetEntries();
    cout << "tree has " << entries << " entries" << endl;
  
  // event loop
  for (int ievt = 0; ievt < entries; ++ievt) {
     tree->GetEntry(ievt);

     //,     if (thedeg < 4.41) continue;  // ???

     hmat[0]->Fill(-thedeg,nlecal);
     hmat[1]->Fill(-thedeg,nlhcal);
     hmat[2]->Fill(-thedeg,nlcoil);
     hmat[3]->Fill(-thedeg,nlyoke);
     hmat[4]->Fill(-thedeg,1);
  }
  
  //   for (int ibin = 0; ibin < nbin+1; ++ibin) {
  //     cout << "Bin " << ibin << " has " << hmat[4]->GetBinContent(ibin) << " entries" << endl;
//   }
  
  c1 = new TCanvas("c1","c1",600,600);
  
  
  hmat[2]->GetYaxis()->SetRangeUser(0.,14.) ;

  //  for (int ihist = 0; ihist < nhist-2; ihist++) {
  for (int ihist = nhist-3 ; ihist >=0 ; --ihist) {
    // average double counts 
    hmat[ihist]->Divide( hmat[4] ) ;
    // draw 

    hmat[ihist]->Draw(option[ihist]);


    if( ihist ==  nhist-3 ){
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
    }

    leg->AddEntry(hmat[ihist],htitle[ihist],"L");
  }
  leg->Draw();
  

  std::string pdfFile( std::string( FILEN ) + std::string( ".pdf" ) ) ;
  c1->Print( pdfFile.c_str() ) ;
  
}
