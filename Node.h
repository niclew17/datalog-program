//
// Created by Nicholas Lewis on 4/1/24.
//
#pragma once

#include <set>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class Node {

private:
    bool visited;
    set<int> adjacentNodeIDs;

public:

    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    string toString() {
        stringstream out;
        int z = 0;
        if (adjacentNodeIDs.size() == 0) {
            out << endl;
        } else {
            for (auto i: adjacentNodeIDs) {
                if (z < adjacentNodeIDs.size() - 1) {
                    out << "R" << i << ",";
                } else {
                    out << "R" << i << endl;
                }
                z++;
            }
        }
        return out.str();
    }

    bool isVisited() const {
        return visited;
    }

    void setVisited(bool visited) {
        Node::visited = visited;
    }

    const set<int> &getAdjacentNodeIDs() const {
        return adjacentNodeIDs;
    }

    bool operator==(const Node &rhs) const {
        return adjacentNodeIDs == rhs.adjacentNodeIDs;
    }

    bool operator!=(const Node &rhs) const {
        return !(rhs == *this);
    }


};



