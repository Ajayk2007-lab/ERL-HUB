#include "../include/FileManager.h"
#include "../include/AerialTransport.h"
#include "../include/GroundConvoy.h"
#include "../include/AmphibiousCraft.h"
#include "../include/HybridRescueVehicle.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdint>

namespace ReliefOps {
namespace Logistics {

FileManager::FileManager(const std::string& dataDir) : dataDirectory(dataDir) {}

std::string FileManager::getDataDirectory() const {
    return dataDirectory;
}

void FileManager::setDataDirectory(const std::string& dir) {
    dataDirectory = dir;
}

void FileManager::writeString(std::ofstream& out, const std::string& str) {
    uint32_t len = static_cast<uint32_t>(str.size());
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    if (len > 0) {
        out.write(str.data(), len);
    }
}

std::string FileManager::readString(std::ifstream& in) {
    uint32_t len = 0;
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (in.eof() || in.fail() || len == 0) {
        return "";
    }
    std::string s(len, '\0');
    in.read(&s[0], len);
    return s;
}

// Unit IV 9.3: Atomic file replacement with rollback guarantee
bool FileManager::atomicCommit(const std::string& tempPath, const std::string& finalPath) {
    // Check temporary file exists and is valid
    std::ifstream checkTemp(tempPath, std::ios::binary | std::ios::ate);
    if (!checkTemp.is_open() || checkTemp.tellg() <= 0) {
        checkTemp.close();
        std::remove(tempPath.c_str()); // Rollback
        return false;
    }
    checkTemp.close();

    // Windows compatibility: std::rename fails if destination file exists
    // We create a backup of finalPath, replace, and remove backup on success
    std::string backupPath = finalPath + ".bak";
    std::remove(backupPath.c_str());

    bool hadOriginal = false;
    std::ifstream testOriginal(finalPath, std::ios::binary);
    if (testOriginal.is_open()) {
        hadOriginal = true;
        testOriginal.close();
        // Rename original to backup
        if (std::rename(finalPath.c_str(), backupPath.c_str()) != 0) {
            std::remove(tempPath.c_str()); // Rollback
            return false;
        }
    }

    // Move temp file to final destination
    if (std::rename(tempPath.c_str(), finalPath.c_str()) != 0) {
        // Rollback: restore backup if available
        if (hadOriginal) {
            std::rename(backupPath.c_str(), finalPath.c_str());
        }
        std::remove(tempPath.c_str());
        return false;
    }

    // Success: remove backup
    if (hadOriginal) {
        std::remove(backupPath.c_str());
    }
    return true;
}

bool FileManager::transactionalFileUpdate(const std::string& targetPath, 
                                         const std::function<bool(std::ofstream&)>& writerFunc) {
    std::string tempPath = targetPath + ".tmp";
    std::ofstream tempOut(tempPath, std::ios::binary | std::ios::trunc);
    if (!tempOut.is_open()) {
        std::cerr << "  [!] Atomic Transaction Error: Cannot create temp file " << tempPath << "\n";
        return false;
    }

    bool writeSuccess = false;
    try {
        writeSuccess = writerFunc(tempOut);
        tempOut.flush();
    } catch (...) {
        writeSuccess = false;
    }

    tempOut.close();

    if (!writeSuccess) {
        std::cerr << "  [!] Atomic Transaction Rollback: Write operation failed or was aborted.\n";
        std::remove(tempPath.c_str());
        return false;
    }

    // Commit temp to final file
    if (!atomicCommit(tempPath, targetPath)) {
        std::cerr << "  [!] Atomic Transaction Rollback: Commit failed for " << targetPath << "\n";
        return false;
    }

    return true;
}

bool FileManager::saveInventory(const std::vector<ReliefOps::Inventory::ReliefCargo>& inventory, 
                                const std::string& filename) {
    std::string fullPath = dataDirectory + "/" + filename;
    return transactionalFileUpdate(fullPath, [&inventory](std::ofstream& out) -> bool {
        uint32_t count = static_cast<uint32_t>(inventory.size());
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        for (const auto& item : inventory) {
            item.serialize(out);
        }
        return out.good();
    });
}

bool FileManager::loadInventory(std::vector<ReliefOps::Inventory::ReliefCargo>& inventory, 
                                const std::string& filename) {
    std::string fullPath = dataDirectory + "/" + filename;
    std::ifstream in(fullPath, std::ios::binary);
    if (!in.is_open()) return false;

    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (in.fail()) return false;

    inventory.clear();
    for (uint32_t i = 0; i < count; ++i) {
        ReliefOps::Inventory::ReliefCargo cargo;
        cargo.deserialize(in);
        inventory.push_back(cargo);
    }
    return in.good() || in.eof();
}

bool FileManager::saveDepots(const std::vector<DistributionDepot>& depots, 
                             const std::string& filename) {
    std::string fullPath = dataDirectory + "/" + filename;
    return transactionalFileUpdate(fullPath, [&depots](std::ofstream& out) -> bool {
        uint32_t count = static_cast<uint32_t>(depots.size());
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        for (const auto& depot : depots) {
            depot.serialize(out);
        }
        return out.good();
    });
}

bool FileManager::loadDepots(std::vector<DistributionDepot>& depots, 
                             const std::string& filename) {
    std::string fullPath = dataDirectory + "/" + filename;
    std::ifstream in(fullPath, std::ios::binary);
    if (!in.is_open()) return false;

    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (in.fail()) return false;

    DistributionDepot::resetDepotCounter();
    depots.clear();
    for (uint32_t i = 0; i < count; ++i) {
        DistributionDepot depot;
        depot.deserialize(in);
        depots.push_back(depot);
    }
    return in.good() || in.eof();
}

bool FileManager::saveVehicles(const std::vector<ReliefVehicle*>& fleet, 
                              const std::string& filename) {
    std::string fullPath = dataDirectory + "/" + filename;
    return transactionalFileUpdate(fullPath, [&fleet](std::ofstream& out) -> bool {
        uint32_t count = static_cast<uint32_t>(fleet.size());
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        for (const auto* v : fleet) {
            if (!v) continue;
            std::string type = v->getVehicleType();
            writeString(out, type);
            v->serialize(out);
        }
        return out.good();
    });
}

bool FileManager::loadVehicles(std::vector<ReliefVehicle*>& fleet, 
                              const std::string& filename) {
    std::string fullPath = dataDirectory + "/" + filename;
    std::ifstream in(fullPath, std::ios::binary);
    if (!in.is_open()) return false;

    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (in.fail()) return false;

    // Clean up existing dynamically allocated fleet
    for (auto* v : fleet) {
        delete v;
    }
    fleet.clear();
    TransportVehicle::resetOperationalVehicleCount();

    for (uint32_t i = 0; i < count; ++i) {
        std::string type = readString(in);
        ReliefVehicle* vehicle = nullptr;
        if (type == "Aerial Transport") {
            vehicle = new AerialTransport();
        } else if (type == "Ground Convoy") {
            vehicle = new GroundConvoy();
        } else if (type == "Amphibious Craft") {
            vehicle = new AmphibiousCraft();
        } else if (type == "Hybrid Rescue Vehicle") {
            vehicle = new HybridRescueVehicle();
        }

        if (vehicle) {
            vehicle->deserialize(in);
            fleet.push_back(vehicle);
        }
    }
    return true;
}

bool FileManager::appendDispatchLog(const std::string& logEntry, const std::string& filename) {
    std::string fullPath = dataDirectory + "/" + filename;
    std::ofstream out(fullPath, std::ios::binary | std::ios::app);
    if (!out.is_open()) return false;
    writeString(out, logEntry);
    return out.good();
}

bool FileManager::readDispatchLogs(std::vector<std::string>& logs, const std::string& filename) {
    std::string fullPath = dataDirectory + "/" + filename;
    std::ifstream in(fullPath, std::ios::binary);
    if (!in.is_open()) return false;

    logs.clear();
    while (in.peek() != EOF) {
        std::string entry = readString(in);
        if (!entry.empty()) {
            logs.push_back(entry);
        }
    }
    return true;
}

} // namespace Logistics
} // namespace ReliefOps
