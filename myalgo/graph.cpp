#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

const int INF = 0xfffffff;

struct Edge {
    int src, dst, cost;
    bool operator > (const Edge &a) const {return cost > a.cost;} // for MIN_HEAP
    Edge operator + (const Edge &a) const {return {src, a.dst, cost+a.cost};} // for Edge NEW
};

struct Graph {
    int V;
    vector<vector<unsigned long>> adj;
    vector<Edge> edges;
    
    Graph(int size) {
        V = size;
        adj.resize(size);
    }
    
    void push(Edge e) {
        adj[e.src].push_back(edges.size());
        edges.push_back(e);
    }
};

struct FloydWashall: Graph {
    FloydWashall(int size): Graph(size) {}
    vector<vector<int>> shortest_path() {
        vector<vector<int>> dist(V, vector<int>(V, INF));
        
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
                    int tmp = dist[s][m] + dist[m][d];
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
    vector<int> shortest_path(int s) {
        vector<int> dist(V, INF);
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
                if (dist[NEW.dst] < NEW.cost) continue;
                if (dist[NEW.dst] == NEW.cost) continue;
                dist[NEW.dst] = NEW.cost;
                pq.push(NEW);
            }
        }
        return dist;
    }
};

struct SPFA: Graph {
    SPFA(int size): Graph(size) {}
    vector<int> shortest_path(int s) {
        vector<int> dist(V, INF);
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
                int cost = e.cost;
                if (dist[dst] > dist[src] + cost) {
                    dist[dst] = dist[src] + cost;
                    if (cnt[dst] == V) return vector<int>();
                    if (!inq[dst]) {
                        cnt[dst]++;
                        q.push(dst);
                        inq[dst] = 1;
                    }
                }
            }
        }
        return dist;
    }
};

struct BOJ1753 {
    int V, E, S;
    Dijkstra dijkstra = 0;
    BOJ1753() {
        cin >> V >> E >> S;
        dijkstra = Dijkstra(V+1);
        for (int i = 0, u, v, w; i < E; ++i) {
            cin >> u >> v >> w;
            dijkstra.push({u, v, w});
        }
    }
    void solve() {
        auto dist = dijkstra.shortest_path(S);
        if (dist.empty()) cout << "-1\n";
        else {
            for (int i = 1; i <= V; ++i) {
                if (dist[i] >= INF) cout << "INF\n";
                else cout << dist[i] << "\n";
            }
        }
    }
};

struct BOJ11657 {
    int V, E;
    SPFA spfa = 0;
    BOJ11657() {
        cin >> V >> E;
        spfa = SPFA(V+1);
        for (int i = 0, u, v, w; i < E; ++i) {
            cin >> u >> v >> w;
            spfa.push({u, v, w});
        }
    }
    void solve() {
        auto dist = spfa.shortest_path(1);
        if (dist.empty()) cout << "-1\n";
        else {
            for (int i = 2; i <= V; ++i) {
                if (dist[i] >= INF) cout << "-1\n";
                else cout << dist[i] << "\n";
            }
        }
    }
};

struct BOJ1389 {
    int N, M;
    FloydWashall floyd = 0;
    BOJ1389() {
        cin >> N >> M;
        floyd = FloydWashall(N+1);
        for (int i = 0, u, v; i < M; ++i) {
            cin >> u >> v;
            floyd.push({u, v, 1});
            floyd.push({v, u, 1});
        }
    }
    void solve() {
        auto dist = floyd.shortest_path();
        vector<int> bacon(N+1, 0);
        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j <= N; ++j) {
                if (i == j) continue;
                bacon[i] += dist[i][j];
            }
        }
        int answer = 1;
        for (int i = 2; i <= N; ++i) {
            if (bacon[i] < bacon[answer])
                answer = i;
        }
        cout << answer << "\n";
    }
};

int main() {
    
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    BOJ1389 p; p.solve();
    return 0;
}
