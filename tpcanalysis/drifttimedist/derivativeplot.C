{
 TFile *f =TFile::Open("timedist.root");
  int nbins=timedist_0_6->GetNbinsX();
  TH1D *startnoise = new TH1D("startnoise","startnoise",200,-.005,.01);
  TH1D *endnoise = new TH1D("endnoise","endnoise",200,-.005,.003);
  TH1D *startevents = new TH1D("startevents","startevents",200,-.005,.01);
  TH1D *endevents = new TH1D("endevents","endevents",200,-.005,.003);


  const int padxdiv=10,padzdiv=10;
  
  TH1D *time[padzdiv][padxdiv]; 
  for(int k=0;k<padzdiv;k++)
    for(int j=0;j<padxdiv;j++)
      { 
	{
	  TString name =Form("timedist_%i_%i",k,j);
	  //cout<<name<<endl;
	  time[k][j]=(TH1D*)f->Get(name);
       
      
	  //front side band
	  double estimatedstarttime=5.58;
	  double estimatedendtime=15.2;  


	  int startbin=time[k][j]->FindBin(estimatedstarttime);
	  int endbin=time[k][j]->FindBin(estimatedendtime);
  
	  for(int i=0;i<nbins;i++)
	    {

	      double deriv=(time[k][j]->GetBinContent(i+3)+time[k][j]->GetBinContent(i+2))-(time[k][j]->GetBinContent(i+1)+time[k][j]->GetBinContent(i))/2;
      
	      if(i<startbin)startnoise->Fill(deriv);
	      else if(i<(startbin+6)&&i>startbin)startevents->Fill(deriv);
	      else if(i>endbin && i<(endbin+20))endnoise->Fill(deriv);
	      else if(i<endbin && i>=(endbin-10))endevents->Fill(deriv);
	    }
	}
      }
  
  
}
