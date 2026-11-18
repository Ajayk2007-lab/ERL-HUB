#include "../include/ReliefCargo.h"
#include "../include/Exceptions.h"
#include <iomanip>
#include <cstdint>

namespace ReliefOps {
namespace Inventory {

ReliefCargo::ReliefCargo()
    : cargoId("CRG-000"), cargoName("Unspecified"), category("General"),
      availableQuantity(0), unitOfMeasure("units"), weightPerUnitKg(1.0),
      emergencyPriorityLevel(1), medicalFlag(false) {}

ReliefCargo::ReliefCargo(const std::string& id, 
                         const std::string& name, 
                         const std::string& cat,
                         int quantity, 
                         const std::string& unit, 
                         double weightPerUnit,
                         int priority, 
                         bool isMed)
    : cargoId(id), cargoName(name), category(cat), unitOfMeasure(unit), medicalFlag(isMed) {
    if (quantity < 0) {
        throw ReliefOps::Exceptions::ERLHubException("Validation Error: Cargo quantity cannot be negative.");
    }
    availableQuantity = quantity;

    if (weightPerUnit <= 0.0) {
        throw ReliefOps::Exceptions::ERLHubException("Validation Error: Cargo unit weight must be positive.");
    }
    weightPerUnitKg = weightPerUnit;

    if (priority < 1 || priority > 5) {
        throw ReliefOps::Exceptions::ERLHubException("Validation Error: Emergency priority level must be between 1 and 5.");
    }
    emergencyPriorityLevel = priority;
}

std::string ReliefCargo::getCargoId() const {
    return cargoId;
}

std::string ReliefCargo::getCargoName() const {
    return cargoName;
}

std::string ReliefCargo::getCategory() const {
    return category;
}

int ReliefCargo::getAvailableQuantity() const {
    return availableQuantity;
}

std::string ReliefCargo::getUnitOfMeasure() const {
    return unitOfMeasure;
}

double ReliefCargo::getWeightPerUnitKg() const {
    return weightPerUnitKg;
}

int ReliefCargo::getEmergencyPriorityLevel() const {
    return emergencyPriorityLevel;
}

bool ReliefCargo::isMedical() const {
    return medicalFlag;
}

double ReliefCargo::getTotalWeightKg() const {
    return calculateTotalCargoWeight(availableQuantity, weightPerUnitKg);
}

void ReliefCargo::setAvailableQuantity(int qty) {
    if (qty < 0) {
        throw ReliefOps::Exceptions::ERLHubException("Validation Error: Quantity cannot be negative (" + std::to_string(qty) + ")");
    }
    availableQuantity = qty;
}

void ReliefCargo::updateQuantity(int delta) {
    int newQty = availableQuantity + delta;
    if (newQty < 0) {
        throw ReliefOps::Exceptions::InsufficientStockException(cargoId, -delta, availableQuantity);
    }
    availableQuantity = newQty;
}

void ReliefCargo::setEmergencyPriorityLevel(int priority) {
    if (priority < 1 || priority > 5) {
        throw ReliefOps::Exceptions::ERLHubException("Validation Error: Priority must be between 1 and 5.");
    }
    emergencyPriorityLevel = priority;
}

void ReliefCargo::setWeightPerUnitKg(double weight) {
    if (weight <= 0.0) {
        throw ReliefOps::Exceptions::ERLHubException("Validation Error: Weight per unit must be greater than zero.");
    }
    weightPerUnitKg = weight;
}

bool ReliefCargo::isUrgentMedicalSupply() const {
    return medicalFlag && (emergencyPriorityLevel >= 4);
}

bool ReliefCargo::hasStock(int requestedQty) const {
    return requestedQty > 0 && availableQuantity >= requestedQty;
}

// Unit IV Binary Serialization
void ReliefCargo::serialize(std::ofstream& out) const {
    auto writeStr = [&out](const std::string& str) {
        uint32_t len = static_cast<uint32_t>(str.size());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        if (len > 0) {
            out.write(str.data(), len);
        }
    };

    writeStr(cargoId);
    writeStr(cargoName);
    writeStr(category);
    out.write(reinterpret_cast<const char*>(&availableQuantity), sizeof(availableQuantity));
    writeStr(unitOfMeasure);
    out.write(reinterpret_cast<const char*>(&weightPerUnitKg), sizeof(weightPerUnitKg));
    out.write(reinterpret_cast<const char*>(&emergencyPriorityLevel), sizeof(emergencyPriorityLevel));
    out.write(reinterpret_cast<const char*>(&medicalFlag), sizeof(medicalFlag));
}

void ReliefCargo::deserialize(std::ifstream& in) {
    auto readStr = [&in]() -> std::string {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (len == 0) return "";
        std::string s(len, '\0');
        in.read(&s[0], len);
        return s;
    };

    cargoId = readStr();
    cargoName = readStr();
    category = readStr();
    in.read(reinterpret_cast<char*>(&availableQuantity), sizeof(availableQuantity));
    unitOfMeasure = readStr();
    in.read(reinterpret_cast<char*>(&weightPerUnitKg), sizeof(weightPerUnitKg));
    in.read(reinterpret_cast<char*>(&emergencyPriorityLevel), sizeof(emergencyPriorityLevel));
    in.read(reinterpret_cast<char*>(&medicalFlag), sizeof(medicalFlag));
}

// Unit II Operator Overloading
bool ReliefCargo::operator==(const ReliefCargo& other) const {
    return cargoId == other.cargoId;
}

bool ReliefCargo::operator!=(const ReliefCargo& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const ReliefCargo& cargo) {
    os << "[" << cargo.cargoId << "] " << std::left << std::setw(28) << cargo.cargoName
       << " | Cat: " << std::setw(22) << cargo.category
       << " | Stock: " << std::right << std::setw(5) << cargo.availableQuantity << " " << std::left << std::setw(6) << cargo.unitOfMeasure
       << " | UnitWt: " << std::right << std::fixed << std::setprecision(1) << std::setw(5) << cargo.weightPerUnitKg << " kg"
       << " | Total: " << std::setw(7) << cargo.getTotalWeightKg() << " kg"
       << " | Prio: " << cargo.emergencyPriorityLevel
       << (cargo.medicalFlag ? " [MED]" : "      ");
    return os;
}

std::istream& operator>>(std::istream& is, ReliefCargo& cargo) {
    std::cout << "Enter Cargo ID (e.g. CRG-105): ";
    is >> cargo.cargoId;
    is.ignore(1000, '\n');

    std::cout << "Enter Cargo Name: ";
    std::getline(is, cargo.cargoName);

    std::cout << "Enter Category (e.g. Clean Water, Triage Medicine): ";
    std::getline(is, cargo.category);

    std::cout << "Enter Available Quantity: ";
    is >> cargo.availableQuantity;

    std::cout << "Enter Unit of Measurement (e.g. Kits, Liters, Boxes): ";
    is >> cargo.unitOfMeasure;

    std::cout << "Enter Weight Per Unit (in kg): ";
    is >> cargo.weightPerUnitKg;

    std::cout << "Enter Emergency Priority Level (1-5, 5 is critical): ";
    is >> cargo.emergencyPriorityLevel;

    std::cout << "Is this a medical supply? (1 for Yes, 0 for No): ";
    int medInt = 0;
    is >> medInt;
    cargo.medicalFlag = (medInt != 0);

    return is;
}

} // namespace Inventory
} // namespace ReliefOps
