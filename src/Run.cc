#include "Run.hh"

#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"

Run::Run()
    : G4Run(), fEDepHCID(-1), fEDep(0.)
{
}

Run::~Run()
{
}

void Run::RecordEvent(const G4Event *anEvent)
{
    if (fEDepHCID == -1)
        fEDepHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Detector/EDep");

    auto HCE = anEvent->GetHCofThisEvent();
    if (!HCE)
        return;

    auto hitsMap = static_cast<G4THitsMap<G4double> *>(HCE->GetHC(fEDepHCID));
    for (const auto &iter : *(hitsMap->GetMap()))
    {
        auto eDep = *(iter.second);
        if (eDep > 0.)
            fEDep += eDep;
    }

    G4Run::RecordEvent(anEvent);
}

void Run::Merge(const G4Run *aRun)
{
    auto localRun = static_cast<const Run *>(aRun);
    fEDep += localRun->fEDep;

    G4Run::Merge(aRun);
}
