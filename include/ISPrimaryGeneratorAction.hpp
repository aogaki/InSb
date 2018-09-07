#ifndef ISPrimaryGeneratorAction_h
#define ISPrimaryGeneratorAction_h 1

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <G4ThreeVector.hh>
#include <G4Threading.hh>
#include <G4Event.hh>
#include <G4GenericMessenger.hh>


class ISPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
   ISPrimaryGeneratorAction();
   virtual ~ISPrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event *);

private:
   G4ParticleGun *fParticleGun;

};

#endif
