#include "../include/Manifest.h"
#include "../include/Exceptions.h"
#include <iomanip>
#include <cstdint>

namespace ReliefOps {
namespace Inventory {

// Internal helper to resize dynamic array
void Manifest::resize(int newCapacity) {
    if (newCapacity <= capacity) return;
    ReliefCargo* newArray = new ReliefCargo[newCapacity];
    for (int i = 0; i < count; ++i) {
        newArray[i] = cargoRecords[i]; // Value copy
    }
    delete[] cargoRecords;
    cargoRecords = newArray;
    capacity = newCapacity;
}

// Unit II Dynamic Constructor
Manifest::Manifest(const std::string& id, 
                   const std::string& destination, 
                   const std::string& vehicleId, 
                   int initialCapacity)
    : manifestId(id), destinationDepot(destination), assignedVehicleId(vehicleId),
      capacity(initialCapacity > 0 ? initialCapacity : 5), count(0) {
    cargoRecords = new ReliefCargo[capacity];
}

// Unit II Rule of Three #1: Deep Copy Constructor
Manifest::Manifest(const Manifest& other)
    : manifestId(other.manifestId), destinationDepot(other.destinationDepot),
      assignedVehicleId(other.assignedVehicleId), capacity(other.capacity), count(other.count) {
    // Allocate completely independent memory buffer
    cargoRecords = new ReliefCargo[capacity];
    for (int i = 0; i < count; ++i) {
        cargoRecords[i] = other.cargoRecords[i]; // Deep value copy of each cargo
    }
}

// Unit II Rule of Three #2: Copy Assignment Operator
Manifest& Manifest::operator=(const Manifest& other) {
    if (this != &other) { // Guard against self-assignment
        // Free existing dynamically allocated resource
        delete[] cargoRecords;

        // Copy scalar members
        manifestId = other.manifestId;
        destinationDepot = other.destinationDepot;
        assignedVehicleId = other.assignedVehicleId;
        capacity = other.capacity;
        count = other.count;

        // Allocate fresh dynamic memory
        cargoRecords = new ReliefCargo[capacity];
        for (int i = 0; i < count; ++i) {
            cargoRecords[i] = other.cargoRecords[i]; // Deep copy
        }
    }
    return *this;
}

// Unit II Rule of Three #3: Explicit Destructor
Manifest::~Manifest() {
    delete[] cargoRecords;
    cargoRecords = nullptr;
}

void Manifest::addCargo(const ReliefCargo& cargo) {
    if (count >= capacity) {
        resize(capacity * 2);
    }
    cargoRecords[count++] = cargo;
}

bool Manifest::removeCargo(const std::string& cargoId) {
    for (int i = 0; i < count; ++i) {
        if (cargoRecords[i].getCargoId() == cargoId) {
            for (int j = i; j < count - 1; ++j) {
                cargoRecords[j] = cargoRecords[j + 1];
            }
            --count;
            return true;
        }
    }
    return false;
}

void Manifest::clear() {
    count = 0;
}

std::string Manifest::getManifestId() const {
    return manifestId;
}

std::string Manifest::getDestinationDepot() const {
    return destinationDepot;
}

std::string Manifest::getAssignedVehicleId() const {
    return assignedVehicleId;
}

int Manifest::getCount() const {
    return count;
}

int Manifest::getCapacity() const {
    return capacity;
}

double Manifest::getTotalWeightKg() const {
    double total = 0.0;
    for (int i = 0; i < count; ++i) {
        total += cargoRecords[i].getTotalWeightKg();
    }
    return total;
}

const ReliefCargo* Manifest::getCargoRecords() const {
    return cargoRecords;
}

const ReliefCargo& Manifest::getCargoAt(int index) const {
    if (index < 0 || index >= count) {
        throw ReliefOps::Exceptions::ERLHubException("Manifest index out of bounds: " + std::to_string(index));
    }
    return cargoRecords[index];
}

void Manifest::setDestinationDepot(const std::string& destination) {
    destinationDepot = destination;
}

void Manifest::setAssignedVehicleId(const std::string& vehicleId) {
    assignedVehicleId = vehicleId;
}

// Unit II Operator Overloading
bool Manifest::operator==(const Manifest& other) const {
    return manifestId == other.manifestId;
}

bool Manifest::operator!=(const Manifest& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Manifest& manifest) {
    os << "================================================================================\n";
    os << " DISPATCH CARGO MANIFEST: " << manifest.manifestId << "\n";
    os << " Destination: " << std::left << std::setw(25) << manifest.destinationDepot
       << " Assigned Vehicle: " << manifest.assignedVehicleId << "\n";
    os << " Cargo Records: " << manifest.count << " (Buffer Capacity: " << manifest.capacity << ")\n";
    os << " Total Payload Weight: " << std::fixed << std::setprecision(2) << manifest.getTotalWeightKg() << " kg\n";
    os << "--------------------------------------------------------------------------------\n";
    if (manifest.count == 0) {
        os << "   [No cargo records registered in this manifest]\n";
    } else {
        for (int i = 0; i < manifest.count; ++i) {
            os << "   " << (i + 1) << ". " << manifest.cargoRecords[i] << "\n";
        }
    }
    os << "================================================================================\n";
    return os;
}

std::istream& operator>>(std::istream& is, Manifest& manifest) {
    std::cout << "Enter Manifest ID: ";
    is >> manifest.manifestId;
    is.ignore(1000, '\n');
    std::cout << "Enter Destination Depot: ";
    std::getline(is, manifest.destinationDepot);
    std::cout << "Enter Assigned Vehicle ID: ";
    std::getline(is, manifest.assignedVehicleId);
    return is;
}

void Manifest::serialize(std::ofstream& out) const {
    auto writeStr = [&out](const std::string& str) {
        uint32_t len = static_cast<uint32_t>(str.size());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        if (len > 0) out.write(str.data(), len);
    };

    writeStr(manifestId);
    writeStr(destinationDepot);
    writeStr(assignedVehicleId);
    out.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (int i = 0; i < count; ++i) {
        cargoRecords[i].serialize(out);
    }
}

void Manifest::deserialize(std::ifstream& in) {
    auto readStr = [&in]() -> std::string {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (len == 0) return "";
        std::string s(len, '\0');
        in.read(&s[0], len);
        return s;
    };

    manifestId = readStr();
    destinationDepot = readStr();
    assignedVehicleId = readStr();
    int fileCap = 0, fileCount = 0;
    in.read(reinterpret_cast<char*>(&fileCap), sizeof(fileCap));
    in.read(reinterpret_cast<char*>(&fileCount), sizeof(fileCount));

    delete[] cargoRecords;
    capacity = (fileCap > 0) ? fileCap : 5;
    count = fileCount;
    cargoRecords = new ReliefCargo[capacity];

    for (int i = 0; i < count; ++i) {
        cargoRecords[i].deserialize(in);
    }
}

} // namespace Inventory
} // namespace ReliefOps
