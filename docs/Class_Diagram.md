# UML Class Diagram & Architecture Specifications
## Post-Earthquake Multi-Agency Emergency Relief Logistics Hub (ERL-HUB)

The following Mermaid diagrams illustrate the class hierarchy, inheritance relationships (including resolution of the Diamond Problem via virtual base classes), containership (composition), and associations across the ERL-HUB software architecture.

---

## 1. Core Vehicle Hierarchy & Diamond Inheritance

```mermaid
classDiagram
    %% Inheritance Hierarchy
    TransportVehicle <|-- ReliefVehicle : public inheritance
    ReliefVehicle <|-- GroundConvoy : public inheritance
    ReliefVehicle <|-- AerialTransport : virtual public
    ReliefVehicle <|-- AmphibiousCraft : virtual public
    AerialTransport <|-- HybridRescueVehicle : public inheritance
    AmphibiousCraft <|-- HybridRescueVehicle : public inheritance

    %% Containership
    ReliefVehicle *-- GPSBeacon : contains
    ReliefVehicle *-- EmergencyLifeSupportUnit : contains

    class TransportVehicle {
        #string vehicleId
        #string vehicleName
        #double maxPayloadCapacityKg
        #double currentLoadKg
        #double fuelLevelPercentage
        #string operationalStatus
        #string currentLocation
        #static int totalOperationalVehicles
        +checkCapacity(double weight) bool
        +checkFuel(double percent) bool
        +loadPayload(double weight) void
        +consumeFuel(double percent) void
        +static getTotalOperationalVehicles() int
    }

    class ReliefVehicle {
        <<abstract>>
        #GPSBeacon gpsBeacon
        #EmergencyLifeSupportUnit lifeSupport
        +evaluateRoute(corridor, dist, sev)* bool
        +calculateOperationalFeasibility(wt, dist) bool
        +displayDetails() void
        +getVehicleType()* string
    }

    class GPSBeacon {
        -string beaconId
        -double latitude
        -double longitude
        -string currentLocationName
        -bool signalActive
        +updateCoordinates(lat, lon, name) void
        +display() void
    }

    class EmergencyLifeSupportUnit {
        -string unitId
        -bool medicalSupportAvailable
        -double oxygenSupplyLiters
        -double maxOxygenCapacityLiters
        -string equipmentStatus
        -bool defibrillatorReady
        +checkLifeSupport() bool
        +consumeOxygen(liters) bool
        +display() void
    }

    class GroundConvoy {
        #int axleCount
        #string driveType
        #double groundClearanceMm
        #bool winchEquipped
        +evaluateRoute(corridor, dist, sev) bool
        +displayDetails() void
    }

    class AerialTransport {
        #double maxAltitudeMeters
        #string rotorConfiguration
        #double maxWindToleranceKnots
        +evaluateRoute(corridor, dist, sev) bool
        +displayDetails() void
    }

    class AmphibiousCraft {
        #string hullType
        #double draftMeters
        #double maxWaterSpeedKnots
        #bool waterjetPropulsion
        +evaluateRoute(corridor, dist, sev) bool
        +displayDetails() void
    }

    class HybridRescueVehicle {
        -string currentOperationalMode
        -double auxiliaryBatteryReserve
        -bool rapidDeployTriageUnit
        +evaluateRoute(corridor, dist, sev) bool
        +displayDetails() void
        +setOperationalMode(mode) void
    }
```

---

## 2. Inventory, Depot, Manifest, and Friend Class Associations

```mermaid
classDiagram
    DistributionDepot o-- ReliefCargo : stores collection of
    Manifest o-- ReliefCargo : dynamic heap array of
    IncidentCommander ..> DistributionDepot : friend class access
    IncidentCommander ..> ReliefCargo : friend class access
    LogisticsManager --> DistributionDepot : manages
    LogisticsManager --> ReliefVehicle : manages polymorphic fleet
    LogisticsManager --> Manifest : archives
    LogisticsManager --> FileManager : utilizes persistence
    LogisticsManager *-- PriorityDispatchQueue : contains

    class ReliefCargo {
        -string cargoId
        -string cargoName
        -string category
        -int availableQuantity
        -string unitOfMeasure
        -double weightPerUnitKg
        -int emergencyPriorityLevel
        -bool medicalFlag
        +getTotalWeightKg() double
        +updateQuantity(delta) void
        +operator==(other) bool
        +operator<<(os, cargo)$
    }

    class DistributionDepot {
        -string depotId
        -string depotName
        -string location
        -double maxStorageCapacityKg
        -double currentStoredWeightKg
        -vector~ReliefCargo~ inventory
        -static int totalRegisteredDepots
        +addOrUpdateCargo(cargo) void
        +restockSupplies(id, qty) void
        +deductSupplies(id, qty) void
        +findCargoById(id) ReliefCargo&
        +operator+=(cargo) DistributionDepot&
        +operator-=(cargo) DistributionDepot&
        +static getTotalRegisteredDepots() int
    }

    class Manifest {
        -string manifestId
        -string destinationDepot
        -string assignedVehicleId
        -int capacity
        -int count
        -ReliefCargo* cargoRecords
        +Manifest(other) "Deep Copy Ctor"
        +operator=(other) "Copy Assignment"
        +~Manifest() "Explicit Destructor"
        +addCargo(cargo) void
        +getTotalWeightKg() double
        +operator==(other) bool
    }

    class IncidentCommander {
        -string commanderId
        -string officerName
        -string authorizationRank
        -int overrideCount
        +emergencyTriageOverride(depot, cargoId, qty) bool
        +forceElevateCargoPriority(cargo, prio) void
        +displayCommanderDashboard(depots, fleet) void
    }

    class PriorityDispatchQueue~T~ {
        -vector~T~ elements
        -Comparator comp
        +push(item) void
        +pop() void
        +top() T&
        +empty() bool
        +size() size_t
        +display() void
    }

    class FileManager {
        -string dataDirectory
        +saveInventory(inv, file) bool
        +loadInventory(inv, file) bool
        +saveDepots(depots, file) bool
        +loadDepots(depots, file) bool
        +saveVehicles(fleet, file) bool
        +loadVehicles(fleet, file) bool
        +transactionalFileUpdate(path, func)$ bool
    }

    class LogisticsManager {
        -vector~ReliefCargo~ centralCatalog
        -vector~DistributionDepot~ depots
        -vector~ReliefVehicle*~ fleet
        -vector~Manifest~ archivedManifests
        -PriorityDispatchQueue~DispatchRequest~ emergencyQueue
        -static double totalDispatchedMetricTonnage
        -static int totalDispatchedMissions
        +allocateSupplies(depot, vehicle, cargoId, qty) bool
        +allocateSupplies(depot, vehicle, prio, maxWt) bool
        +executeDispatch(...) bool
        +evaluateCorridorForFleet(...) void
    }
```

---

## 3. Custom Exception Hierarchy

```mermaid
classDiagram
    std_exception <|-- ERLHubException
    ERLHubException <|-- CargoOverweightException
    ERLHubException <|-- FuelDepletionException
    ERLHubException <|-- DamagedCorridorException
    ERLHubException <|-- InsufficientStockException
    ERLHubException <|-- DuplicateIdException
    ERLHubException <|-- EntityNotFoundException
    ERLHubException <|-- CapacityOverflowException

    class std_exception {
        +what()* const char*
    }

    class ERLHubException {
        #string message
        +what() const char*
    }

    class CargoOverweightException {
        -double attemptedWeight
        -double maxCapacity
        +getAttemptedWeight() double
        +getMaxCapacity() double
    }

    class FuelDepletionException {
        -double currentFuel
        -double requiredFuel
        +getCurrentFuel() double
        +getRequiredFuel() double
    }

    class DamagedCorridorException {
        -string corridorName
        -int damageSeverity
        +getCorridorName() string
        +getDamageSeverity() int
    }
```
