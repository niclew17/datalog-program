#pragma once

#include <string>
#include <vector>
#include <iostream>

class Scheme : public vector<string> {

public:
    Scheme(vector<string> names) : vector<string>(names) {}

    Scheme() {}

};