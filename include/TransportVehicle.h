#ifndef ERL_HUB_TRANSPORT_VEHICLE_H
#define ERL_HUB_TRANSPORT_VEHICLE_H

#include <string>
#include <iostream>
#include <fstream>

namespace ReliefOps {
namespace Logistics {

/**
 * @brief Inline utility to calculate remaining vehicle payload capacity.
 * Demonstrates Unit I Inline Functions.
 */
inline double calculateRemainingCapacity(double maxPayloadKg, double currentLoadKg) {
    return (maxPayloadKg > currentLoadKg) ? (maxPayloadKg - currentLoadKg) : 0.0;
}

/**
 * @brief Inline utility to check fuel sufficiency.
 * Demonstrates Unit I Inline Functions.
 */
inline bool isFuelSufficient(double currentFuelPercent, double requiredFuelPercent) {
    return currentFuelPercent >= requiredFuelPercent;
}

/**
 * @brief Base vehicle class encapsulating basic transport properties.
 * Demonstrates Unit I Encapsulation, Inline Functions, and Unit II Static Members.
 */
class TransportVehicle {
protected:
    std::string vehicleId;
    std::string vehicleName;
    double maxPayloadCapacityKg;
    double currentLoadKg;
    double fuelLevelPercentage; // 0.0% to 100.0%
    std::string operationalStatus; // "Operational", "In Mission", "Maintenance", "Damaged"
    std::string currentLocation;

    // Unit II Static Data Member: Total count of operational vehicles across fleet
    static int totalOperationalVehicles;

public:
    TransportVehicle();
    TransportVehicle(const std::string& id, 
                     const std::string& name, 
                     double maxPayloadKg, 
                     double fuel = 100.0, 
                     const std::string& status = "Operational",
                     const std::string& location = "Central Hub Dehradun");

    virtual ~TransportVehicle();

    // Static Member Functions (Unit II 7.2)
    static int getTotalOperationalVehicles();
    static void adjustOperationalVehicleCount(int delta);
    static void resetOperationalVehicleCount();

    // Getters (const-correctness)
    std::string getVehicleId() const;
    std::string getVehicleName() const;
    double getMaxPayloadCapacityKg() const;
    double getCurrentLoadKg() const;
    double getRemainingPayloadCapacityKg() const;
    double getFuelLevelPercentage() const;
    std::string getOperationalStatus() const;
    std::string getCurrentLocation() const;
    bool isOperational() const;

    // Operational Setters & Modifiers
    bool checkCapacity(double additionalWeightKg) const;
    bool checkFuel(double requiredFuelPercentage) const;
    void loadPayload(double weightKg);
    void unloadPayload(double weightKg);
    void updateFuel(double fuelPercent);
    void consumeFuel(double fuelPercent);
    void setOperationalStatus(const std::string& status);
    void setCurrentLocation(const std::string& location);

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const TransportVehicle& vehicle);

    // Serialization
    virtual void serializeBase(std::ofstream& out) const;
    virtual void deserializeBase(std::ifstream& in);
};

} // namespace Logistics
} // namespace ReliefOps

#endif // ERL_HUB_TRANSPORT_VEHICLE_H
