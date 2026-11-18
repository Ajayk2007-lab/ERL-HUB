#include "../include/ReliefVehicle.h"
#include "../include/FileManager.h"
#include <iomanip>

namespace ReliefOps {
namespace Logistics {

ReliefVehicle::ReliefVehicle()
    : TransportVehicle("REL-000", "Generic Relief Vehicle", 1500.0, 100.0, "Operational", "Central Hub"),
      gpsBeacon("GPS-000", 30.3165, 78.0322, "Central Hub"),
      lifeSupport("ELSU-000", true, 200.0, 500.0, "Operational", true) {}

ReliefVehicle::ReliefVehicle(const std::string& id, 
                             const std::string& name, 
                             double maxPayloadKg, 
                             double fuel, 
                             const std::string& status,
                             const std::string& location,
                             const std::string& beaconId,
                             const std::string& elsuId)
    : TransportVehicle(id, name, maxPayloadKg, fuel, status, location),
      gpsBeacon(beaconId, 30.3165, 78.0322, location),
      lifeSupport(elsuId, true, 200.0, 500.0, "Operational", true) {}

// Unit III Virtual Destructor
ReliefVehicle::~ReliefVehicle() {
    // Explicit virtual destructor ensures proper cleanup of derived class subobjects
}

GPSBeacon& ReliefVehicle::getGPSBeacon() {
    return gpsBeacon;
}

const GPSBeacon& ReliefVehicle::getGPSBeacon() const {
    return gpsBeacon;
}

EmergencyLifeSupportUnit& ReliefVehicle::getLifeSupport() {
    return lifeSupport;
}

const EmergencyLifeSupportUnit& ReliefVehicle::getLifeSupport() const {
    return lifeSupport;
}

bool ReliefVehicle::calculateOperationalFeasibility(double payloadWeightKg, double distanceKm) const {
    // Basic operational check: must be operational, within capacity, and fuel must cover distance
    if (!isOperational()) return false;
    if (!checkCapacity(payloadWeightKg)) return false;
    // Assuming base fuel consumption of 0.25% per km
    double requiredFuel = distanceKm * 0.25;
    return checkFuel(requiredFuel);
}

void ReliefVehicle::displayDetails() const {
    std::cout << "================================================================================\n";
    std::cout << " VEHICLE SPECIFICATIONS: " << vehicleName << " [" << vehicleId << "] Type: " << getVehicleType() << "\n";
    std::cout << " Operational Status: " << operationalStatus << " | Current Location: " << currentLocation << "\n";
    std::cout << " Payload Capacity: " << std::fixed << std::setprecision(1) << currentLoadKg << " / " << maxPayloadCapacityKg 
              << " kg (" << getRemainingPayloadCapacityKg() << " kg available)\n";
    std::cout << " Fuel Level: " << fuelLevelPercentage << "%\n";
    std::cout << " Embedded Subsystems (Containership):\n";
    std::cout << "   ";
    gpsBeacon.display();
    std::cout << "   ";
    lifeSupport.display();
    std::cout << "================================================================================\n";
}

void ReliefVehicle::serialize(std::ofstream& out) const {
    serializeBase(out);
    // Serialize containership states
    FileManager::writeString(out, gpsBeacon.getBeaconId());
    double lat = gpsBeacon.getLatitude();
    double lon = gpsBeacon.getLongitude();
    out.write(reinterpret_cast<const char*>(&lat), sizeof(lat));
    out.write(reinterpret_cast<const char*>(&lon), sizeof(lon));
    FileManager::writeString(out, gpsBeacon.getCurrentLocationName());

    FileManager::writeString(out, lifeSupport.getUnitId());
    bool medAvail = lifeSupport.isMedicalSupportAvailable();
    double o2 = lifeSupport.getOxygenSupplyLiters();
    double maxO2 = lifeSupport.getMaxOxygenCapacityLiters();
    FileManager::writeString(out, lifeSupport.getEquipmentStatus());
    bool defib = lifeSupport.isDefibrillatorReady();

    out.write(reinterpret_cast<const char*>(&medAvail), sizeof(medAvail));
    out.write(reinterpret_cast<const char*>(&o2), sizeof(o2));
    out.write(reinterpret_cast<const char*>(&maxO2), sizeof(maxO2));
    out.write(reinterpret_cast<const char*>(&defib), sizeof(defib));
}

void ReliefVehicle::deserialize(std::ifstream& in) {
    deserializeBase(in);
    std::string bId = FileManager::readString(in);
    double lat = 0.0, lon = 0.0;
    in.read(reinterpret_cast<char*>(&lat), sizeof(lat));
    in.read(reinterpret_cast<char*>(&lon), sizeof(lon));
    std::string bLoc = FileManager::readString(in);
    gpsBeacon = GPSBeacon(bId, lat, lon, bLoc);

    std::string uId = FileManager::readString(in);
    std::string eqStatus = FileManager::readString(in);
    bool medAvail = false, defib = false;
    double o2 = 0.0, maxO2 = 0.0;
    in.read(reinterpret_cast<char*>(&medAvail), sizeof(medAvail));
    in.read(reinterpret_cast<char*>(&o2), sizeof(o2));
    in.read(reinterpret_cast<char*>(&maxO2), sizeof(maxO2));
    in.read(reinterpret_cast<char*>(&defib), sizeof(defib));
    lifeSupport = EmergencyLifeSupportUnit(uId, medAvail, o2, maxO2, eqStatus, defib);
}

} // namespace Logistics
} // namespace ReliefOps
