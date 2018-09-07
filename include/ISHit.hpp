#ifndef ISHit_h
#define ISHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"


class ISHit : public G4VHit
{
public:
   ISHit();
   virtual ~ISHit();
   ISHit(const ISHit &right);
   const ISHit &operator=(const ISHit &right);
   int operator==(const ISHit &right) const;

   inline void *operator new(size_t);
   inline void  operator delete(void *);

   // add setter/getter methods

private:

};

typedef G4THitsCollection<ISHit> ISHitsCollection;

extern G4ThreadLocal G4Allocator<ISHit> *ISHitAllocator;

inline void *ISHit::operator new(size_t)
{
   if (!ISHitAllocator)
      ISHitAllocator = new G4Allocator<ISHit>;
   return (void *)ISHitAllocator->MallocSingle();
}

inline void ISHit::operator delete(void *hit)
{
   ISHitAllocator->FreeSingle((ISHit *) hit);
}

#endif
