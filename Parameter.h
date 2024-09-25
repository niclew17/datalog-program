//
// Created by Nicholas Lewis on 2/12/24.
//
#pragma once

#include <string>
#include "Token.h"
#include <string>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

class Parameter {
private:
    bool isString;
    string param;

public:
    Parameter(bool isString, string param) {
        this->isString = isString;
        this->param = param;
    }

    bool isString1() const {
        return isString;
    }

    void setIsString(bool isString) {
        Parameter::isString = isString;
    }

    const string getParam() const {
        return param;
    }

    void setParam(const string paramet) {
        Parameter::param = paramet;
    }

};


