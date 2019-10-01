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

struct DisjointSet{
    int size;
    vector<int> par;
    
    DisjointSet(int size) {
        this->size = size;
        par.resize(size);
        for(int i = 0; i < size ; ++i) par[i] = i;
    }
    
    int find(int x) {
        if (par[x] == x) return x;
        return par[x] = find(par[x]);
    }
    
    void join(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;
        if (x < y) swap(x, y);
        par[x] = y;
    }
};

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
    
    
    bool detect_cycle() {
        vector<int> inq(adj.size());
        for (int i = 0; i < adj.size(); ++i)
            if (detect_cycle(i, inq)) return 1;
        return 0;
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
        dfs_spanning_tree(root, root, inq, order, cnt, type);
        for (int i = 0; i < type.size(); ++i) {
            printf("%d->%d : %d\n", edges[i].src, edges[i].dst, type[i]);
        }
    }
    void dfs_spanning_tree(int root, int par, vector<bool> &inq,
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
            dfs_spanning_tree(dst, root, inq, order, cnt, type);
        }
        inq[root] = 0;
    }
    
    
    vector<int> topological_sort() {
        vector<int> stack;
        vector<bool> visit(adj.size());
        for (int i = 0; i < adj.size(); ++i)
            topological_sort(i, visit, stack);
        reverse(stack.begin(), stack.end());
        
        return stack;
    }
    void topological_sort(int root, vector<bool> &visit, vector<int> &stack) {
        if (visit[root]) return;
        visit[root] = 1;
        for (auto idx: adj[root])
            topological_sort(edges[idx].dst, visit, stack);
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

struct BOJ14866 { // unsolved
    BOJ14866() {
        int V, E; cin >> V >> E;
        DirectedAcyclicGraph graph = V+1;
        for (int i = 0; i < E; ++i) {
            int a, b; cin >> a >> b;
            graph.push(a, b, 1);
            graph.push(b, a, 1);
        }
        graph.dfs_spanning_tree(1);
    }
};

struct BOJ2623 {
    BOJ2623() {
        int N, M; cin >> N >> M;
        DirectedAcyclicGraph dag = N+1;
        for (int i = 0; i < M; ++i) {
            int n; cin >> n;
            int prev; cin >> prev;
            for (int j = 0; j < n-1; ++j) {
                int curr; cin >> curr;
                dag.push(prev, curr, 1);
                prev = curr;
            }
        }
        if (dag.detect_cycle()) {
            cout << "0\n";
            return;
        }
        auto sorted = dag.topological_sort(); sorted.pop_back();
        for (auto n: sorted)
            cout << n << "\n";
    }
};

struct BOJ2529 {
    BOJ2529() {
        int N; cin >> N;
        DirectedAcyclicGraph dag1 = 10, dag2 = 10;
        
        for (int i = 0; i < N; ++i) {
            char tmp; cin >> tmp;
            if (tmp == '<') {
                dag1.push(9-i, 8-i, 1);
                dag2.push(8-i, 9-i, 1);
            } else {
                dag1.push(8-i, 9-i, 1);
                dag2.push(9-i, 8-i, 1);
            }
        }
        
        auto sorted1 = dag1.topological_sort();
        auto sorted2 = dag2.topological_sort();
        vector<int> arr1, arr2;
        for (int i = 0; i <= N; ++i) {
            arr1.push_back(sorted2[i]);
            arr2.push_back(9-sorted1[i]);
        }
        for (auto n: arr1) cout << n; cout << endl;
        for (auto n: arr2) cout << n; cout << endl;
    }
};

struct UndirectedGraph {
    struct Edge {
        int src, dst; ll cost; unsigned long key;
    };
    
    vector<vector<unsigned long>> adj;
    vector<Edge> edges;
    
    UndirectedGraph(int size) {
        adj.resize(size);
    }
    
    void push(int src, int dst, ll cost) {
        push(src, dst, cost, edges.size());
    }
    
    void push(int src, int dst, ll cost, unsigned long key) {
        adj[src].push_back(key);
        adj[dst].push_back(key);
        edges.push_back({src, dst, cost, key});
    }
    
    ll query(int src, int dst) {
        for (auto idx: adj[src]) {
            auto s = edges[idx].src, d = edges[idx].dst;
            if (s != src) swap(s, d);
            if (d == dst) return edges[idx].cost;
        }
        return INF;
    }
    
    UndirectedGraph minimum_spanning_tree() {
        UndirectedGraph ret = (int)adj.size();
        DisjointSet ds = (int)adj.size();
        auto edges = this->edges;
        sort(edges.begin(), edges.end(), [](Edge l, Edge r){
            return l.cost < r.cost;
        });
        for (int i = 0; i < edges.size(); ++i) {
            int src = edges[i].src, dst = edges[i].dst;
            if (ds.find(src) != ds.find(dst)) {
                ds.join(src, dst);
                ret.push(src, dst, edges[i].cost);
            }
        }
        return ret;
    }
};

struct BOJ1197 {
    int V, E;
    BOJ1197() {
        cin >> V >> E;
        UndirectedGraph graph = V+1;
        for (int i = 0; i < E; ++i) {
            int A, B; ll C; cin >> A >> B >> C;
            graph.push(A, B, C);
            graph.push(B, A, C);
        }
        
        UndirectedGraph mst = graph.minimum_spanning_tree();
        
        ll answer = 0;
        for (auto edge: mst.edges)
            answer += edge.cost;
        
        cout << answer << endl;
    }
};

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    BOJ2529 p;
    return 0;
}
