#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

#define INF 0x6fffffff

using namespace std;

struct Edge {
    int src, dst; ll flw, cap;
    ll residual() {return cap-flw;}
};

struct FlowGraph {
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
    int maximum_flow(int s, int t) {
        int mf = 0;
        while (1) {
            vector<int> par(V, -1);
            vector<unsigned long> path(V, 0);
            //vector<int> dist(V, INF);
            queue<int> q;
            
            //dist[s] = 0;
            q.push(s);
            while (!q.empty() && par[t] == -1) {
                int curr = q.front();
                q.pop();
                for (auto idx: adj[curr]) {
                    auto e = edges[idx];
                    int next = e.dst;
                    if (e.residual() > 0 && par[next] == -1) {
                        q.push(next);
                        par[next] = curr;
                        path[next] = idx;
                        //dist[next] = dist[curr] + 1;
                        if (next == t) break;
                    }
                }
            }
            if (par[t] == -1) break;
            int flw = INF;
            for (int i = t; i != s; i = par[i])
                flw = min(flw, edges[path[i]].residual());
            for (int i = t; i != s; i = par[i]) {
                flow(path[i], flw);
            }
            mf += flw;
        }
        return mf;
    }
};

struct Dinic: FlowGraph {
    int S, T;
    vector<ll> dist, work;
    
    Dinic(int size): FlowGraph(size) {}
    
    bool bfs() {
        dist.clear(); dist.resize(V, INF);
        dist[S] = 0;
        
        queue<int> q;
        q.push(S);
        while (!q.empty()){
            int curr = q.front(); q.pop();
            for(auto idx: adj[curr]){
                int next = edges[idx].dst;
                if (dist[next] == INF && edges[idx].residual() > 0) {
                    dist[next] = dist[curr] + 1;
                    q.push(next);
                }
            }
        }
        return dist[T] != INF;
    }
    
    ll dfs(int curr, ll flw) {
        if (curr == T) return flw;
        
        for(; work[curr] < adj[curr].size(); ++work[curr]) {
            auto idx = adj[curr][work[curr]];
            auto e = edges[idx];
            int next = e.dst;
            if (dist[next] == dist[curr] + 1 && e.residual() > 0) {
                ll df = dfs(next, min(e.residual(), flw));
                if (df > 0){
                    flow(idx, df);
                    return df;
                }
            }
        }
        return 0;
    }
    
    ll maximum_flow(int s, int t) {
        S = s; T = t;
        ll mf = 0;
        while (bfs()) {
            work.clear(); work.resize(V, 0);
            while (1) {
                ll flow = dfs(S, INF);
                if (flow == 0) break;
                mf += flow;
            }
        }
        return mf;
    }
    
    auto team_a() {
        vector<bool> visit(V);
        visit[S] = 1;
        queue<int> q; q.push(S);
        while (!q.empty()) {
            int curr = q.front(); q.pop();
            for (auto idx: adj[curr]) {
                int next = edges[idx].dst;
                if (!visit[next] && edges[idx].residual() > 0) {
                    visit[next] = true;
                    q.push(next);
                }
            }
        }
        return visit;
    }
};

struct BOJ11375 {
    int N, M;
    int S, T;
    Dinic dinic = 0;
    
    BOJ11375() {
        cin >> N >> M;
        S = 0; T = N + M + 1;
        dinic = T + 1;
        for (int i = 1; i <= N; ++i) {
            int n; cin >> n;
            for (int j = 0; j < n; ++j) {
                int tmp; cin >> tmp;
                dinic.push({i, tmp+N, 0, 1}, {tmp+N, i, 0, 0});
            }
        }
        for (int i = 1; i <= N; ++i)
            dinic.push({0, i, 0, 1}, {i, 0, 0, 0});
        for (int i = N+1; i <= N+M+1; ++i)
            dinic.push({i, T, 0, 1}, {T, i, 0, 0});
    }
    void solve() {
        cout << dinic.maximum_flow(S, T) << endl;
    }
};

int main(){
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    BOJ11375 p; p.solve();
    return 0;
}

