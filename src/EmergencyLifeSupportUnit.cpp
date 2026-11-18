#include "../include/EmergencyLifeSupportUnit.h"
#include <iomanip>

namespace ReliefOps {
namespace Logistics {

EmergencyLifeSupportUnit::EmergencyLifeSupportUnit(const std::string& id, 
                                                   bool available, 
                                                   double oxygenLiters, 
                                                   double maxOxygen,
                                                   const std::string& status,
                                                   bool defib)
    : unitId(id), medicalSupportAvailable(available), 
      oxygenSupplyLiters(oxygenLiters), maxOxygenCapacityLiters(maxOxygen),
      equipmentStatus(status), defibrillatorReady(defib) {}

bool EmergencyLifeSupportUnit::checkLifeSupport() const {
    return medicalSupportAvailable && (oxygenSupplyLiters > 10.0) && (equipmentStatus == "Operational");
}

void EmergencyLifeSupportUnit::refillOxygen(double liters) {
    if (liters > 0.0) {
        oxygenSupplyLiters += liters;
        if (oxygenSupplyLiters > maxOxygenCapacityLiters) {
            oxygenSupplyLiters = maxOxygenCapacityLiters;
        }
    }
}

bool EmergencyLifeSupportUnit::consumeOxygen(double liters) {
    if (liters > 0.0 && oxygenSupplyLiters >= liters) {
        oxygenSupplyLiters -= liters;
        return true;
    }
    return false;
}

void EmergencyLifeSupportUnit::setEquipmentStatus(const std::string& status) {
    equipmentStatus = status;
}

void EmergencyLifeSupportUnit::setDefibrillatorStatus(bool ready) {
    defibrillatorReady = ready;
}

std::string EmergencyLifeSupportUnit::getUnitId() const {
    return unitId;
}

bool EmergencyLifeSupportUnit::isMedicalSupportAvailable() const {
    return medicalSupportAvailable;
}

double EmergencyLifeSupportUnit::getOxygenSupplyLiters() const {
    return oxygenSupplyLiters;
}

double EmergencyLifeSupportUnit::getMaxOxygenCapacityLiters() const {
    return maxOxygenCapacityLiters;
}

std::string EmergencyLifeSupportUnit::getEquipmentStatus() const {
    return equipmentStatus;
}

bool EmergencyLifeSupportUnit::isDefibrillatorReady() const {
    return defibrillatorReady;
}

void EmergencyLifeSupportUnit::display() const {
    std::cout << "[Life Support Unit " << unitId << "] Status: " << equipmentStatus
              << " | O2 Supply: " << std::fixed << std::setprecision(1) << oxygenSupplyLiters 
              << "/" << maxOxygenCapacityLiters << " L"
              << " | Defibrillator: " << (defibrillatorReady ? "ARMED" : "OFFLINE")
              << " | MedReady: " << (checkLifeSupport() ? "YES" : "NO") << "\n";
}

} // namespace Logistics
} // namespace ReliefOps
