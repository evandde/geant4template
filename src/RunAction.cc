#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "g4csv.hh"

#include "RunAction.hh"

RunAction::RunAction()
    : G4UserRunAction()
{
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->CreateH1("EDep", "Energy Deposition", 1024, 0., 3.);

    analysisManager->CreateNtuple("EDep", "Energy Deposition");
    analysisManager->CreateNtupleIColumn("EvtID");
    analysisManager->CreateNtupleDColumn("E(MeV)");
    analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run *)
{
    G4RunManager::GetRunManager()->SetPrintProgress(static_cast<G4int>(G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed() * .1));

    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->OpenFile("Result");
}

void RunAction::EndOfRunAction(const G4Run *)
{
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();
}
