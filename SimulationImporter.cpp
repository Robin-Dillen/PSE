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
        centra_data.push_back(importerData);
    } while ((centrum = centrum->NextSiblingElement("CENTRUM")) != NULL);
    ENSURE(isProperlyInitialized(), "Object isn't properly initialized when exiting the constructor");
}

int SimulationImporter::getHubVaccinCount(int hubnr, const std::string &vaccin) {
    ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
    return hubs_data[hubnr].data.at(vaccin).aantal_vaccins;
}

int SimulationImporter::getCentrumVaccinCount(int centrumnr, const std::string &vaccin) {
    ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
    return centra_data[centrumnr].data.at(vaccin).aantal_vaccins;
}

int SimulationImporter::getAantalVaccinatiesCentrum(int centrumnr) {
    ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
    int count = 0;
    for(std::map<std::string, SimulationImporterVaccinData>::iterator it = centra_data[centrumnr].data.begin(); it != centra_data[centrumnr].data.end(); it++){
        count += (*it).second.tweede_prikken;
    }
    return count;
}

int SimulationImporter::getAantalEerstePrikken(int centrumnr, const std::string &vaccin) {
    ENSURE(isProperlyInitialized(), "Object wasn't properly initialised!");
    if (centra_data[centrumnr].data.find(vaccin) == centra_data[centrumnr].data.end()) return 0;
    return centra_data[centrumnr].data.at(vaccin).eerste_prikken;
}

bool SimulationImporter::isProperlyInitialized() const {
    return this == _initCheck;
}

SimulationImporterVaccinData::SimulationImporterVaccinData() : aantal_vaccins(0), eerste_prikken(0),
                                                               tweede_prikken(0) {}

SimulationImporterData::SimulationImporterData() {}
