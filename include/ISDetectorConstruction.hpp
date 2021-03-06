#ifndef ISDetectorConstruction_h
#define ISDetectorConstruction_h 1

#include <vector>

#include <G4GenericMessenger.hh>
#include <G4Material.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4VisAttributes.hh>

class ISDetectorConstruction : public G4VUserDetectorConstruction
{
 public:
  ISDetectorConstruction();
  virtual ~ISDetectorConstruction();

  virtual G4VPhysicalVolume *Construct();
  virtual void ConstructSDandField();

  void SetPlateT(G4double t);

 private:
  G4bool fCheckOverlap;

  void DefineCommands();
  G4GenericMessenger *fMessenger;

  // Materials
  void DefineMaterials();
  G4Material *fVacuumMat;
  G4Material *fInSbMat;

  G4double fPlateT;
  G4VPhysicalVolume *fPlatePV;

  std::vector<G4VisAttributes *> fVisAttributes;
};

#endif
