#include "../../include/GroundConvoy.h"
#include "../../include/FileManager.h"
#include <iomanip>

namespace ReliefOps {
namespace Logistics {

GroundConvoy::GroundConvoy()
    : ReliefVehicle("GND-001", "Heavy Relief Truck Alpha", 8000.0, 100.0, "Operational", "Central Base", "GPS-GND-01", "ELSU-GND-01"),
      axleCount(6), driveType("6x6 Heavy All-Terrain"), groundClearanceMm(450.0), winchEquipped(true) {}

GroundConvoy::GroundConvoy(const std::string& id, 
                           const std::string& name, 
                           double maxPayloadKg, 
                           double fuel, 
                           const std::string& status,
                           const std::string& location,
                           int axles,
                           const std::string& drive,
                           double clearance,
                           bool winch)
    : ReliefVehicle(id, name, maxPayloadKg, fuel, status, location, "GPS-" + id, "ELSU-" + id),
      axleCount(axles), driveType(drive), groundClearanceMm(clearance), winchEquipped(winch) {}

GroundConvoy::~GroundConvoy() {}

int GroundConvoy::getAxleCount() const {
    return axleCount;
}

std::string GroundConvoy::getDriveType() const {
    return driveType;
}

double GroundConvoy::getGroundClearanceMm() const {
    return groundClearanceMm;
}

bool GroundConvoy::isWinchEquipped() const {
    return winchEquipped;
}

// Unit III Polymorphic implementation: evaluates ground road conditions
bool GroundConvoy::evaluateRoute(const std::string& corridorName, 
                                 double distanceKm, 
                                 int terrainSeverity) const {
    std::cout << "  [Ground Route Evaluation] Vehicle: " << vehicleName << " (" << vehicleId << ")\n";
    std::cout << "  -> Road Corridor: " << corridorName << " | Distance: " << distanceKm 
              << " km | Terrain Damage Severity: " << terrainSeverity << "/10\n";

    // Ground routes with severe earthquake damage (severity >= 6) are impassable due to landslides
    if (terrainSeverity >= 6) {
        std::cout << "  [!] Road Hazard Alert: Corridor '" << corridorName 
                  << "' blocked by massive landslide debris and bridge fracture. Ground convoy impassable.\n";
        return false;
    }

    double fuelNeeded = distanceKm * 0.15; // 0.15% fuel per km on ground
    if (!checkFuel(fuelNeeded)) {
        std::cout << "  [!] Range Warning: Convoy requires " << fuelNeeded 
                  << "% fuel, but current reserve is " << fuelLevelPercentage << "%.\n";
        return false;
    }

    std::cout << "  [✓] Ground Corridor FEASIBLE: All-terrain " << driveType << " cleared for transit.\n";
    return true;
}

bool GroundConvoy::calculateOperationalFeasibility(double payloadWeightKg, double distanceKm) const {
    if (!isOperational()) return false;
    if (!checkCapacity(payloadWeightKg)) return false;
    double fuelNeeded = distanceKm * 0.15;
    return checkFuel(fuelNeeded);
}

void GroundConvoy::displayDetails() const {
    ReliefVehicle::displayDetails();
    std::cout << "   [Ground Convoy Specs] Axles: " << axleCount 
              << " | Drive: " << driveType 
              << " | Clearance: " << groundClearanceMm << " mm"
              << " | Winch: " << (winchEquipped ? "EQUIPPED" : "NONE") << "\n";
}

std::string GroundConvoy::getVehicleType() const {
    return "Ground Convoy";
}

void GroundConvoy::serialize(std::ofstream& out) const {
    ReliefVehicle::serialize(out);
    out.write(reinterpret_cast<const char*>(&axleCount), sizeof(axleCount));
    FileManager::writeString(out, driveType);
    out.write(reinterpret_cast<const char*>(&groundClearanceMm), sizeof(groundClearanceMm));
    out.write(reinterpret_cast<const char*>(&winchEquipped), sizeof(winchEquipped));
}

void GroundConvoy::deserialize(std::ifstream& in) {
    ReliefVehicle::deserialize(in);
    in.read(reinterpret_cast<char*>(&axleCount), sizeof(axleCount));
    driveType = FileManager::readString(in);
    in.read(reinterpret_cast<char*>(&groundClearanceMm), sizeof(groundClearanceMm));
    in.read(reinterpret_cast<char*>(&winchEquipped), sizeof(winchEquipped));
}

} // namespace Logistics
} // namespace ReliefOps
