#include <iostream>
#include "TClonesArray.h"
#include <TChain.h>
#include <TH1D.h>
#include "STEvent.hh"
#include "STRiemannTrack.hh"
#include <vector>
#include <TGraph.h>
#include "TFile.h"
#include "TApplication.h"
#include "TF1.h"
#include "TH2D.h"

using namespace std;

int main(){
  // TApplication app("mon", &argc, argv);

  int padxdiv=20,padzdiv=30;
  TH2D *padplane = new TH2D("padplane","padplane",padzdiv,0,1400,padxdiv,-440,440);
 
 
  TH1D *xdiff[padzdiv][padxdiv]; 
  for(int i=0;i<padzdiv;i++){
    for(int j=0;j<padxdiv;j++){
      TString name =Form("xdiff_%i_%i",i,j);
      //cout<<name<<endl;
      xdiff[i][j]= new TH1D(name,name,200,-20,20);
    }
  }
 
  TH1D *ydiff[padzdiv][padxdiv]; 
  for(int i=0;i<padzdiv;i++){
    for(int j=0;j<padxdiv;j++){
      TString name =Form("ydiff_%i_%i",i,j);
      //cout<<name<<endl;
      ydiff[i][j]= new TH1D(name,name,200,-20,20);
    }
  }
 

  TChain *chain = new TChain("cbmsim");
  //TFile *_file0 = TFile::Open("/cache/scr/spirit/spiritroot_repo/macros/cosmic_threshold50.root");

  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0346_riemann.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0336_riemann.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0339_riemann.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0338_riemann.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0341_riemann.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0335_riemann.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0334_riemann.root");
   chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0346_riemann_1.root");


  //TTree *cbmsim = (TTree *) _file0 -> Get("cbmsim");
  //cbmsim -> Print();
  TClonesArray *trackArray=NULL; //array of objects
  TClonesArray *eventArray=NULL; //array of objects

  chain -> SetBranchAddress("STEventHC", &eventArray);//Store array of events into TClonesArray
  chain -> SetBranchAddress("STRiemannTrack", &trackArray);
  Int_t nentries=chain->GetEntries();
  cout<<"Total entries "<<nentries<<endl;


  int goodevents=0;

  // nentries=100;
  for(int i=0;i<nentries;i++)
    {

     
      if(i%1000==0)cout<<i<<endl;
      chain -> GetEntry(i);//This changes the event index
      STEvent *event;
      event = (STEvent *) eventArray -> At(0);//there is only one event in each index
      if(event->IsGood()==false)
	{
	  cout<<"Event "<<i<<" is bad!!!"<<endl;
	  continue;
	}
      
      //I think the way I would like to classify linear tracks is perform a linear fit on the tracks
      //if the chi squared is to high I will reject these events
      //Then I can form an array with the track number and the fit parameter array
      //I can then loop over the fit parameter array and see if the parameters are within each other then the 
      //tracks form one straight line

     
      TF1 *xzline = new TF1("xzline","[0]*x+[1]",0,800);
      TF1 *zyline = new TF1("zyline","[2]*x+[3]",0,800);
      // vector<vector<double> > tracks;

      int  ntracks=trackArray->GetEntries();
  
      for(int itrack=0;itrack<ntracks;itrack++)
	{
	  TGraph *xz= new TGraph();
	  TGraph *zy= new TGraph();
	  STRiemannTrack *riemann = (STRiemannTrack *) trackArray->At(itrack);
	  const vector<STRiemannHit *> *hits=riemann->GetHits();	  
	  int numhits = hits->size();

	  bool lessthanz=false;
	  for(int j=0;j<numhits;j++)
	    {
	      int clusID=hits->at(j)->GetCluster()->GetClusterID();
	      STHitCluster *hit=event->GetCluster(clusID);
	      double xpos=hit->GetPosition().X();
	      double ypos=hit->GetPosition().Y();
	      double zpos=hit->GetPosition().Z();
	     
	      //multiply by two because I halved the sampling rate to display full events in the viewer was
	      // 25  MHz now its 50Mhz that time buckets went from 40ns to 20ns
	      //Thus the  length is artifically scaled by two to fit the window in the viewer. 
	      //check the parameter file to verify the sampling rate is as such.
	      //if the sampling rate is 25MHz then comment the next line out. If 50 then leave the line uncommented 
	      ypos*=2;

	      xz->SetPoint(j,zpos,xpos);
	      zy->SetPoint(j,zpos,ypos);
	      if(zpos<120 && numhits>15)lessthanz=true;
	    }

	  xz->Fit(xzline,"Q","",120,800);
	  zy->Fit(zyline,"Q","",120,800);
	  double chisquare=xzline->GetChisquare();
	  chisquare/=xzline->GetNDF();
	  //checking if the line is straight
	  if(chisquare>1 && chisquare<20)lessthanz*=true;
	  else lessthanz*=false;

	   
	  if(lessthanz==true)
	    {
	      goodevents++;
	      //cout<<lessthanz<<" track  "<<" event "<<i<<" "<<itrack<<" "<<chisquare<<endl;
	      double xzparam[2]={xzline->GetParameter(0),xzline->GetParameter(1)};	  
	      double zyparam[2]={zyline->GetParameter(2),zyline->GetParameter(3)};	  
	     
	      for(int j=0;j<numhits;j++)
		{
		  int clusID=hits->at(j)->GetCluster()->GetClusterID();
		  STHitCluster *hit=event->GetCluster(clusID);
		  double xpos=hit->GetPosition().X();
		  double ypos=hit->GetPosition().Y();
		  double zpos=hit->GetPosition().Z();

		  //multiply by two because I halved the sampling rate to display full events in the viewer was
		  // 25  MHz now its 50Mhz that time buckets went from 40ns to 20ns
		  //Thus the  length is artifically scaled by two to fit the window in the viewer. 
		  //check the parameter file to verify the sampling rate is as such.
		  //if the sampling rate is 25MHz then comment the next line out. If 50 then leave the line uncommented 
		  ypos*=2;

		  double expectedx=xzparam[0]*zpos+xzparam[1];//from xz line
		  double expectedy=zyparam[0]*zpos+zyparam[1];//from zy line

		  double diffx=xpos-expectedx;
		  double diffy=ypos-expectedy;	  
		  // cout<<diffx<<" "<<diffy<<endl;
		  Int_t binz=padplane->GetXaxis()->FindBin(zpos);
		  Int_t binx=padplane->GetYaxis()->FindBin(expectedx);
		  //  if(zpos<120)
		  //{
		      xdiff[binz][binx]->Fill(diffx);
		      ydiff[binz][binx]->Fill(diffy);
		      //}
		}
	      
	    }
	  
	}
   
    }
      
  TFile *f = new TFile("driftcorrection.root","RECREATE");
  f->cd();
  
  for(int i=0;i<padzdiv;i++){
    for(int j=0;j<padxdiv;j++){
      xdiff[i][j]->Write();
    }
  }
      
  for(int i=0;i<padzdiv;i++){
    for(int j=0;j<padxdiv;j++){
      ydiff[i][j]->Write();
    }
  }
 
  //      xz->Write();
  // xz->Draw();
      
  cout<<goodevents<<" "<<nentries<<endl;
  return 0 ;    
}





