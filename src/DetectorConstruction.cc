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
    // materials
    auto nist = G4NistManager::Instance();
    auto airMat = nist->FindOrBuildMaterial("G4_AIR");
    auto BGOMat = nist->FindOrBuildMaterial("G4_BGO");

    // vis
    auto whiteWireVis = new G4VisAttributes(G4Color::White());
    whiteWireVis->SetForceWireframe();
    auto cyanSolVis = new G4VisAttributes(G4Color::Cyan());
    cyanSolVis->SetForceSolid();

    // World
    auto worldSize = 1. * m;
    auto worldSol = new G4Box("World", .5 * worldSize, .5 * worldSize, .5 * worldSize);
    auto worldLog = new G4LogicalVolume(worldSol, airMat, "World");
    worldLog->SetVisAttributes(whiteWireVis);
    auto worldPhys = new G4PVPlacement(nullptr, G4ThreeVector(), worldLog, "World", nullptr, false, 0);

    // Detector
    auto detectorSize = 5. * cm;
    auto detectorSol = new G4Box("Detector", .5 * detectorSize, .5 * detectorSize, .5 * detectorSize);
    auto detectorLog = new G4LogicalVolume(detectorSol, BGOMat, "Detector");
    detectorLog->SetVisAttributes(cyanSolVis);
    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., .5 * detectorSize), detectorLog, "Detector", worldLog, false, 0);

    return worldPhys;
}

void DetectorConstruction::ConstructSDandField()
{
    auto mfd = new G4MultiFunctionalDetector("Detector");
    G4SDManager::GetSDMpointer()->AddNewDetector(mfd);
    auto eDepPS = new G4PSEnergyDeposit("EDep");
    mfd->RegisterPrimitive(eDepPS);
    SetSensitiveDetector("Detector", mfd);
}
