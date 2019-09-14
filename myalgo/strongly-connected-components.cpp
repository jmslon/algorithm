#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

typedef long long ll;

struct Graph {
    struct Edge {
        int src, dst; unsigned long key;
    };
    
    int V;
    vector<vector<unsigned long>> adj;
    vector<Edge> edges;
    
    Graph(int size) {
        V = size;
        adj.resize(size);
    }
    
    void push(int src, int dst) {
        push(src, dst, edges.size());
    }
    
    void push(int src, int dst, unsigned long key) {
        adj[src].push_back(key);
        edges.push_back({src, dst, key});
    }
};

struct StronglyConnectedComponent: Graph {
    vector<int> f, g, stack, g_id;
    vector<bool> visit;
    vector<vector<int>> SCC;
    
    StronglyConnectedComponent(int size): Graph(size) {
        visit.resize(V);
        f.resize(V, -1);
        g_id.resize(V);
    }
    
    auto strongly_connected_component() {
        for (int i = 1; i < V; ++i) if (f[i] == -1)
            dfs(i);
    }
    
    int dfs(int src) {
        if (f[src] != -1) return f[src];
        
        f[src] = (int)g.size();
        g.push_back(src);
        stack.push_back(src);
        int ret = f[src];
        for (auto idx: adj[src]) {
            auto dst = edges[idx].dst;
            if (!visit[dst])
                ret = min(ret, dfs(dst));
        }
        
        if (ret == f[src]) {
            vector<int> scc;
            while (1) {
                auto t = stack.back(); stack.pop_back();
                visit[t] = 1;
                scc.push_back(t);
                g_id[t] = (int) SCC.size();
                if (t == src) break;
            }
            SCC.push_back(scc);
        }
        return ret;
    }
};

struct BOJ11280 {
    int N, M;
    BOJ11280() {
        cin >> N >> M;
        StronglyConnectedComponent graph = 2*(N+1);
        for (int i = 0; i < M; ++i) {
            int u, v; cin >> u >> v;
            if (u < 0) u = N-u;
            if (v < 0) v = N-v;
            graph.push(f(u), v);
            graph.push(f(v), u);
        }
        graph.strongly_connected_component();
        for (int i = 1; i <= N; ++i) {
            if (graph.g_id[i] == graph.g_id[i+N]) {
                printf("0\n");
                return;
            }
        }
        printf("1\n");
    }
    inline int f(int u) {return u > N ? u-N : u+N;}
};

struct BOJ10451 {
    int N;
    BOJ10451() {
        cin >> N;
        StronglyConnectedComponent graph = N+1;
        for (int i = 1; i <= N ;++i) {
            int tmp; cin >> tmp;
            graph.push(i, tmp);
        }
        graph.strongly_connected_component();
        cout << graph.SCC.size() << "\n";
    }
};

struct BOJ2150 {
    BOJ2150() {
        int V, E; cin >> V >> E;
        StronglyConnectedComponent scc = V+1;
        for (int i = 0; i < E; ++i) {
            int a, b; cin >> a >> b;
            scc.push(a, b);
        }
        auto components = scc.strongly_connected_component();
        for (auto &c: components)
            sort(c.begin(), c.end());
        sort(components.begin(), components.end());
        cout << components.size() << endl;
        for (auto c: components) {
            for (auto n: c)
                cout << n << " ";
            cout << "-1\n";
        }
        
    }
};

int main(){
    BOJ2150 p;
}
