#ifndef ERL_HUB_FILE_MANAGER_H
#define ERL_HUB_FILE_MANAGER_H

#include <string>
#include <vector>
#include <functional>
#include "ReliefCargo.h"
#include "DistributionDepot.h"
#include "ReliefVehicle.h"

namespace ReliefOps {
namespace Logistics {

/**
 * @brief Manages binary persistence, safe serialization, and atomic file transactions.
 * Demonstrates Unit IV Binary File Handling and Atomic File Updates.
 */
class FileManager {
private:
    std::string dataDirectory;

    // Internal helper for atomic file updates
    static bool atomicCommit(const std::string& tempPath, const std::string& finalPath);

public:
    explicit FileManager(const std::string& dataDir = "data");

    // Serialization helper functions for safe binary string & numeric I/O
    static void writeString(std::ofstream& out, const std::string& str);
    static std::string readString(std::ifstream& in);

    // Persistence methods with atomic transaction guarantees
    bool saveInventory(const std::vector<ReliefOps::Inventory::ReliefCargo>& inventory, 
                       const std::string& filename = "inventory.bin");
    bool loadInventory(std::vector<ReliefOps::Inventory::ReliefCargo>& inventory, 
                       const std::string& filename = "inventory.bin");

    bool saveDepots(const std::vector<DistributionDepot>& depots, 
                    const std::string& filename = "depots.bin");
    bool loadDepots(std::vector<DistributionDepot>& depots, 
                    const std::string& filename = "depots.bin");

    bool saveVehicles(const std::vector<ReliefVehicle*>& fleet, 
                     const std::string& filename = "vehicles.bin");
    bool loadVehicles(std::vector<ReliefVehicle*>& fleet, 
                     const std::string& filename = "vehicles.bin");

    // Append and read dispatch audit history logs
    bool appendDispatchLog(const std::string& logEntry, 
                           const std::string& filename = "dispatch_logs.bin");
    bool readDispatchLogs(std::vector<std::string>& logs, 
                          const std::string& filename = "dispatch_logs.bin");

    // Atomic update demonstration function
    static bool transactionalFileUpdate(const std::string& targetPath, 
                                        const std::function<bool(std::ofstream&)>& writerFunc);

    std::string getDataDirectory() const;
    void setDataDirectory(const std::string& dir);
};

} // namespace Logistics
} // namespace ReliefOps

#endif // ERL_HUB_FILE_MANAGER_H
