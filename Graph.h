//
// Created by Nicholas Lewis on 4/1/24.
//
#pragma once

#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include "Node.h"
#include <map>
#include <stack>
#include <vector>

using namespace std;

class Graph {

private:

    map<int, Node> nodes;
    vector<bool> visited;
    stack<int> postordernums;

public:

    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++) {
            nodes[nodeID] = Node();
            visited.push_back(false);
        }
    }

    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }

    string toString() {
        stringstream out;
        for (auto &pair: nodes) {
            int nodeID = pair.first;
            Node node = pair.second;
            out << "R" << nodeID << ": ";
            out << node.toString();
        }
        return out.str();
    }


    bool isDependent(int id) {
        if (nodes[id].getAdjacentNodeIDs().count(id) == 1) {
            return true;
        }
        return false;
    }


    void dfs(int x) {
        visited[x] = true;
        set<int>::iterator i;
        for (i = nodes[x].getAdjacentNodeIDs().begin(); i != nodes[x].getAdjacentNodeIDs().end(); ++i) {
            if (!visited[*i]) {
                dfs(*i);
            }
        }
        postordernums.push(x);
    }

    void dfsSCC(int x, set<int> &onescc) {
        visited[x] = true;
        set<int>::iterator i;
        for (i = nodes[x].getAdjacentNodeIDs().begin(); i != nodes[x].getAdjacentNodeIDs().end(); ++i) {
            if (!visited[*i]) {
                dfsSCC(*i, onescc);
            }
        }
        onescc.insert(x);
    }

    void dfsForest() {
        for (bool x: visited) {
            if (x == true) {
                x = false;
            }
        }
        int y = 0;
        for (bool x: visited) {
            if (x == false) {
                dfs(y);
            }
            y++;
        }
    }

    vector<set<int> > dfsForestSCC(stack<int> posts) {
        vector<set<int> > scc;
        for (bool x: visited) {
            if (x == true) {
                x = false;
            }
        }
        while (!posts.empty()) {
            if (!visited[posts.top()]) {
                set<int> onescc;
                int x = posts.top();
                posts.pop();
                dfsSCC(x, onescc);
                scc.push_back(onescc);
            } else {
                posts.pop();
            }
        }
        return scc;
    }

    const stack<int> &getPostordernums() const {
        return postordernums;
    }
};



