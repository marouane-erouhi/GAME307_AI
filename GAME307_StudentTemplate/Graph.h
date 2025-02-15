#ifndef GRAPH_H
#define GRAPH_H

// DO NOT USE THIS OUTSIDE THIS COURSE

#include <vector>
#include <map>
#include <iostream>
#include "Node.h"
#include <queue>

struct NodeAndPriority {
public:

    // member variables
    Node* node;
    float priority;

    void print() {
        printf("Node: %i, priority: %.2f \n", node->getLabel(), priority);
    }

    bool operator()(NodeAndPriority const& lhs, NodeAndPriority const& rhs) {
        // make it a min queue: lowest value of priority at top of the queue
        return lhs.priority > rhs.priority;
    }
};


class Graph {
private:
	// a list of nodes
	std::map<int, Node*> node; // essentially an array, e.g. node[2]

	// a 2D matrix of the connection costs/weights
	std::vector< std::vector< float > > cost;
	// cost [1][3]     NOT cost[1,3]

public:
	Graph();
	~Graph();
	bool OnCreate(std::vector< Node* > nodes_);
	int numNodes();
	Node* getNode(int label) { return node[label]; }
	void addWeightedConnection(Node* fromNode, Node* toNode, float weight);
	std::vector<Node*> neighbours(Node* fromNode);
	std::vector<Node*> findPath(Node* startNode, Node* goalNode);
};

#endif
