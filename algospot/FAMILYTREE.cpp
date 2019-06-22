/*
 algospot.FAMILYTREE.cpp
 Lowest Common Ancestor using Heavy-Light Decomsition
*/

#include <cstdio>
#include <vector>

using namespace std;

struct HeavyLightDecomposition {
    vector<int> cnt, depth, h_node, p_node, f;
    vector<vector<int> > adj;
    int size = 0;
    HeavyLightDecomposition(int size) {
        cnt.resize(size);
        depth.resize(size);
        h_node.resize(size,-1);
        p_node.resize(size);
        f.resize(size);
        adj.resize(size);
    }
    
    // build cnt (counting descendants including root), depth
    int traverse1(int root) {
        cnt[root] = 1;
        for (auto child : adj[root]) {
            depth[child] = depth[root] + 1;
            cnt[root] += traverse1(child);
        }
        return cnt[root];
    }
    
    // build h_node, f, size
    void traverse2(int root) {
        int first = -1;
        for (auto child : adj[root]) {
            if (first < 0) first = child;
            else if (cnt[child] > cnt[first])
                first = child;
        }
        for (auto child : adj[root]) {
            if (child ^ first)
                traverse2(child);
        }
        if (h_node[root] == -1)
            h_node[root] = root;
        if (first != -1) {
            h_node[first] = h_node[root];
            traverse2(first);
        }
        f[root] = ++size;
    }
    
    int lca(int s_node, int d_node) {
        while (h_node[s_node] ^ h_node[d_node]) {
            if (cnt[h_node[s_node]] > cnt[h_node[d_node]])
                swap(s_node, d_node);
            s_node = p_node[h_node[s_node]];
        }
        // s_node and d_node's h_node is same
        return cnt[s_node] > cnt[d_node] ? s_node : d_node;
    }
};

int main() {
    
    int C;
    scanf("%d", &C);
    for (int i = 0; i < C; ++i) {
        int N, Q;
        scanf("%d%d", &N, &Q);
        HeavyLightDecomposition hld(N);
        for (int j = 1; j < N; ++j) {
            int node;
            scanf("%d", &node);
            hld.adj[node].push_back(j);
            hld.p_node[j] = node;
        }
        hld.traverse1(0);
        hld.traverse2(0);
        
        for (int j = 0; j < Q; ++j) {
            int s_node, d_node;
            scanf("%d%d", &s_node, &d_node);
            printf("%d\n", hld.depth[s_node] + hld.depth[d_node] - 2 * hld.depth[hld.lca(s_node, d_node)]);
        }
        
    }
    
    return 0;
}
