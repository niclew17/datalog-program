//
// Created by Nicholas Lewis on 2/12/24.
//
#pragma once

#include <string>
#include "Token.h"
#include "Predicate.h"
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

class Rule {
private:
    Predicate headpred;
    vector<Predicate> bodypredicates;
public:
    Rule(Predicate headpred1, vector<Predicate> bodypredicates1) : headpred(headpred1),
                                                                   bodypredicates(bodypredicates1) {
    }

    Rule(Predicate headpred1) : headpred(headpred1) {
    }

    const Predicate getHeadpred() const {
        return headpred;
    }

    void setHeadpred(const Predicate headpred) {
        Rule::headpred = headpred;
    }

    const vector<Predicate> getPredicates() const {
        return bodypredicates;
    }

    void addPredicates(const Predicate pred) {
        bodypredicates.push_back(pred);
    }

    void toString() {
        headpred.toString();
        cout << " :- ";
        if (bodypredicates.empty()) {
            return;
        }
        bodypredicates[0].toString();
        for (int i = 1; i < bodypredicates.size(); i++) {
            cout << ",";
            bodypredicates[i].toString();
        }
        cout << "." << endl;
    }
};
