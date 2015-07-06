{
   //macro for opening a root file output from SpiRITROO
  Double_t driftvel=54.3/2;//mm/us divide by two because I halved the sampling rate to display full events in the viewer was 50  MHz now its 25Mhz that time buckets went from 20ns to 40ns.
  TH1D *time = new TH1D("time","time position",2000,0,20);
  TFile *_file0 = TFile::Open("~/fair_install/SpiRITROOT/macros/output.root");
  TTree *cbmsim = (TTree *) _file0 -> Get("cbmsim");
  cbmsim -> Print();
  TClonesArray *eventArray; //array of objects
  cbmsim -> SetBranchAddress("STEventH", &eventArray);//Store array of events into TClonesArray
  Int_t nentries=cbmsim->GetEntries();
  bool flag=false;//if any no event between time buckets then true
  
  for(int i=0;i<nentries;i++)
    {

      cbmsim -> GetEntry(i);//This changes the event index
      STEvent *event;
      event = (STEvent *) eventArray -> At(0);//there is only one event in each index
      Int_t nhits=event -> GetNumHits();//total number of hits

      for(int j=0;j<nhits;j++)
	{
	  STHit *hit = event -> GetHit(j);//increase hit
	  Double_t xpos=hit->GetPosition()->X();
	  Double_t zpos=hit->GetPosition()->Z();
	  Double_t ypos=hit -> GetPosition()->Y();
	  Double_t drifttime=-ypos/driftvel;

	  time->Fill(drifttime);

	}

    }

  time->Draw();
       
}
