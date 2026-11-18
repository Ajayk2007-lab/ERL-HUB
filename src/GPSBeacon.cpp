#include "../include/GPSBeacon.h"
#include <iomanip>

namespace ReliefOps {
namespace Logistics {

GPSBeacon::GPSBeacon(const std::string& id, double lat, double lon, const std::string& locName)
    : beaconId(id), latitude(lat), longitude(lon), currentLocationName(locName), signalActive(true) {}

void GPSBeacon::updateCoordinates(double lat, double lon, const std::string& locName) {
    latitude = lat;
    longitude = lon;
    if (!locName.empty()) {
        currentLocationName = locName;
    }
}

void GPSBeacon::setSignalStatus(bool active) {
    signalActive = active;
}

std::string GPSBeacon::getBeaconId() const {
    return beaconId;
}

double GPSBeacon::getLatitude() const {
    return latitude;
}

double GPSBeacon::getLongitude() const {
    return longitude;
}

std::string GPSBeacon::getCurrentLocationName() const {
    return currentLocationName;
}

bool GPSBeacon::isSignalActive() const {
    return signalActive;
}

void GPSBeacon::display() const {
    std::cout << "[GPS Beacon " << beaconId << "] Loc: " << currentLocationName
              << " (" << std::fixed << std::setprecision(4) << latitude << " N, " 
              << longitude << " E) | Signal: " 
              << (signalActive ? "LOCKED" : "NO FIX") << "\n";
}

} // namespace Logistics
} // namespace ReliefOps
