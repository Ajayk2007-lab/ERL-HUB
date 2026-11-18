#ifndef ERL_HUB_LOGISTICS_MANAGER_H
#define ERL_HUB_LOGISTICS_MANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <iomanip>

#include "ReliefCargo.h"
#include "DistributionDepot.h"
#include "ReliefVehicle.h"
#include "AerialTransport.h"
#include "GroundConvoy.h"
#include "AmphibiousCraft.h"
#include "HybridRescueVehicle.h"
#include "Manifest.h"
#include "PriorityDispatchQueue.h"
#include "IncidentCommander.h"
#include "FileManager.h"
#include "Exceptions.h"

namespace ReliefOps {
namespace Logistics {

/**
 * @brief User-Defined Custom Stream Manipulator for Tabular Headers.
 * Demonstrates Unit IV 9.8 User-Defined I/O Manipulators.
 */
class TableHeaderManipulator {
private:
    std::string title;
    int width;
public:
    TableHeaderManipulator(const std::string& t, int w = 78) : title(t), width(w) {}
    friend std::ostream& operator<<(std::ostream& os, const TableHeaderManipulator& m) {
        os << "+" << std::string(m.width - 2, '-') << "+\n";
        int pad = (m.width - 2 - static_cast<int>(m.title.length())) / 2;
        if (pad < 0) pad = 0;
        os << "| " << std::string(pad, ' ') << m.title 
           << std::string(m.width - 4 - pad - m.title.length(), ' ') << " |\n";
        os << "+" << std::string(m.width - 2, '-') << "+";
        return os;
    }
};

inline TableHeaderManipulator tableHeader(const std::string& title, int width = 78) {
    return TableHeaderManipulator(title, width);
}

/**
 * @brief User-Defined Custom Stream Manipulator for Section Dividers.
 */
inline std::ostream& sectionDivider(std::ostream& os) {
    os << "--------------------------------------------------------------------------------\n";
    return os;
}

/**
 * @brief Central Logistics Orchestrator for ERL-HUB.
 * Coordinates inventory, depots, vehicle fleet, dispatch requests, and file persistence.
 */
class LogisticsManager {
private:
    // STL Containers (Unit IV 9.4)
    std::vector<ReliefOps::Inventory::ReliefCargo> centralCatalog;
    std::unordered_map<std::string, size_t> cargoIndexMap; // Fast lookup

    std::vector<DistributionDepot> depots;
    std::unordered_map<std::string, size_t> depotIndexMap;

    std::vector<ReliefVehicle*> fleet; // Polymorphic vehicle pointers
    std::unordered_map<std::string, ReliefVehicle*> fleetMap;

    std::vector<ReliefOps::Inventory::Manifest> archivedManifests;
    PriorityDispatchQueue<DispatchRequest> emergencyQueue;
    std::vector<std::string> dispatchHistoryLogs;

    IncidentCommander incidentCommander;
    FileManager fileManager;

    // Unit II 7.2: Static Data Members
    static double totalDispatchedMetricTonnage;
    static int totalDispatchedMissions;

public:
    LogisticsManager(const std::string& dataDirectory = "data");
    ~LogisticsManager();

    // Unit II 7.2: Static Member Functions
    static double getTotalDispatchedMetricTonnage();
    static int getTotalDispatchedMissions();
    static void recordDispatchTonnage(double tonnageMetricTons);
    static void resetDispatchStatistics();

    // Initialization & Data Loading
    void initializeSampleData();
    bool loadAllData();
    bool saveAllData();

    // Module A: Cargo Catalog Management
    void registerCargo(const ReliefOps::Inventory::ReliefCargo& cargo);
    void displayCargoCatalog() const;
    void displayCriticalMedicalSupplies();
    ReliefOps::Inventory::ReliefCargo& findCargo(const std::string& cargoId); // Return by Reference
    bool hasCargo(const std::string& cargoId) const;

    // Module B: Depot Management
    void registerDepot(const DistributionDepot& depot);
    void displayAllDepots() const;
    DistributionDepot& getDepot(const std::string& depotId); // Return by Reference
    const DistributionDepot& getDepot(const std::string& depotId) const;
    bool hasDepot(const std::string& depotId) const;

    // Module C: Vehicle Fleet Management
    void registerVehicle(ReliefVehicle* vehicle);
    void displayAllVehicles() const;
    ReliefVehicle& getVehicle(const std::string& vehicleId); // Return by Reference
    const ReliefVehicle& getVehicle(const std::string& vehicleId) const;
    bool hasVehicle(const std::string& vehicleId) const;
    const std::vector<ReliefVehicle*>& getFleet() const;

    // Unit I 6.2: Function Overloading for Supply Allocation
    // 1. Volume/Quantity-based payload allocation
    bool allocateSupplies(DistributionDepot& depot, 
                          ReliefVehicle& vehicle, 
                          const std::string& cargoId, 
                          int quantity);

    // 2. Priority-based emergency medical allocation
    bool allocateSupplies(DistributionDepot& depot, 
                          ReliefVehicle& vehicle, 
                          int minPriorityLevel, 
                          double maxWeightKg);

    // Module D: Emergency Dispatch Management
    bool executeDispatch(const std::string& depotId, 
                         const std::string& vehicleId, 
                         const std::string& destination, 
                         const std::string& corridor, 
                         const std::vector<std::pair<std::string, int>>& cargoList,
                         int terrainSeverity = 4,
                         double distanceKm = 45.0);

    // Priority Medical Dispatch Queue operations
    void submitEmergencyRequest(const DispatchRequest& req);
    bool processNextEmergencyDispatch();
    void displayEmergencyQueue() const;

    // Incident Commander Dashboard and Override
    IncidentCommander& getIncidentCommander();
    void displayIncidentCommanderDashboard() const;
    bool executeCommanderOverride(const std::string& depotId, 
                                  const std::string& cargoId, 
                                  int quantity);

    // Route Evaluation (Unit III Polymorphism demonstration)
    void evaluateCorridorForFleet(const std::string& corridorName, 
                                 double distanceKm, 
                                 int terrainSeverity) const;

    // Analytics and Logs
    void displayStatistics() const;
    void displayDispatchHistory() const;

    // Manifest deep copy test and operations
    void archiveManifest(const ReliefOps::Inventory::Manifest& manifest);
    const std::vector<ReliefOps::Inventory::Manifest>& getArchivedManifests() const;
};

} // namespace Logistics
} // namespace ReliefOps

#endif // ERL_HUB_LOGISTICS_MANAGER_H
