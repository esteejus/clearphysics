{



  TFile *f = new TFile("driftcorrection.root","OPEN");
  int padxdiv=20,padzdiv=30;
  TH2D *padplanexdiff = new TH2D("padplanexdiff","padplane",padzdiv,0,1400,padxdiv,-440,440);
  TH2D *padplaneydiff = new TH2D("padplaneydiff","padplane",padzdiv,0,1400,padxdiv,-440,440);


  TH2D *padplanexsigma = new TH2D("padplanexsigma","padplane",padzdiv,0,1400,padxdiv,-440,440);
  TH2D *padplaneysigma = new TH2D("padplaneysigma","padplane",padzdiv,0,1400,padxdiv,-440,440);

  TH1D *xdiff[padzdiv][padxdiv];
TH1D *ydiff[padzdiv][padxdiv];

 TF1 *g = new TF1("g","[0]*(TMath::Exp(-[1]*TMath::Abs(x-[2])))",-10,10);
 // g->SetParameter(0,15);
 // g->SetParameter(1,.3);
 // g->SetParameter(2,-.1);

  for(int i=0;i<padzdiv;i++)
    {
      for(int j=0;j<padxdiv;j++)
	{
	  TString name =Form("xdiff_%i_%i",i,j);
	  xdiff[i][j]=(TH1D*)f->Get(name);

	  TString name2 =Form("ydiff_%i_%i",i,j);
	  ydiff[i][j]=(TH1D*)f->Get(name2);
    
	  double xmean=0;
	  double ymean=0;
	  double sigmay=0;
	  double sigmax=0;

	  if(xdiff[i][j]->GetEntries()>20){
	   xdiff[i][j]->Fit("gaus");
	   xmean=gaus->GetParameter(1);
	   sigmax=gaus->GetParameter(2);
	   
	   padplanexdiff->SetBinContent(i+1,j+1,xmean);
	   padplanexsigma->SetBinContent(i+1,j+1,sigmax);

	  
	  }

	  if(ydiff[i][j]->GetEntries()>20){
	  ydiff[i][j]->Fit("gaus");
	   ymean=gaus->GetParameter(1);
	   sigmay=gaus->GetParameter(2);

	   padplaneydiff->SetBinContent(i+1,j+1,ymean);
	   padplaneysigma->SetBinContent(i+1,j+1,sigmay);
	  }
	 

	}
    }


 
  
  TFile *h = new TFile("distribution.root","RECREATE");
  h->cd();
  padplaneydiff->Write();
  padplanexdiff->Write();
  padplanexsigma->Write();
  padplaneysigma->Write();



}
