#include <G4AutoLock.hh>
#include <G4ChargedGeantino.hh>
#include <G4IonTable.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>
#include <Randomize.hh>
#include <g4root.hh>

#include "ISPrimaryGeneratorAction.hpp"

static G4int nEveInPGA = 0;
G4Mutex mutexInPGA = G4MUTEX_INITIALIZER;

ISPrimaryGeneratorAction::ISPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr)
{
  fParticleGun = new G4ParticleGun(1);

  auto parTable = G4ParticleTable::GetParticleTable();

  auto particle = parTable->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(particle);

  auto sourcePos = -5. * cm;
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., sourcePos));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

  fEnergy = 1. * keV;
  fParticleGun->SetParticleEnergy(fEnergy);

  DefineCommands();
}

ISPrimaryGeneratorAction::~ISPrimaryGeneratorAction() { delete fParticleGun; }

void ISPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
  fParticleGun->GeneratePrimaryVertex(event);

  G4AutoLock lock(&mutexInPGA);
  if (nEveInPGA++ % 10000 == 0)
    G4cout << nEveInPGA - 1 << " events done" << G4endl;
}

void ISPrimaryGeneratorAction::DefineCommands()
{
  fMessenger = new G4GenericMessenger(this, "/InSb/Primary/", "Beam control");

  // Kinetic energy
  G4GenericMessenger::Command &setEneCmd = fMessenger->DeclareMethodWithUnit(
      "K", "keV", &InSbPrimaryGeneratorAction::SetEnergy,
      "Set the kinetic energy.");

  setEneCmd.SetParameterName("K", true);
  setEneCmd.SetRange("K>=1. && k<10000.");
  setEneCmd.SetDefaultValue("10");
}

void ISPrimaryGeneratorAction::SetEnergy(G4double ene)
{
  fEnergy = ene;
  fParticleGun->SetParticleEnergy(fEnergy);
}
