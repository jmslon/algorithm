#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <cmath>
#include <string>
#include <iostream>
#include <unordered_map>

#define INF 0x7fffffff

using namespace std;

typedef long long ll;

struct DirectedAcyclicGraph {
    struct Edge {
        int src, dst; ll cost; unsigned long key;
    };
    
    vector<vector<unsigned long>> adj, radj;
    vector<Edge> edges;
    
    DirectedAcyclicGraph(int size) {
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
    
    bool detect_cycle(int root, vector<int> &inq) {
        if (inq[root] > 0) return 1;
        if (inq[root] < 0) return 0;
        
        inq[root] = 1;
        for (auto idx: adj[root])
            if (detect_cycle(edges[idx].dst, inq)) return 1;
        inq[root] = -1;
        return 0;
    }
    
    
    void dfs_spanning_tree(int root) {
        vector<bool> inq(adj.size());
        vector<int> order(adj.size(), INF);
        int cnt = 0;
        vector<int> type(edges.size());
        dfs(root, root, inq, order, cnt, type);
        for (int i = 0; i < type.size(); ++i) {
            printf("%d->%d : %d\n", edges[i].src, edges[i].dst, type[i]);
        }
    }
    void dfs(int root, int par, vector<bool> &inq,
             vector<int> &order, int &cnt, vector<int> &type) {
        if (order[root] != INF) return;
        inq[root] = 1; order[root] = ++cnt;
        for (auto idx: adj[root]) {
            auto dst = edges[idx].dst;
            if (order[dst] == INF) { // tree edge
                type[idx] = 1;
            } else if (inq[dst]) { // back edge
                type[idx] = 2; // never if directed acyclic graph
            } else if (order[root] < order[dst]) { // forward edge
                type[idx] = 3; // meaningless if undirected graph
            } else {
                type[idx] = 4; // never if undirected graph
            }
            dfs(dst, root, inq, order, cnt, type);
        }
        inq[root] = 0;
    }
    
    
    vector<int> topological_sort() {
        vector<int> stack;
        vector<bool> visit(adj.size());
        for (int i = 0; i < adj.size(); ++i)
            dfs(i, visit, stack);
        reverse(stack.begin(), stack.end());
        
        return stack;
    }
    void dfs(int root, vector<bool> &visit, vector<int> &stack) {
        if (visit[root]) return;
        visit[root] = 1;
        for (auto idx: adj[root])
            dfs(edges[idx].dst, visit, stack);
        stack.push_back(root);
    }
};

struct BOJ2252 {
    BOJ2252() {
        int V, E; cin >> V >> E;
        DirectedAcyclicGraph dag = V+1;
        for (int i = 0; i < E; ++i) {
            int a, b; cin >> a >> b;
            dag.push(a, b, 1);
        }
        auto sorted = dag.topological_sort();
        for (int i = 0; i < sorted.size()-1; ++i)
            cout << sorted[i] << " ";
        cout << endl;
    }
};

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    BOJ2252 p;
    return 0;
}
