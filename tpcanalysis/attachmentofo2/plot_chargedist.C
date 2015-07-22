{
TFile *f = TFile::Open("chargedist.root");
  int ndiv=50;

  Int_t MyPalette[50];
  Double_t r[]    = {0., 0.0, 1.0, 1.0, 1.0};
  Double_t g[]    = {0., 0.0, 0.0, 1.0, 1.0};
  Double_t b[]    = {0., 1.0, 0.0, 0.0, 1.0};
  Double_t stop[] = {0., .25, .50, .75, 1.0};
  Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 50);
  for (int i=0;i<50;i++) MyPalette[i] = FI+i;

  TF1 *f1 = new TF1("f1","[0]*TMath::Exp([1]*x)",0,1000);
  TH1D *decay = new TH1D("decay","decay",49,0,49);
  TH1D *charge[ndiv];
 
  for(int i=0;i<ndiv;i++)
    {
	  TString name =Form("name_%i",i);
	  charge[i]=(TH1D*)f->Get(name);
	  charge[i]->Rebin(5);
	  charge[i]->Scale(1./charge[i]->Integral());
	  //	  charge[i]->Fit("f1","","",0,1000);
	  // double decayconst=f1->GetParameter(1);
	  //decay->SetBinContent(i,decayconst);
	  if(i%3==0)
	    {
	      charge[i]->SetLineWidth(2);
	      charge[i]->SetLineColor(MyPalette[i]);
	      charge[i]->GetXaxis()->SetRangeUser(40,500);
	      charge[i]->Draw("C same");
	    }

      }  



  
}
