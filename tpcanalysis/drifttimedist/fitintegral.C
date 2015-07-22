{
   TFile::Open("timedist.root");
  int nbins=timedist_0_6->GetNbinsX();

  double content=0,content_1=0;
  double starttime=0,endtime=0;



  TH1D *integral = new TH1D("integral","integral",nbins,0,20);

  for(int i=0;i<nbins;i++)
    {
      content+=timedist_0_6->GetBinContent(i);
      integral->SetBinContent(i,content);
    }
  
  TF1 *start1 = new TF1("start1","[0]*x+[1]",0,20);
  TF1 *start2 = new TF1("start2","[2]*x+[3]",0,20);
  TF1 *end1 = new TF1("end1","[4]*x+[5]",0,20);
  TF1 *end2 = new TF1("end2","[6]*x+[7]",0,20);

  integral->Fit("start1","","",4.4,5.4);
  integral->Fit("start2","","",5.8,6.8);
  integral->Fit("end1","","",13.8,14.8);
  integral->Fit("end2","","",15.3,16.8);

  starttime=(start1->GetParameter(1)-start2->GetParameter(3))/(start2->GetParameter(2)-start1->GetParameter(0));
  endtime=(end1->GetParameter(5)-end2->GetParameter(7))/(end2->GetParameter(6)-end1->GetParameter(4));


  //cout<<i<<" "<<foundend<<" "<<foundend<<endl;
cout<<starttime<<" "<<endtime<<endl;

}

