#include <iostream>
#include <TH1D.h>
#include <TF2.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TRandom3.h>
#include <time.h>
#include <TGraph.h>
#include <cmath>
#include <TLorentzVector.h>

using namespace std;



bool detect(double coor[]);
void project(double thetalab, double philab, double coor[] );
void shift(double coor[],TF2 *);

//***************************
//ENTER PARAMETERS HERE
//***************************
//BEAM & TARGET PARAMETERS
//Units MeV/A or MeV
double amu2mev = 931.494028;
double beamA=132;
double targetA=124;
double beamE=350;

//Sigma of Gaussian beam profile
//beam measurment from 2012; probably old but gives and idea of the profile
// meanx=.3 meany=.5 sigx=.56 sigy=.53

double meanx=.3;//x & y mean position of beam; units cm
double meany=.5;
double sigx=.56;//units cm x-dim is horizontal axis along detector; axis of target mech
double sigy=.53;//units cm y-dim is vertical axis along detector; direction of drift field
//target size; assumed square target
double targetx=3;//units cm
double targety=3;//units cm

//Detector geometry
//x & y width of rectangular window
//full x and y dimension
double xwidth=5.74;//units cm
double ywidth=7.01;//units cm

//DISTANCE Target TO WINDOW
double targetdist=.2;//distance target is from window in cm

//Number of MC events
int ntrials =1e6;

//***************
//
//***************




void geometriceff(){

  //beam
  targetx=targetx/2;
  targety=targety/2;  
  TF2 *beamxy = new TF2("f1","TMath::Gaus(x,[3],[1])*TMath::Gaus(y,[4],[2])",-targetx,targetx,-targety,targety);
    beamxy->SetParameter(1,sigx);
    beamxy->SetParameter(2,sigy);
    beamxy->SetParameter(3,meanx);
    beamxy->SetParameter(4,meany);  


  
  double theta=-1.;
  double theta_lab=-1;
  double phi=-1.;
  double energy=0;


  const Double_t pi=TMath::Pi();
  const Double_t deg2rad= pi/180.;
  const Double_t rad2deg= 180/pi;
  

    
  int seed=40641547;
  TRandom3 *ran = new TRandom3(seed);
 
  
  //book lorentz vector and energy hisotram
  TLorentzVector *pion = new TLorentzVector();
  TH1D *enrg = new TH1D("energylab","Energy in Lab",100,0,600);
  TH1D *phicom = new TH1D("phicom","phicom",360,0,360);
  
  TH1D *thetacom = new TH1D("thetacom","theta",180,0,180);
  TH1D *thetacomgate = new TH1D("thetacomgate","thetacomgate",180,0,180);
  
  TH1D *thetal = new TH1D("thetalab","thetalab",180,0,180);
  TH1D *phil = new TH1D("philab","philab",360,0,360);
  TH1D *thetaeff = new TH1D("thetalabeff","theta_lab_eff",180,0,180);
  TH1D *thetatotal = new TH1D("thetalabtotal","thetalabtotal",180,0,180);
  
  TH1D *thetacomeff = new TH1D("thetacomeff","thetacomeff",180,0,180);

  
  TH1D *raptotal = new TH1D("rapidcomtotal","rapidcomtotal",100,-.5,0);
  TH1D *rapeff = new TH1D("rapideff","rapid_COM_eff",100,-.5,0);

  /*
  //Units MeV/A or MeV
  double amu2mev = 931.494028;
  double beamA=132;
  double targetA=124;
  double beamE=350;
  */  

  // ***calculate the transformation to the CoM frame ****
  double beamMass = beamA*amu2mev;
  double beamEnergy = beamE*beamA+beamA*amu2mev;//includes mass of proj.
  double totalEnergy = beamEnergy+targetA*amu2mev;//include mass of target
  double beam_momentum=sqrt(beamEnergy*beamEnergy-beamMass*beamMass);
  double beta=(beam_momentum/totalEnergy);//positive to go into COM to LAB!!
  cout<<" beta is:\t"<<beta<<endl;  

  /*********
	    uniform isotropic distribution in COM frame
	    defined beam momenta
  *********/
  double eff=0;
  for(int i=0;i<ntrials;i++){
    double r1=0.;//uniform
    //beta is the velocity of the COM system with respects to the lab. Thus beta is positive in the z direction.
    double mom = 242;//pion max momentum
    double energy = TMath::Sqrt(pow(mom,2)+pow(140,2));
    
    //Get random angles;isotropic
    phi=ran->Uniform()*360;
    r1=ran->Uniform();
  
    theta=TMath::RadToDeg()*acos(1-2*r1);//isotropic dist;not uniform
     thetacom->Fill(theta);
     phicom->Fill(phi);
     
    //set lorentz vector
    pion->SetPxPyPzE(mom*cos(deg2rad*phi)*sin(deg2rad*theta),mom*sin(deg2rad*phi)*sin(deg2rad*theta),mom*cos(deg2rad*theta),energy);

    double comrapid = pion->Rapidity();//COM rapidity
    //raptotal->Fill(pion->Rapidity());
    pion->Boost(0,0,beta);//boost from COM to LAB
    raptotal->Fill(comrapid);
    enrg->Fill(pion->E());
    double thetalab=TMath::RadToDeg()*pion->Theta();

    double philab=phi;//NEVER use this function again defined from -pi to pi
    //i only want particles in the lab that are going forward into the detector
    //i.e. thetalab<90 and all 2pi in the philab
    thetatotal->Fill(thetalab);    
    if(thetalab<=90){
      double coor[2]={};
      project(TMath::DegToRad()*thetalab,phi,coor);//project to detector window
      shift(coor,beamxy);//shift due to incoming beam has a distribution
      if(detect(coor)==true){
	thetal->Fill(thetalab);
	phil->Fill(philab);	
	thetacomgate->Fill(theta);
	rapeff->Fill(comrapid);
	eff+=1;
      }
      
      //if(i%(ntrials/10)==0)cout<<i<<endl;
    }
    
  }

  
  TCanvas *c1 = new TCanvas("c1","c1",1);
  c1->cd();
  cout<<"Total eff "<<eff/ntrials<<endl;
  thetaeff->Divide(thetal,thetatotal);//Lab frame theta correction
  thetaeff->Draw();

  TCanvas *c2 = new TCanvas("c2","c2",1);
  c2->cd();
  thetacomeff->Divide(thetacomgate,thetacom);//COM frame theta correction
  thetacomeff->Draw();

  TCanvas *c3 = new TCanvas("c3","c3",1);
  c3->cd();
  rapeff->Divide(raptotal);//Rapididty COM correction
  rapeff->Draw();



  return;

}
  

//project takes in theta phi (LAB frame) and projects onto window plane of TPC
void project(double thetalab, double philab, double coor[] ){
  //direction of beam is z
  //looking down stream x is to the left
  //looking down stream y is up
  //theta is the polar angle with respect to z axis
  //phi is the angle with respect to x-y
  //double targetdist=1;//distance target is from window in cm
  double x=0,y=0;//x,y coordinates on window plane
  
  y=TMath::Sin(philab)*TMath::Tan(thetalab)*targetdist;
  x=TMath::Cos(philab)*TMath::Tan(thetalab)*targetdist;

  coor[0]=x;
  coor[1]=y;
  
  return;
}

//shift grabs random x,y from a beam dist and will shift x,y on window plane
void shift(double coor[],TF2 *beamxy){
  double x=0,y=0;
  beamxy->GetRandom2(x,y);
    //cout<<"before "<<coor[0]<<" "<<coor[1]<<endl;
    coor[0]+=x;
    coor[1]+=y;
    //  cout<<"after "<<coor[0]<<" "<<coor[1]<<endl;
    
    return;
}

//detect if the x,y coordinates (LAB frame) pass thru detector window
bool detect(double coor[]){
  bool flag=false;//false if it did not pass thru  
  double x=coor[0];
  double y=coor[1];
  
  if((x<=xwidth/2 && x>=-xwidth/2) && (y<=ywidth/2 && y>=-ywidth/2))flag=true;
  else flag=false;


  return flag;

}
