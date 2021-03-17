//============================================================================
// Name        : VaccinatieSimulatieTests.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : defines a VaccinatieCentrum
//============================================================================

#include "gtest/gtest.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "Parser.h"
#include "Lib.h"

class VaccinSimulatieTest : public ::testing::Test {
protected:
//    friend class Hub;
//    // You should make the members protected s.t. they can be
//    // accessed from sub-classes.
//
//    friend class VaccinatieCentrum;
//    // You should make the members protected s.t. they can be
//    // accessed from sub-classes.

/**
virtual void SetUp() will be called before each test is run.  You
should define it if you need to initialize the variables.
Otherwise, this can be skipped.
*/
    virtual void SetUp() {
    }

/**
virtual void TearDown() will be called after each test is run.
You should define it if there is cleanup work to do.  Otherwise,
you don't have to provide it.
*/
    virtual void TearDown() {
    }

/**
Declares the variables your tests want to use.
*/
};


TEST_F(VaccinSimulatieTest, DefaultConstructor) {

    Parser *P = new Parser("../XMLfiles/test001.xml");
    Hub *hub = P->getFhubs()[0];
    EXPECT_TRUE(hub->isProperlyInitialized());
    unsigned int zero = 0;
    EXPECT_NE(zero, hub->getFverbondenCentra().size());
    for (map<string, VaccinatieCentrum *>::const_iterator it = hub->getFverbondenCentra().begin(), end = hub->getFverbondenCentra().end();
         it != end; it++) {
        EXPECT_TRUE(it->second->isProperlyInitialized());
        EXPECT_EQ(0, it->second->getAantalVaccinaties());
        EXPECT_EQ(0, it->second->getAantalGeleverdeVaccins());
        EXPECT_EQ(0, it->second->getAantalVaccins());
    }
    delete P;
}

//SYSTEM TESTS
TEST_F(VaccinSimulatieTest, HappyDay1) {
    Parser P("../XMLfiles/test001.xml");
    Hub *H = P.getFhubs()[0];

    int end_day = 0; // we kunnen ook een grens zetten op de duur van de simulatie, zet op 0 om geen grens te hebben

    int current_day = 1; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
    while (!H->isIedereenGevaccineerd() && (!end_day || current_day < end_day)) {
        // increase current_day
        current_day++;

        if (current_day % H->getLeveringenInterval() == 0) {
            // door in de simulatie het aantal vaccins mee te geven kunnen we war randomness toevoegen aan het aantal
            // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
            H->ontvangLevering(H->getKaantalVaccinsPerLevering());
        }

        // stuur signaal nieuwe dag
        H->nieuweDag();
    }

    // tests
    EXPECT_GE(current_day, 1);

    for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
         it != end; it++) {
        EXPECT_TRUE(it->second->isIedereenGevaccineerd());
        EXPECT_EQ(it->second->getKaantalInwoners(), it->second->getAantalVaccinaties());
    }
    EXPECT_TRUE(H->isIedereenGevaccineerd());
}

TEST_F(VaccinSimulatieTest, HappyDay2) {
    Parser P("../XMLfiles/test003.xml");
    Hub *H = P.getFhubs()[0];

    int end_day = 0; // we kunnen ook een grens zetten op de duur van de simulatie, zet op 0 om geen grens te hebben

    int current_day = 1; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
    while (!H->isIedereenGevaccineerd() && (!end_day || current_day < end_day)) {
        // increase current_day
        current_day++;

        if (current_day % H->getLeveringenInterval() == 0) {
            // door in de simulatie het aantal vaccins mee te geven kunnen we war randomness toevoegen aan het aantal
            // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
            H->ontvangLevering(H->getKaantalVaccinsPerLevering());
        }

        // stuur signaal nieuwe dag
        H->nieuweDag();
    }

    // tests
    EXPECT_GE(current_day, 1);

    for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
         it != end; it++) {
        EXPECT_TRUE(it->second->isIedereenGevaccineerd());
        EXPECT_EQ(it->second->getKaantalInwoners(), it->second->getAantalVaccinaties());
    }
    EXPECT_TRUE(H->isIedereenGevaccineerd());
}

TEST_F(VaccinSimulatieTest, HappyDay3) {
    Parser P("../XMLfiles/test004.xml");
    Hub *H = P.getFhubs()[0];

    int end_day = 0; // we kunnen ook een grens zetten op de duur van de simulatie, zet op 0 om geen grens te hebben

    int current_day = 1; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
    while (!H->isIedereenGevaccineerd() && (!end_day || current_day < end_day)) {
        // increase current_day
        current_day++;

        if (current_day % H->getLeveringenInterval() == 0) {
            // door in de simulatie het aantal vaccins mee te geven kunnen we war randomness toevoegen aan het aantal
            // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
            H->ontvangLevering(H->getKaantalVaccinsPerLevering());
        }

        // stuur signaal nieuwe dag
        H->nieuweDag();
    }

    // tests
    EXPECT_GE(current_day, 1);

    for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
         it != end; it++) {
        EXPECT_TRUE(it->second->isIedereenGevaccineerd());
        EXPECT_EQ(it->second->getKaantalInwoners(), it->second->getAantalVaccinaties());
    }
    EXPECT_TRUE(H->isIedereenGevaccineerd());
}

TEST_F(VaccinSimulatieTest, HappyDay4) {
    Parser P("../XMLfiles/test007.xml");
    Hub *H = P.getFhubs()[0];

    int end_day = 0; // we kunnen ook een grens zetten op de duur van de simulatie, zet op 0 om geen grens te hebben

    int current_day = 1; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
    while (!H->isIedereenGevaccineerd() && (!end_day || current_day < end_day)) {
        // increase current_day
        current_day++;

        if (current_day % H->getLeveringenInterval() == 0) {
            // door in de simulatie het aantal vaccins mee te geven kunnen we war randomness toevoegen aan het aantal
            // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
            H->ontvangLevering(H->getKaantalVaccinsPerLevering());
        }

        // stuur signaal nieuwe dag
        H->nieuweDag();
    }

    // tests
    EXPECT_GE(current_day, 1);

    for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
         it != end; it++) {
        EXPECT_TRUE(it->second->isIedereenGevaccineerd());
        EXPECT_EQ(it->second->getKaantalInwoners(), it->second->getAantalVaccinaties());
    }
    EXPECT_TRUE(H->isIedereenGevaccineerd());
}


//UNIT TESTS
//VACCINATIECENTRUM
TEST_F(VaccinSimulatieTest, VaccinatieCentrum_getname) {
    VaccinatieCentrum *V = new VaccinatieCentrum(10, 10, "A", "B");
    EXPECT_EQ("A", V->getKfname());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_getaddress) {
    VaccinatieCentrum *V = new VaccinatieCentrum(10, 10, "A", "B");
    EXPECT_EQ("B", V->getKfaddress());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_AantalVaccinaties) {
    VaccinatieCentrum *V = new VaccinatieCentrum(10, 10, "A", "B");
    V->setAantalVaccinaties(10);
    EXPECT_EQ(10, V->getAantalVaccinaties());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_AantalVaccins) {
    VaccinatieCentrum *V = new VaccinatieCentrum(10, 10, "A", "B");
    V->setVaccins(10);
    EXPECT_EQ(10, V->getAantalVaccins());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_getAantalInwoners) {
    VaccinatieCentrum *V = new VaccinatieCentrum(20, 10, "A", "B");
    EXPECT_EQ(10, V->getKaantalInwoners());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_getCapaciteit) {
    VaccinatieCentrum *V = new VaccinatieCentrum(20, 10, "A", "B");
    EXPECT_EQ(20, V->getKcapaciteit());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_getMaxStock) {
    VaccinatieCentrum *V = new VaccinatieCentrum(20, 10, "A", "B");
    EXPECT_EQ(40, V->getMaxStock());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_AantalGeleverdeVaccins) {
    VaccinatieCentrum *V = new VaccinatieCentrum(20, 10, "A", "B");
    EXPECT_EQ(0, V->getAantalGeleverdeVaccins());
    V->ontvangLevering(3);
    EXPECT_EQ(3, V->getAantalGeleverdeVaccins());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_isVol) {
    VaccinatieCentrum *V = new VaccinatieCentrum(20, 10, "A", "B");
    EXPECT_FALSE(V->isVol());
    V->setVaccins(40);
    EXPECT_TRUE(V->isVol());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_isVolNaLevering) {
    VaccinatieCentrum *V = new VaccinatieCentrum(20, 10, "A", "B");
    EXPECT_FALSE(V->isVolNaLevering(36));
    EXPECT_TRUE(V->isVolNaLevering(80));
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_isIedereenGevaccineerd) {
    VaccinatieCentrum *V = new VaccinatieCentrum(20, 10, "A", "B");
    EXPECT_FALSE(V->isIedereenGevaccineerd());
    V->setAantalVaccinaties(10);
    EXPECT_TRUE(V->isIedereenGevaccineerd());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_nieuweDag1) {
    VaccinatieCentrum *V = new VaccinatieCentrum(20, 10, "A", "B");
    V->ontvangLevering(10);
    V->nieuweDag();
    EXPECT_EQ(0, V->getAantalGeleverdeVaccins());
    EXPECT_EQ(10, V->getAantalVaccinaties());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_nieuweDag2) {
    VaccinatieCentrum *V = new VaccinatieCentrum(10, 40, "A", "B");
    V->ontvangLevering(15);
    V->nieuweDag();
    EXPECT_EQ(0, V->getAantalGeleverdeVaccins());
    EXPECT_EQ(10, V->getAantalVaccinaties());
    EXPECT_EQ(5, V->getAantalVaccins());
    delete V;
}

TEST_F(VaccinSimulatieTest, VaccinatieCentrum_nieuweDag3) {
    VaccinatieCentrum *V = new VaccinatieCentrum(10, 5, "A", "B");
    V->ontvangLevering(15);
    V->nieuweDag();
    EXPECT_EQ(0, V->getAantalGeleverdeVaccins());
    EXPECT_EQ(5, V->getAantalVaccinaties());
    EXPECT_EQ(10, V->getAantalVaccins());
    delete V;
}

//HUB
TEST_F(VaccinSimulatieTest, Hub_addFverbondenCentra1) {
    Hub *H = new Hub(10, 10, 10);
    VaccinatieCentrum *V1 = new VaccinatieCentrum(10, 10, "A", "B");
    VaccinatieCentrum *V2 = new VaccinatieCentrum(10, 10, "C", "D");

    vector<VaccinatieCentrum *> VaccinatieCentra;
    VaccinatieCentra.push_back(V1);
    VaccinatieCentra.push_back(V2);
    unsigned int i = 0;
    EXPECT_EQ(i, H->getFverbondenCentra().size());
    H->addFverbondenCentra(VaccinatieCentra);
    i = 2;
    EXPECT_EQ(i, H->getFverbondenCentra().size());
    delete H;
    delete V1;
    delete V2;
}

TEST_F(VaccinSimulatieTest, Hub_addFverbondenCentra2) {
    Hub *H = new Hub(10, 10, 10);
    VaccinatieCentrum *V1 = new VaccinatieCentrum(10, 10, "A", "B");
    VaccinatieCentrum *V2 = new VaccinatieCentrum(10, 10, "C", "D");

    map<string, VaccinatieCentrum *> VaccinatieCentra;
    VaccinatieCentra[V1->getKfname()] = V1;
    VaccinatieCentra[V2->getKfname()] = V2;
    unsigned int i = 0;
    EXPECT_EQ(i, H->getFverbondenCentra().size());
    H->addFverbondenCentra(VaccinatieCentra);
    i = 2;
    EXPECT_EQ(i, H->getFverbondenCentra().size());
    delete H;
    delete V1;
    delete V2;
}

TEST_F(VaccinSimulatieTest, Hub_AantalVaccins) {
    Hub *H = new Hub(10, 10, 10);
    H->setAantalVaccins(230);
    EXPECT_EQ(230, H->getAantalVaccins());
    delete H;
}

TEST_F(VaccinSimulatieTest, Hub_getLeveringenInterval) {
    Hub *H = new Hub(100, 3, 500);
    EXPECT_EQ(4, H->getLeveringenInterval());
    delete H;
}

TEST_F(VaccinSimulatieTest, Hub_getKaantalVaccinsPerLevering) {
    Hub *H = new Hub(16, 10, 10);
    EXPECT_EQ(16, H->getKaantalVaccinsPerLevering());
    delete H;
}

TEST_F(VaccinSimulatieTest, Hub_getKaantalVaccinsPerLading) {
    Hub *H = new Hub(10, 10, 500);
    EXPECT_EQ(500, H->getKaantalVaccinsPerLading());
    delete H;
}

TEST_F(VaccinSimulatieTest, Hub_isIedereengevaccineerd) {
    Hub *H = new Hub(10, 10, 10);
    VaccinatieCentrum *V1 = new VaccinatieCentrum(10, 10, "A", "B");
    vector<VaccinatieCentrum *> VaccinatieCentra;
    VaccinatieCentra.push_back(V1);
    H->addFverbondenCentra(VaccinatieCentra);
    EXPECT_FALSE(H->isIedereenGevaccineerd());
    V1->setAantalVaccinaties(V1->getKaantalInwoners());
    EXPECT_TRUE(H->isIedereenGevaccineerd());
    delete H;
}

TEST_F(VaccinSimulatieTest, Hub_minAantalLeveringen) {
    Hub *H = new Hub(20, 10, 6);
    VaccinatieCentrum *V = new VaccinatieCentrum(8, 10, "A", "B");
    vector<VaccinatieCentrum *> VaccinatieCentra;
    VaccinatieCentra.push_back(V);
    H->addFverbondenCentra(VaccinatieCentra);
    map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin();
    EXPECT_EQ(2, H->minAantalLeveringen(it));
    delete H;
    delete V;
}

TEST_F(VaccinSimulatieTest, Hub_verdeelVaccins) {
    Hub *H = new Hub(30, 10, 3);
    VaccinatieCentrum *V1 = new VaccinatieCentrum(10, 10, "A", "B");
    VaccinatieCentrum *V2 = new VaccinatieCentrum(5, 12, "C", "D");
    vector<VaccinatieCentrum *> centra;
    centra.push_back(V1);
    centra.push_back(V2);
    H->addFverbondenCentra(centra);
    H->verdeelVaccins();
    EXPECT_EQ(18, V1->getAantalGeleverdeVaccins());
    EXPECT_EQ(9, V2->getAantalGeleverdeVaccins());
    EXPECT_EQ(3, H->getAantalVaccins());

    delete H;
    delete V1;
    delete V2;
}

TEST_F(VaccinSimulatieTest, Hub_ontvangLevering) {
    Hub *H = new Hub(10, 10, 10);
    H->ontvangLevering(2643);
    EXPECT_EQ(2653, H->getAantalVaccins());
    delete H;
}

TEST_F(VaccinSimulatieTest, Hub_nieuweDag) {
    Hub *H = new Hub(10, 10, 10);
    VaccinatieCentrum *V = new VaccinatieCentrum(10, 10, "A", "B");
    vector<VaccinatieCentrum *> centra;
    centra.push_back(V);
    H->addFverbondenCentra(centra);
    V->ontvangLevering(10);
    EXPECT_EQ(10, V->getAantalGeleverdeVaccins());
    H->nieuweDag();
    EXPECT_EQ(0, V->getAantalGeleverdeVaccins());
    EXPECT_TRUE(V->isIedereenGevaccineerd());
    delete H;
    delete V;
}


//INPUT TESTS
TEST_F(VaccinSimulatieTest, ParserSucces) {
    {
        Parser P("../XMLfiles/test001.xml");
        EXPECT_TRUE(P.isProperlyInitialized());
    }
    {
        Parser P("../XMLfiles/test003.xml");
        EXPECT_TRUE(P.isProperlyInitialized());
    }
    {
        Parser P("../XMLfiles/test004.xml");
        EXPECT_TRUE(P.isProperlyInitialized());
    }
    {
        Parser P("../XMLfiles/test007.xml");
        EXPECT_TRUE(P.isProperlyInitialized());
    }
}

TEST_F(VaccinSimulatieTest, ParserDeath) {
    EXPECT_DEATH(Parser P("../XMLfiles/test002.xml"), "");
    EXPECT_DEATH(Parser P("../XMLfiles/test006.xml"), "");
    EXPECT_DEATH(Parser P("../XMLfiles/test008.xml"), "");
    EXPECT_DEATH(Parser P("../XMLfiles/test009.xml"), "");
    EXPECT_DEATH(Parser P("../XMLfiles/test011.xml"), "");
    EXPECT_DEATH(Parser P("../XMLfiles/test014.xml"), "");
}

TEST_F(VaccinSimulatieTest, ParserExeption1) {
    {
        Parser P("../XMLfiles/test005.xml");
        EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
    }
}

TEST_F(VaccinSimulatieTest, ParserExeption2) {
    {
        Parser P("../XMLfiles/test010.xml");
        EXPECT_EQ(2, P.errorOccured(WRONG_VALUE));
        EXPECT_EQ(3, P.errorOccured(MISSING_TAG));
    }
}

TEST_F(VaccinSimulatieTest, ParserExeption3) {
    {
        Parser P("../XMLfiles/test012.xml");
        EXPECT_EQ(3, P.errorOccured(UNKNOWN_TAG));
    }
}

TEST_F(VaccinSimulatieTest, ParserExeption4) {
    {
        Parser P("../XMLfiles/test013.xml");
        EXPECT_EQ(3, P.errorOccured(UNKNOWN_TAG));
        EXPECT_EQ(2, P.errorOccured(WRONG_VALUE));
    }
}

TEST_F(VaccinSimulatieTest, ParserExeption5) {
    {
        Parser P("../XMLfiles/test015.xml");
        EXPECT_EQ(1, P.errorOccured(UNKNOWN_TAG));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

