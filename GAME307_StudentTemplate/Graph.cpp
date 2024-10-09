#include "Graph.h"

Graph::Graph() {
}

Graph::~Graph() {
}

bool Graph::OnCreate(std::vector<Node*> nodes_) {
    // given a list of nodes, initialize a matrix of costs with 0.0
    int numNodes = nodes_.size();

    cost.resize(numNodes);
    for (int i = 0; i < numNodes; i++) {
        // populate the internal map
        if (i != nodes_[i]->getLabel()) {
            std::cerr << "node with label 'i' not in the 'i' position for " << i << "\n";
            return false;
        }
        node[i] = nodes_[i];

        // set up connections
        cost[i].resize(numNodes);

        for (int j = 0; j < numNodes; j++) {
            cost[i][j] = 0.0f;
        }
    }

    return true;
}

int Graph::numNodes() {
    return node.size();
}

void Graph::addWeightedConnection(Node* fromNode, Node* toNode, float weight) {
    cost[fromNode->getLabel()][toNode->getLabel()] = weight;
}

std::vector<Node*> Graph::neighbours(Node* fromNode) {
    std::vector<Node*> result = {};
    int from = fromNode->getLabel();
    for (int j = 0; j < numNodes(); j++) {
        if (cost[from][j] > 0.0f) {
            result.push_back(getNode(j));
        }
    }
    return result;
}

std::vector<Node*> Graph::findPath(Node* startNode, Node* goalNode) {
    std::vector<Node*> result;

    float new_cost, priority;
    Node* currentNode = startNode;

    std::priority_queue<NodeAndPriority, std::deque<NodeAndPriority>, NodeAndPriority> frontier;
    frontier.push(NodeAndPriority{ currentNode, 0.0f });

    // track solution path
    std::vector<int> came_from;
    came_from.resize(numNodes(), -1); // fill with invalid values

    int start = startNode->getLabel();
    std::map<int, float> cost_so_far;
    cost_so_far[start] = 0.0f;

    //while not frontier.empty
    while (!frontier.empty()) {
        //    current = frontier.get()
        currentNode = frontier.top().node;
        //    if current == goal
        if (currentNode->getLabel() == goalNode->getLabel()) {
            break;
        }

        std::vector<Node*> neighbours = this->neighbours(currentNode);
        //    for next in graph.neighbours( current )
        for (Node* next : neighbours) {
            //new_cost = cost_so_far[current] + graph.cost(current, next)
            new_cost = cost_so_far[currentNode->getLabel()] + cost[currentNode->getLabel()][next->getLabel()];
            //        if next not in cost_so_far or new_cost < cost_so_far[next]

        }
    }


    //while not frontier.empty
    //    current = frontier.get()
    //    if current == goal
    //        break
    //    for next in graph.neighbours( current )
    //        new_cost = cost_so_far[current] + graph.cost(current,next)
    //        if next not in cost_so_far or new_cost < cost_so_far[next]
    //            cost_so_far[next] = new_cost
    //            priority = new_cost + heuristic( goal, next )
    //            frontier.put( next, priority )
    //            came_from[next] = current

    return result;
}
