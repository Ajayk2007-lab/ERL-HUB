#include "../include/DistributionDepot.h"
#include "../include/Exceptions.h"
#include <iomanip>
#include <algorithm>
#include <cstdint>

namespace ReliefOps {
namespace Logistics {

// Unit II Static Data Member Initialization
int DistributionDepot::totalRegisteredDepots = 0;

DistributionDepot::DistributionDepot()
    : depotId("DPT-00"), depotName("Unassigned Depot"), location("Base"),
      maxStorageCapacityKg(50000.0), currentStoredWeightKg(0.0) {
    ++totalRegisteredDepots;
}

DistributionDepot::DistributionDepot(const std::string& id, 
                                     const std::string& name, 
                                     const std::string& loc, 
                                     double maxCapacityKg)
    : depotId(id), depotName(name), location(loc),
      maxStorageCapacityKg(maxCapacityKg), currentStoredWeightKg(0.0) {
    ++totalRegisteredDepots;
}

// Unit II Static Member Functions
int DistributionDepot::getTotalRegisteredDepots() {
    return totalRegisteredDepots;
}

void DistributionDepot::resetDepotCounter() {
    totalRegisteredDepots = 0;
}

std::string DistributionDepot::getDepotId() const {
    return depotId;
}

std::string DistributionDepot::getDepotName() const {
    return depotName;
}

std::string DistributionDepot::getLocation() const {
    return location;
}

double DistributionDepot::getMaxStorageCapacityKg() const {
    return maxStorageCapacityKg;
}

double DistributionDepot::getCurrentStoredWeightKg() const {
    return currentStoredWeightKg;
}

double DistributionDepot::getRemainingCapacityKg() const {
    return (maxStorageCapacityKg > currentStoredWeightKg) 
            ? (maxStorageCapacityKg - currentStoredWeightKg) : 0.0;
}

const std::vector<ReliefOps::Inventory::ReliefCargo>& DistributionDepot::getInventory() const {
    return inventory;
}

size_t DistributionDepot::getCargoCount() const {
    return inventory.size();
}

void DistributionDepot::recalculateStoredWeight() {
    double total = 0.0;
    for (const auto& item : inventory) {
        total += item.getTotalWeightKg();
    }
    currentStoredWeightKg = total;
}

void DistributionDepot::addOrUpdateCargo(const ReliefOps::Inventory::ReliefCargo& cargo) {
    double additionalWeight = cargo.getTotalWeightKg();
    if (currentStoredWeightKg + additionalWeight > maxStorageCapacityKg) {
        throw ReliefOps::Exceptions::CapacityOverflowException(
            "Depot '" + depotId + "' capacity overflow: Cannot add " + 
            std::to_string(additionalWeight) + " kg. Remaining capacity: " + 
            std::to_string(getRemainingCapacityKg()) + " kg.");
    }

    for (auto& item : inventory) {
        if (item.getCargoId() == cargo.getCargoId()) {
            item.updateQuantity(cargo.getAvailableQuantity());
            recalculateStoredWeight();
            return;
        }
    }
    inventory.push_back(cargo);
    recalculateStoredWeight();
}

void DistributionDepot::restockSupplies(const std::string& cargoId, int quantity) {
    if (quantity <= 0) {
        throw ReliefOps::Exceptions::ERLHubException("Restock quantity must be positive.");
    }
    for (auto& item : inventory) {
        if (item.getCargoId() == cargoId) {
            double addWt = quantity * item.getWeightPerUnitKg();
            if (currentStoredWeightKg + addWt > maxStorageCapacityKg) {
                throw ReliefOps::Exceptions::CapacityOverflowException(
                    "Restock would exceed depot storage limit by " + 
                    std::to_string((currentStoredWeightKg + addWt) - maxStorageCapacityKg) + " kg.");
            }
            item.updateQuantity(quantity);
            recalculateStoredWeight();
            return;
        }
    }
    throw ReliefOps::Exceptions::EntityNotFoundException("Cargo '" + cargoId + "' not found in depot '" + depotId + "'.");
}

void DistributionDepot::deductSupplies(const std::string& cargoId, int quantity) {
    if (quantity <= 0) {
        throw ReliefOps::Exceptions::ERLHubException("Deduction quantity must be positive.");
    }
    for (auto& item : inventory) {
        if (item.getCargoId() == cargoId) {
            if (!item.hasStock(quantity)) {
                throw ReliefOps::Exceptions::InsufficientStockException(cargoId, quantity, item.getAvailableQuantity());
            }
            item.updateQuantity(-quantity);
            recalculateStoredWeight();
            return;
        }
    }
    throw ReliefOps::Exceptions::EntityNotFoundException("Cargo '" + cargoId + "' not found in depot '" + depotId + "'.");
}

bool DistributionDepot::hasStock(const std::string& cargoId, int quantity) const {
    for (const auto& item : inventory) {
        if (item.getCargoId() == cargoId) {
            return item.hasStock(quantity);
        }
    }
    return false;
}

// Unit I 6.4: Return by Reference
ReliefOps::Inventory::ReliefCargo& DistributionDepot::findCargoById(const std::string& cargoId) {
    for (auto& item : inventory) {
        if (item.getCargoId() == cargoId) {
            return item; // Direct reference to existing object in container
        }
    }
    throw ReliefOps::Exceptions::EntityNotFoundException("Cargo '" + cargoId + "' does not exist in depot '" + depotId + "'.");
}

const ReliefOps::Inventory::ReliefCargo& DistributionDepot::findCargoById(const std::string& cargoId) const {
    for (const auto& item : inventory) {
        if (item.getCargoId() == cargoId) {
            return item;
        }
    }
    throw ReliefOps::Exceptions::EntityNotFoundException("Cargo '" + cargoId + "' does not exist in depot '" + depotId + "'.");
}

void DistributionDepot::displayDepotSummary() const {
    std::cout << "[" << depotId << "] " << std::left << std::setw(30) << depotName
              << " | Loc: " << std::setw(20) << location
              << " | Stored: " << std::right << std::fixed << std::setprecision(1) << currentStoredWeightKg 
              << "/" << maxStorageCapacityKg << " kg (" 
              << ((maxStorageCapacityKg > 0) ? (currentStoredWeightKg / maxStorageCapacityKg * 100.0) : 0.0) << "% full)"
              << " | Items: " << inventory.size() << "\n";
}

void DistributionDepot::displayDetailedInventory() const {
    std::cout << "\n================================================================================\n";
    std::cout << " DEPOT INVENTORY AUDIT: " << depotName << " [" << depotId << "] Location: " << location << "\n";
    std::cout << " Capacity: " << std::fixed << std::setprecision(1) << currentStoredWeightKg << " / " << maxStorageCapacityKg 
              << " kg (" << getRemainingCapacityKg() << " kg available)\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    if (inventory.empty()) {
        std::cout << "   [No relief cargo currently stored at this depot]\n";
    } else {
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << inventory[i] << "\n";
        }
    }
    std::cout << "================================================================================\n";
}

// Unit II 7.5: Operator Overloading
// depot += cargo (restocks inventory)
DistributionDepot& DistributionDepot::operator+=(const ReliefOps::Inventory::ReliefCargo& cargo) {
    addOrUpdateCargo(cargo);
    return *this;
}

// depot -= cargo (deducts cargo based on ID and available quantity)
DistributionDepot& DistributionDepot::operator-=(const ReliefOps::Inventory::ReliefCargo& cargo) {
    deductSupplies(cargo.getCargoId(), cargo.getAvailableQuantity());
    return *this;
}

std::ostream& operator<<(std::ostream& os, const DistributionDepot& depot) {
    os << "[" << depot.depotId << "] " << depot.depotName << " (" << depot.location << ") "
       << "| Stored: " << depot.currentStoredWeightKg << "/" << depot.maxStorageCapacityKg << " kg "
       << "| Cargo Items: " << depot.inventory.size();
    return os;
}

void DistributionDepot::serialize(std::ofstream& out) const {
    auto writeStr = [&out](const std::string& str) {
        uint32_t len = static_cast<uint32_t>(str.size());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        if (len > 0) out.write(str.data(), len);
    };

    writeStr(depotId);
    writeStr(depotName);
    writeStr(location);
    out.write(reinterpret_cast<const char*>(&maxStorageCapacityKg), sizeof(maxStorageCapacityKg));
    out.write(reinterpret_cast<const char*>(&currentStoredWeightKg), sizeof(currentStoredWeightKg));

    uint32_t count = static_cast<uint32_t>(inventory.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& item : inventory) {
        item.serialize(out);
    }
}

void DistributionDepot::deserialize(std::ifstream& in) {
    auto readStr = [&in]() -> std::string {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (len == 0) return "";
        std::string s(len, '\0');
        in.read(&s[0], len);
        return s;
    };

    depotId = readStr();
    depotName = readStr();
    location = readStr();
    in.read(reinterpret_cast<char*>(&maxStorageCapacityKg), sizeof(maxStorageCapacityKg));
    in.read(reinterpret_cast<char*>(&currentStoredWeightKg), sizeof(currentStoredWeightKg));

    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    inventory.clear();
    for (uint32_t i = 0; i < count; ++i) {
        ReliefOps::Inventory::ReliefCargo item;
        item.deserialize(in);
        inventory.push_back(item);
    }
}

} // namespace Logistics
} // namespace ReliefOps
