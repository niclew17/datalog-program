#pragma once

#include <string>
#include <vector>
#include "Scheme.h"
#include <sstream>
#include <iostream>

using namespace std;

class Tuple : public vector<string> {

public:
    Tuple(vector<string> values) : vector<string>(values) {}


    string toString(const Scheme &scheme) const {
        const Tuple &tuple = *this;
        stringstream out;
        // fix the code to print "name=value" pairs
        for (int i = 0; i < tuple.size(); i++) {
            out << scheme.at(i);
            out << "=";
            out << tuple.at(i);
            if (i < tuple.size() - 1) {
                out << ", ";
            }
        }
        return out.str();
    }
};