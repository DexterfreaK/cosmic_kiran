#ifndef TRACKERCONSTRUCTION_HH
#define TRACKERCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"

class TrackerConstruction : public G4VUserDetectorConstruction {
public:
  TrackerConstruction(G4double distanceFromCalorimeter);
  virtual ~TrackerConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

private:
  void DefineMaterials();
  G4VPhysicalVolume* ConstructTracker();
  void PrintTrackerParameters();

  // Materials
  G4Material* siliconMat;
  G4Material* defaultMat;

  // Logical volumes
  G4LogicalVolume* lvol_layer;
  G4LogicalVolume* lvol_module;
  G4LogicalVolume* lvol_tracker;
  G4LogicalVolume* lvol_world;

  // Physical volumes
  G4VPhysicalVolume* pvol_world;

  // Parameters
  G4double layerThickness;
  G4double moduleThickness;
  G4double trackerThickness;
  G4double worldSizeX;
  G4int nbOfLayers;
  G4int nbOfModules;
  G4double distanceFromCalorimeter;

  G4GlobalMagFieldMessenger* fFieldMessenger;
};

#endif

