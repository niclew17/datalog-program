#pragma once

#include <string>
#include <set>
#include <sstream>
#include <iostream>
#include <map>

using namespace std;

class Database {

private:

    map<string, Relation> relations;

public:

    Database(const map<string, Relation> relations)
            : relations(relations) {}

    Relation getRelation(const string name) const {
        return relations.at(name);
    }

    void addRelation(const string name, const Relation rel) {
        relations[name] = rel;
        //relations.insert(std::make_pair(name, rel));
    }

    void addFactRelation(const string name, Tuple value) {
        relations.at(name).addTuple(value);
    }

    int countTuples() {
        map<string, Relation>::iterator it;
        int i = 0;
        vector<Relation> valuerel;
        for (it = relations.begin(); it != relations.end(); it++) {
            valuerel.push_back(it->second);
        }
        for (Relation x: valuerel) {
            i += x.getTuples().size();
        }
        return i;
    }


};
