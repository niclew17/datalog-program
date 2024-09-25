//
// Created by Nicholas Lewis on 1/19/24.
//
#pragma once

#include <string>
#include "Token.h"
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

class Scanner {
private:
    int line;
    vector<Token> tokens;


public:
    Scanner() {
        this->line = 1;
    }

    int getLine() const {
        return line;
    }

public:
    void addToken(Token token) {
        tokens.push_back(token);
    }

    const vector<Token> &getTokens() const {
        return tokens;
    }

    Token isKeyword(string curinput) {
        if (!isalpha(curinput.at(0))) {
            return Token(UNDEFINED, "", line);
        }
        int index = 1;
        while (index < curinput.length() && (isalpha(curinput.at(index)) || isdigit(curinput.at(index)))) {
            index++;
        }
        if (curinput.substr(0, index) == "Schemes") {
            return Token(SCHEMES, "Schemes", line);
        } else if (curinput.substr(0, index) == "Facts") {
            return Token(FACTS, "Facts", line);
        } else if (curinput.substr(0, index) == "Rules") {
            return Token(RULES, "Rules", line);
        } else if (curinput.substr(0, index) == "Queries") {
            return Token(QUERIES, "Queries", line);
        } else {
            return Token(UNDEFINED, "", line);
        }
    }

    Token isID(string curinput) {
        if (!isalpha(curinput.at(0))) {
            return Token(UNDEFINED, "", line);
        }
        int index = 1;
        while (index < curinput.length() && (isalpha(curinput.at(index)) || isdigit(curinput.at(index)))) {
            index++;
        }
        return Token(ID, curinput.substr(0, index), line);

    }

    Token isTString(string curinput) {
        int previousstart = line;
        if (curinput.at(0) != '\'') {
            return Token(UNDEFINED, "", line);
        }
        int index = 1;
        while (index < curinput.length() && curinput.at(index) != '\'') {
            if (curinput.at(index) == '\n') {
                line++;
            }
            index++;
        }
        if (index == curinput.length()) {
            return Token(UNDEFINED, curinput, previousstart);
        }
        return Token(STRING, curinput.substr(0, index + 1), previousstart);

    }

    Token isComment(string curinput) {
        if (curinput.at(0) != '#') {
            return Token(UNDEFINED, "", line);
        }
        int index = 1;
        while (index < curinput.length() && curinput.at(index) != '\n') {
            index++;
        }
        return Token(COMMENT, curinput.substr(0, index), line);

    }

    Token scanToken(string &input) {
        while (!input.empty() && (isspace(input.at(0)) || input.at(0) == '\n')) {
            if (input.at(0) == '\n') {
                line++;
                input = input.substr(1);
            } else {
                input = input.substr(1);
            }

        }
        if (input.empty()) {
            return Token(EOFILE, "EOF", line);
        } else if (input.at(0) == ',') {
            return Token(COMMA, ",", line);
        } else if (input.at(0) == '.') {
            return Token(PERIOD, ".", line);
        } else if (input.at(0) == '?') {
            return Token(Q_MARK, "?", line);
        } else if (input.at(0) == '(') {
            return Token(LEFT_PAREN, "(", line);
        } else if (input.at(0) == ')') {
            return Token(RIGHT_PAREN, ")", line);
        } else if (input.at(0) == ':' && input.length() > 0 && input.at(1) == '-') {
            return Token(COLON_DASH, ":-", line);
        } else if (input.at(0) == ':') {
            return Token(COLON, ":", line);
        } else if (input.at(0) == '*') {
            return Token(MULTIPLY, "*", line);
        } else if (input.at(0) == '+') {
            return Token(ADD, "+", line);
        }


        vector<Token> matchedtokens;
        matchedtokens.push_back(isKeyword(input));
        matchedtokens.push_back(isID(input));
        matchedtokens.push_back(isTString(input));
        matchedtokens.push_back(isComment(input));

        int maxtoken = 0;
        Token outputtoken = Token(UNDEFINED, "", line);
        for (Token curtoken: matchedtokens) {
            if (curtoken.getValue().length() > maxtoken) {
                maxtoken = curtoken.getValue().length();
                outputtoken = curtoken;
            }
        }
        if (outputtoken.getType() == UNDEFINED && outputtoken.getValue().size() == 0)
            outputtoken = Token(UNDEFINED, input.substr(0, 1), line);

        return outputtoken;

    }

};
