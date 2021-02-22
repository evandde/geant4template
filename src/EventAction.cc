#include "EventAction.hh"

#include "G4Event.hh"
#include "G4AutoLock.hh"
#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4SystemOfUnits.hh"

namespace
{
    G4Mutex aMutex = G4MUTEX_INITIALIZER;
}
std::ofstream EventAction::ofs;

EventAction::EventAction()
    : G4UserEventAction(), fHCID(-1)
{
    G4AutoLock lock(&aMutex);
    if (!ofs.is_open())
    {
        ofs.open("out.txt");
        ofs << "# Header\n";
        ofs << "# EvtID\tDE(MeV)\n";
    }
}

EventAction::~EventAction()
{
    G4AutoLock lock(&aMutex);
    if (ofs.is_open())
        ofs.close();
}

void EventAction::EndOfEventAction(const G4Event *anEvent)
{
    if (fHCID == -1)
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Detector/DepositedEnergy");

    auto hce = anEvent->GetHCofThisEvent();
    if (!hce)
        return;

    auto hitsMap = static_cast<G4THitsMap<G4double> *>(hce->GetHC(fHCID));
    if (hitsMap->entries() == 0)
        return;

    G4AutoLock lock(&aMutex);
    ofs << anEvent->GetEventID() << "\t"
        << *(*hitsMap)[0] / MeV << "\n";
}