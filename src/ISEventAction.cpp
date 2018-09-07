#include <G4Event.hh>
#include <G4SDManager.hh>
#include <g4root.hh>

#include "ISEventAction.hpp"


ISEventAction::ISEventAction()
   : G4UserEventAction(),
     fHitsCollectionID(-1)
{}

ISEventAction::~ISEventAction()
{}

ISHitsCollection *ISEventAction::GetHitsCollection(G4int hcID, const G4Event *event)
const
{
   ISHitsCollection *hitsCollection 
      = static_cast<ISHitsCollection *>(
         event->GetHCofThisEvent()->GetHC(hcID));
  
   if ( ! hitsCollection ) {
      G4ExceptionDescription msg;
      msg << "Cannot access hitsCollection ID " << hcID;
      // check how to use G4Exception
      G4Exception("ISEventAction::GetHitsCollection()",
                  "ISCode0003", FatalException, msg);
   }         

   return hitsCollection;
}

void ISEventAction::BeginOfEventAction(const G4Event *)
{}

void ISEventAction::EndOfEventAction(const G4Event *event)
{
   if (fHitsCollectionID == -1)
      fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("HC");
   
   ISHitsCollection *hc = GetHitsCollection(fHitsCollectionID, event);
   const G4int kHit = hc->entries();
   for (G4int iHit = 0; iHit < kHit; iHit++) {
      ISHit *newHit = (*hc)[iHit];
   }

}

