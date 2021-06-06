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
    std::map<std::string, SimulationImporterVaccinData> data;
};

class SimulationImporter {
public:
    explicit SimulationImporter(const std::string &filename);

    /*!
     * geeft het aantal vaccins in de gegeven hub terug (hubnr is de index van hubs in simulatie)
     * @param hubnr nummer van de hub
     * @param vaccin type van het vaccin
     * @return het aantal vaccins in de hub met gegeven type
     * \n ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
     * \n ENSURE(aantal >= 0, "Een hub kan geen negatief aantal vaccinaties hebben!");
     */
    int getHubVaccinCount(int hubnr, const std::string &vaccin);

    /*!
     * geeft het aantal vaccins in het gegeven centrum terug (centrumnr is de index van centra in simulatie)
     * @param centrumnr nummer van het centrum
     * @param vaccin type van het vaccin
     * @return het aantal vaccins in het centrum met gegeven type
     * \n ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
     * \n ENSURE(aantal >= 0, "Een centrum kan geen negatief aantal vaccinaties hebben!");
     */
    int getCentrumVaccinCount(const std::string &centrum_naam, const std::string &vaccin);

    /*!
     * geeft het totaal aantal volledige vaccinaties terug van het centrum
     * @param centrumnr vaccin type van het vaccin
     * @return het totaal aantal volledige vaccinaties
     * \n ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
     * \n ENSURE(count >= 0, "Het aantal vaccinaties in een centrum kan nie negatief zijn!");
     */
    int getAantalVaccinatiesCentrum(const std::string &centrum_naam);

    /*!
     * geeft het aantal eerste prikken van het centrum met het bepaalde type terug
     * @param centrumnr centrumnr vaccin type van het vaccin
     * @param vaccin type van het vaccin
     * @return het aantal eerste prikken in hub met gegeven type
     * \n ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
     * \n ENSURE(totaal >= 0, "Een centrum kan geen negatief aantal eerste prikken hebben!");
     */
    int getTotaalAantalEerstePrikken(const std::string &centrum_naam);

    /*!
     * @return geeft terug of het object juist geïnitialiseert is
     */
    bool isProperlyInitialized() const;

private:
    const SimulationImporter *const _initCheck;
    std::vector<SimulationImporterData> hubs_data;
    std::map<std::string, SimulationImporterData> centra_data;
};


#endif //PSE_SIMULATIONIMPORTER_H
