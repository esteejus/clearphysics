void test()
{
  TEveManager::Create();


  // Get Geometry
  TFile* file = new TFile("~/fair_install/SpiRITROOT/geometry/geomSPiRIT.root","read");
  file -> Get("SPiRIT");

  TGeoNode* geoNode = gGeoManager -> GetTopNode();
  TEveGeoTopNode* topNode = new TEveGeoTopNode(gGeoManager,geoNode);

  gEve -> AddGlobalElement(topNode);




  // Example of line set.
  TEveStraightLineSet* lines = new TEveStraightLineSet("lines");
  lines -> SetLineColor(46);
  lines -> AddLine(0,0,0, 0,-20,50);
  lines -> AddLine(0,0,0, 0,-25,50);
  lines -> AddLine(0,0,0, 0,-30,50);
  gEve -> AddElement(lines);




  // Example of point set.
  TEvePointSet* points = new TEvePointSet("points");
  points -> SetMarkerColor(kGray+2);
  points -> SetNextPoint(0,-20,50);
  points -> SetNextPoint(0,-25,50);
  points -> SetNextPoint(0,-30,50);
  gEve -> AddElement(points);




  // Draw
  gEve -> FullRedraw3D(kTRUE);
  gEve -> GetDefaultGLViewer() -> SetClearColor(kWhite);
}
