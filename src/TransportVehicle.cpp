#include "../include/TransportVehicle.h"
#include "../include/Exceptions.h"
#include <iomanip>
#include <cstdint>

namespace ReliefOps {
namespace Logistics {

// Unit II Static Data Member Initialization
int TransportVehicle::totalOperationalVehicles = 0;

TransportVehicle::TransportVehicle()
    : vehicleId("VEH-000"), vehicleName("Generic Transport"),
      maxPayloadCapacityKg(1000.0), currentLoadKg(0.0),
      fuelLevelPercentage(100.0), operationalStatus("Operational"),
      currentLocation("Central Base") {
    ++totalOperationalVehicles;
}

TransportVehicle::TransportVehicle(const std::string& id, 
                                   const std::string& name, 
                                   double maxPayloadKg, 
                                   double fuel, 
                                   const std::string& status,
                                   const std::string& location)
    : vehicleId(id), vehicleName(name), maxPayloadCapacityKg(maxPayloadKg),
      currentLoadKg(0.0), fuelLevelPercentage(fuel), operationalStatus(status),
      currentLocation(location) {
    if (status == "Operational") {
        ++totalOperationalVehicles;
    }
}

TransportVehicle::~TransportVehicle() {
    if (operationalStatus == "Operational") {
        --totalOperationalVehicles;
        if (totalOperationalVehicles < 0) totalOperationalVehicles = 0;
    }
}

int TransportVehicle::getTotalOperationalVehicles() {
    return totalOperationalVehicles;
}

void TransportVehicle::adjustOperationalVehicleCount(int delta) {
    totalOperationalVehicles += delta;
    if (totalOperationalVehicles < 0) totalOperationalVehicles = 0;
}

void TransportVehicle::resetOperationalVehicleCount() {
    totalOperationalVehicles = 0;
}

std::string TransportVehicle::getVehicleId() const {
    return vehicleId;
}

std::string TransportVehicle::getVehicleName() const {
    return vehicleName;
}

double TransportVehicle::getMaxPayloadCapacityKg() const {
    return maxPayloadCapacityKg;
}

double TransportVehicle::getCurrentLoadKg() const {
    return currentLoadKg;
}

double TransportVehicle::getRemainingPayloadCapacityKg() const {
    return calculateRemainingCapacity(maxPayloadCapacityKg, currentLoadKg);
}

double TransportVehicle::getFuelLevelPercentage() const {
    return fuelLevelPercentage;
}

std::string TransportVehicle::getOperationalStatus() const {
    return operationalStatus;
}

std::string TransportVehicle::getCurrentLocation() const {
    return currentLocation;
}

bool TransportVehicle::isOperational() const {
    return operationalStatus == "Operational";
}

bool TransportVehicle::checkCapacity(double additionalWeightKg) const {
    return (currentLoadKg + additionalWeightKg) <= maxPayloadCapacityKg;
}

bool TransportVehicle::checkFuel(double requiredFuelPercentage) const {
    return isFuelSufficient(fuelLevelPercentage, requiredFuelPercentage);
}

void TransportVehicle::loadPayload(double weightKg) {
    if (weightKg < 0) {
        throw ReliefOps::Exceptions::ERLHubException("Payload weight cannot be negative.");
    }
    if (currentLoadKg + weightKg > maxPayloadCapacityKg) {
        throw ReliefOps::Exceptions::CargoOverweightException(
            currentLoadKg + weightKg, maxPayloadCapacityKg, 
            "Vehicle '" + vehicleId + "' payload capacity exceeded.");
    }
    currentLoadKg += weightKg;
}

void TransportVehicle::unloadPayload(double weightKg) {
    if (weightKg < 0) {
        throw ReliefOps::Exceptions::ERLHubException("Unload weight cannot be negative.");
    }
    currentLoadKg -= weightKg;
    if (currentLoadKg < 0.0) currentLoadKg = 0.0;
}

void TransportVehicle::updateFuel(double fuelPercent) {
    if (fuelPercent < 0.0) fuelPercent = 0.0;
    if (fuelPercent > 100.0) fuelPercent = 100.0;
    fuelLevelPercentage = fuelPercent;
}

void TransportVehicle::consumeFuel(double fuelPercent) {
    if (fuelPercent > fuelLevelPercentage) {
        throw ReliefOps::Exceptions::FuelDepletionException(
            fuelLevelPercentage, fuelPercent,
            "Vehicle '" + vehicleId + "' fuel depleted during mission execution.");
    }
    fuelLevelPercentage -= fuelPercent;
}

void TransportVehicle::setOperationalStatus(const std::string& status) {
    if (operationalStatus != status) {
        if (operationalStatus == "Operational" && status != "Operational") {
            adjustOperationalVehicleCount(-1);
        } else if (operationalStatus != "Operational" && status == "Operational") {
            adjustOperationalVehicleCount(1);
        }
        operationalStatus = status;
    }
}

void TransportVehicle::setCurrentLocation(const std::string& location) {
    currentLocation = location;
}

std::ostream& operator<<(std::ostream& os, const TransportVehicle& vehicle) {
    os << "[" << vehicle.vehicleId << "] " << std::left << std::setw(32) << vehicle.vehicleName
       << " | Load: " << std::right << std::fixed << std::setprecision(1) << std::setw(6) << vehicle.currentLoadKg
       << "/" << std::setw(6) << vehicle.maxPayloadCapacityKg << " kg"
       << " | Fuel: " << std::setw(5) << vehicle.fuelLevelPercentage << "%"
       << " | Status: " << std::left << std::setw(12) << vehicle.operationalStatus
       << " | Loc: " << vehicle.currentLocation;
    return os;
}

void TransportVehicle::serializeBase(std::ofstream& out) const {
    auto writeStr = [&out](const std::string& str) {
        uint32_t len = static_cast<uint32_t>(str.size());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        if (len > 0) out.write(str.data(), len);
    };

    writeStr(vehicleId);
    writeStr(vehicleName);
    out.write(reinterpret_cast<const char*>(&maxPayloadCapacityKg), sizeof(maxPayloadCapacityKg));
    out.write(reinterpret_cast<const char*>(&currentLoadKg), sizeof(currentLoadKg));
    out.write(reinterpret_cast<const char*>(&fuelLevelPercentage), sizeof(fuelLevelPercentage));
    writeStr(operationalStatus);
    writeStr(currentLocation);
}

void TransportVehicle::deserializeBase(std::ifstream& in) {
    auto readStr = [&in]() -> std::string {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (len == 0) return "";
        std::string s(len, '\0');
        in.read(&s[0], len);
        return s;
    };

    vehicleId = readStr();
    vehicleName = readStr();
    in.read(reinterpret_cast<char*>(&maxPayloadCapacityKg), sizeof(maxPayloadCapacityKg));
    in.read(reinterpret_cast<char*>(&currentLoadKg), sizeof(currentLoadKg));
    in.read(reinterpret_cast<char*>(&fuelLevelPercentage), sizeof(fuelLevelPercentage));
    operationalStatus = readStr();
    currentLocation = readStr();
}

} // namespace Logistics
} // namespace ReliefOps
