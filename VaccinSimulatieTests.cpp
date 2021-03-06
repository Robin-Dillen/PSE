//
// Created by nibor on 2/03/2021.
//

#include "gtest/gtest.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "Parser.h"

class VaccinSimulatieTest : public ::testing::Test {
public:
    VaccinSimulatieTest() {
        P = new Parser("../XMLfiles/vb.xml");
        H = P->getFhub();
    }

protected:
    friend class Hub;
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    friend class VaccinatieCentrum;
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

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
    Hub *H;
private:
    Parser *P;
};


TEST_F(VaccinSimulatieTest, DefaultConstructor) {
    EXPECT_TRUE(H->isProperlyInitialized());
    for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
         it != end; it++) {
        EXPECT_TRUE(it->second->isProperlyInitialized());
    }
}

TEST_F(VaccinSimulatieTest, AantalGeleverdeVaccins) {
    unsigned int current_day = 1; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
    while (!H->isIedereenGevaccineerd()) {

        if (current_day % H->getLeveringenInterval() == 0) {
            H->ontvangLevering(H->getKaantalVaccinsPerLevering());
            // door in de simulatie het aantal vaccins mee te geven kunnen we wat randomness toevoegen aan het aantal
            // geleverde vaccins. Want ze zijn toch niet te vertrouwen die farmareuzen!
        }

        // verdeel de vaccins als er nog op voorraad zijn + output
        // VB Er werden 4 ladingen (8000 vaccins) getransporteerd naar Park Spoor Oost.
        H->verdeelVaccins();

        for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
             it != end; it++) {
            EXPECT_TRUE(it->second->getAantalGeleverdeVaccins() % H->getKaantalVaccinsPerLading() == 0);
            EXPECT_TRUE(it->second->getAantalGeleverdeVaccins() + it->second->getAantalVaccins() <=
                        it->second->getMaxStock());
        }

        // stuur signaal nieuwe dag naar alle centra
        for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
             it != end; it++) {
            it->second->nieuweDag();
        }

        for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
             it != end; it++) {
            EXPECT_TRUE(it->second->getAantalGeleverdeVaccins() == 0);
        }

        // increase current_day
        current_day++;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

