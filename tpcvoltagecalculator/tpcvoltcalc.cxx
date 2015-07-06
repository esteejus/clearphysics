#include <armadillo>
#include <iostream>
#include <cmath>

using namespace std;
using namespace arma;

//pad plane is located at z=0
float z1=3.96; //average distance of anode plane
float z2=8.011;//avg. dist. ground plane
float z3=14.061;//avg. dist. g.g. plane
float zp=509.5862;//dist of cathode plane units in mm

//============
//Pitch of wires
//============
float s1=4;//andoe wire pitch
float s2=1;//ground wire pitch
float s3=1;//gating grid pitch

//Radii of wires in mm
float rs=.01; //anode
float rz=.0381;//ground
float rg=.0381;//g.g.
double e_o=8.854e-15;//farads/mm
double pi=3.14159265359;



int main(){
  float vs=1400,vz=0,vg=-100,vp=-6294;//andoe,ground,g.g.,cathode, in volts
  float efield=12.5;//desired e field V/mm 
  /* g
  cout<<"Enter voltage of sense(anode) wire "<<endl;
  cin>>vs;
  cout<<"Enter ground "<<endl;
  cin>>vz;
  cout<<"Enter gating grid "<<endl;
  cin>>vg;
  cout<<"Enter cathode "<<endl;
  cin>>vp;
  */

  //void calsig(mat &capmatrix  
  //==========
  //Solving for charge densities
  //==========
  mat capmatrix=Mat<double>(4,4);
  capmatrix.zeros();
  capmatrix(0,0)=z1-(s1/(2*pi))*log(2*pi*rs/s1);
  capmatrix(0,1)=z1;
  capmatrix(0,2)=z1;
  capmatrix(0,3)=z1;
  
  capmatrix(1,0)=z1;
  capmatrix(1,1)=z2-(s2/(2*pi))*log(2*pi*rz/s2);
  capmatrix(1,2)=z2;
  capmatrix(1,3)=z2;

  capmatrix(2,0)=z1;
  capmatrix(2,1)=z2;
  capmatrix(2,2)=z3-(s3/(2*pi))*log(2*pi*rg/s3);
  capmatrix(2,3)=z3;

  capmatrix(3,0)=z1;
  capmatrix(3,1)=z2;
   capmatrix(3,2)=z3;
  capmatrix(3,3)=zp;

  capmatrix=capmatrix*(1/e_o);
  cout<<endl;
  mat invcapmatrix=inv(capmatrix);
  invcapmatrix.print();
  cout<<endl;
  vec volt=zeros<vec>(4);
  //voltage vector should be vs,vz,vg,vp
  volt(0)=vs;
  volt(1)=vz;
  volt(2)=vg;
  volt(3)=0;//for calc of vinf
  volt.print();
  //invcapmatrix.print();
 vec sig=invcapmatrix*volt;
  float vinf=(sig(0)*z1+sig(1)*z2+sig(2)*z3)*(1/e_o);
  cout<<"vinf "<<vinf<<endl;
  //sig0 is sigsignal,sig1 is sigz, sig2 is siggating, sig3 is sigp
  cout<<volt(0)<<" "<<volt(1)<<" "<<volt(2)<<" "<<volt(3)<<endl;
  cout<<sig(0)<<" "<<sig(1)<<" "<<sig(2)<<endl;

  volt(3)=vp;//reset voltage
  sig=invcapmatrix*volt;//real sigma

  float vgg=0;
  float vzeff=vz+(s2/(2*pi*e_o))*sig(1)*log(2*pi*rz/s2);
  cout<<"vzeff "<<vzeff<<endl;
  float vcath=(-efield*(zp-z2))+vinf;
  cout<<"vcath "<<vcath<<endl;

  vzeff=0;
  //===========
  //Solve for transparency
  //===========
bool direction=false;//true denotes more negative direction; false points to more positive direction
 bool minfound=false;
 double lambdagate=0,sigp=0,plambgate=0;
 vgg=vg;
double stepsize =.0001; //voltage step size, in volts

 while(minfound==false){
   //   cout<<vgg<<endl;
  if(direction==false) vgg+=stepsize;
   else vgg-=stepsize;

  //    float vzeff=vz+(s2/(2*pi*e_o))*sig(1)*log(2*pi*rz/s2);
  //cout<<vzeff<<endl;
    volt(1)=vzeff;//must use effective voltage for setting grid VERY IMPORTANT for transparency calculation
    volt(2)=vgg;//vary voltage for gating grid
    sig=invcapmatrix*volt;//calculate sigma again for effective voltages
    lambdagate=sig(2)*s3;//calculate linear charge density on gatting grid
    sigp=sig(3);//charge density on HV cathode

    //conditions for finding the amount of positive charge on the gating grid wire
    //see rolandi and blum for reference
    if((lambdagate/(2*pi*rg))<(-2*efield*e_o)){
      plambgate=0;
      direction=false;//could be too much negative charge; go in postive direction to find minimum
      //could also be at minimum; will check that latter
    }
    else if((lambdagate/(2*pi*rg))>(2*efield*e_o)){
      plambgate=lambdagate;
      direction=true;//too much positive charge; go in negative direciton
    }
  else if((lambdagate/(2*pi*rg))>(-2*efield*e_o)&&(lambdagate/(2*pi*rg))<(2*efield*e_o)){
    double theta=0;
    theta=acos(-lambdagate/(4*pi*e_o*efield*rg));
    plambgate=(lambdagate*theta/pi)+4*efield*e_o*rg*sin(theta);
    direction=true;//some positve charge on grid; to find T=1 go in negative direction
      }

    //the minimum condition for the gating grid given in rolandi and blum
    //l/2*pi*r=-2*E*e_o
    //since the units are so small e_o~10-15 Farads/mm ive divided by 2*E*e_o
    //to get the fractional difference
    double fracdiff=abs((lambdagate/(2*pi*rg))-(-2*efield*e_o))/(2*efield*e_o);
    if(fracdiff<.0001)minfound=true;//if difference bewtween the positive & negative charge is beblow .01% you found the minimum
    
 }


 //Calculate the transparency of the grid.
    float transparency=0;
    transparency=1-((plambgate/s3)/abs(sigp));
    cout<<"gatinggrid volt "<<vgg<<" transp "<<transparency<<endl;



    cout<<"SET SETTINGS TO"<<endl;
    cout<<"Cathode voltage "<<vcath<<endl;
    cout<<"Gating grid "<<vgg<<endl;
    
  return 0;

  }
