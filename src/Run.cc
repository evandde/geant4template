#include "Run.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4AnalysisManager.hh"

void Run::RecordEvent(const G4Event *anEvent)
{
    auto HCE = anEvent->GetHCofThisEvent();
    if (!HCE)
        return;

    if (fHCID == -1)
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Detector/EDep");

    auto hitsMap = static_cast<G4THitsMap<G4double> *>(HCE->GetHC(fHCID));

    auto analysisManager = G4AnalysisManager::Instance();

    for (const auto &iter : *(hitsMap->GetMap()))
    {
        auto eDep = *(iter.second);
        if (eDep > 0.)
        {
            analysisManager->FillH1(0, eDep);

            analysisManager->FillNtupleIColumn(0, anEvent->GetEventID());
            analysisManager->FillNtupleDColumn(1, eDep);
            analysisManager->AddNtupleRow();
        }
    }

    G4Run::RecordEvent(anEvent);
}

void Run::Merge(const G4Run *aRun)
{
    G4Run::Merge(aRun);
}
