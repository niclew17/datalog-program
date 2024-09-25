//
// Created by Nicholas Lewis on 1/19/24.
//
#pragma once

#include <string>
#include <sstream>
#include <map>
#include <iostream>

using namespace std;

enum TokenType {
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COLON_DASH,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    ID,
    STRING,
    COMMENT,
    UNDEFINED,
    EOFILE
};

class Token {
private:
    TokenType type;
    string value;
    int linenumber;

public:
    Token(TokenType type, string value, int linenumber) {
        this->type = type;
        this->value = value;
        this->linenumber = linenumber;
    }

    string toString() const {
        stringstream out;
        out << "(" << typeName(type) << "," << "\"" << value << "\"" << "," << linenumber << ")" << endl;
        return out.str();
    }

    TokenType getType() const {
        return type;
    }

    const string &getValue() const {
        return value;
    }

    string typeName(TokenType type) const {
        string word = "";
        switch (type) {
            case 0:
                word = "COMMA";
                break;
            case 1:
                word = "PERIOD";
                break;
            case 2:
                word = "Q_MARK";
                break;
            case 3:
                word = "LEFT_PAREN";
                break;
            case 4:
                word = "RIGHT_PAREN";
                break;
            case 5:
                word = "COLON";
                break;
            case 6:
                word = "COLON_DASH";
                break;
            case 7:
                word = "MULTIPLY";
                break;
            case 8:
                word = "ADD";
                break;
            case 9:
                word = "SCHEMES";
                break;
            case 10:
                word = "FACTS";
                break;
            case 11:
                word = "RULES";
                break;
            case 12:
                word = "QUERIES";
                break;
            case 13:
                word = "ID";
                break;
            case 14:
                word = "STRING";
                break;
            case 15:
                word = "COMMENT";
                break;
            case 16:
                word = "UNDEFINED";
                break;
            case 17:
                word = "EOF";
                break;
        }
        return word;
    };
};



