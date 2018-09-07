#include <G4SystemOfUnits.hh>
#include <G4AutoLock.hh>
#include <G4ParticleTable.hh>
#include <G4ChargedGeantino.hh>
#include <G4IonTable.hh>
#include <Randomize.hh>
#include <g4root.hh>

#include "ISPrimaryGeneratorAction.hpp"

static G4int nEveInPGA = 0;
G4Mutex mutexInPGA = G4MUTEX_INITIALIZER;


ISPrimaryGeneratorAction::ISPrimaryGeneratorAction()
   : G4VUserPrimaryGeneratorAction(),
     fParticleGun(nullptr)
{
   fParticleGun = new G4ParticleGun(1);
}

ISPrimaryGeneratorAction::~ISPrimaryGeneratorAction()
{
   delete fParticleGun;
}

void ISPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
   fParticleGun->GeneratePrimaryVertex(event);

   G4AutoLock lock(&mutexInPGA);
   if (nEveInPGA++ % 10000 == 0)
      G4cout << nEveInPGA - 1 << " events done" << G4endl;
}
