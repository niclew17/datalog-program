#include <fstream>
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include <string>
#include "Relation.h"
#include "Interpreter.h"


using namespace std;

int main(int argc, char *argv[]) {
    ifstream in;
    in.open(argv[1]);
    stringstream ss;
    ss << in.rdbuf();
    string input = ss.str();
    in.close();
    Scanner s = Scanner();
    while (!input.empty()) {
        Token token = s.scanToken(input);
        if (token.getValue() == "EOF")
            break;
        if (token.getType() != COMMENT) {
            s.addToken(token);
            input = input.substr(token.getValue().length());
        } else {
            input = input.substr(token.getValue().length());
        }
    }
    s.addToken(Token(EOFILE, "", s.getLine()));
    Parser myparse = Parser(s.getTokens());
    myparse.datalogProgram();
    map<string, Relation> mymap;
    Database mydatabase(mymap);
    Interpreter interpreter(myparse.getSchemes(), myparse.getFacts(), myparse.getQuerys(), myparse.getRules(),
                            mydatabase);
    interpreter.addDatabase();
    interpreter.evaluateRules();
    interpreter.evaluateQuerys();
    return 0;
}
