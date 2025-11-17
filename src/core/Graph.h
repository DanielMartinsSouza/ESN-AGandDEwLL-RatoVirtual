//
// Created by Daniel on 24/09/2025.
//

#ifndef ESN_AGANDDEWLL_RATOVIRTUAL_GRAPH_H
#define ESN_AGANDDEWLL_RATOVIRTUAL_GRAPH_H
#include <list>

using namespace std;

class Graph
{
    int V; // Number of vertices
    list<int>* adj; // Points to adjacency list
    void DFSUtil(int v, bool visited[], int* connected_v, int comp_index);

public:
    explicit Graph(int V);

    ~Graph();

    void addUndirectedEdge(int v, int w) const;

    void addDirectedEdge(int v, int w) const;

    void connectedComponents(int* connected_v);

    [[nodiscard]] int isThereEdge(int v, int w) const;

    [[nodiscard]] int nEdges(int v) const;

    void printGraph() const;
};


#endif //ESN_AGANDDEWLL_RATOVIRTUAL_GRAPH_H
