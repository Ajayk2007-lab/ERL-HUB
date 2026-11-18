#ifndef ERL_HUB_DISTRIBUTION_DEPOT_H
#define ERL_HUB_DISTRIBUTION_DEPOT_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "ReliefCargo.h"

namespace ReliefOps {
namespace Logistics {

// Forward declaration of friend class
class IncidentCommander;

/**
 * @brief Manages emergency distribution depots and their localized inventory.
 * Demonstrates Unit I Return by Reference, Friend Class, Operator Overloading, and Static Members.
 */
class DistributionDepot {
    // IncidentCommander has direct access to private members for emergency triage overrides
    friend class IncidentCommander;

private:
    std::string depotId;
    std::string depotName;
    std::string location;
    double maxStorageCapacityKg;
    double currentStoredWeightKg;
    std::vector<ReliefOps::Inventory::ReliefCargo> inventory;

    // Unit II Static Data Member
    static int totalRegisteredDepots;

public:
    // Constructors
    DistributionDepot();
    DistributionDepot(const std::string& id, 
                      const std::string& name, 
                      const std::string& loc, 
                      double maxCapacityKg = 50000.0);

    // Static Member Functions (Unit II 7.2)
    static int getTotalRegisteredDepots();
    static void resetDepotCounter();

    // Getters (const-correctness)
    std::string getDepotId() const;
    std::string getDepotName() const;
    std::string getLocation() const;
    double getMaxStorageCapacityKg() const;
    double getCurrentStoredWeightKg() const;
    double getRemainingCapacityKg() const;
    const std::vector<ReliefOps::Inventory::ReliefCargo>& getInventory() const;
    size_t getCargoCount() const;

    // Core Inventory Operations (Call by Reference)
    void addOrUpdateCargo(const ReliefOps::Inventory::ReliefCargo& cargo);
    void restockSupplies(const std::string& cargoId, int quantity);
    void deductSupplies(const std::string& cargoId, int quantity);
    bool hasStock(const std::string& cargoId, int quantity) const;

    // Unit I 6.4: Return by Reference
    // Returns a reference to existing cargo object in inventory.
    // Throws EntityNotFoundException if not found.
    ReliefOps::Inventory::ReliefCargo& findCargoById(const std::string& cargoId);
    const ReliefOps::Inventory::ReliefCargo& findCargoById(const std::string& cargoId) const;

    void recalculateStoredWeight();
    void displayDepotSummary() const;
    void displayDetailedInventory() const;

    // Unit II 7.5: Operator Overloading
    // depot += cargo (restocks/adds cargo)
    DistributionDepot& operator+=(const ReliefOps::Inventory::ReliefCargo& cargo);

    // depot -= cargo (deducts cargo based on cargo.getCargoId() and cargo.getAvailableQuantity())
    DistributionDepot& operator-=(const ReliefOps::Inventory::ReliefCargo& cargo);

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, const DistributionDepot& depot);

    // Binary serialization
    void serialize(std::ofstream& out) const;
    void deserialize(std::ifstream& in);
};

} // namespace Logistics
} // namespace ReliefOps

#endif // ERL_HUB_DISTRIBUTION_DEPOT_H
