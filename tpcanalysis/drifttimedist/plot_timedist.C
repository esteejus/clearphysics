{
TFile *f = TFile::Open("timedist_realpadplane.root");

 int zpadrange=5;//how many pads in z to summary
 TCanvas *summary[zpadrange];
 for(int i=0;i<zpadrange;i++){
   TString histname=Form("summarytimedist_%i",i);
   summary[i]=new TCanvas(histname,histname,1);
 }

  int nxdiv=20;
  int nzdiv=112;

  TH1D *time[nzdiv][nxdiv];

  Int_t MyPalette[20];
  Double_t r[]    = {0., 0.0, 1.0, 1.0, 1.0};
  Double_t g[]    = {0., 0.0, 0.0, 1.0, 1.0};
  Double_t b[]    = {0., 1.0, 0.0, 0.0, 1.0};
  Double_t stop[] = {0., .25, .50, .75, 1.0};
  Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 50);
  for (int i=0;i<20;i++) MyPalette[i] = FI+i;

  for(int i=0;i<zpadrange;i++)
    {
      summary[i]->cd();
      for(int j=0;j<nxdiv;j++){
      TString name =Form("timedist_%i_%i",i,j);
      cout<<name<<endl;
	  time[i][j]=(TH1D*)f->Get(name);
	  time[i][j]->Scale(1./time[i][j]->Integral());

	      time[i][j]->SetLineWidth(2);
	      time[i][j]->SetLineColor(MyPalette[j]);
  	      time[i][j]->Draw("same");

      }  

  
}
  TFile *f = new TFile("summary.root","RECREATE");
  f->cd();

  for(int i=0;i<zpadrange;i++)summary[i]->Write();

}
