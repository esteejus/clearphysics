 {
  TF1 *decay = new TF1("decay","[1]*TMath::Exp([0]*x)",0,100);
  TH1D *measurement = new TH1D("measure","measurement",1000,0,100);
  TH1D *prob = new TH1D("prob","prob",1000,-1e-3,1e-3);

  decay->SetParameter(0,5e-6);
  decay->SetParameter(1,1);

  int nevents=1e3;

  for(int j=0;j<1e3;j++)
    {
  for(int i=0;i<nevents;i++)
    {
      double random=decay->GetRandom();
	measurement->Fill(random);
    }

  measurement->Fit("decay");
  double lambda=decay->GetParameter(0);
  prob->Fill(lambda);
    }

}
