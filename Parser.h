//
// Created by Nicholas Lewis on 2/6/24.
//
#pragma once

#include <string>
#include <set>
#include "Token.h"
#include "Predicate.h"
#include "Rule.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Predicate> querys;
    vector<Rule> rules;
    set<string> domain;

public:
    Parser(const vector<Token> &tokens) : tokens(tokens) {}

    TokenType tokenType() const {
        return tokens.at(0).getType();
    }

    void advanceToken() {
        tokens.erase(tokens.begin());
    }

    void throwError() {
        throw tokens.at(0);
    }

    Token match(TokenType t) {
        if (tokenType() == t) {
            Token val = tokens.at(0);
            advanceToken();
            return val;
        } else {
            throwError();
        }
    }

    void datalogProgram() {
        try {
            match(SCHEMES);
            match(COLON);
            scheme();
            schemeList();
            match(FACTS);
            match(COLON);
            factList();
            match(RULES);
            match(COLON);
            ruleList();
            match(QUERIES);
            match(COLON);
            query();
            queryList();
            match(EOFILE);
        }
        catch (Token errorToken) {
            cout << "Failure!\n" << "  " << errorToken.toString();
        }
    }


    void schemeList() {
        if (tokenType() == ID) {
            scheme();
            schemeList();
        } else {
            // lambda
        }
    }

    void factList() {
        if (tokenType() == ID) {
            fact();
            factList();
        } else {
            // lambda
        }
    }

    void ruleList() {
        if (tokenType() == ID) {
            rule();
            ruleList();
        } else {
            // lambda
        }
    }

    void queryList() {
        if (tokenType() == ID) {
            query();
            queryList();
        } else {
            // lambda
        }
    }

    vector<Parameter> makeParams(vector<Token> tokens) {
        vector<Parameter> params;
        for (int i = 0; i < tokens.size(); i++) {
            bool val = false;
            if (tokens.at(i).getType() == STRING) {
                val = true;
            }
            Parameter param = Parameter(val, tokens.at(i).getValue());
            params.push_back(param);
        }
        return params;
    }

    void makeStringSet(vector<Token> tokens) {
        for (int i = 0; i < tokens.size(); i++) {
            domain.insert(tokens.at(i).getValue());
        }
    }

    void scheme() {
        Token name = match(ID);
        match(LEFT_PAREN);
        Token first = match(ID);
        vector<Token> ids;
        ids.push_back(first);
        idList(ids);
        match(RIGHT_PAREN);
        schemes.push_back(Predicate(name.getValue(), makeParams(ids)));
    }

    void fact() {
        Token name = match(ID);
        match(LEFT_PAREN);
        Token first = match(STRING);
        vector<Token> ids;
        ids.push_back(first);
        stringList(ids);
        match(RIGHT_PAREN);
        match(PERIOD);
        facts.push_back(Predicate(name.getValue(), makeParams(ids)));
        makeStringSet(ids);
    }

    void rule() {
        Predicate name = headPredicate();
        match(COLON_DASH);
        Predicate pred1 = predicate();
        vector<Predicate> preds;
        preds.push_back(pred1);
        predicateList(preds);
        match(PERIOD);
        rules.push_back(Rule(name, preds));
    }

    void query() {
        Predicate first = predicate();
        match(Q_MARK);
        querys.push_back(first);
    }

    Predicate headPredicate() {
        Token name = match(ID);
        match(LEFT_PAREN);
        Token first = match(ID);
        vector<Token> tokens;
        tokens.push_back(first);
        idList(tokens);
        match(RIGHT_PAREN);
        return Predicate(name.getValue(), makeParams(tokens));
    }

    Predicate predicate() {
        Token pred = match(ID);
        match(LEFT_PAREN);
        Token param1 = parameter();
        vector<Token> params;
        params.push_back(param1);
        parameterList(params);
        match(RIGHT_PAREN);
        return Predicate(pred.getValue(), makeParams(params));
    }

    vector<Predicate> predicateList(vector<Predicate> &predicates) {
        if (tokenType() == COMMA) {
            match(COMMA);
            Predicate val = predicate();
            predicates.push_back(val);
            predicateList(predicates);
        } else {
            // lambda
        }
        return predicates;
    }

    Token parameter() {
        Token name = Token(UNDEFINED, "", 0);
        if (tokenType() == STRING) {
            name = match(STRING);
        } else {
            name = match(ID);
        }
        return name;
    }

    vector<Token> parameterList(vector<Token> &parameters) {
        if (tokenType() == COMMA) {
            match(COMMA);
            parameters.push_back(parameter());
            parameterList(parameters);
        } else {
            // lambda
        }
        return parameters;
    }

    vector<Token> stringList(vector<Token> &ids) {
        if (tokenType() == COMMA) {
            match(COMMA);
            ids.push_back(match(STRING));
            stringList(ids);
        } else {
            // lambda
        }
        return ids;
    }

    vector<Token> idList(vector<Token> &ids) {
        if (tokenType() == COMMA) {
            match(COMMA);
            ids.push_back(match(ID));
            idList(ids);
        } else {
            // lambda
        }
        return ids;
    }


    const vector<Predicate> getSchemes() const {
        return schemes;
    }

    void setSchemes(const vector<Predicate> schemes) {
        Parser::schemes = schemes;
    }

    const vector<Predicate> getFacts() const {
        return facts;
    }

    void setFacts(const vector<Predicate> facts) {
        Parser::facts = facts;
    }

    const vector<Predicate> getQuerys() const {
        return querys;
    }

    void setQuerys(const vector<Predicate> querys) {
        Parser::querys = querys;
    }

    const vector<Rule> getRules() const {
        return rules;
    }

    void setRules(const vector<Rule> rules) {
        Parser::rules = rules;
    }

    const set<string> getDomain() const {
        return domain;
    }

    void setDomain(const set<string> domain) {
        Parser::domain = domain;
    }

    void toString() {
        cout << "Success!" << endl;
        cout << "Schemes(" << schemes.size() << "):" << endl;
        if (schemes.size() == 0) {
        } else {
            for (int i = 0; i < schemes.size(); i++) {
                cout << "  ";
                schemes[i].toString();
                if (i < schemes.size() - 1) {
                    cout << "\n";
                }
            }
        }
        cout << "\n";
        cout << "Facts(" << facts.size() << "):" << endl;
        if (facts.size() == 0) {
        } else {
            for (int i = 0; i < facts.size(); i++) {
                cout << "  ";
                facts[i].toString();
                cout << ".";
                if (i < facts.size() - 1) {
                    cout << "\n";
                }
            }
            cout << "\n";
        }
        cout << "Rules(" << rules.size() << "):" << endl;
        if (rules.size() == 0) {
        } else {
            for (int i = 0; i < rules.size(); i++) {
                cout << "  ";
                rules[i].toString();
                if (i < rules.size() - 1) {
                    cout << "\n";
                }
            }
            cout << "\n";
        }
        cout << "Queries(" << querys.size() << "):" << endl;
        if (querys.size() == 0) {
        } else {
            for (int i = 0; i < querys.size(); i++) {
                cout << "  ";
                querys[i].toString();
                cout << "?";
                if (i < querys.size() - 1) {
                    cout << "\n";
                }
            }
            cout << "\n";
        }
        cout << "Domain(" << domain.size() << "):" << endl;
        for (auto it = domain.begin(); it != domain.end(); it++) {
            cout << "  ";
            cout << *it << endl;
        }
        querys.clear();
        rules.clear();
        schemes.clear();
        facts.clear();
    }


};


