#include "RunAction.hh"
#include "Run.hh"

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericAnalysisManager.hh"

RunAction::RunAction()
    : G4UserRunAction()
{
    auto analysisManager = G4GenericAnalysisManager::Instance();

    analysisManager->SetDefaultFileType("csv");
    analysisManager->SetFileName("Result");

    analysisManager->CreateH1("EDep", "Energy Deposition", 1024, 0., 3.);

    analysisManager->CreateNtuple("EDep", "Energy Deposition");
    analysisManager->CreateNtupleIColumn("EvtID");
    analysisManager->CreateNtupleDColumn("E(MeV)");
    analysisManager->FinishNtuple();
}

G4Run* RunAction::GenerateRun()
{
    return new Run;
}

void RunAction::BeginOfRunAction(const G4Run *)
{
    G4RunManager::GetRunManager()->SetPrintProgress(static_cast<G4int>(G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed() * .1));

    auto analysisManager = G4GenericAnalysisManager::Instance();

    analysisManager->OpenFile("Result.csv");
}

void RunAction::EndOfRunAction(const G4Run *)
{
    auto analysisManager = G4GenericAnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();
}
