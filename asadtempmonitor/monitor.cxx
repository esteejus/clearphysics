#include <fstream>
#include <iostream>
#include <cstdlib>

#include "TString.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TPaveText.h"

using namespace std;

void monitor() {
  //  TApplication app("mon", &argc, argv);

  gStyle->SetOptStat(0);
  ifstream logfile;
  logfile.open("asad-monitoring.log");
  string temp="";
  int par[11]={};

  //start for 06/26 149818 to 301225
  
  int startline=15350;//actual start43143;
  int finishline=15393;
  string junk;
  string at="";//for temp storage of the .at() function
  int starttime=0;//in seconds
  int currenttime=0;//in seconds
  int elapsedtime=0;//secs
  //char character='\n';//for loop
  bool nextgraphic=false;//true makes a new graphic; false does not

  TH2D *tempmap=new TH2D("h1","temp map of asad",4,0,4,12,0,12);
  TCanvas *c1 = new TCanvas("c1","c1",1);
  //TPaveText *text=new TPaveText(.75,.75,.85,.85,"ndc");
  int linecount=0;
  for(int l=0;l<startline;l++){getline(logfile,junk);linecount++;}
  //cout<<junk<<endl;


  for(int j=0;j<finishline-startline;j++){
    //cout<<j<<endl;
    cout<<"On line "<<linecount<<endl;
   
  for(int i=0;i<9;i++){
      logfile>>temp;
      // cout<<temp<<endl;
      //if(temp==""){cout<<"blank space"<<endl;par[i]=0;}
      if(i==0){
	//ip address for cobo
	//starts from ####.##.40 to ###.##.46
	at=temp.at(8);
	at+=temp.at(9);
	par[i]=atoi(at.c_str());
      }

      else if(i==1){at=temp.at(4); par[i]=atoi(at.c_str());}
      else if(i==3){
        at=temp.at(0);
	at+=temp.at(1);
	par[i]=atoi(at.c_str());//hours
	at=temp.at(3);
	at+=temp.at(4);
	par[9]=atoi(at.c_str());//mins
	at=temp.at(6);
	at+=temp.at(7);
	par[10]=atoi(at.c_str());//seconds
      }
      else par[i]=atoi(temp.c_str());//two temps stored in par[7] & par[8}

    }

           
    if(j==0)starttime=par[3]*3600+par[9]*60+par[10];
    int lasttime=currenttime;//time from last loop
    currenttime=par[3]*3600+par[9]*60+par[10];
    elapsedtime=currenttime-starttime;
    if(currenttime!=lasttime)nextgraphic=true;//if the time has changed make a new graphic
    else nextgraphic=false;
   
    
    //cobomap top view
    //========
    //<<Upstream beam left
    //c4a1//c5a3//c6a2
    //c4a0//c5a2//c6a1
    //c3a3//c5a1//c6a0
    //c3a2//c5a0//----
    //.
    //.
    //.
    //c0a0//c1a2//----
    //there are only 4 asad boards
    //near the top there are only partial cobos; i.e. c4a1 is the max 
    //Thus cobo0asad0 goes to TH2D (0,0)
    //
    int coboid=par[0]-40;//starts at 40
    int binx=99,biny=99;
    int asadid=par[1];

    
    if(coboid==0 && asadid==0){
      binx=0;biny=0;
    }
    else if(coboid==0 && asadid==1){
      binx=0;biny=1;
    }
    else if(coboid==0 && asadid==2){
      binx=0;biny=2;
    }
    else if(coboid==0 && asadid==3){
      binx=0;biny=3;
    }
    else if(coboid==1 && asadid==0){
      binx=0;biny=4;
    }
    else if(coboid==1 && asadid==1){
      binx=0;biny=5;
    }
    else if(coboid==3 && asadid==0){
      binx=0;biny=6;
    }
    else if(coboid==3 && asadid==1){
      binx=0;biny=7;
    }
    else if(coboid==3 && asadid==2){
      binx=0;biny=8;
    }
    else if(coboid==3 && asadid==3){
      binx=0;biny=9;
    }
    else if(coboid==4 && asadid==0){
      binx=0;biny=10;
    }
    else if(coboid==4 && asadid==1){
      binx=0;biny=11;
    }
    else if(coboid==1 && asadid==2){
      binx=1;biny=0;
    }
    else if(coboid==1 && asadid==3){
      binx=1;biny=1;
    }
     else if(coboid==2 && asadid==0){
      binx=1;biny=2;
    }
    else if(coboid==2 && asadid==1){
      binx=1;biny=3;
    }
    else if(coboid==2 && asadid==2){
      binx=1;biny=4;
    }
    else if(coboid==2 && asadid==3){
      binx=1;biny=5;
    }
    else if(coboid==4 && asadid==2){
      binx=1;biny=6;
    }
    else if(coboid==4 && asadid==3){
      binx=1;biny=7;
    }
    else if(coboid==5 && asadid==0){
      binx=1;biny=8;
    }
    else if(coboid==5 && asadid==1){
      binx=1;biny=9;
    }
    else if(coboid==5 && asadid==2){
      binx=1;biny=10;
    }
    else if(coboid==5 && asadid==3){
      binx=1;biny=11;
    }
    else if(coboid==6 && asadid==0){
      binx=2;biny=9;
    }
     else if(coboid==6 && asadid==1){
      binx=2;biny=10;
    }
    else if(coboid==6 && asadid==2){
      binx=2;biny=11;
    }
    else {cout<<"Cobo was not mounted"<<endl;cout<<"cobo id"<<coboid<<" asad "<<asadid<<endl;}
   
   
//    string timetitle=to_string(par[3])+":"+to_string(par[9])+":"+to_string(par[10]);//time string for histogram title
    TString timetitle=Form("%d:%d:%d", par[3], par[9], par[10]);//time string for histogram title
//    string elaptime=to_string(elapsedtime);//+" seconds";
    TString elaptime = Form("%d", elapsedtime);//+" seconds";
    
    //    TH2D *tempmap=new TH2D("h1","temp map of asad",4,0,4,12,0,12);
    //If for skipping blank data
    if(par[4]==0){
      j++;
      linecount++;
        cout<<"Skipped this line"<<endl<<endl;
      continue;//skip the blank lines with no data
    }
    //Write to histogram
    else{
      //always update the histogram with average of two sensors on one asad
      	float tempavg=.5*(par[7]+par[8]);//units in degrees C
	//if(binx==0 && biny==0)cout<<"Temp avg "<<tempavg;//cout<<binx<<" "<<biny<<endl;
	tempmap->SetBinContent(binx+1,biny+1,tempavg);
	tempmap->GetZaxis()->SetRangeUser(20,45);//units in degrees C
	
	if(nextgraphic==true){
	  TPaveText *text =new TPaveText(.7,.85,.90,1,"ndc");
//	  text->AddText(elaptime.c_str());
	  text->AddText(elaptime);
	  //if time changed we need to write a new image
	  c1->cd();
//	  tempmap->SetTitle(timetitle.c_str());
	  tempmap->SetTitle(timetitle);
	  tempmap->Draw("same colz TEXT");
	  text->Draw("same");
//	  string filename="./histpics/"+elaptime+".jpg";
//	  c1->SaveAs(filename.c_str());
	  TString filename="./histpics/"+elaptime+".jpg";
	  c1->SaveAs(filename);
	  //cout<<"saved graphic"<<endl;
	  //cout<<"Im on line "<<linecount<<endl;
	  nextgraphic=false;//reset bool flag
	}
	
      
    }
      
    linecount++;    
    
  }//end of read loop
 
  
  
  
  
  //  app.Run();
}

#ifndef __CINT__
int main(int argc, char **argv){
  monitor();
  
  return 0;
}
#endif
