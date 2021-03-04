#include "Parser.h"
#include "TinyXML/tinyxml.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"

#include <string>
#include <iostream>
#include <sstream>

Parser::Parser(const string &filename) : _initCheck(this) {
    TiXmlDocument doc;
    //Kijkt na of de file is ingeladen
    if (!doc.LoadFile(filename.c_str())) {
        std::cerr << doc.ErrorDesc() << std::endl;
        return;
    }
    //Kijkt na of er een root aanwezig is
    TiXmlElement *root = doc.RootElement();
    if (root == NULL) {
        std::cerr << "Failed to load file: No root element." << std::endl;
        doc.Clear();
        return;
    }
    vector<VaccinatieCentrum*> vaccinatieCentra;
    //Loop over alle kinderen van root
    for(TiXmlElement* firstElement = root->FirstChildElement(); firstElement != NULL; firstElement = firstElement->NextSiblingElement()) {
        string firstName = firstElement->Value();
        //Vacinatiecentrum aanmaken
        if(firstName == "VACCINATIECENTRUM") {
            unsigned int inwoners = 0;
            unsigned int capaciteit = 0;
            //Loop over alle atributen van Vaccinatiecentrum
            string naam = firstElement->FirstChildElement("naam")->GetText();
            string adres = firstElement->FirstChildElement("adres")->GetText();

            stringstream g(firstElement->FirstChildElement("inwoners")->GetText());
            g >> inwoners;

            stringstream h(firstElement->FirstChildElement("capaciteit")->GetText());
            h >> capaciteit;

            //variabelen moeten een waarde hebben gekregen
            ENSURE(naam != "" && adres != "" && inwoners != 0 && capaciteit != 0, "Sommige variabelen van VACCINATIECENTRUM werden niet correct meegegeven.");
            VaccinatieCentrum *V = new VaccinatieCentrum(capaciteit, inwoners, naam, adres);
            vaccinatieCentra.push_back(V);
        }
    }
    ENSURE(vaccinatieCentra.size() != 0, "Er is geen vaccinatiecentrum gegeven.");
    //Hub aanmaken
    if(root->FirstChildElement("HUB")){
        TiXmlElement* hub = root->FirstChildElement("HUB");
        unsigned int levering = 0;
        unsigned int interval = 0;
        unsigned int transport = 0;
        stringstream g(hub->FirstChildElement("levering")->GetText());
        g >> levering;

        stringstream h(hub->FirstChildElement("interval")->GetText());
        h >> interval;

        stringstream i(hub->FirstChildElement("transport")->GetText());
        i >> transport;

        //variabelen moeten een waarde hebben gekregen
        ENSURE(levering != 0 && interval != 0 && transport != 0 , "Sommige variabelen van HUB werden niet correct meegegeven.");
        Hub* H = new Hub(levering,interval,transport);
        H->addFverbondenCentra(vaccinatieCentra);
        fhub = H;
        ENSURE(isProperlyInitialized(), "constructor must end in properlyInitialized state");
    }
    //Hub moet aanwezig zijn
    else{
        std::cerr << "Failed to load file: No HUB element." << std::endl;
        doc.Clear();
        return;
    }

}

Hub *Parser::getFhub() const {
    REQUIRE(isProperlyInitialized(), "Parser wasn't initialized when calling getFhub()");
    return fhub;
}

bool Parser::isProperlyInitialized() const {
    return _initCheck == this;
}
