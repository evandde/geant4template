#include "RunAction.hh"
#include "Run.hh"

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
    : G4UserRunAction()
{
}

RunAction::~RunAction()
{
}

G4Run *RunAction::GenerateRun()
{
    return new Run;
}

void RunAction::BeginOfRunAction(const G4Run *aRun)
{
    G4RunManager::GetRunManager()->SetPrintProgress(static_cast<G4int>(aRun->GetNumberOfEventToBeProcessed() * .1));
}

void RunAction::EndOfRunAction(const G4Run *aRun)
{
    auto nOfEvents = aRun->GetNumberOfEvent();
    if (nOfEvents == 0)
        return;

    auto theRun = static_cast<const Run *>(aRun);
    auto eDep = theRun->GetEDep();

    G4cout << "--- Run results ---\n"
           << "Total events in this run: " << nOfEvents << "\n"
           << "Deposited energy per primary [MeV]: " << (eDep / nOfEvents) / MeV << "\n\n";
}
