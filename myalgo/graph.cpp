#include <iostream>
#include <queue>
#include <algorithm>
#include <cmath>
#include <set>
#include <unordered_map>
#include <vector>

#define INF 0xfffffff

using namespace std;


struct Edge {
    int src, dst, weight;
    bool operator > (const Edge &a) const {return weight > a.weight;} // for MIN_HEAP
    Edge operator + (const Edge &a) const {return {src, a.dst, weight+a.weight};} // for Edge NEW
};

struct Graph {
    int V;
    vector<unordered_map<int, Edge> > adj;
    Edge query(int s, int d) {
        if (s == d) return {s, d, 0};
        return adj[s].count(d) ? adj[s][d] : (Edge) {s, d, INF};
    }
    void update(int s, int d, int w) {
        adj[s][d] = {s, d, w};
    }
    void update(Edge e) {
        adj[e.src][e.dst] = e;
    }
};

struct Dijkstra: Graph {
    Dijkstra(int size) {
        V = size;
        adj.resize(size);
    }
    
    auto dijkstra(int s) {
        vector<int> dist(V, INF);
        priority_queue<Edge, vector<Edge>, greater<Edge> > pq;
        dist[s] = query(s, s).weight;
        pq.push({s, s, 0});
        for (;!pq.empty();) {
            auto curr = pq.top();
            // curr == {s, curr.dst, curr.weight}
            pq.pop();
            if (curr.weight > dist[curr.dst]) continue;
            for (auto dstedge: adj[curr.dst]) {
                auto next = dstedge.second;
                // next == {curr.dst, next.dst, next.weight}
                Edge NEW = curr + next;
                // Edge NEW = {curr.src, next.dst, curr.weight + next.weight};
                
                if (dist[NEW.dst] < NEW.weight) continue;
                if (dist[NEW.dst] == NEW.weight) continue;
                dist[NEW.dst] = NEW.weight;
                pq.push(NEW);
            }
        }
        return dist;
    }
    
    int dijkstra(int s, int d) {
        return dijkstra(s)[d];
    }
};

struct FloydWarshall: Graph {
    FloydWarshall(int size) {
        V = size;
        adj.resize(size);
    }
    
    auto floyd_warshall() {
        vector<vector<int> > dist(V, vector<int>(V, INF));
        
        for (int s = 1; s < V; ++s) {
            for (auto a: adj[s]) {
                int d = a.first;
                auto w = a.second.weight;
                dist[s][d] = w;
            }
        }
        
        for (int m = 1; m < V; ++m) {
            for (int s = 1; s < V; ++s) {
                for (int d = 1; d < V; ++d) {
                    if (dist[s][d] > dist[s][m] + dist[m][d])
                        dist[s][d] = dist[s][m] + dist[m][d];
                }
            }
        }
        return dist;
    }
};
