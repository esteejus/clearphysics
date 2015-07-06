//THIS IS NOT IN THE TPC HIT COOR SPACE
//GIVES WEIRD LOOKING TRACKS. USE SPIRIT ROOT ROOTFILE


void event_viewer(int eventnum=15)
{

   TEveManager::Create();
	      
	      
  // Get Geometry
  TFile* file = new TFile("~/fair_install/SpiRITROOT/geometry/geomSPiRIT.root","read");
  file -> Get("SPiRIT");
	      
  TGeoNode* geoNode = gGeoManager -> GetTopNode();
  TEveGeoTopNode* topNode = new TEveGeoTopNode(gGeoManager,geoNode);
	      
  gEve -> AddGlobalElement(topNode);

	      
  Double_t yCage      =  51.01; //cm dist of pad plane to cathode
  Int_t timeoffset    = 389.0;  // in time buckets. represents bottom of cathode
                                // must get this value from ADCIdx distribution
                                // it is where the last sharp cut off is in the ADC dist
  Double_t timebucket =    .04; //units in micro seconds
  Double_t driftvel   =   5.46; //drift velocity cm/microsecond
  Int_t adcthreshold  =  25;//max adc threshold

  STCore *c = new STCore("~/tpcdata/run_0334.dat");
  c -> SetAGETMap("/home/justin/clearphysics/tpcanalysis/asad_get_maps/06_07_2015_cosmic/AGET.map");
  c -> SetUAMap("/home/justin/clearphysics/tpcanalysis/asad_get_maps/06_07_2015_cosmic/UnitAsAd.map");
  c -> SetFPNPedestal();
  c -> SetData(0);

  STRawEvent *event;
  event = c -> GetRawEvent(eventnum);

  // Example of point set.
  TEvePointSet* points = new TEvePointSet("points");
  points -> SetMarkerColor(kGray+2);
  int numpads=event->GetNumPads();

  for(int i=0;i<numpads;i++)
    {

      STPad *pad;
      pad=event->GetPad(i);
      int maxidx=pad->GetMaxADCIdx();
      double maxadc=pad->GetADC(maxidx);
      int layer=pad->GetLayer();
      int row=pad->GetRow();
      ++layer;//root starts with (1,1) not (0,0)
      ++row;
      //define center of pad
      Double_t zpos=layer*.6;//6mm is half the pad in layer direction
      Double_t xpos=row*.4;//4mm half in row direction
      Double_t roughypos=-driftvel*(maxidx)*timebucket;//neg y toward cath
      roughypos+=(timeoffset*driftvel*timebucket)-yCage;//roughypos was 0 when maxidx=timeoffset which is cathode
      //bad asad from 70<layer<75 && 81<row<90 gives signal when no events

      bool badregion = ((layer<=75 && layer>70 && row<=90 && row>81));//eval to true in bad region
      if(maxadc>adcthreshold && badregion==false)
	{   
	  /*
	  // Example of line set.
	  TEveStraightLineSet* lines = new TEveStraightLineSet("lines");
	  lines -> SetLineColor(46);
	  lines -> AddLine(0,0,0, 0,-20,50);
	  lines -> AddLine(0,0,0, 0,-25,50);
	  lines -> AddLine(0,0,0, 0,-30,50);
	  gEve -> AddElement(lines);
	  */
	      
	      
	      
	 
	  //if(maxidx>150 && maxidx<185)
	    points -> SetNextPoint(xpos,roughypos,zpos);
	  //points -> SetNextPoint(0,-30.1,50);
	  // points -> SetNextPoint(0,-30,50);
	  
	      
  
	  cout<<"x pos "<<xpos<<" ypos "<<roughypos<<" zpos "<<zpos<<'\n';
	 
	}
      
    }
  gEve -> AddElement(points);
  // Draw
  gEve -> FullRedraw3D(kTRUE);
  gEve -> GetDefaultGLViewer() -> SetClearColor(kWhite);
}
