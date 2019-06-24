/*
 boj.15899.cpp
 Heavy-Light Decomposition using Sum Segment Tree by value with Lazy Propagation
*/

#include <cstdio>
#include <vector>
#include <algorithm>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

#define MOD 1000000007

using namespace std;

struct SegmentTree {
    vector<int> tree, lazy;
    int MEANINGLESS;
    
    SegmentTree(int size) {
        tree.resize(size<<2);
        lazy.resize(size<<2);
    }
    
    int update_range(int node, int begin, int end, int l_pos, int r_pos, int diff) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = MEANINGLESS;
        if (r_pos < begin || end < l_pos) return tree[node];
        if (l_pos <= begin && end <= r_pos) {
            propagate(node, begin, end, diff);
            return tree[node];
        }
        int l_update = update_range(l_node, begin, mid, l_pos, r_pos, diff);
        int r_update = update_range(r_node, mid+1, end, l_pos, r_pos, diff);
        return tree[node] = whichof(l_update, r_update);
    }
    
    int query(int node, int begin, int end, int l_pos, int r_pos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = MEANINGLESS;
        if (r_pos < begin || end < l_pos) return MEANINGLESS;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        int l_query = query(l_node, begin, mid, l_pos, r_pos);
        int r_query = query(r_node, mid+1, end, l_pos, r_pos);
        return whichof(l_query, r_query);
    }
    
    virtual int whichof(int l, int r) = 0;
    virtual void propagate(int node, int begin, int end, int diff) = 0;
};

struct SumSegmentTree : public SegmentTree {
    SumSegmentTree(int size): SegmentTree(size) {
        MEANINGLESS = 0;
    }
    
    virtual int whichof(int l, int r) {
        return (l + r) % MOD;
    }
    
    virtual void propagate(int node, int begin, int end, int dif) {
        tree[node] += (end - begin +1) * dif;
        if (begin ^ end) {
            lazy[l_node] += dif;
            lazy[r_node] += dif;
        }
    }
};

struct HeavyLightDecomposition {
    SumSegmentTree segment_tree = NULL;
    vector<int> h_node, p_node, f, cnt;
    vector<vector<int> > adj;
    int size = 0;
    
    HeavyLightDecomposition(int size) {
        segment_tree = SumSegmentTree(size);
        h_node.resize(size+1);
        p_node.resize(size+1);
        f.resize(size+1);
        cnt.resize(size+1);
        adj.resize(size+1);
    }
    
    int traverse1(int root) {
        cnt[root] = 1;
        for (auto child : adj[root]) {
            if (child ^ p_node[root]) {
                p_node[child] = root;
                cnt[root] += traverse1(child);
            }
        }
        return cnt[root];
    }
    
    void traverse2(int root) {
        int first = 0;
        for (auto child : adj[root])
            if (child ^ p_node[root] && cnt[first] < cnt[child])
                first = child;
        for (auto child : adj[root])
            if (child ^ p_node[root] && child ^ first)
                traverse2(child);
        if (h_node[root] == 0)
            h_node[root] = root;
        if (first) {
            h_node[first] = h_node[root];
            traverse2(first);
        }
        f[root] = ++size;
    }
    
    void update_range(int s_node, int d_node, int dif) {
        while (h_node[s_node] ^ h_node[d_node]) {
            if (cnt[h_node[s_node]] > cnt[h_node[d_node]]) swap(s_node, d_node);
            segment_tree.update_range(1, 1, size, f[s_node], f[h_node[s_node]], dif);
            s_node = p_node[h_node[s_node]];
        }
        if (f[s_node] > f[d_node]) swap(s_node, d_node);
        segment_tree.update_range(1, 1, size, f[s_node], f[d_node], dif);
    }
    
    int query(int s_node, int d_node) {
        int ret = segment_tree.MEANINGLESS;
        while (h_node[s_node] ^ h_node[d_node]) {
            if (cnt[h_node[s_node]] > cnt[h_node[d_node]]) swap(s_node, d_node);
            ret += segment_tree.query(1, 1, size, f[s_node], f[h_node[s_node]]);
            ret %= MOD;
            s_node = p_node[h_node[s_node]];
        }
        if (f[s_node] > f[d_node]) swap(s_node, d_node);
        ret += segment_tree.query(1, 1, size, f[s_node], f[d_node]);
        return ret % MOD;
    }
};

struct Color {
    int v, c;
    bool operator < (const Color &a) const {
        return c < a.c;
    }
};

struct Query {
    int v, c;
    bool operator < (const Query &a) const {
        return c < a.c;
    }
};

int main() {
    
    int N, M, C;
    scanf("%d%d%d", &N, &M, &C);
    HeavyLightDecomposition hld(N);
    
    vector<Color> colors(N+1);
    for (int i = 1; i <= N; ++i) {
        int c;
        scanf("%d", &c);
        colors[i] = {i, c};
    }
    
    for (int i = 0; i < N-1; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        hld.adj[a].push_back(b);
        hld.adj[b].push_back(a);
    }
    
    hld.traverse1(1);
    hld.traverse2(1);
    
    vector<Query> queries(M);
    for (int i = 0; i < M; ++i) {
        int v, c;
        scanf("%d%d", &v, &c);
        queries[i] = {v, c};
    }
    
    sort(colors.begin(), colors.end());
    sort(queries.begin(), queries.end());
    
    int result = 0;
    int q_idx = 0, c_idx = 1;
    for (int c = 1; c <= C; ++c) {
        for (; c_idx <= N && colors[c_idx].c == c; ++c_idx) {
            hld.update_range(colors[c_idx].v, 1, 1);
        }
        for (; q_idx < M && queries[q_idx].c == c; ++q_idx) {
            result += hld.query(queries[q_idx].v, queries[q_idx].v);
            result %= MOD;
        }
    }
    printf("%d\n", result);
    
    
    return 0;
}
