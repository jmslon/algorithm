#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>
#include <vector>

#define INF 0x6fffffffffffffff

using namespace std;

typedef long long ll;

struct DisjointSet{
    ll size;
    vector<ll> par;
    
    DisjointSet(ll size) {
        this->size = size;
        par.resize(size);
        for(ll i = 0; i < size ; ++i) par[i] = i;
    }
    
    ll find(ll x) {
        if (par[x] == x) return x;
        return par[x] = find(par[x]);
    }
    
    void join(ll x, ll y) {
        x = find(x);
        y = find(y);
        if (x == y) return;
        if (x < y) swap(x, y);
        par[x] = y;
    }
};

struct Edge {
    int src, dst; ll cost; unsigned long key;
    bool operator < (const Edge &a) const {return cost < a.cost;} // for MAX_HEAP
    bool operator > (const Edge &a) const {return cost > a.cost;} // for MIN_HEAP (Dijkstra)
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
    
    void push(int src, int dst, ll cost) {
        push(src, dst, cost, edges.size());
    }
    
    void push(int src, int dst, ll cost, unsigned long key) {
        adj[src].push_back(key);
        edges.push_back({src, dst, cost, key});
    }
    
    void update(unsigned long key, ll cost) {
        edges[key].cost = cost;
    }
    
    Graph minimum_spanning_tree() {
        Graph ret = V;
        auto edges = this->edges;
        sort(edges.begin(), edges.end());
        DisjointSet ds = V;
        for (int i = 0, cnt = 0; i < edges.size() && cnt < V-1; ++i) {
            int src = edges[i].src, dst = edges[i].dst;
            if (ds.find(src) != ds.find(dst)) {
                ds.join(src, dst);
                ++cnt;
                ret.push(src, dst, edges[i].cost);
                ret.push(dst, src, edges[i].cost);
            }
        }
        return ret;
    }
};

struct BOJ1197 {
    int V, E;
    BOJ1197() {
        cin >> V >> E;
        Graph graph = V+1;
        for (int i = 0; i < E; ++i) {
            int A, B; ll C; cin >> A >> B >> C;
            graph.push(A, B, C);
            graph.push(B, A, C);
        }
        Graph mst = graph.minimum_spanning_tree();
        ll answer = 0;
        for (auto edge: mst.edges) {
            answer += edge.cost;
        }
        cout << answer / 2 << endl;
    }
};

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    BOJ1197 p;
    return 0;
}
