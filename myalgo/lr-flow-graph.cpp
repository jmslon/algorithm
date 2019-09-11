#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>

#define INF 0x6fffffffffffffff

using namespace std;

typedef long long ll;

struct LRFlowGraph {
    struct Edge {
        int src, dst; ll flw, cap;
        ll residual() {return cap-flw;}
    };
    
    int V;
    vector<vector<unsigned long>> adj;
    vector<Edge> edges;
    int NS, NT;
    
    LRFlowGraph(int size){
        V = size;
        adj.resize(size);
    }
    
    void push(Edge e1, Edge e2) {
        adj[e1.src].push_back(edges.size());
        edges.push_back(e1);
        adj[e2.src].push_back(edges.size());
        edges.push_back(e2);
    }
    
    void push(int s, int d, int l, int r) {
        
    }
    
    void flow(unsigned long idx, ll flw) {
        edges[idx].flw += flw;
        edges[idx%2?idx-1:idx+1].flw -= flw;
    }
};

struct Dinic: LRFlowGraph {
    int S, T;
    vector<ll> dist, work;
    
    Dinic(int size): LRFlowGraph(size) {}
    
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
};

struct BOJ13332 {
    int M, N, P1, P2;
    vector<int> Q1, Q2;
    struct Vacation {
        ll d, r1, r2; int node;
    };
    vector<vector<Vacation>> vacations;
    vector<int> plan_owner;
    BOJ13332() {
        cin >> M >> N >> P1 >> P2;
        Q1.resize(N+1); Q2.resize(N+1);
        for (int i = 1; i <= N; ++i)
            cin >> Q1[i] >> Q2[i];
        vacations.resize(M+1);
        plan_owner.resize(1);
        int P = 0;
        for (int i = 1; i <= M; ++i) {
            int k; cin >> k;
            for (int j = 0; j < k; ++j) {
                int d, r1, r2; cin >> d >> r1 >> r2;
                vacations[i].push_back({d, r1, r2, P++});
                plan_owner.push_back(i);
            }
        }
        Dinic dinic = M+N+P+4;
        int S = 0, T = M+N+P+1, NS = M+N+P+2, NT = M+N+P+3;
        // 1<=member<=M M+1<=plan<=M+P M+P+1<=M+P+N;
        ll demand = 0;
        dinic.push({T, S, 0, INF}, {S, T, 0, 0});
        for (int i = 1; i <= M; ++i) {
            ll l = N-P2, r = N-P1;
            dinic.push({S, NT, 0, l}, {NT, S, 0, 0});
            dinic.push({NS, i, 0, l}, {i, NS, 0, 0});
            dinic.push({S, i, 0, r-l}, {i, S, 0, 0});
            demand += l;
            for (int j = 0; j < vacations[i].size(); ++j) {
                ll l = vacations[i][j].d, r = vacations[i][j].r2-vacations[i][j].r1+1;
                int key = vacations[i][j].node;
                int plan = M+key+1;
                dinic.push({i, NT, 0, l}, {NT, i, 0, 0});
                dinic.push({NS, plan, 0, l}, {plan, NS, 0, 0});
                dinic.push({i, plan, 0, r-l}, {plan, i, 0, 0});
                demand += l;
                for (int k = vacations[i][j].r1; k <= vacations[i][j].r2; ++k)
                    dinic.push({plan, M+P+k, 0, 1}, {M+P+k, plan, 0, 0});
            }
        }
        for (int i = 1; i <= N; ++i) {
            ll l = M-Q2[i], r = M-Q1[i];
            dinic.push({M+P+i, NT, 0, l}, {NT, M+P+i, 0, 0});
            dinic.push({NS, T, 0, l}, {T, NS, 0, 0});
            dinic.push({M+P+i, T, 0, r-l}, {T, M+P+i, 0, 0});
            demand += l;
        }
        auto mf = dinic.maximum_flow(NS, NT);
        if (demand == mf) {
            cout << "1\n";
            vector<vector<int>> vacation_plans(M+1);
            for (int i = 0; i < dinic.edges.size(); ++i) {
                auto e = dinic.edges[i];
                auto plan = e.src-M;
                auto day = e.dst-M-P;
                if (1<=plan&&plan<=P) {
                    if (1<=day&&day<=N) {
                        if (e.flw > 0) {
                            vacation_plans[plan_owner[plan]].push_back(day);
                        }
                    }
                }
            }
            for (int i = 1; i <= M; ++i) {
                sort(vacation_plans[i].begin(), vacation_plans[i].end());
                cout << vacation_plans[i].size();
                for (auto t: vacation_plans[i]) {
                    cout << " " << t;
                } cout << "\n";
            }
        } else cout << "-1\n";
    }
};

int main(){
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    BOJ13332 p;
    return 0;
}
