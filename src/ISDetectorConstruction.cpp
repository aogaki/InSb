#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4RunManager.hh>
#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4Tubs.hh>

#include "ISDetectorConstruction.hpp"
#include "ISSD.hpp"

ISDetectorConstruction::ISDetectorConstruction() : fVacuumMat(nullptr)
{
  fCheckOverlap = true;

  fPlateT = 500 * um;

  DefineMaterials();
  DefineCommands();
}

ISDetectorConstruction::~ISDetectorConstruction() {}

void ISDetectorConstruction::DefineMaterials()
{
  G4NistManager *manager = G4NistManager::Instance();

  // NIST database materials
  fVacuumMat = manager->FindOrBuildMaterial("G4_Galactic");

  auto In = manager->FindOrBuildElement("In");
  auto Sb = manager->FindOrBuildElement("Sb");

  auto InSb = new G4Material("InSb", 5.775 * g / cm3, 2, kStateSolid);
  InSb->AddElement(In, 1);
  InSb->AddElement(Sb, 1);

  fInSbMat = InSb;
}

G4VPhysicalVolume *ISDetectorConstruction::Construct()
{
  // world volume
  G4double worldX = 0.1 * m;
  G4double worldY = 0.1 * m;
  G4double worldZ = 0.6 * m;

  G4Box *worldS = new G4Box("World", worldX / 2., worldY / 2., worldZ / 2.);
  G4LogicalVolume *worldLV = new G4LogicalVolume(worldS, fVacuumMat, "World");

  G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour::White());
  visAttributes->SetVisibility(false);
  worldLV->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  // InSb plate
  auto plateS = new G4Box("Plate", worldX / 2., worldY / 2., fPlateT / 2.);
  auto plateLV = new G4LogicalVolume(plateS, fInSbMat, "Plate");
  visAttributes = new G4VisAttributes(G4Colour::Cyan());
  plateLV->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  // auto pos = G4ThreeVector(0., 0., -fPlateT / 2.);
  auto pos = G4ThreeVector(0., 0., 0.);
  fPlatePV = new G4PVPlacement(nullptr, pos, plateLV, "Plate", worldLV, false,
                               0, fCheckOverlap);

  G4VPhysicalVolume *worldPV = new G4PVPlacement(
      nullptr, G4ThreeVector(), worldLV, "World", 0, false, 0, fCheckOverlap);

  return worldPV;
}

void ISDetectorConstruction::ConstructSDandField()
{
  // Sensitive Detectors
  G4VSensitiveDetector *SD = new ISSD("SD", "HC");
  G4SDManager::GetSDMpointer()->AddNewDetector(SD);
  SetSensitiveDetector("Plate", SD);
}

void ISDetectorConstruction::DefineCommands()
{
  fMessenger = new G4GenericMessenger(this, "/InSb/Geo/", "Plate control");

  // Thickness
  G4GenericMessenger::Command &scintiTCmd = fMessenger->DeclareMethodWithUnit(
      "T", "um", &ISDetectorConstruction::SetPlateT,
      "Set the thickness of plate.");
  scintiTCmd.SetParameterName("t", true);
  scintiTCmd.SetRange("t>=0. && t<10000.");
  scintiTCmd.SetDefaultValue("0.5");
}

void ISDetectorConstruction::SetPlateT(G4double t)
{
  fPlateT = t;
  G4Box *plate = (G4Box *)(fPlatePV->GetLogicalVolume()->GetSolid());
  plate->SetZHalfLength(fPlateT / 2.);

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
