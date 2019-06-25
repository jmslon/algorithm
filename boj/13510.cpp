/*
 boj.13510.cpp
 Heavy-Light Decomposition using Maximum Segment Tree by value.
 */

#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>

#define mid ((begin+end)>>1)
#define l_node (node<<1)
#define r_node (l_node+1)

using namespace std;

struct SegmentTree {
    vector<int> tree;
    int MEANINGLESS;
    
    SegmentTree(int size) {
        tree.resize(size<<2);
    }
    
    int update(int node, int begin, int end, int pos, int val) {
        if (pos < begin || end < pos) return tree[node];
        if (begin == end) return tree[node] = val;
        int l_update = update(l_node, begin, mid, pos, val);
        int r_update = update(r_node, mid+1, end, pos, val);
        return tree[node] = whichof(l_update, r_update);
    }
    
    int query(int node, int begin, int end, int l_pos, int r_pos) {
        if (r_pos < begin || end < l_pos) return MEANINGLESS;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        int l_query = query(l_node, begin, mid, l_pos, r_pos);
        int r_query = query(r_node, mid+1, end, l_pos, r_pos);
        return whichof(l_query, r_query);
    }
    
    virtual int whichof(int l, int r) = 0;
};

struct MaxSegmentTree: public SegmentTree {
    MaxSegmentTree(int size) : SegmentTree(size) {
        MEANINGLESS = -1;
    }
    virtual int whichof(int l, int r) {
        return l > r ? l : r;
    }
};

struct Edge {
    int c, p, val;
};

struct HeavyLightDecomposition {
    MaxSegmentTree max_segment_tree = NULL;
    vector<int> cnt, p_node, h_node, f;
    // f's parameter MUST be a node NOT idx.
    // f's return would be segment tree's pos.
    vector<Edge> edges;
    vector<vector<int> > adj;
    int size = 0;
    
    HeavyLightDecomposition(int size) {
        max_segment_tree = MaxSegmentTree(size-1);
        edges.resize(size);
        cnt.resize(size+1);
        p_node.resize(size+1);
        h_node.resize(size+1);
        f.resize(size+1);
        adj.resize(size+1);
    }
    
    // build p_node, count (counting descendants including root)
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
    
    // build h_node, f
    void traverse2(int root) {
        int first = 0;
        for (auto child : adj[root])
            if (child ^ p_node[root] && cnt[first] < cnt[child])
                first = child;
        for (auto child : adj[root])
            if (child ^ p_node[root] && child ^ first)
                traverse2(child);
        
        if (!h_node[root])
            h_node[root] = root;
        if (first) {
            h_node[first] = h_node[root];
            traverse2(first);
        }
        f[root] = ++size;
    }
    
    void init() {
        for (int i = 1; i < size; ++i) {
            if (p_node[edges[i].p] == edges[i].c)
                swap(edges[i].c, edges[i].p);
            update(i, edges[i].val);
        }
    }
    
    void update(int idx, int val) {
        int node = edges[idx].c;
        max_segment_tree.update(1, 1, size-1, f[node], val);
    }
    
    int query(int s_node, int d_node) {
        int ret = max_segment_tree.MEANINGLESS;
        while (h_node[s_node] ^ h_node[d_node]) {
            if (cnt[h_node[s_node]] > cnt[h_node[d_node]])
                swap(s_node, d_node);
            ret = max(ret, max_segment_tree.query(1, 1, size-1, f[s_node], f[h_node[s_node]]));
            s_node = p_node[h_node[s_node]];
        }
        // h_node is s_node and d_node's LCA
        if (cnt[s_node] > cnt[d_node])
            swap(s_node, d_node);
        // s_node is lower level than d_node
        ret = max(ret, max_segment_tree.query(1, 1, size-1, f[s_node], f[d_node]-1));
        return ret;
    }
};

int main() {
    
    int N, M;
    scanf("%d", &N);
    HeavyLightDecomposition hld(N);
    for (int i = 1; i < N; ++i) {
        int a, b, w;
        scanf("%d%d%d", &a, &b, &w);
        hld.edges[i] = {a, b, w};
        hld.adj[a].push_back(b);
        hld.adj[b].push_back(a);
    }
    
    hld.traverse1(1);
    hld.traverse2(1);
    hld.init();
    
    scanf("%d", &M);
    for (int i = 0; i < M; ++i) {
        int cmd;
        scanf("%d", &cmd);
        if (cmd == 1) {
            int idx, val;
            scanf("%d%d", &idx, &val);
            hld.update(idx, val);
        } else if (cmd == 2) {
            int s_node, d_node;
            scanf("%d%d", &s_node, &d_node);
            printf("%d\n", hld.query(s_node, d_node));
        }
    }
    
    return 0;
}
