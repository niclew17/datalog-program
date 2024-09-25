#pragma once

#include <string>
#include <set>
#include <sstream>
#include <iostream>
#include "Token.h"
#include "Predicate.h"
#include "Rule.h"
#include "Database.h"
#include <unordered_map>
#include <vector>
#include "Graph.h"

using namespace std;

class Interpreter {

private:
    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Predicate> querys;
    vector<Rule> rules;
    Database database;

public:

    Interpreter(vector<Predicate> schemes, vector<Predicate> facts, vector<Predicate> querys,
                vector<Rule> rules, Database database)
            : schemes(schemes), facts(facts), querys(querys), rules(rules), database(database) {}

    void addDatabase() {
        for (Predicate x: schemes) {
            vector<Parameter> values;
            vector<string> ids;
            values = x.getParameters();
            for (Parameter y: values) {
                ids.push_back(y.getParam());
            }
            Scheme newscheme(ids);
            Relation result(x.getName(), newscheme);
            database.addRelation(x.getName(), result);
        }
        for (Predicate x: facts) {
            vector<Parameter> values;
            vector<string> ids;
            values = x.getParameters();
            for (Parameter y: values) {
                ids.push_back(y.getParam());
            }
            Tuple newtuple(ids);
            database.addFactRelation(x.getName(), newtuple);
        }
    }

    Relation evaluatePredicate(Predicate x) {
        Relation myrelation = database.getRelation(x.getName());
        int i = 0;
        unordered_map<string, int> variables;
        vector<int> value;
        vector<string> key;
        bool allconst = true;
        for (Parameter z: x.getParameters()) {
            if (z.isString1()) {
                myrelation = myrelation.select(i, z.getParam());
            } else {
                if (variables.find(z.getParam()) == variables.end()) {
                    allconst = false;
                    key.push_back(z.getParam());
                    value.push_back(i);
                    variables.insert(std::make_pair(z.getParam(), i));
                } else {
                    vector<int> index;
                    allconst = false;
                    index.push_back(i);
                    index.push_back(variables.at(z.getParam()));
                    myrelation = myrelation.select2(index);
                }
            }
            i++;
        }

        myrelation.setAllconst(allconst);
        myrelation = myrelation.project(value);
        for (int z = 0; z < key.size(); z++) {
            myrelation = myrelation.rename(z, key.at(z));
        }
        cout << myrelation.toString();
        return myrelation;
    }

    Relation evaluatePredicateRules(Predicate x) {
        Relation myrelation = database.getRelation(x.getName());
        int i = 0;
        unordered_map<string, int> variables;
        vector<int> value;
        vector<string> key;
        bool allconst = true;
        for (Parameter z: x.getParameters()) {
            if (z.isString1()) {
                myrelation = myrelation.select(i, z.getParam());
            } else {
                if (variables.find(z.getParam()) == variables.end()) {
                    allconst = false;
                    key.push_back(z.getParam());
                    value.push_back(i);
                    variables.insert(std::make_pair(z.getParam(), i));
                } else {
                    vector<int> index;
                    allconst = false;
                    index.push_back(i);
                    index.push_back(variables.at(z.getParam()));
                    myrelation = myrelation.select2(index);
                }
            }
            i++;
        }

        myrelation.setAllconst(allconst);
        myrelation = myrelation.project(value);
        for (int z = 0; z < key.size(); z++) {
            myrelation = myrelation.rename(z, key.at(z));
        }
        return myrelation;
    }

    vector<Rule> getRelatedRules(set<int> scc) {
        vector<Rule> rulescheck;
        set<int>::iterator itr;
        for (itr = scc.begin(); itr != scc.end(); itr++) {
            rulescheck.push_back(rules.at(*itr));
        }
        return rulescheck;
    }

    void getSCCNum(set<int> scc) {
        vector<int> rulescheck;
        set<int>::iterator itr;
        for (itr = scc.begin(); itr != scc.end(); itr++) {
            rulescheck.push_back(*itr);
        }
        for (int i = 0; i < rulescheck.size(); i++) {
            if (i < (rulescheck.size() - 1)) {
                cout << "R" << rulescheck.at(i) << ",";
            } else {
                cout << "R" << rulescheck.at(i);
            }
        }
    }

    void evaluateRules() {
        vector<set<int> > sccs;
        Graph normal = makeGraph(rules, false);
        cout << "Dependency Graph\n" << normal.toString() << endl;
        Graph reverse = makeGraph(rules, true);
        reverse.dfsForest();
        sccs = normal.dfsForestSCC(reverse.getPostordernums());
        cout << "Rule Evaluation" << endl;
        bool changes;
        for (set<int> scc: sccs) {
            int times = 0;
            cout << "SCC: ";
            getSCCNum(scc);
            cout << "\n";
            changes = true;
            while (changes) {
                int tuplestart = database.countTuples();
                vector<Rule> vals = getRelatedRules(scc);
                if (vals.size() == 1 && !normal.isDependent(*scc.begin())) {
                    Rule x = vals.at(0);
                    x.toString();
                    vector<Predicate> predicates;
                    vector<Relation> evalRelations;
                    predicates = x.getPredicates();
                    // evaluate all predicates and puts them in a vector
                    for (Predicate z: predicates) {
                        evalRelations.push_back(evaluatePredicateRules(z));
                    }

                    // joins every evaluated relation together
                    Relation result = evalRelations.at(0);
                    for (int i = 1; i < evalRelations.size(); i++) {
                        result = result.join(evalRelations.at(i));
                    }

                    // projects only the desired indexes
                    Predicate headpred = x.getHeadpred();
                    vector<Parameter> values;
                    vector<string> ids;
                    values = headpred.getParameters();
                    for (Parameter y: values) {
                        ids.push_back(y.getParam());
                    }
                    vector<int> projections;
                    for (string params: ids) {
                        for (int i = 0; i < result.getScheme().size(); i++) {
                            if (result.getScheme().at(i) == params) {
                                projections.push_back(i);
                            }
                        }
                    }
                    result = result.project(projections);

                    // renames to the schemes relation
                    Relation name = database.getRelation(x.getHeadpred().getName());
                    Scheme namescheme = name.getScheme();
                    for (int index = 0; index < namescheme.size(); index++) {
                        result = result.rename(index, namescheme.at(index));
                    }

                    //unionizes with relation in DB
                    Relation newrelation = name.unionize(result);
                    //changes = true;
                    database.addRelation(newrelation.getName(), newrelation);
                    changes = false;
                } else {
                    for (Rule x: vals) {
                        x.toString();
                        vector<Predicate> predicates;
                        vector<Relation> evalRelations;
                        predicates = x.getPredicates();
                        // evaluate all predicates and puts them in a vector
                        for (Predicate z: predicates) {
                            evalRelations.push_back(evaluatePredicateRules(z));
                        }

                        // joins every evaluated relation together
                        Relation result = evalRelations.at(0);
                        for (int i = 1; i < evalRelations.size(); i++) {
                            result = result.join(evalRelations.at(i));
                        }

                        // projects only the desired indexes
                        Predicate headpred = x.getHeadpred();
                        vector<Parameter> values;
                        vector<string> ids;
                        values = headpred.getParameters();
                        for (Parameter y: values) {
                            ids.push_back(y.getParam());
                        }
                        vector<int> projections;
                        for (string params: ids) {
                            for (int i = 0; i < result.getScheme().size(); i++) {
                                if (result.getScheme().at(i) == params) {
                                    projections.push_back(i);
                                }
                            }
                        }
                        result = result.project(projections);

                        // renames to the schemes relation
                        Relation name = database.getRelation(x.getHeadpred().getName());
                        Scheme namescheme = name.getScheme();
                        for (int index = 0; index < namescheme.size(); index++) {
                            result = result.rename(index, namescheme.at(index));
                        }

                        //unionizes with relation in DB
                        Relation newrelation = name.unionize(result);
                        //changes = true;
                        database.addRelation(newrelation.getName(), newrelation);
                    }
                }
                if (tuplestart == database.countTuples()) {
                    changes = false;
                }
                times++;
            }
            cout << times << " passes: ";
            getSCCNum(scc);
            cout << "\n";
        }
        cout << "\n";
    }

    void evaluateQuerys() {
        cout << "Query Evaluation" << endl;
        for (Predicate x: querys) {
            x.toString();
            cout << "? ";
            Relation newrelation = evaluatePredicate(x);
        }
    }

    static Graph makeGraph(const vector<Rule> &ruler, bool reverse) {

        Graph graph(ruler.size());
        // add code to add edges to the graph for the rule dependencies
        int i = 0;
        for (Rule x: ruler) {
            i++;
            for (Predicate y: x.getPredicates()) {
                int d = 0;
                for (Rule z: ruler) {
                    d++;
                    if (y.getName() == z.getHeadpred().getName()) {
                        if (reverse) {
                            graph.addEdge(d - 1, i - 1);
                        } else {
                            graph.addEdge(i - 1, d - 1);
                        }
                    }
                }
            }

        }
        return graph;
    }


};

