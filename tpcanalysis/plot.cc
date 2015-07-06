{
  gStyle->SetOptStat(0);
  //gSystem -> Load("libSTConverter.so");
  STCore *c = new STCore("~/tpcdata/run_0334.dat");
  c -> SetAGETMap("/home/justin/clearphysics/tpcanalysis/asad_get_maps/06_07_2015_cosmic/AGET.map");
  c -> SetUAMap("/home/justin/clearphysics/tpcanalysis/asad_get_maps/06_07_2015_cosmic/UnitAsAd.map");
  c -> SetFPNPedestal();
  c -> SetData(0);

  STRawEvent *event;
  event = c -> GetRawEvent(51);// p -> SetEvent(event); p -> DrawPadplane();
  TH1D *adc = new TH1D("maxadc","max adc dist",100,0,100);
  TH1D *time = new TH1D("time","time",1000,0,1000);
  
  TH2D* padplane = new TH2D("padplane","padplane max adc",112,0,112,108,0,108);
  TH2D* padplaneraw = new TH2D("padplaneall","padplane max adc all",112,0,112,108,0,108);

  int adcthreshold=30;//max adc threshold
  for(int j=0;j<2000;j++)
    {
      //    STRawEvent *event;
      event = c -> GetRawEvent(j);// p -> SetEvent(event); p -> DrawPadplane();
      
      for(int i=0;i<(event->GetNumPads());i++)
	{
	  STPad *pad;
	  pad=event->GetPad(i);
	  int maxidx=pad->GetMaxADCIdx();
	  double maxadc=pad->GetADC(maxidx);
	  int layer=pad->GetLayer();
	  int row=pad->GetRow();
	  ++layer;//root starts with (1,1) not (0,0)
	  ++row;
	  
	  //bad asad from 70<layer<75 && 81<row<90 gives signal when no events
	  bool badregion = ((layer<=75 && layer>70 && row<=90 && row>81));//eval to true in bad region
	  
	  //       adc->Fill(maxadc);
	  //if( maxidx>400 && badregion==false)
	  if( maxadc>adcthreshold && badregion==false)
	    {
	      double bincont=padplaneraw->GetBinContent(layer,row);
	      maxadc+=bincont;
	      time->Fill(maxidx);
	      if(maxidx>150 && maxidx<185)padplaneraw->SetBinContent(layer,row,maxadc);
	      else padplane->SetBinContent(layer,row,maxadc);
	    }
	 
	}
      
    }
  
  TCanvas *c1 = new TCanvas("c1","c1",1000,1000);
  c1->Divide(2,2);
  
  c1->cd(1);
  padplane->Draw("colz");

  c1->cd(2);
  adc->Draw();
  
  c1->cd(3);
  // padplaneraw->Add(padplane,-1);
  padplaneraw->Draw("colz");

  c1->cd(4);
  time->Scale(1./(time->Integral()));
  time->Draw();
  
}
