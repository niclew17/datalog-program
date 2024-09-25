//
// Created by Nicholas Lewis on 2/12/24.
//
#pragma once

#include <string>
#include "Token.h"
#include "Parameter.h"
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

class Predicate {
private:
    string name;
    vector<Parameter> parameters;

public:
    Predicate(string name, vector<Parameter> parameters) {
        this->name = name;
        this->parameters = parameters;
    }

    Predicate(string name) {
        this->name = name;
    }

    const string getName() const {
        return name;
    }

    const vector<Parameter> getParameters() const {
        return parameters;
    }

    void setName(const string name) {
        Predicate::name = name;
    }

    void addParameters(const Parameter param) {
        parameters.push_back(param);
    }

    void toString() {
        cout << name << "(";
        if (parameters.empty()) {
            cout << "";
        } else {
            cout << parameters[0].getParam();
            for (int i = 1; i < parameters.size(); i++) {
                cout << "," << parameters[i].getParam();
            }
        }
        cout << ")";
    }

};

