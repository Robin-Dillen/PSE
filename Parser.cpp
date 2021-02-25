#include "Parser.h"

Parser::Parser(const string &filename) {
// read in with tiny xml
    TiXmlDocument doc;

    //TiXmlDocument doc;
    if (!doc.LoadFile(filename.c_str())) {
        std::cerr << doc.ErrorDesc() << std::endl;
        return 1;
    }
    TiXmlElement *root = doc.FirstChildElement();
    if (root == NULL) {
        std::cerr << "Failed to load file: No root element." << std::endl;
        doc.Clear();
        return 1;
    }

    for (TiXmlElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
        string elemName = elem->Value();
        const char *attr;
        if (elemName == "Element1") {
            attr = elem->Attribute("attribute1");
            if (attr != NULL) {
                cout << attr << endl;
            }// Do stuff with it
        } else if (elemName == "Element2") {
            attr = elem->Attribute("attribute2");
            if (attr != NULL) {
                cout << attr << endl;
            } // Do stuff with it
            attr = elem->Attribute("attribute3");
            if (attr != NULL) {
                cout << attr << endl;
            }// Do stuff with it
            for (TiXmlElement *e = elem->FirstChildElement("Element3"); e != NULL;
                 e = e->NextSiblingElement("Element3")) {
                attr = e->Attribute("attribute4");
                if (attr != NULL) {
                    cout << attr << endl;
                } // Do stuff with it
            }
        }
        for (TiXmlNode *e = elem->FirstChild(); e != NULL; e = e->NextSibling()) {
            TiXmlText *text = e->ToText();
            if (text == NULL)
                continue;
            string t = text->Value();
            // Do stuff
        }
    }
    doc.Clear();

//put all the data in according variables
    hub = new Hub(/*input vector of vaccinatie centra*/);
}
