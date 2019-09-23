/*
 Heavy-Light Decomposition
 */

#include <cstdio>
#include <vector>
#include <algorithm>

#define l_node  (node<<1)
#define r_node  ((l_node)+1)
#define mid     ((begin+end)>>1)

typedef long long ll;

using namespace std;

struct Graph {
    vector<vector<int>> adj;
    void push(int src, int dst) {
        adj[src].push_back(dst);
    }
};

struct HeavyLightDecomposition: Graph { // Min
    int size = 0;
    vector<int> c, h, p, f;
    vector<ll> tree;
    
    HeavyLightDecomposition(int size) {
        c.resize(size, 1);
        h.resize(size);
        p.resize(size);
        f.resize(size);
        adj.resize(size);
    }
    
    void init(int root) { // only lca version
        traverse1(root);
        traverse2(root);
    }
    
    void init(int root, ll val) { // segment tree version
        traverse1(root);
        traverse2(root);
        tree.resize(size<<2, val);
    }
    
    void update(int pos, ll val) {
        update(1, 0, size-2, f[pos], val);
    }
    
    ll query(int l, int r) {
        if (f[l] > f[r]) swap (l, r);
        if (h[l] == h[r]) return query(1, 0, size-2, f[l], f[r]-1);
        if (!query(1, 0, size-2, f[l], f[h[l]])) return 0;
        return query(p[h[l]], r);
    }
    
    int lca(int l, int r) {
        if (f[l] > f[r]) swap(l, r);
        if (h[l] == h[r]) return r;
        return lca(p[h[l]], r);
    }
    
private:
    
    int traverse1(int root) {
        c[root] = 1;
        for (int child : adj[root]) {
            if (child ^ p[root]) {
                p[child] = root;
                c[root] += traverse1(child);
            }
        }
        return c[root];
    }
    
    void traverse2(int root) {
        int first = 0;
        for (int child : adj[root])
            if (c[first]<c[child]&&child^p[root])
                first = child;
        for (int child : adj[root])
            if (child^first&&child^p[root])
                traverse2(child);
        if (!h[root])
            h[root] = root;
        if (first) {
            h[first] = h[root];
            traverse2(first);
        }
        f[root] = size++;
    }
    
    ll update(int node, int begin, int end, int pos, ll val) {
        if (pos < begin || end < pos) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l = update(l_node, begin, mid, pos, val);
        ll r = update(r_node, mid+1, end, pos, val);
        return tree[node] = l<r?l:r;
    }
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        if (r_pos < begin || end < l_pos) return INF;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l = query(l_node, begin, mid, l_pos, r_pos);
        ll r = query(r_node, mid+1, end, l_pos, r_pos);
        return l<r?l:r;
    }
};

struct BOJ11438 {
    BOJ11438() {
        int N, K; cin >> N;
        HeavyLightDecomposition hld(N+1);
        for (int i = 0; i < N-1; ++i) {
            int a, b; cin >> a >> b;
            hld.push(a, b);
            hld.push(b, a);
        }
        
        hld.init(1); // root
        
        cin >> K;
        for (;K--;) {
            int s, d; cin >> s >> d;
            cout << hld.lca(s, d) << "\n";
        }
    }
};

struct BOJ13309 {
    BOJ13309() {
        int N, Q; cin >> N >> Q;
        HeavyLightDecomposition hld = N+1;
        for (int i = 1; i < N; ++i) {
            int a; cin >> a;
            hld.p[i+1] = a;
            hld.adj[a].push_back(i+1);
        }
        
        hld.init(1, 1); // root, init_val
        
        for (;Q--;) {
            int b, c, d; cin >> b >> c >> d;
            if (d == 0) {
                if (hld.query(b, c)) cout << "YES\n";
                else cout << "NO\n";
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


struct Graph {
    struct Edge {
        int src, dst; ll w;
    };
    vector<Edge> edges;
    vector<vector<int>> adj;
    void push(int src, int dst, ll cst) {
        adj[src].push_back(dst);
        adj[dst].push_back(src);
        edges.push_back({src, dst, cst});
    }
};

struct HeavyLightDecomposition: Graph { // Sum
    int size = 0;
    vector<int> c, h, p, f;
    vector<ll> tree;
    
    HeavyLightDecomposition(int size) {
        c.resize(size, 1);
        h.resize(size);
        p.resize(size);
        f.resize(size);
        adj.resize(size);
    }
    
    void init(int root) {
        traverse1(root);
        traverse2(root);
        tree.resize(size<<2);
        for (auto edge: edges) {
            if (p[edge.dst] == edge.src) update(1, 0, size-2, f[edge.dst], edge.w);
            else update(1, 0, size-2, f[edge.src], edge.w);
        }
    }
    
    void update(int pos, ll val) {
        update(1, 0, size-2, f[pos], val);
    }
    
    ll query(int s, int d) {
        if (f[s] > f[d]) swap(s, d);
        if (h[s] == h[d]) return query(1, 0, size-2, f[s], f[d]-1);
        return query(1, 0, size-2, f[s], f[h[s]])+query(p[h[s]], d);
    }
    
private:
    
    int traverse1(int root) {
        c[root] = 1;
        for (int child : adj[root]) {
            if (child ^ p[root]) {
                p[child] = root;
                c[root] += traverse1(child);
            }
        }
        return c[root];
    }
    
    void traverse2(int root) {
        int first = 0;
        for (int child : adj[root])
            if (child ^ p[root]&& c[first] < c[child])
                first = child;
        for (int child : adj[root])
            if (child ^ p[root] && child ^ first)
                traverse2(child);
        if (!h[root])
            h[root] = root;
        if (first) {
            h[first] = h[root];
            traverse2(first);
        }
        f[root] = size++;
    }
    
    ll update(int node, int begin, int end, int pos, ll val) {
        if (pos < begin || end < pos) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l = update(l_node, begin, mid, pos, val);
        ll r = update(r_node, mid+1, end, pos, val);
        return tree[node] = l+r;
    }
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        if (r_pos < begin || end < l_pos) return 0;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l = query(l_node, begin, mid, l_pos, r_pos);
        ll r = query(r_node, mid+1, end, l_pos, r_pos);
        return l+r;
    }
};

struct BOJ1761 {
    BOJ1761() {
        int N, M; cin >> N;
        HeavyLightDecomposition hld = N+1;
        for (int i = 1; i < N; ++i) {
            int u, v; ll w; cin >> u >> v >> w;
            hld.push(u, v, w);
        }
        
        hld.init(1);
        
        cin >> M;
        for (;M--;) {
            int s, d; cin >> s >> d;
            cout << hld.query(s, d) << "\n";
        }
    }
};
