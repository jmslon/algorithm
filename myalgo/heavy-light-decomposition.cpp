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

struct SegmentTree {
    vector<ll> tree;
    ll update(int node, int begin, int end, int pos, ll val) {
        if (pos < begin || end < pos) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l_update = update(l_node, begin, mid, pos, val);
        ll r_update = update(r_node, mid+1, end, pos, val);
        return tree[node] = l_update + r_update;
    }
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        if (r_pos < begin || end < l_pos) return 0;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l_query = query(l_node, begin, mid, l_pos, r_pos);
        ll r_query = query(r_node, mid+1, end, l_pos, r_pos);
        return l_query + r_query;
    }
};

struct Edge {
    int c, p, w;
    bool operator < (const Edge &a) const {
        return c < a.c;
    }
};

struct HLD {
    int size = 0;
    vector<Edge> e;
    vector<int> c, h, p, f;
    vector<vector<int> > adj;
    SegmentTree st;
    
    HLD(int size) {
        // size == N+1
        // this->size == N
        e.resize(size);
        c.resize(size);
        h.resize(size);
        p.resize(size);
        f.resize(size);
        adj.resize(size);
        st.tree.resize(size<<2);
    }
    
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
        f[root] = ++size;
    }
    
    void init() {
        for (int i = 1; i < size; ++i) {
            if (p[e[i].p] == e[i].c) swap(e[i].c, e[i].p);
            st.update(1, 1, size-1, f[e[i].c], e[i].w);
        }
        sort(e.begin(), e.end());
    }
    
    ll query(int s, int d) {
        if (f[s] > f[d]) swap(s, d);
        if (h[s] == h[d]) return st.query(1, 1, size-1, f[s], f[d]-1);
        return query(s, h[s]) + query(p[h[s]], d) + e[h[s]].w;
    }
    
    int lca(int s, int d) {
        if (f[s] > f[d]) swap(s, d);
        if (h[s] == h[d]) return d;
        return lca(p[h[s]], d);
    }
};


struct Graph {
    vector<vector<int>> adj;
    void push(int src, int dst) {
        adj[src].push_back(dst);
    }
};

struct HeavyLightDecomposition: Graph {
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
    
    void init(int root, ll val) {
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


// only lca version
struct Graph {
    vector<vector<int>> adj;
    void push(int src, int dst) {
        adj[src].push_back(dst);
    }
};

struct HeavyLightDecomposition: Graph { 
    int size = 0;
    vector<int> c, h, p, f;
    
    HeavyLightDecomposition(int size) {
        c.resize(size);
        h.resize(size);
        p.resize(size);
        f.resize(size);
        adj.resize(size);
    }
    
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
        for (int child : adj[root]) if (child^p[root]&&c[first]<c[child])
            first = child;
        for (int child : adj[root]) if (child^p[root]&&child^first)
            traverse2(child);
        if (!h[root]) h[root] = root;
        if (first) {
            h[first] = h[root];
            traverse2(first);
        }
        f[root] = size++;
    }
    
    int lca(int l, int r) {
        if (f[l] > f[r]) swap(l, r);
        if (h[l] == h[r]) return r;
        return lca(p[h[l]], r);
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
        hld.traverse1(1);
        hld.traverse2(1);
        cin >> K;
        for (;K--;) {
            int s, d; cin >> s >> d;
            cout << hld.lca(s, d) << "\n";
        }
    }
};
