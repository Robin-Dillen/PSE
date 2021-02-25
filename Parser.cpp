#include "Parser.h"
#include "TinyXML/tinyxml.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"
#include <string>
#include <iostream>
#include <sstream>

Parser::Parser(const string &filename) {
    TiXmlDocument doc;
    //if(!doc.LoadFile(filename)) {
    //    std::cerr << doc.ErrorDesc() << std::endl;
    //    return;
    //}
    TiXmlElement* root = doc.RootElement();
    if(root == NULL) {
        std::cerr << "Failed to load file: No root element." << std::endl;
        doc.Clear();
        return;
    }
    for(TiXmlElement* firstelem = root->FirstChildElement(); firstelem != NULL; firstelem = firstelem->NextSiblingElement()) {
        string elemName = firstelem->Value();
        if (elemName == "HUB") {
            int levering;
            int interval;
            int transport;
            vector<VaccinatieCentrum*> vaccinatieCentra;
            for(TiXmlElement* secondelem = firstelem->FirstChildElement(); secondelem != NULL; secondelem = secondelem->NextSiblingElement()) {
                string secondname = secondelem->Value();
                string s = secondelem->GetText();
                stringstream g(s);
                if (secondname == "levering") {
                    g >> levering;
                }
                if (secondname == "interval") {
                    g >> interval;
                }
                if (secondname == "transport") {
                    g >> transport;
                }
                if (secondname == "CENTRA") {
                    for (TiXmlElement *thirdelem = secondelem->FirstChildElement(); thirdelem != NULL; thirdelem = thirdelem->NextSiblingElement()) {
                        cout<<thirdelem->GetText()<<endl;
                        //vaccinatiecentrum aanmaken
                    }
                }
            }
        }
        else if(elemName == "VACCINATIECENTRUM") {

            for(TiXmlElement* secondelem = firstelem->FirstChildElement(); secondelem != NULL; secondelem = secondelem->NextSiblingElement()) {
                string secondname = secondelem->Value();
                string tekst = secondelem->GetText();
                if (secondname == "naam") {
                    cout << "naam: " << tekst << endl;
                }
                if (secondname == "adres") {
                    cout << "adres: " << tekst << endl;
                }
                if (secondname == "inwoners") {
                    cout << "inwoners: " << tekst << endl;
                }
                if (secondname == "capaciteit") {
                    cout << "capaciteit: " << tekst << endl;
                }
            }
        }
    }
}

Hub *Parser::getFhub() const {
    return fhub;
}
