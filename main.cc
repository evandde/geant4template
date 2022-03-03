//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \author Evan Kim
/// \email evandde@gmail.com
/// \homepage evandde.github.io
/// \brief Template code for Geant4 (Geant4 11.0)

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4PhysListFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "QBBC.hh"


namespace
{
    void PrintUsage()
    {
        G4cerr << " Usage: " << G4endl
               << " ProjectName [-option1 value1] [-option2 value2] ..." << G4endl;
        G4cerr << "\t--- Option lists ---"
               << "\n\t[-m] <Set macrofile> default: "
                  "vis.mac"
                  ", inputtype: string"
               << "\n\t[-t] <Set nThreads> default: 1, inputtype: int, Max: "
               << G4Threading::G4GetNumberOfCores()
               << "\n\t[-p] <Set physics> default: 'QBBC', inputtype: string"

               << G4endl;
    }
} // anonymous namespace

int main(int argc, char **argv)
{
    // Default setting for main() arguments
    G4String macroFilePath;
    G4int nThreads = 1;
    G4String physName;

    // Parsing main() Arguments
    for (G4int i = 1; i < argc; i = i + 2)
    {
        if (G4String(argv[i]) == "-m")
            macroFilePath = argv[i + 1];
        else if (G4String(argv[i]) == "-t")
            nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
        else if (G4String(argv[i]) == "-p")
            physName = argv[i + 1];
        else
        {
            PrintUsage();
            return 1;
        }
    }
    if (argc > 7)
    {
        PrintUsage();
        return 1;
    }

    // Set random engine and seed number
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheSeed(time(nullptr));

    // Construct the default run manager
    auto runManager = G4RunManagerFactory::CreateRunManager(
        nThreads == 1 ? G4RunManagerType::Serial : G4RunManagerType::Default, nThreads);

    // Set mandatory initialization classes
    runManager->SetUserInitialization(new DetectorConstruction);
    G4VModularPhysicsList *phys;
    if (physName.empty())
        phys = new QBBC;
    else
    {
        G4PhysListFactory factory;
        phys = factory.GetReferencePhysList(physName);
    }
    runManager->SetUserInitialization(phys);
    runManager->SetUserInitialization(new ActionInitialization);

    // Initialize run
    runManager->Initialize();

    // Initialize visualization
    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    auto UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    if (macroFilePath.empty())
    {
        // interactive mode (if no macrofile)
        auto ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else
    {
        // batch mode
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + macroFilePath);
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    delete visManager;
    delete runManager;
}
