#pragma once

#include <string>
#include "Tuple.h"
#include <set>
#include <sstream>
#include <iostream>

using namespace std;

class Relation {

private:

    string name;
    Scheme scheme;
    set<Tuple> tuples;
    bool allconst;

public:

    Relation(const string &name, const Scheme &scheme)
            : name(name), scheme(scheme) {}

    Relation() {}

    const string &getName() const {
        return name;
    }

    bool addTuple(const Tuple &tuple) {
        return tuples.insert(tuple).second;
        //tuples.insert(tuple);
    }

    bool isAllconst() const {
        return allconst;
    }

    void setAllconst(bool allconst) {
        this->allconst = allconst;
    }

    Relation rename(int index, string rename) const {
        vector<string> values;
        for (int i = 0; i < scheme.size(); i++) {
            if (index == i) {
                values.push_back(rename);
            } else {
                values.push_back(scheme.at(i));
            }
        }


        Scheme newscheme(values);
        Relation result(name, newscheme);
        for (Tuple x: tuples) {
            result.addTuple(x);
        }
        return result;
    }

    Relation select(int index, const string &value) const {
        Relation result(name, scheme);
        for (auto i: tuples) {
            if (i[index] == value) {
                result.addTuple(i);
            }
        }
        // add tuples to the result if they meet the condition
        return result;
    }

    Relation select2(vector<int> indexes) {
        Relation result(name, scheme);
        for (auto i: tuples) {
            int j = 0;
            bool isequal = false;
            while (j + 1 < indexes.size()) {
                if (i[indexes.at(j)] == i[indexes.at(j + 1)]) {
                    isequal = true;
                    j++;
                } else {
                    isequal = false;
                    break;
                }
            }
            if (isequal) {
                result.addTuple(i);
            }
        }
        return result;
    }

    Relation project(vector<int> indexes) const {
        vector<string> values;
        for (int y: indexes) {
            values.push_back(scheme.at(y));
        }

        Scheme newscheme(values);
        Relation result(name, newscheme);
        for (const Tuple x: tuples) {
            vector<string> tuplevalues;
            for (int y: indexes) {
                tuplevalues.push_back(x.at(y));
            }
            result.addTuple(tuplevalues);
        }
        return result;
    }

    static bool joinable(const Scheme &leftScheme, const Scheme &rightScheme,
                         const Tuple &leftTuple, const Tuple &rightTuple) {

        for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
            const string &leftName = leftScheme.at(leftIndex);
            const string &leftValue = leftTuple.at(leftIndex);
            for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
                const string &rightName = rightScheme.at(rightIndex);
                const string &rightValue = rightTuple.at(rightIndex);
                if (rightName == leftName && leftValue != rightValue) {
                    return false;
                }
            }
        }
        return true;
    }

    Scheme joinSchemes(const Scheme &leftScheme, const Scheme &rightScheme) {
        vector<string> newscheme;
        for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
            const string &leftName = leftScheme.at(leftIndex);
            newscheme.push_back(leftName);
        }
        for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
            bool isin = false;
            for (int i = 0; i < leftScheme.size(); i++) {
                if (rightScheme.at(rightIndex) == leftScheme.at(i)) {
                    isin = true;
                }
            }
            if (!isin) {
                newscheme.push_back(rightScheme.at(rightIndex));
            }
        }
        Scheme relation(newscheme);
        return relation;
    }

    Tuple joinTuples(const Scheme &leftScheme, const Scheme &rightScheme,
                     const Tuple &leftTuple, const Tuple &rightTuple) {
        vector<string> newtuple;
        for (unsigned leftIndex = 0; leftIndex < leftTuple.size(); leftIndex++) {
            const string &leftName = leftTuple.at(leftIndex);
            newtuple.push_back(leftName);
        }
        for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
            bool isin = false;
            for (int i = 0; i < leftScheme.size(); i++) {
                if (rightScheme.at(rightIndex) == leftScheme.at(i)) {
                    isin = true;
                }
            }
            if (!isin) {
                newtuple.push_back(rightTuple.at(rightIndex));
            }
        }
        Tuple newTupe(newtuple);
        return newTupe;
    }

    Relation join(const Relation &right) {
        const Relation &left = *this;
        Scheme newscheme = joinSchemes(left.scheme, right.scheme);
        Relation result(name, newscheme);
        for (Tuple leftTuple: left.tuples) {
            for (Tuple rightTuple: right.tuples) {
                if (joinable(left.scheme, right.scheme, leftTuple, rightTuple)) {
                    Tuple z = joinTuples(left.scheme, right.scheme, leftTuple, rightTuple);
                    result.addTuple(z);
                }
            }
        }
        return result;
    }

    Relation unionize(const Relation &resultrel) {
        const Relation &left = *this;
        for (auto i: resultrel.getTuples()) {
            if (this->addTuple(i)) {
                cout << "  " << i.toString(resultrel.scheme) << endl;
            }
        }
        return left;
    }


    const Scheme &getScheme() const {
        return scheme;
    }

    const set<Tuple> &getTuples() const {
        return tuples;
    }

    string toString() const {
        stringstream out;
        // add code to print the Tuples, one per line
        if (tuples.size() > 0) {
            if (scheme.size() == 0) {
                out << "Yes(" << tuples.size() << ")" << endl;
            } else {
                out << "Yes(" << tuples.size() << ")" << endl;
                for (auto i: tuples) {
                    out << "  " << i.toString(scheme) << endl;
                }
            }
        } else {
            out << "No" << endl;
        }
        return out.str();
    }

};