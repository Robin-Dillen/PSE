#include <iostream>
#include <vector>

#include "TinyXML/tinyxml.h"
#include "Parser.h"

using namespace std;

int main(int argc, char const *argv[]) {
    vector<string> args = vector<string>(argv + 1, argv + argc);
    Parser P(args[0]);

}

