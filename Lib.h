//============================================================================
// Name        : Lib.h
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : contains constants
//============================================================================

#ifndef PSE_LIB_H
#define PSE_LIB_H

#include <string>
#include <map>

#define OUTPUT_FILE_LOCATION string("../Outputfiles/")

typedef map<string, int>::iterator MapSIIterator;
typedef map<string, int>::const_iterator MapSICIterator;
typedef map<string, pair<Vaccin *, int> >::iterator MapSP_VI_Iterator;
typedef map<string, pair<Vaccin *, int> >::const_iterator MapSP_VI_CIterator;

#endif //PSE_LIB_H
