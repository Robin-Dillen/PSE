//
// Created by Niels on 5/06/2021.
//

#ifndef PSE_SIMULATIONIMPORTER_H
#define PSE_SIMULATIONIMPORTER_H

#include <string>
#include <map>
#include <vector>

class SimulationImporter;

struct SimulationImporterData;

struct SimulationImporterVaccinData {
    friend SimulationImporterData;
    friend SimulationImporter;
private:
    SimulationImporterVaccinData();

    int aantal_vaccins;
    int eerste_prikken;
    int tweede_prikken;
};

struct SimulationImporterData {
    friend SimulationImporter;
private:
    SimulationImporterData();

    // int[4] = aantal_vaccins, eerste_prikken, 2deprikken
    std::map<std::string, SimulationImporterVaccinData> data;
};

class SimulationImporter {
public:
    SimulationImporter(const std::string &filename);

    int getHubVaccinCount(int hubnr, const std::string &vaccin);

    int getCentrumVaccinCount(int centrumnr, const std::string &vaccin);

    int getAantalVaccinatiesCentrum(int centrumnr, const std::string &vaccin);

    int getAantalEerstePrikken(int centrumnr, const std::string &vaccin);

    bool isProperlyInitialized() const;

private:
    const SimulationImporter *const _initCheck;
    std::vector<SimulationImporterData> hubs_data;
    std::vector<SimulationImporterData> centra_data;
};


#endif //PSE_SIMULATIONIMPORTER_H
