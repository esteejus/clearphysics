#include "MediumMagboltz.hh"

using namespace Garfield;
using namespace std;

int main(){

  // Describe the gas: composition, pressure, temperature
MediumMagboltz* gas = new MediumMagboltz();
gas->SetComposition("ar",90.0,"ic4h10",10.0);
gas->SetTemperature(293.15);
gas->SetPressure(760.);

// Ask for calculations from 1 kV/cm to 1.5 MV/cm in 40 steps
gas->SetFieldGrid(1000., 1500000., 40, true, 0., 0., 1, 0., 0., 1);

// Check that these settings were transmitted.
gas->EnableDebugging();
gas->Initialise();  
gas->DisableDebugging();

// No Penning transfer
const double lP = 0.0;
const double rP = 0.0;
gas->EnablePenningTransfer(rP, lP, "ar");

// Run Magboltz with moderate precision
gas->GenerateGasTable(3, true);

// Write the gas file
gas->WriteGasFile("ar-90-ic4h10-10-p1.gas");

 return 0;

}
