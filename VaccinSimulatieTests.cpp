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

/*
TEST_F(VaccinSimulatieTest, DefaultConstructor) {
    EXPECT_TRUE(H->isProperlyInitialized());
    for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
         it != end; it++) {
        EXPECT_TRUE(it->second->isProperlyInitialized());
    }
}

TEST_F(VaccinSimulatieTest, HappyDayIedereenGevaccineert) {
    unsigned int end_day = 0; // we kunnen ook een grens zetten op de duur van de simulatie, zet op 0 om geen grens te hebben

    unsigned int current_day = 1; // we houden de datum hier bij zodat we aan het einde van de simulatie de duur van de simulatie kunnen opvragen
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

    unsigned int years = current_day / 356;
    current_day -= years * 356;
    unsigned int months = current_day / 30;
    current_day -= months * 30;
    unsigned int weeks = current_day / 7;
    current_day -= weeks * 7;

    // tests
    EXPECT_GE(current_day, 1);

    for (map<string, VaccinatieCentrum *>::const_iterator it = H->getFverbondenCentra().begin(), end = H->getFverbondenCentra().end();
         it != end; it++) {
        EXPECT_TRUE(it->second->isIedereenGevaccineerd());
        EXPECT_EQ(it->second->getKaantalInwoners(), it->second->getAantalVaccinaties());
    }
    EXPECT_TRUE(H->isIedereenGevaccineerd());

}
*/

TEST_F(VaccinSimulatieTest, Parser1){
    Parser P("../XMLfiles/vb.xml");
    EXPECT_TRUE(P.isProperlyInitialized());
}

TEST_F(VaccinSimulatieTest, Parser2){
    Parser P("../XMLfiles/fout_bestand.xml");
    EXPECT_FALSE(P.isProperlyInitialized());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

