#include "SensitiveDetector.hh"

#include "G4SDManager.hh"

SensitiveDetector::SensitiveDetector(G4String detName)
    : G4VSensitiveDetector(detName), fHitsMap(nullptr), fHCName("DepositedEnergy")
{
    collectionName.insert("DepositedEnergy");
}

SensitiveDetector::~SensitiveDetector()
{
}

void SensitiveDetector::Initialize(G4HCofThisEvent *hce)
{
    fHitsMap = new G4THitsMap<G4double>(GetName(), fHCName);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(fHCName);
    hce->AddHitsCollection(hcID, fHitsMap);
}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    G4double dE = aStep->GetTotalEnergyDeposit();
    if (dE == 0.)
        return false;

    fHitsMap->add(0, dE);

    return true;
}