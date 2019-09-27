#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
#include <iostream>
#include <unordered_map>

#define INF 0x7fffffffffffff

using namespace std;

typedef long long ll;

struct Graph {
    struct Edge {
        int src, dst; ll cost;
        bool operator < (Edge a) const {
            return cost > a.cost;
        }
        Edge operator + (Edge a) const {
            if (cost == INF || a.cost == INF) return {src, a.dst, INF};
            return {src, a.dst, cost+a.cost};
        }
    };
    vector<vector<unsigned long>> adj, radj;
    vector<Edge> edges;
    Graph(int size) {
        adj.resize(size);
        radj.resize(size);
    }
    void push(int src, int dst, ll cost) {
        adj[src].push_back(edges.size());
        radj[dst].push_back(edges.size());
        edges.push_back({src, dst, cost});
    }
};

struct Dijkstra: Graph {
    Dijkstra(int size): Graph(size){}
    vector<ll> shortest_dist(int s) {
        vector<ll> dist(adj.size(), INF);
        priority_queue<Edge> pq;
        pq.push({s, s, 0});
        while (!pq.empty()) {
            auto curr = pq.top(); pq.pop();
            if (dist[curr.dst] == INF) {
                dist[curr.dst] = curr.cost;
                for (auto idx: adj[curr.dst]) {
                    auto next = edges[idx];
                    pq.push(curr+next);
                }
            }
        }
        return dist;
    }
    
    ll shortest_dist(int s, int t) {
        vector<ll> dist(adj.size(), INF);
        priority_queue<Edge> pq;
        pq.push({s, s, 0});
        while (!pq.empty()) {
            auto curr = pq.top(); pq.pop();
            if (dist[curr.dst] == INF) {
                if (curr.dst == t) return curr.cost;
                dist[curr.dst] = curr.cost;
                for (auto idx: adj[curr.dst]) {
                    auto next = edges[idx];
                    pq.push(curr+next);
                }
            }
        }
        return dist[t];
    }
    
    void cancel_shortest_path(int S, int T) {
        auto dist = shortest_dist(S);
        queue<int> q; q.push(T);
        while (!q.empty()) {
            auto dst = q.front(); q.pop();
            for (auto idx: radj[dst]) {
                auto prev = edges[idx];
                if (dist[prev.src]+prev.cost==dist[dst]) {
                    q.push(prev.src);
                    edges[idx].cost = INF;
                }
            }
        }
    }
};

struct SPFA: Graph {
    SPFA(int size): Graph(size){}
    vector<ll> shortest_dist(int s) {
        vector<ll> dist(adj.size(), INF);
        vector<int> cnt(adj.size());
        vector<bool> inq(adj.size());
        queue<int> q; dist[s] = 0; q.push(s); inq[s] = 1;
        while (!q.empty()) {
            auto src = q.front(); q.pop(); inq[src] = 0;
            for (auto idx: adj[src]) {
                auto edge = edges[idx];
                auto dst = edge.dst;
                auto cost = edge.cost;
                if (cnt[dst] == adj.size()) {
                    dist[dst] = -INF;
                } else if (dist[dst] > dist[src] + cost) {
                    dist[dst] = dist[src] + cost;
                    if (!inq[dst]) {
                        cnt[dst]++;
                        q.push(dst); inq[dst] = 1;
                    }
                }
            }
        }
        vector<bool> called(adj.size());
        for (int i = 0; i < adj.size(); ++i) if (dist[i] == -INF)
            dfs(i, called, dist);
        return dist;
    }
    
    void dfs(int s, vector<bool> &called, vector<ll> &dist) {
        if (called[s]) return;
        called[s] = 1;
        dist[s] = -INF;
        for (auto idx: adj[s])
            dfs(edges[idx].dst, called, dist);
    }
};

struct FloydWashall: Graph {
    FloydWashall(int size): Graph(size) {}
    vector<vector<ll>> shortest_dist() {
        vector<vector<ll>> dist(adj.size(), vector<ll>(adj.size(), INF));
        for (auto edge: edges)
            dist[edge.src][edge.dst] = edge.cost;
        for (int m = 0; m < adj.size(); ++m) {
            for (int s = 0; s < adj.size(); ++s) if (dist[s][m] != INF) {
                for (int d = 0; d < adj.size(); ++d) if (dist[m][d] != INF) {
                    ll tmp = dist[s][m] + dist[m][d];
                    if (tmp < dist[s][d]) dist[s][d] = tmp;
                }
            }
        }
        return dist;
    }
};

struct BOJ1753 {
    BOJ1753() {
        int V, E, S; cin >> V >> E >> S;
        Dijkstra dijkstra = V+1;
        for (int i = 0; i < E; ++i) {
            int u, v; ll w; cin >> u >> v >> w;
            dijkstra.push(u, v, w);
        }
        auto dist = dijkstra.shortest_dist(S);
        for (int i = 1; i <= V; ++i) {
            if (dist[i] == INF) cout << "INF\n";
            else cout << dist[i] << "\n";
        }
    }
};

struct BOJ5719 {
    BOJ5719() {
        while (1) {
            int N, M; cin >> N >> M;
            if (N == 0 && M == 0) break;
            Dijkstra dijkstra(N);
            int S, T; cin >> S >> T;
            for (int i = 0; i < M; ++i) {
                int U, V; ll P; cin >> U >> V >> P;
                dijkstra.push(U, V, P);
            }
            dijkstra.cancel_shortest_path(S, T);
            auto answer = dijkstra.shortest_dist(S, T);
            if (answer == INF) cout << "-1\n";
            else cout << answer << "\n";
        }
    }
};

 struct BOJ1219 {
     BOJ1219() {
         int N, S, T, M; cin >> N >> S >> T >> M;
         SPFA spfa = N;
         for (int i = 0; i < M; ++i) {
             int src, dst; ll cost; cin >> src >> dst >> cost;
             spfa.push(src, dst, cost);
         }
         vector<ll> money(N);
         for (int i = 0; i < N; ++i)
             cin >> money[i];
         for (int i = 0; i < M; ++i)
             spfa.edges[i].cost -= money[spfa.edges[i].dst];
         auto dist = spfa.shortest_dist(S);
         if (dist[T] == INF) cout << "gg\n";
         else if (dist[T] == -INF) cout << "Gee\n";
         else cout << -dist[T]+money[S] << "\n";
     }
 };

struct BOJ11403 {
    BOJ11403() {
        int N; cin >> N;
        FloydWashall floyd = N;
        for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) {
            int a; cin >> a;
            if (a) floyd.push(i, j, 1);
        }
        auto dist = floyd.shortest_dist();
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (dist[i][j] == INF) cout << "0 ";
                else cout << "1 ";
            } cout << "\n";
        }
    }
};

 int main() {
     ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
     BOJ11403 p;
     return 0;
 }
