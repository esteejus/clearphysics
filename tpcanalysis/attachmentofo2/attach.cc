{
  /*
====================================
macro for evaluating the attachment effects inside the TPC
  Since at the time we do not have absolute calibration of ADC to e- number
  we will use a relative measurement and an assumpiton.
  The assumption is the tracks nearest to the anode wires will exibit less
  attachment than those with longer drift lengths.
  The data we were using was cosmic data. Which also presents the difficulty
  in not knowing how much initial charge was depositied. Also its a stocastic
  process. To overcome this difficulty we will assume there is no variation
  of the O2 attachment in the chamber. The attachment depends on the cross
  section which depends on the e- energy. Also the ammount of O2 in the
  chamber.
  We will need to calibrate the pads so they all give the same gain.
  We achive this by doing a gain calibration using a pulser. The pulser is
  pulsing from the ground grid with different voltages. The induced signal
  response in the electronics gives a curve relating ADC to voltage of the
  pulser. We use one pad as a reference pad. The other pads get the ADC 
corresponding to a voltage. Then Get the voltage of the reference pad and its
corresponding ADC. It calibrates in this fashion. 

First method
From ADC time bucket distribution I know where the anode and cathode signals are
1.Determine first(anode) signals and last(cathode)
2.Create a calibrated ADC height (charge) distribution for small layers in drift
3.Look at local regions of thin layer and the global regions

Considerations
---------------
consider cutting in time at leas the time it takes to travel to anode
this region is not constant drift. Also the e-field region from gating to ground is different than the gating grid to cathode. Make sure you calculate the correct drift length from these. I.e. its piece wise. 
 consider cutting near front wall. Cut a region where you think field is unifm
consider a MC Chi2 analysis where you assume some cosmic ray dist



=====================================
  */
  Double_t driftvel=54.3/2;//mm/us divide by two because I halved the sampling rate to display full events in the viewer was 50  MHz now its 25Mhz that time buckets went from 20ns to 40ns.
  TH1D *time = new TH1D("time","time position",2000,0,20);
  TChain *chain = new TChain("cbmsim");
  //TFile *_file0 = TFile::Open("/cache/scr/spirit/spiritroot_repo/macros/cosmic_threshold50.root");

  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0335.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0336.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0339.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0334.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0342.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0346.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0346_1.root");
  
	    //TTree *cbmsim = (TTree *) _file0 -> Get("cbmsim");
	    //cbmsim -> Print();
  TClonesArray *eventArray; //array of objects

  chain -> SetBranchAddress("STEventHC", &eventArray);//Store array of events into TClonesArray
  Int_t nentries=chain->GetEntries();
  cout<<"Total entries "<<nentries<<endl;//   nentries=16200;
  bool flag=false;//if any no event between time buckets then true
  
  
  int ndiv=50;
  double starttime=5.7+.4;//us .4us should be bellow the gating grid. 
  double endtime=15.1;//us

  TH1D *ydist= new TH1D("ydist","ydist",5000,-1000,0);
  TH1D *charge[ndiv];
  
  for(int i=0;i<ndiv;i++)
    {
      TString name =Form("name_%i",i);
      charge[i]=new TH1D(name,"charge dist",5000,0,5000);
    }

  //   nentries=2000;
  for(int i=0;i<nentries;i++)
    {
      if(i%1000==0)cout<<i<<endl;
      chain -> GetEntry(i);//This changes the event index
      STEvent *event;
      event = (STEvent *) eventArray -> At(0);//there is only one event in each index
      Int_t nhits=event -> GetNumClusters();//Hits();//total number of hits
      //cout<<nhits<<endl;
      if(event->IsGood()==false)
	{
	  cout<<"Event "<<i<<" is bad!!!"<<endl;
	  continue;
	}

      for(int j=0;j<nhits;j++)
	{
	  STHitCluster *hit = event->GetCluster(j);
	  Double_t xpos=hit->GetPosition().X();
	  Double_t zpos=hit->GetPosition().Z();
	  Double_t ypos=hit -> GetPosition().Y();
	  Double_t drifttime=-ypos/driftvel;
	  Double_t hitcharge=hit->GetCharge();
	  time->Fill(drifttime);
	  ydist->Fill(ypos);

	  //bool badregion =((zpos<=900 && zpos>840 && xpos<=-288 && xpos>-216));//bad cobo region
	  bool badpad=(zpos<=354.5 && xpos<=92.5 && xpos>=91.5 && zpos>=353.5);
	 	  //int num=event->GetNumHits();
	  //	  if(nhits<10)cout<<"event "<<i<<" "<<xpos<<" "<<zpos<<" "<<hitcharge<<endl;
	  //	        if(badpad==true)cout<<"event "<<i<<" "<<xpos<<" "<<zpos<<" "<<hitcharge<<endl;

	  for(int k=0;k<ndiv;k++)
	    {
	      double lowerlim=starttime+(k*(endtime-starttime)/ndiv);
	      double upperlim=starttime+((k+1)*(endtime-starttime)/ndiv);
	      if(drifttime<upperlim && drifttime>lowerlim && badpad==false)charge[k]->Fill(hitcharge);      
	      
	    }
	  

	 
	  
	}

    }
  TFile *f = new TFile("chargedist.root","RECREATE");
  f->cd();
  for(int i=0;i<ndiv;i++)charge[i]->Write();
  f->Write();
  f->Close();
  //ydist->Draw();
  // time->Draw();
       
}
