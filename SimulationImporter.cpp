//
// Created by Niels on 5/06/2021.
//

#include "SimulationImporter.h"
#include "TinyXML/tinyxml.h"
#include "lib/DesignByContract.h"
#include "Utils.h"

SimulationImporter::SimulationImporter(const std::string &filename) : _initCheck(this) {
    TiXmlDocument doc;
    //Kijkt na of de file is ingeladen
    REQUIRE(doc.LoadFile(filename.c_str()), doc.ErrorDesc());
    //Kijkt na of er een root aanwezig is
    TiXmlElement *root = doc.RootElement();
    REQUIRE(root->NextSibling() == NULL, "Er is geen root aanwezig in het document!");

    TiXmlElement *hub = root->FirstChildElement("HUB");
    // a wild do while loop has appeared!
    do {
        SimulationImporterData importerData;

        TiXmlElement *vaccin = hub->FirstChildElement("VACCIN");
        ENSURE(vaccin != NULL, "Er zijn geen vaccins in hub!");
        do {
            std::string type = to_string(vaccin->FirstChildElement("type")->GetText());
            // map can't access constructor so we need to create the object
            importerData.data.insert(std::make_pair(type, SimulationImporterVaccinData()));
            // can't use [] -> requires access to constructor
            importerData.data.at(type).aantal_vaccins = to_int(vaccin->FirstChildElement("aantal")->GetText());
        } while ((vaccin = vaccin->NextSiblingElement("VACCIN")) != NULL);

        hubs_data.push_back(importerData);
    } while ((hub = hub->NextSiblingElement("HUB")) != NULL);

    // another one bites the dust
    TiXmlElement *centrum = root->FirstChildElement("CENTRUM");
    do {
        std::string naam = centrum->FirstChildElement("naam")->GetText();
        if (centrum->FirstChildElement() == NULL) continue;
        SimulationImporterData importerData;

        TiXmlElement *vaccin = centrum->FirstChildElement("VACCIN");
        ENSURE(vaccin != NULL, "Er zijn geen vaccins in centrum!");
        do {
            std::string type = to_string(vaccin->FirstChildElement("type")->GetText());
            // map can't access constructor so we need to create the object
            importerData.data.insert(std::make_pair(type, SimulationImporterVaccinData()));
            // can't use [] -> requires access to constructor
            importerData.data.at(type).aantal_vaccins = to_int(vaccin->FirstChildElement("aantal")->GetText());
            importerData.data.at(type).eerste_prikken = to_int(vaccin->FirstChildElement("eersteprik")->GetText());
            importerData.data.at(type).tweede_prikken = to_int(vaccin->FirstChildElement("tweedeprik")->GetText());

        } while ((vaccin = vaccin->NextSiblingElement("VACCIN")) != NULL);
        centra_data.insert(std::make_pair(naam, importerData));
    } while ((centrum = centrum->NextSiblingElement("CENTRUM")) != NULL);
    ENSURE(isProperlyInitialized(), "Object isn't properly initialized when exiting the constructor");
}

int SimulationImporter::getHubVaccinCount(int hubnr, const std::string &vaccin) {
    ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
    int aantal = hubs_data[hubnr].data.at(vaccin).aantal_vaccins;
    ENSURE(aantal >= 0, "Een hub kan geen negatief aantal vaccinaties hebben!");
    return aantal;
}

int SimulationImporter::getCentrumVaccinCount(const std::string &centrum_naam, const std::string &vaccin) {
    ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
    int aantal = centra_data.at(centrum_naam).data.at(vaccin).aantal_vaccins;
    ENSURE(aantal >= 0, "Een centrum kan geen negatief aantal vaccinaties hebben!");
    return aantal;
}

int SimulationImporter::getAantalVaccinatiesCentrum(const std::string &centrum_naam) {
    ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
    int count = 0;
    if (centra_data.empty() || centra_data.find(centrum_naam) == centra_data.end()) return 0;
    for (std::map<std::string, SimulationImporterVaccinData>::iterator it = centra_data.at(centrum_naam).data.begin();
         it != centra_data.at(centrum_naam).data.end(); it++) {
        count += (*it).second.tweede_prikken;
    }
    ENSURE(count >= 0, "Het aantal vaccinaties in een centrum kan nie negatief zijn!");
    return count;
}

int SimulationImporter::getTotaalAantalEerstePrikken(const std::string &centrum_naam) {
    ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
    if (centra_data.empty() || centra_data.find(centrum_naam) == centra_data.end())
        return 0;
    int totaal = 0;
    for (map<string, SimulationImporterVaccinData>::iterator eerste_prikken = centra_data.at(centrum_naam).data.begin();
         eerste_prikken != centra_data.at(centrum_naam).data.end(); ++eerste_prikken) {
        totaal += eerste_prikken->second.eerste_prikken;
    }
    ENSURE(totaal >= 0, "Een centrum kan geen negatief aantal eerste prikken hebben!");
    return totaal;
}

bool SimulationImporter::isProperlyInitialized() const {
    return this == _initCheck;
}

SimulationImporterVaccinData::SimulationImporterVaccinData() : aantal_vaccins(0), eerste_prikken(0),
                                                               tweede_prikken(0) {}

SimulationImporterData::SimulationImporterData() {}
