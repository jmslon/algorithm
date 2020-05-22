#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#define MOD 1000000007
#define INF 0x6fffffff
#define lnode (node<<1)
#define rnode (lnode+1)
#define mid ((begin+end)>>1)

using namespace std;

typedef unsigned long long ull;
typedef unsigned long ul;
typedef long long ll;

struct Graph {
    struct Edge {
        ul src, dst; ll cost;
    };
    int size = 0;
    vector<ul> c, h, p, f;
    vector<Edge> edges;
    vector<vector<ul>> adj;
    void push(ul src, ul dst, ll cost) {
        adj[src].push_back(edges.size());
        edges.push_back({src, dst, cost});
    }
    
    Graph(ul size) {
        adj.resize(size);
        c.resize(size);
        h.resize(size);
        p.resize(size);
        f.resize(size);
    }
    
    void init(ul root) {
        traverse1(root);
        traverse2(root);
    }
    
    ul lca(ul s, ul d) {
        if (f[s] > f[d]) swap(s, d);
        if (h[s] == h[d]) return d;
        return lca(p[h[s]], d);
    }
    
protected:
    ul traverse1(ul root) {
        c[root] = 1;
        for (ul idx : adj[root]) {
            auto edge = edges[idx];
            ul child = edge.dst;
            if (child ^ p[root]) {
                p[child] = root;
                c[root] += traverse1(child);
            }
        }
        return c[root];
    }
    
    void traverse2(ul root) {
        ul first = 0;
        for (ul idx : adj[root]) {
            auto edge = edges[idx];
            ul child = edge.dst;
            if (child ^ p[root]&& c[first] < c[child])
                first = child;
        }
        for (ul idx : adj[root]) {
            auto edge = edges[idx];
            ul child = edge.dst;
            if (child ^ p[root] && child ^ first)
                traverse2(child);
        }
        if (!h[root])
            h[root] = root;
        if (first) {
            h[first] = h[root];
            traverse2(first);
        }
        f[root] = size++;
    }
};

struct BOJ11438 {
    BOJ11438() {
        ul V; cin >> V;
        Graph hld(V+1);
        for (int i = 0; i < V-1; ++i) {
            ul src, dst;
            cin >> src >> dst;
            hld.push(src, dst, 1);
            hld.push(dst, src, 1);
        }
        hld.init(1);
        ul M; cin >> M;
        for (int i = 0; i < M; ++i) {
            ul src, dst;
            cin >> src >> dst;
            cout << hld.lca(dst, src) << "\n";
        }
    }
};




struct SumSegmentTree: Graph { // Heavy-Light Decomidxition
    vector<ll> tree;
    
    SumSegmentTree(ul size): Graph(size) {
        tree.resize(size<<2);
    }
    
    void init(ul root) {
        traverse1(root);
        traverse2(root);
        for (auto edge: edges) {
            if (p[edge.dst] == edge.src) swap(edge.dst, edge.src);
            update(1, 0, size-2, f[edge.src], edge.cost);
        }
    }
    
    ll query(ul s, ul d) {
        if (f[s] > f[d]) swap(s, d);
        if (h[s] == h[d]) return query(1, 0, size-2, f[s], f[d]-1);
        return query(1, 0, size-2, f[s], f[h[s]]) + query(p[h[s]], d);
    }
    
private:
    ll update(ul node, ul begin, ul end, ul idx, ll val) {
        if (idx < begin || end < idx) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l = update(lnode, begin, mid, idx, val);
        ll r = update(rnode, mid+1, end, idx, val);
        return tree[node] = l + r;
    }
    
    ll query(ul node, ul begin, ul end, ul lidx, ul ridx) {
        if (ridx < begin || end < lidx) return 0;
        if (lidx <= begin && end <= ridx) return tree[node];
        ll l = query(lnode, begin, mid, lidx, ridx);
        ll r = query(rnode, mid+1, end, lidx, ridx);
        return l + r;
    }
};

struct BOJ1761 {
    BOJ1761() {
        ul V; cin >> V;
        SumSegmentTree hld(V+1);
        for (int i = 0; i < V-1; ++i) {
            ul src, dst; ll cost;
            cin >> src >> dst >> cost;
            hld.push(src, dst, cost);
            hld.push(dst, src, cost);
        }
        hld.init(1);
        ul M; cin >> M;
        for (int i = 0; i < M; ++i) {
            ul s, d;
            cin >> s >> d;
            cout << hld.query(s, d) << "\n";
        }
    }
};




struct MaxSegmentTree: Graph { // Heavy-Light Decomidxition
    vector<ll> tree;
    MaxSegmentTree(ul size): Graph(size) {
        tree.resize(size<<2);
    }
    
    void init(ul root) {
        traverse1(root);
        traverse2(root);
        for (auto edge: edges) {
            ul src = edge.src, dst = edge.dst; ll cost = edge.cost;
            if (p[dst] == src) swap(dst, src);
            update(1, 0, size-2, f[src], cost);
        }
    }
    
    void update(ul s, ll val) {
        ul src = edges[(s-1)<<1].src, dst = edges[(s-1)<<1].dst;
        if (p[dst] == src) swap(src, dst);
        update(1, 0, size-2, f[src], val);
    }
    
    ll query(ul s, ul d) {
        if (f[s] > f[d]) swap(s, d);
        if (h[s] == h[d]) return query(1, 0, size-2, f[s], f[d]-1);
        ll first = query(1, 0, size-2, f[s], f[h[s]]);
        ll second = query(p[h[s]], d);
        return first > second ? first : second;
    }
    
private:
    ll update(ul node, ul begin, ul end, ul idx, ll val) {
        if (idx < begin || end < idx) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l = update(lnode, begin, mid, idx, val);
        ll r = update(rnode, mid+1, end, idx, val);
        return tree[node] = l > r ? l : r;
    }
    
    ll query(ul node, ul begin, ul end, ul lidx, ul ridx) {
        if (ridx < begin || end < lidx) return 0;
        if (lidx <= begin && end <= ridx) return tree[node];
        ll l = query(lnode, begin, mid, lidx, ridx);
        ll r = query(rnode, mid+1, end, lidx, ridx);
        return l > r ? l : r;
    }
};

struct BOJ13510 {
    BOJ13510() {
        ul V; cin >> V;
        MaxSegmentTree hld(V+1);
        for (int i = 0; i < V-1; ++i) {
            ul src, dst; ll cost;
            cin >> src >> dst >> cost;
            hld.push(src, dst, cost);
            hld.push(dst, src, cost);
        }
        hld.init((V+1)/2);
        ul M; cin >> M;
        for (int i = 0; i < M; ++i) {
            int n;
            cin >> n;
            if (n == 1) {
                ul s; ll val;
                cin >> s >> val;
                hld.update(s, val);
            } else if (n == 2) {
                ul s, d;
                cin >> s >> d;
                cout << hld.query(s, d) << "\n";
            }
        }
    }
};




struct MinSegmentTree: Graph { // Heavy-Light Decomidxition
    vector<ll> tree;
    
    MinSegmentTree(ul size): Graph(size) {
        tree.resize(size<<2, 1);
    }
    
    void init(ul root) {
        traverse1(root);
        traverse2(root);
    }
    
    void update(ul src, ll val) {
        update(1, 0, size-2, f[src], val);
    }
    
    ll query(ul s, ul d) {
        if (f[s] > f[d]) swap(s, d);
        if (h[s] == h[d]) return query(1, 0, size-2, f[s], f[d]-1);
        ll first = query(1, 0, size-2, f[s], f[h[s]]);
        ll second = query(p[h[s]], d);
        return first < second ? first : second;
    }
    
private:
    ll update(ul node, ul begin, ul end, ul idx, ll val) {
        if (idx < begin || end < idx) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l = update(lnode, begin, mid, idx, val);
        ll r = update(rnode, mid+1, end, idx, val);
        return tree[node] = l < r ? l : r;
    }
    
    ll query(ul node, ul begin, ul end, ul lidx, ul ridx) {
        if (ridx < begin || end < lidx) return INF;
        if (lidx <= begin && end <= ridx) return tree[node];
        ll l = query(lnode, begin, mid, lidx, ridx);
        ll r = query(rnode, mid+1, end, lidx, ridx);
        return l < r ? l : r;
    }
};

struct BOJ13309 {
    BOJ13309() {
        ul V; int Q; cin >> V >> Q;
        MinSegmentTree hld(V+1);
        for (int i = 1; i <= V-1; ++i) {
            ul a; cin >> a;
            hld.push(a, i+1, -1);
            hld.push(i+1, a, -1);
        }
        hld.init(1);
        for (int i = 0; i < Q; ++i) {
            int b, c, d; cin >> b >> c >> d;
            if (d == 0) {
                if (hld.query(b, c)) {
                    cout << "YES\n";
                } else {
                    cout << "NO\n";
                }
            } else if (d == 1) {
                if (hld.query(b, c)) {
                    cout << "YES\n";
                    hld.update(b, 0);
                } else {
                    cout << "NO\n";
                    hld.update(c, 0);
                }
            }
        }
    }
};




struct SumLazySegmentTree: Graph { // Heavy-Light Decomidxition
    vector<ll> tree, lazy;
    SumLazySegmentTree(ul size): Graph(size) {
        tree.resize(size<<2);
        lazy.resize(size<<2);
    }
    
    void init(ul root) {
        traverse1(root);
        traverse2(root);
    }
    
    void update(ul s, ul d, ll dif) {
        if (f[s] > f[d]) swap(s, d);
        if (h[s] == h[d]) update(1, 0, size-1, f[s], f[d], dif);
        else {
            update(s, h[s], dif);
            update(p[h[s]], d, dif);
        }
    }
    
    ll query(ul s, ul d) {
        if (f[s] > f[d]) swap(s, d);
        if (h[s] == h[d]) return query(1, 0, size-1, f[s], f[d]);
        ll first = query(s, h[s]);
        ll second = query(p[h[s]], d);
        return (first + second) % MOD;
    }
    
private:
    ll update(ul node, ul begin, ul end, ul lidx, ul ridx, ll dif) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (ridx < begin || end < lidx) return tree[node];
        if (lidx <= begin && end <= ridx) {
            propagate(node, begin, end, dif);
            return tree[node];
        }
        ll l = update(lnode, begin, mid, lidx, ridx, dif);
        ll r = update(rnode, mid+1, end, lidx, ridx, dif);
        return tree[node] = (l + r) % MOD;
    }
    
    ll query(ul node, ul begin, ul end, ul lidx, ul ridx) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (ridx < begin || end < lidx) return 0;
        if (lidx <= begin && end <= ridx) return tree[node];
        ll l = query(lnode, begin, mid, lidx, ridx);
        ll r = query(rnode, mid+1, end, lidx, ridx);
        return (l + r) % MOD;
    }
    
    void propagate(ul node, ul begin, ul end, ll dif) {
        if (dif == 0) return;
        tree[node] += (end + 1 - begin) * dif;
        if (begin ^ end) {
            lazy[lnode] += dif;
            lazy[rnode] += dif;
        }
    }
};

struct BOJ15899 {
    BOJ15899() {
        int N, M, C; cin >> N >> M >> C;
        SumLazySegmentTree hld(N+1);
        vector<pair<int, ul>> colors(N+1), queries(M);
        for (int i = 1; i <= N; ++i) {
            cin >> colors[i].first;
            colors[i].second = i;
        }
        for (int i = 0; i < N-1; ++i) {
            ul s, d; cin >> s >> d;
            hld.push(s, d, -1);
            hld.push(d, s, -1);
        }
        hld.init(1);
        for (int i = 0; i < M; ++i)
            cin >> queries[i].second >> queries[i].first;
        
        sort(colors.begin(), colors.end());
        sort(queries.begin(), queries.end());
        
        ll answer = 0;
        for (int color = 1, q = 0, c = 1; color <= C; ++color) {
            for (;c <= N && colors[c].first == color; ++c)
                hld.update(1, colors[c].second, 1);
            for (;q < M && queries[q].first == color; ++q)
                answer = (answer + hld.query(queries[q].second, queries[q].second)) % MOD;
        }
        cout << answer << "\n";
    }
};


int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    BOJ15899 solve;
    return 0;
}
