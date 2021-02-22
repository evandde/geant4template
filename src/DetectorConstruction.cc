#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

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
    G4NistManager *nist = G4NistManager::Instance();
    auto matAir = nist->FindOrBuildMaterial("G4_AIR");
    auto matBGO = nist->FindOrBuildMaterial("G4_BGO");

    // vis
    auto visWhiteWire = new G4VisAttributes(G4Color::White());
    visWhiteWire->SetForceWireframe();
    auto visCyanSol = new G4VisAttributes(G4Color::Cyan());
    visCyanSol->SetForceSolid();

    // World
    auto worldSize = 1. * m;
    auto solWorld = new G4Box("World", .5 * worldSize, .5 * worldSize, .5 * worldSize);
    auto lvWorld = new G4LogicalVolume(solWorld, matAir, "World");
    lvWorld->SetVisAttributes(visWhiteWire);
    auto pvWorld = new G4PVPlacement(0, G4ThreeVector(), lvWorld, "World", nullptr, false, 0);

    // Detector
    auto detectorSize = 5. * cm;
    auto solDetector = new G4Box("Detector", .5 * detectorSize, .5 * detectorSize, .5 * detectorSize);
    auto lvDetector = new G4LogicalVolume(solDetector, matBGO, "Detector");
    lvDetector->SetVisAttributes(visCyanSol);
    new G4PVPlacement(0, G4ThreeVector(0., 0., .5 * detectorSize), lvDetector, "Detector", lvWorld, false, 0);

    return pvWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    auto sd = new SensitiveDetector("Detector");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    SetSensitiveDetector("Detector", sd);
}
