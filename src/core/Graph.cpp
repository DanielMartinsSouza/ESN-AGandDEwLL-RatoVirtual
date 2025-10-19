//
// Created by Daniel on 24/09/2025.
//

#include "Graph.h"
#include <iostream>

Graph::Graph(const int V) {
    this->V = V;
    adj = new list<int>[V];
}

Graph::~Graph() {
    delete[] adj;
}

void Graph::addUndirectedEdge(int v, int w) const {
    adj[v].push_back(w);
    adj[w].push_back(v);
}

void Graph::addDirectedEdge(int v, int w) const {
    adj[v].push_back(w);
}

void Graph::connectedComponents(int *connected_v) {
    int v, comp_index = 0; // index of the component

    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for (v = 0; v < V; v++)
        visited[v] = false;

    for (v = 0; v < V; v++) {
        if (visited[v] == false) {
            // find all reachable vertices from v
            DFSUtil(v, visited, connected_v, comp_index);
            comp_index++;
            cout << endl;
        }
    }
    delete[] visited;
}

void Graph::DFSUtil(const int v, bool visited[], int *connected_v, const int comp_index) {
    // Mark the current node as visited
    visited[v] = true;
    //cout << v << " ";
    connected_v[v] = comp_index;

    // Recurrence for all the vertices adjacent to vertex v
    for (const int &i: adj[v])
        if (!visited[i])
            DFSUtil(i, visited, connected_v, comp_index);
}

int Graph::isThereEdge(const int v, const int w) const {
    auto i = adj[v].begin();
    while (i != adj[v].end() && w != *i)
        ++i;

    if (i != adj[v].end())
        return 1;
    return 0;
}

int Graph::nEdges(int v) const {
    return (adj[v].size());
}

void Graph::printGraph() const {
    cout << "Graph:" << endl;
    for (int v = 0; v < V; ++v) {
        cout << v << "->";
        for (const int x: adj[v]) {
            cout << x << " ";
        }
        cout << endl;
    }
}
