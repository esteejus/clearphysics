{
  //  gStyle->SetOptStat(0);
  TH2D *padplane= new TH2D("padplane","padplane raw",72,0,72,108,0,108);
  TH1D *adc = new TH1D("adc","acd dist",50000,0,5000);
  TFile *_file0 = TFile::Open("cosmic_badresistor_0threshold.root");
  TTree *cbmsim = (TTree *) _file0 -> Get("cbmsim");
  TClonesArray *eventArray; //array of objects
  TClonesArray *rawArray; //array of objects

  cbmsim -> SetBranchAddress("STEventH", &eventArray);//Store array of events into TClonesArray
  cbmsim -> SetBranchAddress("STRawEvent", &rawArray);//Store array of events into TClonesArray

  Int_t nentries=cbmsim->GetEntries();

  nentries=200;
  for(int i=0;i<nentries;i++)
    {

      cbmsim -> GetEntry(i);//This changes the event index
      STEvent *event;
      event = (STEvent *) eventArray -> At(0);//there is only one event in each index
      Int_t nhits=event -> GetNumHits();//total number of hits

      STRawEvent *raw;
      raw=(STRawEvent*)rawArray->At(0);
      Int_t npads=raw->GetNumPads();
      /*
      for(int j=0;j<npads;j++)
	{
	  STPad *pad=raw->GetPad(j);
	  Int_t adcidx=pad->GetADCMaxIdx();
	  Int_t adcvalue=pad->GetADC(adcidx);
	}
      */
      for(int j=0;j<nhits;j++)
        {
          STHit *hit = event -> GetHit(j);//increase hit
	  Double_t charge=hit->GetCharge();
	  adc->Fill(charge);
	  Double_t xpos=hit->GetPosition()->X();
          Double_t zpos=hit->GetPosition()->Z();
	  xpos+=432;
	  zpos=zpos-137;
	  zpos=zpos/12;
	  xpos=xpos-3;
	  xpos=xpos/8;
	  //	  Int_t row=pad->GetRow();
	  //	  Int_t layer=pad->GetLayer();
	  //	  ++row;
	  //	  ++layer;
	  Double_t bincont=padplane->GetBinContent(zpos,xpos);
	  bincont+=charge;
	   if(charge>10)padplane->SetBinContent(zpos,xpos,bincont);

        }

    }

    adc->Draw();
  //  padplane->Draw("colz");














}
