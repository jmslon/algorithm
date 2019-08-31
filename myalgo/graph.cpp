#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#define INF 0x6fffffffffffffff

using namespace std;

typedef long long ll;

struct Edge {
    int src, dst; ll cost; unsigned long key;
    bool operator > (const Edge &a) const {return cost > a.cost;} // for MIN_HEAP
    Edge operator + (const Edge &a) const {return {src, a.dst, cost+a.cost};} // for Edge NEW
};

struct Graph {
    int V;
    vector<vector<unsigned long>> adj, radj;
    vector<Edge> edges;
    
    Graph(int size) {
        V = size;
        adj.resize(size);
        radj.resize(size);
    }
    
    void push(int src, int dst, ll cost) {
        push(src, dst, cost, edges.size());
    }
    
    void push(int src, int dst, ll cost, unsigned long key) {
        adj[src].push_back(key);
        radj[dst].push_back(key);
        edges.push_back({src, dst, cost, key});
    }
    
    void update(unsigned long key, ll cost) {
        edges[key].cost = cost;
    }
};

struct FloydWashall: Graph {
    FloydWashall(int size): Graph(size) {}
    vector<vector<ll>> shortest_dist() {
        vector<vector<ll>> dist(V, vector<ll>(V, INF));
        
        for (int s = 1; s < V; ++s) {
            for (auto idx: adj[s]) {
                auto e = edges[idx];
                dist[s][e.dst] = e.cost;
            }
        }
        
        for (int m = 1; m < V; ++m) {
            for (int s = 1; s < V; ++s) {
                if (dist[s][m] == INF) continue;
                for (int d = 1; d < V; ++d) {
                    ll tmp = dist[s][m] + dist[m][d];
                    if (dist[s][d] > tmp)
                        dist[s][d] = tmp;
                }
            }
        }
        
        return dist;
    }
};

struct Dijkstra: Graph {
    Dijkstra(int size): Graph(size) {}
    vector<ll> shortest_dist(int s) {
        vector<ll> dist(V, INF);
        priority_queue<Edge, vector<Edge>, greater<Edge>> pq;
        
        dist[s] = 0;
        pq.push({s, s, 0});
        while (!pq.empty()) {
            auto curr = pq.top();
            pq.pop();
            if (curr.cost > dist[curr.dst]) continue;
            for (auto idx: adj[curr.dst]) {
                auto next = edges[idx];
                Edge NEW = curr + next;
                if (dist[NEW.dst] > NEW.cost) {
                    dist[NEW.dst] = NEW.cost;
                    pq.push(NEW);
                }
            }
        }
        return dist;
    }
    
    Graph shortest_path(int s, int d) {
        auto dist = shortest_dist(s);
        Graph ret = V;
        
        queue<int> q; q.push(d);
        while (!q.empty()) {
            auto dst = q.front(); q.pop();
            for (auto idx: radj[dst]) {
                auto src = edges[idx].src;
                if (dist[src] + edges[idx].cost == dist[dst]) {
                    q.push(src);
                    ret.push(src, dst, edges[idx].cost, edges[idx].key);
                }
            }
        }
        
        return ret;
    }
};

struct SPFA: Graph {
    vector<ll> dist;
    vector<bool> called;
    SPFA(int size): Graph(size),dist(size, INF){}
    vector<ll> shortest_dist(int s) {
        vector<ll> dist = this->dist;
        vector<int> cnt(V, 0);
        vector<bool> inq(V, 0);
        queue<int> q;
        
        dist[s] = 0;
        q.push(s);
        inq[s] = 1;
        while (!q.empty()) {
            int src = q.front();
            q.pop();
            inq[src] = 0;
            for (auto idx: adj[src]) {
                auto e = edges[idx];
                int dst = e.dst;
                ll cost = e.cost;
                if (cnt[dst] == V) {
                    // negative cycle!
                    dist[dst] = -INF;
                } else if (dist[dst] > dist[src] + cost) {
                    dist[dst] = dist[src] + cost;
                    if (!inq[dst]) {
                        cnt[dst]++;
                        q.push(dst);
                        inq[dst] = 1;
                    }
                }
            }
        }
        this->dist = dist;
        called.resize(V);
        for (int i = 0; i < V; ++i) {
            if (dist[i] == -INF)
                dfs(i);
        }
        return this->dist;
    }
    
    void dfs(int s) {
        if (called[s]) return;
        called[s] = 1;
        dist[s] = -INF;
        for (auto idx: adj[s])
            dfs(edges[idx].dst);
    }
};
