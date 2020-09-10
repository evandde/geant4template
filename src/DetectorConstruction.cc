#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSEnergyDeposit.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // Get nist material manager
    G4NistManager *nist = G4NistManager::Instance();

    // World
    auto worldSize = 1. * m;
    auto worldSol = new G4Box("World", .5 * worldSize, .5 * worldSize, .5 * worldSize);

    auto matAir = nist->FindOrBuildMaterial("G4_AIR");
    auto worldLv = new G4LogicalVolume(worldSol, matAir, "World");

    auto worldPv = new G4PVPlacement(0, G4ThreeVector(), worldLv, "World", nullptr, false, 0);

    auto worldVis = new G4VisAttributes(G4Color::White());
    worldVis->SetForceWireframe();
    worldLv->SetVisAttributes(worldVis);

    // Detector
    auto detectorSize = 5. * cm;
    auto detectorSol = new G4Box("Detector", .5 * detectorSize, .5 * detectorSize, .5 * detectorSize);

    auto matBGO = nist->FindOrBuildMaterial("G4_BGO");
    auto detectorLv = new G4LogicalVolume(detectorSol, matBGO, "Detector");

    new G4PVPlacement(0, G4ThreeVector(0., 0., .5 * detectorSize), detectorLv, "Detector", worldLv, false, 0);

    auto detectorVis = new G4VisAttributes(G4Color::Cyan());
    detectorVis->SetForceSolid();
    detectorLv->SetVisAttributes(detectorVis);

    return worldPv;
}

void DetectorConstruction::ConstructSDandField()
{
    auto detectorMFD = new G4MultiFunctionalDetector("DetectorMFD");
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorMFD);
    auto psEDep = new G4PSEnergyDeposit("EDep");
    detectorMFD->RegisterPrimitive(psEDep);
    SetSensitiveDetector("Detector", detectorMFD);
}
