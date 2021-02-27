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
    unsigned int levering;
    unsigned int interval;
    unsigned int transport;
    vector<VaccinatieCentrum*> vaccinatieCentra;
    for(TiXmlElement* firstElement = root->FirstChildElement(); firstElement != NULL; firstElement = firstElement->NextSiblingElement()) {
        string firstName = firstElement->Value();
        if (firstName == "HUB") {
            for(TiXmlElement* secondElement = firstElement->FirstChildElement(); secondElement != NULL; secondElement = secondElement->NextSiblingElement()) {
                string secondName = secondElement->Value();
                if(secondElement->GetText() != NULL) {
                    string s = secondElement->GetText();
                    stringstream g(s);
                    if(secondName == "levering") {
                        g >> levering;
                    }
                    else if(secondName == "interval") {
                        g >> interval;
                    }
                    else if(secondName == "transport") {
                        g >> transport;
                    }
                }
                /*if (secondname == "CENTRA") {
                    vector<VaccinatieCentrum*> VCentra;
                    for (TiXmlElement *thirdelem = secondelem->FirstChildElement(); thirdelem != NULL; thirdelem = thirdelem->NextSiblingElement()) {

                    }
                }
                */
            }
        }
        else if(firstName == "VACCINATIECENTRUM") {
            string naam;
            string adres;
            unsigned int inwoners;
            unsigned int capaciteit;
            for(TiXmlElement* secondElement = firstElement->FirstChildElement(); secondElement != NULL; secondElement = secondElement->NextSiblingElement()) {
                string secondName = secondElement->Value();
                string tekst = secondElement->GetText();
                if(secondElement->GetText() != NULL) {
                    string s = secondElement->GetText();
                    stringstream g(s);
                    if(secondName == "naam") {
                        naam = s;
                    } else if (secondName == "adres") {
                        adres = s;
                    } else if (secondName == "inwoners") {
                        g >> inwoners;
                    } else if (secondName == "capaciteit") {
                        g >> capaciteit;
                    }
                }
            }
            ENSURE(naam != "" && adres != "" && inwoners != 0 && capaciteit != 0, "fout bij het parsen!");
            VaccinatieCentrum *V = new VaccinatieCentrum(capaciteit, inwoners, naam, adres);
            vaccinatieCentra.push_back(V);
        }
    }
    ENSURE(levering != 0 && interval != 0 && transport != 0 , "fout bij het parsen!");
    Hub* H = new Hub(levering,interval,transport);
    H->setFverbondenCentra(vaccinatieCentra);
    ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    fhub = H;
}

Hub *Parser::getFhub() const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getFhub()");
    return fhub;
}

bool Parser::isProperlyInitialized() const {
    return _initCheck == this;
}
