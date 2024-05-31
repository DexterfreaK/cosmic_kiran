#include "TrackerConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4Transform3D.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UnitsTable.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4GeometryManager.hh" // Include necessary header

// Constructor
TrackerConstruction::TrackerConstruction(G4double distance)
  : siliconMat(nullptr),
    lvol_layer(nullptr),
    lvol_module(nullptr),
    lvol_tracker(nullptr),
    lvol_world(nullptr),
    pvol_world(nullptr),
    defaultMat(nullptr),
    layerThickness(0.3 * mm),
    nbOfLayers(4),
    nbOfModules(4),
    distanceFromCalorimeter(distance)
{
  DefineMaterials();
}

// Destructor
TrackerConstruction::~TrackerConstruction() {}

// Define materials
void TrackerConstruction::DefineMaterials()
{
  // Get materials from NIST manager
  G4NistManager* nist = G4NistManager::Instance();

  siliconMat = nist->FindOrBuildMaterial("G4_Si");
  defaultMat = nist->FindOrBuildMaterial("G4_Galactic");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

// Construct tracker geometry
G4VPhysicalVolume* TrackerConstruction::Construct()
{
  return ConstructTracker();
}

// Construct tracker
G4VPhysicalVolume* TrackerConstruction::ConstructTracker()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Layer
  G4double sizeX = layerThickness;
  G4double sizeY = 10 * cm;
  G4double sizeZ = 10 * cm;

  G4Box* svol_layer = new G4Box("layer", 0.5 * sizeX, 0.5 * sizeY, 0.5 * sizeZ);
  lvol_layer = new G4LogicalVolume(svol_layer, siliconMat, "layer");

  // Module
  moduleThickness = layerThickness * nbOfLayers;
  sizeX = moduleThickness;

  G4Box* svol_module = new G4Box("module", 0.5 * sizeX, 0.5 * sizeY, 0.5 * sizeZ);
  lvol_module = new G4LogicalVolume(svol_module, defaultMat, "module");

  // Place layers within module
  G4double Xcenter = -0.5 * (moduleThickness + layerThickness);

  for (G4int k = 0; k < nbOfLayers; k++) {
    Xcenter += layerThickness;
    new G4PVPlacement(0, G4ThreeVector(Xcenter, 0., 0.), lvol_layer, "layer", lvol_module, false, k + 1);
  }

  // Tracker
  trackerThickness = moduleThickness * nbOfModules;
  sizeX = trackerThickness;

  G4Box* svol_tracker = new G4Box("tracker", 0.5 * sizeX, 0.5 * sizeY, 0.5 * sizeZ);
  lvol_tracker = new G4LogicalVolume(svol_tracker, defaultMat, "tracker");

  // Place modules within tracker
  Xcenter = -0.5 * (trackerThickness + moduleThickness);

  for (G4int k = 0; k < nbOfModules; k++) {
    Xcenter += moduleThickness;
    new G4PVPlacement(0, G4ThreeVector(Xcenter, 0., 0.), lvol_module, "module", lvol_tracker, false, k + 1);
  }

  // World
  sizeX = 1.2 * (trackerThickness + distanceFromCalorimeter + 10 * cm);
  sizeY = 1.2 * 10 * cm;
  sizeZ = 1.2 * 10 * cm;

  worldSizeX = sizeX;

  G4Box* svol_world = new G4Box("world", 0.5 * sizeX, 0.5 * sizeY, 0.5 * sizeZ);
  lvol_world = new G4LogicalVolume(svol_world, defaultMat, "world");

  pvol_world = new G4PVPlacement(0, G4ThreeVector(), lvol_world, "world", 0, false, 0);

  // Place tracker in world
  G4double trackerPositionX = distanceFromCalorimeter + (trackerThickness / 2);
  new G4PVPlacement(0, G4ThreeVector(trackerPositionX, 0., 0.), lvol_tracker, "tracker", lvol_world, false, 0);

  PrintTrackerParameters();

  // Visualization attributes
  lvol_layer->SetVisAttributes(G4VisAttributes::GetInvisible());
  lvol_world->SetVisAttributes(G4VisAttributes::GetInvisible());

  // Always return the physical World
  return pvol_world;
}

// Print tracker parameters
void TrackerConstruction::PrintTrackerParameters()
{
  G4cout << "\n-------------------------------------------------------------"
         << "\n ---> The tracker is " << nbOfModules << " Modules"
         << "\n ---> A Module is " << nbOfLayers << " Layers";

  G4cout << "\n ---> A Layer is " << G4BestUnit(layerThickness, "Length") << " thickness of "
         << siliconMat->GetName();

  G4cout << "\n\n ---> Module thickness " << G4BestUnit(moduleThickness, "Length");

  G4cout << "\n\n ---> Total tracker thickness " << G4BestUnit(trackerThickness, "Length")
         << "\n      Tranverse size        " << G4BestUnit(10 * cm, "Length")
         << "\n      Distance from calorimeter " << G4BestUnit(distanceFromCalorimeter, "Length");

  G4cout << "\n-------------------------------------------------------------\n";
  G4cout << G4endl;
}

// Construct magnetic field
void TrackerConstruction::ConstructSDandField()
{
  if (fFieldMessenger == nullptr) {
    // Create global magnetic field messenger
    G4ThreeVector fieldValue = G4ThreeVector(1 * tesla, 0.0, 0.0);
    fFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
   // G4AutoDelete::Register(fFieldMessenger.Get());
  }
}

