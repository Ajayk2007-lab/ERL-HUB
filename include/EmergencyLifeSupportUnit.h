#ifndef ERL_HUB_EMERGENCY_LIFE_SUPPORT_UNIT_H
#define ERL_HUB_EMERGENCY_LIFE_SUPPORT_UNIT_H

#include <string>
#include <iostream>

namespace ReliefOps {
namespace Logistics {

/**
 * @brief Represents an onboard emergency medical and life support unit.
 * Demonstrates Unit III Containership (Composition).
 */
class EmergencyLifeSupportUnit {
private:
    std::string unitId;
    bool medicalSupportAvailable;
    double oxygenSupplyLiters;
    double maxOxygenCapacityLiters;
    std::string equipmentStatus; // "Operational", "Standby", "Maintenance"
    bool defibrillatorReady;

public:
    EmergencyLifeSupportUnit(const std::string& id = "ELSU-01", 
                             bool available = true, 
                             double oxygenLiters = 200.0, 
                             double maxOxygen = 500.0,
                             const std::string& status = "Operational",
                             bool defib = true);

    bool checkLifeSupport() const;
    void refillOxygen(double liters);
    bool consumeOxygen(double liters);
    void setEquipmentStatus(const std::string& status);
    void setDefibrillatorStatus(bool ready);

    // Getters
    std::string getUnitId() const;
    bool isMedicalSupportAvailable() const;
    double getOxygenSupplyLiters() const;
    double getMaxOxygenCapacityLiters() const;
    std::string getEquipmentStatus() const;
    bool isDefibrillatorReady() const;

    void display() const;
};

} // namespace Logistics
} // namespace ReliefOps

#endif // ERL_HUB_EMERGENCY_LIFE_SUPPORT_UNIT_H
