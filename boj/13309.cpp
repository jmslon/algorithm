/*
 boj.13309.cpp
 Heavy-Light Decomposition using Min Segment Tree.
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
    
    SegmentTree(int size) {
        tree.resize(size<<2, 1);
    }
    
    ll update(int node, int begin, int end, int pos, ll val) {
        if (pos < begin || end < pos) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l_update = update(l_node, begin, mid, pos, val);
        ll r_update = update(r_node, mid+1, end, pos, val);
        return tree[node] = l_update < r_update ? l_update : r_update;
    }
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        if (r_pos < begin || end < l_pos) return 0x7fffffffffffffff;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l_query = query(l_node, begin, mid, l_pos, r_pos);
        ll r_query = query(r_node, mid+1, end, l_pos, r_pos);
        return l_query < r_query ? l_query : r_query;
    }
};

struct HLD {
    int size = 0;
    vector<int> c, h, p, f;
    vector<vector<int> > adj;
    SegmentTree st;
    
    HLD(int size) : st(size-1) {
        // size == N+1
        // this->size == N
        c.resize(size);
        h.resize(size);
        p.resize(size);
        f.resize(size);
        adj.resize(size);
    }
    
    int traverse1(int root) {
        c[root] = 1;
        for (int child : adj[root])
            c[root] += traverse1(child);
        return c[root];
    }
    
    void traverse2(int root) {
        int first = 0;
        for (int child : adj[root])
            if (c[first] < c[child])
                first = child;
        for (int child : adj[root])
            if (child ^ first)
                traverse2(child);
        if (!h[root])
            h[root] = root;
        if (first) {
            h[first] = h[root];
            traverse2(first);
        }
        f[root] = ++size;
    }
    
    void update(int node, ll val) {
        st.update(1, 1, size-1, f[node], val);
    }
    
    ll query(int s, int d) {
        if (h[s] == h[d]) {
            if (f[s] > f[d]) swap(s, d);
            return st.query(1, 1, size-1, f[s], f[d]-1);
        }
        if (c[h[s]] > c[h[d]]) swap(s, d);
        return min(min(query(s, h[s]), st.query(1, 1, size-1, f[h[s]], f[h[s]])), query(p[h[s]], d));
    }
};

int main(int argc, const char * argv[]) {
    int N, Q;
    scanf("%d%d", &N, &Q);
    HLD hld(N+1);
    for (int i = 1; i < N; ++i) {
        int a;
        scanf("%d", &a);
        hld.p[i+1] = a;
        hld.adj[a].push_back(i+1);
    }
    
    hld.traverse1(1);
    hld.traverse2(1);
    
    for (; Q--;) {
        int b, c, d;
        scanf("%d%d%d", &b, &c, &d);
        if (d == 0) {
            if (hld.query(b, c)) printf("YES\n");
            else printf("NO\n");
        } else if (d == 1) {
            if (hld.query(b, c)) {
                printf("YES\n");
                hld.update(b, 0);
            } else {
                printf("NO\n");
                hld.update(c, 0);
            }
        }
    }
    return 0;
}
