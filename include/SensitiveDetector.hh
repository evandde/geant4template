#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4THitsMap.hh"

class SensitiveDetector: public G4VSensitiveDetector
{
public:
    SensitiveDetector(G4String);
    virtual ~SensitiveDetector() override;

    virtual void Initialize(G4HCofThisEvent*) override;
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;

private:
    G4THitsMap<G4double>* fHitsMap;
    G4String fHCName;
};

#endif
