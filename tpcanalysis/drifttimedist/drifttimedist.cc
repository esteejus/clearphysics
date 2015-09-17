
/*#ifndef __CINT__
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TH2D.h>
#include <iostream>
#include <TMath.h>
#include "STEvent.hh"
#include "STHit.hh"
#include <TVector3.h>


using namespace std;

double calc_derivative(TH1D *);
void drifttimedist();
#endif

#ifndef __CINT__
int main(){
  drifttimedist();
  return 0;
}

#endif
*/
  /*
====================================



=====================================
  */

  void drifttimedist(){
    gStyle->SetOptStat(0);

    Double_t driftvel=54.3/2;//mm/us divide by two because I halved the sampling rate to display full events in the viewer was 50  MHz now its 25Mhz that time buckets went from 20ns to 40ns.

    const int padxdiv=20,padzdiv=112;
    //    double zbins[16]={0,25,50,100,200,300,400,500,600,700,800,900,1000,1200,1300,1400};  

  TH1D *time[padzdiv][padxdiv]; 
  for(int i=0;i<padzdiv;i++)
    for(int j=0;j<padxdiv;j++)
      { 
	{
	  TString name =Form("timedist_%i_%i",i,j);
	  //cout<<name<<endl;
	  time[i][j]= new TH1D(name,name,1000,0,20);
	}
      }
  TH2D *padplane = new TH2D("padplane","padplane",padzdiv,0,1344,padxdiv,-432,432);
  // TFile *_file0 = TFile::Open("/cache/scr/spirit/spiritroot_repo/macros/cosmic_2015_badresistor.root");
  //TTree *cbmsim = (TTree *) _file0 -> Get("cbmsim");
  //cbmsim -> Print();

TChain *chain = new TChain("cbmsim");
  //TFile *_file0 = TFile::Open("/cache/scr/spirit/spiritroot_repo/macros/cosmic_threshold50.root");
//   chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_342.root");
   //  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_340.root");
   //  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_334.root");
   //  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_336.root");
   //  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_335.root");
   //  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_338.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0346_riemann.root");
  chain->Add("/cache/scr/spirit/spiritroot_repo/macros/cosmic_badresistor_run0346_riemann_1.root");
  

  TClonesArray *eventArray=NULL; //array of objects

  chain -> SetBranchAddress("STEventH", &eventArray);//Store array of events into TClonesArray
  Int_t nentries=chain->GetEntries();
    bool flag=false;//if any no event between time buckets then true
    cout<<"Num events "<<nentries<<endl;//nentries=16200;


 
  for(int i=0;i<nentries;i++)
    {

      if(i%1000==0)cout<<i<<endl;
      chain -> GetEntry(i);//This changes the event index
      STEvent *event;
      event = (STEvent *) eventArray -> At(0);//there is only one event in each index
      Int_t nhits=event -> GetNumHits();//total number of hits

      if(event->IsGood()==false)
	{
	  cout<<"Event "<<i<<" is not good!!!"<<endl;
	  continue;
	}

   
      for(int j=0;j<nhits;j++)
	{
	  STHit *hit = event->GetHit(j);//increase hit
	  TVector3 vect=(TVector3)hit->GetPosition();
	  Double_t xpos=vect.X();
	  Double_t zpos=vect.Z();
	  Double_t ypos=vect.Y();
	  Double_t drifttime=-ypos/driftvel;
	  Double_t hitcharge=hit->GetCharge();
	  Int_t binz=padplane->GetXaxis()->FindBin(zpos);
	  Int_t binx=padplane->GetYaxis()->FindBin(xpos);
	  
	  time[binz-1][binx-1]->Fill(drifttime);
	  //	  cout<<binz-1<<" "<<xpos<<" "<<binx-1<<" "<<drifttime<<endl;
	}

    }

  //  time->Draw();
  //padplane->Draw("colz") 

  /*
 for(int i=0;i<padzdiv;i++)
    for(int j=0;j<padxdiv;j++)
      { 
	{
	  double drifttime=calc_derivative(time[i][j]);
	  padplane->SetBinContent(i+1,j+1,drifttime);
	}
      }
  
  */
  // padplane->Draw("colz");
  
 TFile *f = new TFile("timedist_realpadplane.root","RECREATE");
 f->cd(); 
for(int i=0;i<padzdiv;i++)
    for(int j=0;j<padxdiv;j++)
      { 
	{
       	  time[i][j]->Write();
	}
      }
 padplane->Write();
  f->Write();
  f->Close();


}



double calc_derivative(TH1D *time){

  int nbins=time->GetNbinsX();
  double content=0;
  double starttime=0,endtime=0;

  TH1D *integral = new TH1D("integral","integral",nbins,0,20);

  for(int i=0;i<nbins;i++)
    {
      content+=time->GetBinContent(i);
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

  integral->Delete();
  return (endtime-starttime);

}

