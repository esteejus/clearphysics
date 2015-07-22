{
  TFile::Open("timedist.root");
  int nbins=timedist_0_6->GetNbinsX();
  double threshold=.0007;//threshold for derivative 
  double endthreshold=.0003;//endthreshold
  double content=0,content_1=0;
  double starttime=0,endtime=0;
  double derivative[nbins]={};
  bool foundstart=false,foundend=false;
  bool below=false,above=false;
  TH1D *integral = new TH1D("integral","integral",nbins,0,20);

  for(int i=0;i<nbins;i++)
    {
      content+=timedist_0_6->GetBinContent(i);
      content_1=content+timedist_0_6->GetBinContent(i+1);
      double temp=TMath::Abs(content_1-content);
      integral->SetBinContent(i,content);
      derivative[i]=temp;  
    }
  //front side band
  double estimatedstarttime=5.5;
  double estimatedendtime=15.3;  
  int sidebandwidth=50;//width in bins

  int startbin=timedist_0_6->FindBin(estimatedstarttime);
  int endbin=timedist_0_6->FindBin(estimatedendtime);

  double avgbkgstart=0,avgbkgend=0;
  for(int i=0;i<sidebandwidth;i++)
    {
      avgbkgstart+=timedist_0_6->GetBinContent(startbin-i);
      avgbkgend+=timedist_0_6->GetBinContent(endbin+i);
    }
  avgbkgstart/=sidebandwidth;
  avgbkgend/=sidebandwidth;
  cout<<avgbkgstart<<" "<<avgbkgend<<endl;

 for(int i=0;i<nbins;i++)
    {
      double content=timedist_0_6->GetBinContent(i);
      double backwardcontent=timedist_0_6->GetBinContent(nbins-i);
      if((content-avgbkgstart)>=0 && foundstart==false && i>=startbin)
	{
	  starttime=timedist_0_6->GetBinCenter(i);
	  foundstart=true;
	}
      else if((backwardcontent-avgbkgend)>=0 && foundend==false && (nbins-i)<=endbin)
	{
	  endtime=timedist_0_6->GetBinCenter(nbins-i);
	  foundend=true;
	}
    }


 cout<<starttime<<" "<<endtime<<endl;



}
