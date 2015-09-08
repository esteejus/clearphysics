//In this macro I want to find the relationship between diffusion and how much HIT charge on pads
//This will be accomplished by a given decay distf
//We will sample from this distribution and apply diffusion for given lengths
//the diffusion is modeled by simple gaussian dist with sigma that depends on sqrt(length)
//the final dist is treated with the gatti dist to get the pad response funciton
//the pad dist of charge is then found and plotted 
// so the wires confine charge to the 1D regions on the wire.


{
  TH1D *pad0 = new TH1D("pad0","pad0",100,0,4);
  TH1D *pad1 = new TH1D("pad1","pad1",100,0,4);

  TF1 *decay = new TF1("decay","TMath::Exp(-x)",0,2000);//initial dist of charge
  
  //for(int i=0;i<50;i++)
  {
    double sigma =.1+.57*TMath::Sqrt(20);
    TF2 *dist = new TF2("dist","TMath::Gaus(x,0,[0])*TMath::Gaus(y,0,[0])",-30,30,-30,30);
    dist->SetParameter(0,sigma);
    //    dist->Draw("colz");    
 

 }

  
  
  
  
  /*
}


void integrate(TF2 *finaldist)
{
  */
  Double_t K1 = 0.2699; // K2*sqrt(K3)/(4TMath::ATan(sqrt(K3)))
  Double_t K2 = 0.8890; // TMath::Pi()/2*(1 - sqrt(K3)/2);
  Double_t K3 = 0.7535;

  Double_t w  = 6; // [mm] pad length in direction, perpendicular to the wire
  Double_t h  = 8;      // [mm] groundwire separation

  double simppar[2]={K2/h,(K2*w)/(2*h)}
  Double_t ratiosignals=1.3; //ratio of integrated charge on one pad. Large signal/small signal ratio
  //we think the small signal is the wire is 
  //  TF1 *f= new TF1("f","TMath::ATan(TMath::Sqrt([3])*TMath::TanH([2]*(x/[4]+[5]/2/[4])))-TMath::ATan(TMath::Sqrt([3])*TMath::TanH([2]*(x/[4]-[5]/2/[4])))",-24,24);
  TF1 *f= new TF1("f","[2]*(TMath::ATan(TMath::Sqrt([3])*TMath::TanH(([0]*x+[1])))-TMath::ATan(TMath::Sqrt([3])*TMath::TanH(([0]*x-[1]))))",-24,24);
  
  /*
f->SetParameter(3,K3);
 f->SetParameter(2,K2);
f->SetParameter(4,h);
 f->SetParameter(5,w);
  */
  //  f->SetParameter(2,0);//the mean value of the dist. 
  f->SetParameter(3,K3);
 f->SetParameter(1,simppar[1]);
f->SetParameter(0,simppar[0]);
 f->SetParameter(2,1);
/*
//NOTE!!!!
//x axis is really z axis on tpc
//y axis is really x axis on tpc

 double xmin=-1,xmax=-1,ymin=-1,ymax=-1;
 finaldist->GetRange(zmin,ymin,xmax,zmax);

 int steps=1e3;//integration steps for both x & y directions
 double zstep=(zmax-zmin)/steps,ystep=(ymax-ymin)/steps;//step sizezeze


 for(int i=0;i<steps;i++)

   {
     xmin+=xstep;
     for(int j=0,j<steps,j++)
       {
	 ymin+=ystep;
	 double prob=finaldist->Integral(	 




       }
   }

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
 double scale=f->Integral(-30,30);  
 f->SetParameter(2,1./scale);

 for(int i=0;i<100;i++)
   {
     double stepsize=(4./100);
     double pad0cont=f->Integral(-4+stepsize*i,4+stepsize*i);
     double pad1cont=f->Integral(4+stepsize*i,12+stepsize*i);
     pad0->SetBinContent(i,pad0cont);
pad1->SetBinContent(i,pad1cont);
		 //f->Draw();
		 }  
       //f2->Draw("same");
       
}
