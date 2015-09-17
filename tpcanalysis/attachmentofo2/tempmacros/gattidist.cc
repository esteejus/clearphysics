{
  Double_t K1 = 0.2699; // K2*sqrt(K3)/(4TMath::ATan(sqrt(K3)))
  Double_t K2 = 0.8890; // TMath::Pi()/2*(1 - sqrt(K3)/2);
  Double_t K3 = 0.7535;

  Double_t w  = 6; // [mm] pad length in direction, perpendicular to the wire
  Double_t h  = 8;      // [mm] groundwire separation

  
  Double_t ratiosignals=1.3; //ratio of integrated charge on one pad. Large signal/small signal ratio
  //we think the small signal is the wire is 
TF1 *f= new TF1("f","TMath::ATan(TMath::Sqrt([3])*TMath::TanH([2]*(x/[4]+[5]/2/[4])))-TMath::ATan(TMath::Sqrt([3])*TMath::TanH([2]*(x/[4]-[5]/2/[4])))",-24,24);
 f->SetParameter(3,K3);
 f->SetParameter(2,K2);
f->SetParameter(4,h);
 f->SetParameter(5,w);

 /*
  Double_t chargel = f->Integral(-6,6);
  double ratio=0;
  double stepsize=.01;//mm
  cout<<chargel<<endl;
  for(int i=0;i<500;i++){
    if(ratio<ratiosignals){
      h=8+(i*stepsize);
 TF1 *f2= new TF1("f2","TMath::ATan(TMath::Sqrt([3])*TMath::TanH([2]*(x/[4]+[5]/2/[4])))-TMath::ATan(TMath::Sqrt([3])*TMath::TanH([2]*(x/[4]-[5]/2/[4])))",-24,24);
f2->SetParameter(3,K3);
 f2->SetParameter(2,K2);
f2->SetParameter(4,h);
 f2->SetParameter(5,w);
 
 ratio = chargel/f2->Integral(-6,6);
 //f2->Delete();
 // cout<<ratio<<endl;
    }
    else if(ratio>ratiosignals)break;
  }
  
  cout<<"h is "<<h<<endl;
  */  
f->Draw();
  
//f2->Draw("same");

}
