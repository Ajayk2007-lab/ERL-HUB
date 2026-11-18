#include "../../include/AerialTransport.h"
#include "../../include/FileManager.h"
#include <iomanip>

namespace ReliefOps {
namespace Logistics {

AerialTransport::AerialTransport()
    : ReliefVehicle("AER-001", "Emergency Chopper Alpha", 1200.0, 100.0, "Operational", "Helipad Alpha", "GPS-AER-01", "ELSU-AER-01"),
      maxAltitudeMeters(4500.0), rotorConfiguration("Twin-Turbine Medical Chopper"), maxWindToleranceKnots(50.0) {}

AerialTransport::AerialTransport(const std::string& id, 
                                 const std::string& name, 
                                 double maxPayloadKg, 
                                 double fuel, 
                                 const std::string& status,
                                 const std::string& location,
                                 double maxAlt,
                                 const std::string& rotor,
                                 double maxWind)
    : ReliefVehicle(id, name, maxPayloadKg, fuel, status, location, "GPS-" + id, "ELSU-" + id),
      maxAltitudeMeters(maxAlt), rotorConfiguration(rotor), maxWindToleranceKnots(maxWind) {}

AerialTransport::~AerialTransport() {}

double AerialTransport::getMaxAltitudeMeters() const {
    return maxAltitudeMeters;
}

std::string AerialTransport::getRotorConfiguration() const {
    return rotorConfiguration;
}

double AerialTransport::getMaxWindToleranceKnots() const {
    return maxWindToleranceKnots;
}

// Unit III Polymorphic implementation: evaluates air routes bypassing ground debris
bool AerialTransport::evaluateRoute(const std::string& corridorName, 
                                    double distanceKm, 
                                    int terrainSeverity) const {
    std::cout << "  [Aerial Route Evaluation] Vehicle: " << vehicleName << " (" << vehicleId << ")\n";
    std::cout << "  -> Flight Corridor: " << corridorName << " | Distance: " << distanceKm 
              << " km | Ground Severity: " << terrainSeverity << "/10\n";
    std::cout << "  -> Flight Profile: Chopper bypasses ground obstacles, landslides, and collapsed bridges.\n";

    // Aerial vehicles are unaffected by ground rubble, but affected by mountain gale winds (severity >= 8)
    if (terrainSeverity >= 8) {
        std::cout << "  [!] Weather Alert: High-altitude mountain gales exceed safe operating limit (" 
                  << maxWindToleranceKnots << " knots). Flight aborted.\n";
        return false;
    }

    double fuelNeeded = distanceKm * 0.35; // 0.35% fuel per km in mountain air
    if (!checkFuel(fuelNeeded)) {
        std::cout << "  [!] Range Warning: Flight distance requires " << fuelNeeded 
                  << "% fuel, but current reserve is " << fuelLevelPercentage << "%.\n";
        return false;
    }

    std::cout << "  [✓] Aerial Corridor FEASIBLE: Airspace clear. Landing zone verified.\n";
    return true;
}

bool AerialTransport::calculateOperationalFeasibility(double payloadWeightKg, double distanceKm) const {
    if (!isOperational()) return false;
    if (!checkCapacity(payloadWeightKg)) return false;
    double fuelNeeded = distanceKm * 0.35;
    return checkFuel(fuelNeeded);
}

void AerialTransport::displayDetails() const {
    ReliefVehicle::displayDetails();
    std::cout << "   [Aerial Transport Specs] Max Altitude: " << maxAltitudeMeters 
              << " m | Rotor: " << rotorConfiguration 
              << " | Wind Tolerance: " << maxWindToleranceKnots << " knots\n";
}

std::string AerialTransport::getVehicleType() const {
    return "Aerial Transport";
}

void AerialTransport::serialize(std::ofstream& out) const {
    ReliefVehicle::serialize(out);
    out.write(reinterpret_cast<const char*>(&maxAltitudeMeters), sizeof(maxAltitudeMeters));
    FileManager::writeString(out, rotorConfiguration);
    out.write(reinterpret_cast<const char*>(&maxWindToleranceKnots), sizeof(maxWindToleranceKnots));
}

void AerialTransport::deserialize(std::ifstream& in) {
    ReliefVehicle::deserialize(in);
    in.read(reinterpret_cast<char*>(&maxAltitudeMeters), sizeof(maxAltitudeMeters));
    rotorConfiguration = FileManager::readString(in);
    in.read(reinterpret_cast<char*>(&maxWindToleranceKnots), sizeof(maxWindToleranceKnots));
}

} // namespace Logistics
} // namespace ReliefOps
