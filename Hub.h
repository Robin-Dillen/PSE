#ifndef PSE_HUB_H
#define PSE_HUB_H

#include <map>
#include <string>

#include "VaccinatieCentrum.h"

using namespace std;

class Hub {

private:
    map<string, VaccinatieCentrum *> vaccin_centra;
};


#endif //PSE_HUB_H
