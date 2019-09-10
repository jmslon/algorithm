#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>

#define INF 0x6fffffff

using namespace std;

typedef int ll;

struct FlowGraph {
    struct Edge {
        int src, dst; ll cost, flw, cap;
        ll residual() {return cap-flw;}
    };
    
    int V;
    vector<vector<unsigned long>> adj;
    vector<Edge> edges;
    
    FlowGraph(int size){
        V = size;
        adj.resize(size);
    }
    
    void push(Edge e1, Edge e2) {
        adj[e1.src].push_back(edges.size());
        edges.push_back(e1);
        
        adj[e2.src].push_back(edges.size());
        edges.push_back(e2);
    }
    
    void flow(unsigned long idx, ll flw) {
        edges[idx].flw += flw;
        edges[idx%2?idx-1:idx+1].flw -= flw;
    }
};

struct Edmond: FlowGraph {
    Edmond(int size): FlowGraph(size) {}
    auto minimum_cost_maximum_flow(int s, int t) {
        ll mc = 0;
        ll mf = 0;
        while (1) {
            vector<unsigned long> key(V, -1); // back edge index of nodes
            vector<bool> inq(V, 0);
            vector<ll> dist(V, INF);
            queue<int> q;
            
            dist[s] = 0; q.push(s); inq[s] = 1;
            
            while (!q.empty()) {
                int src = q.front(); q.pop(); inq[src] = 0;
                for (auto idx: adj[src]) {
                    auto e = edges[idx];
                    int dst = e.dst;
                    if (e.residual() > 0 && dist[dst] > dist[src] + e.cost) {
                        dist[dst] = dist[src] + e.cost;
                        key[dst] = idx;
                        if (!inq[dst]) {
                            q.push(dst);
                            inq[dst] = 1;
                        }
                    }
                }
            }
            if (dist[t] == INF) break;
            
            ll flw = INF;
            for (int node = t; node != s; node = edges[key[node]].src)
                flw = min(flw, edges[key[node]].residual());
            for (int node = t; node != s; node = edges[key[node]].src)
                flow(key[node], flw);
            
            mc += flw * dist[t];
            mf += flw;
        }
        
        return vector<ll>{mc, mf};
    }
};

struct BOJ11408 {
    BOJ11408() {
        int N, M; cin >> N >> M;
        int S = 0, T = N+M+1;
        Edmond edmond = N+M+2;
        for (int i = 1; i <= N; ++i) {
            int n; cin >> n;
            for (int j = 0; j < n; ++j) {
                int tmp1; ll tmp2; cin >> tmp1 >> tmp2;
                edmond.push({i, N+tmp1, tmp2, 0, 1}, {N+tmp1, i, -tmp2, 0, 0});
            }
        }
        for (int i = 1; i <= N; ++i)
            edmond.push({S, i, 0, 0, 1}, {i, S, 0, 0, 0});
        for (int i = 1; i <= M; ++i)
            edmond.push({N+i, T, 0, 0, 1}, {T,N+i, 0, 0, 0});
        
        auto mcmf = edmond.minimum_cost_maximum_flow(S, T);
        cout << mcmf[1] << endl << mcmf[0] << endl;
    }
};

int main(){
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    BOJ11408 p;
    return 0;
}
