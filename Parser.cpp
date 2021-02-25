#include "Parser.h"
#include "TinyXML/tinyxml.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"

#include <string>
#include <iostream>
#include <sstream>

Parser::Parser(const string &filename) : _initCheck(this) {
    TiXmlDocument doc;
    if (!doc.LoadFile(filename.c_str())) {
        std::cerr << doc.ErrorDesc() << std::endl;
        return;
    }
    TiXmlElement *root = doc.RootElement();
    if (root == NULL) {
        std::cerr << "Failed to load file: No root element." << std::endl;
        doc.Clear();
        return;
    }
    Hub* H = new Hub;
    for(TiXmlElement* firstelem = root->FirstChildElement(); firstelem != NULL; firstelem = firstelem->NextSiblingElement()) {
        string elemName = firstelem->Value();
        if (elemName == "HUB") {
            unsigned int levering;
            unsigned int interval;
            unsigned int transport;
            for(TiXmlElement* secondelem = firstelem->FirstChildElement(); secondelem != NULL; secondelem = secondelem->NextSiblingElement()) {
                string secondname = secondelem->Value();
                if(secondelem->GetText() != NULL) {
                    string s = secondelem->GetText();
                    stringstream g(s);
                    if (secondname == "levering") {
                        g >> levering;
                        H->setAantalVaccinsPerLevering(levering);
                    }
                    if (secondname == "interval") {
                        g >> interval;
                        H->setLeveringenInterval(interval);
                    }
                    if (secondname == "transport") {
                        g >> transport;
                        H->setAantalVaccinsPerLading(levering);
                    }
                }
                if (secondname == "CENTRA") {
                    vector<VaccinatieCentrum*> VCentra;
                    for (TiXmlElement *thirdelem = secondelem->FirstChildElement(); thirdelem != NULL; thirdelem = thirdelem->NextSiblingElement()) {

                    }
                    //H->setFverbondenCentra(VCentra);
                }
            }
            cout<<"levering: "<<levering<<endl;
            cout<<"transport: "<<transport<<endl;
            cout<<"interval: "<<interval<<endl;
        }
        else if(elemName == "VACCINATIECENTRUM") {
            vector<VaccinatieCentrum*> vaccinatieCentra;
            unsigned int naam;
            unsigned int adres;
            unsigned int inwoners;
            unsigned int capaciteit;
            for(TiXmlElement* secondelem = firstelem->FirstChildElement(); secondelem != NULL; secondelem = secondelem->NextSiblingElement()) {
                string secondname = secondelem->Value();
                string tekst = secondelem->GetText();
                if(secondelem->GetText() != NULL) {
                    string s = secondelem->GetText();
                    stringstream g(s);
                    if (secondname == "naam") {
                        g >> naam;
                    }
                    if (secondname == "adres") {
                        g >> adres;
                    }
                    if (secondname == "inwoners") {
                        g >> inwoners;
                    }
                    if (secondname == "capaciteit") {
                        g >> capaciteit;
                    }
                }
                VaccinatieCentrum* V = new VaccinatieCentrum(capaciteit,inwoners,naam,adres);
                vaccinatieCentra.push_back(V);
            }
        }
    }

    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
}

Hub *Parser::getFhub() const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getFhub()");
    return fhub;
}

bool Parser::isProperlyInitialized() const {
    return false;
}
