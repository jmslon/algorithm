#include <cstdio>
#include <vector>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

// boj 13306.cpp
// Heavy-Light Decomposition

class SegmentTree {
public:
    vector<int> tree;
    const int IDENTITY = 1;
    
    int update(int node, int begin, int end, int pos, int val) {
        if (end < pos || pos < begin) return tree[node];
        if (begin == end) return tree[node] = val;
        return tree[node] = min(update(l_node, begin, mid, pos, val),
                                update(r_node, mid+1, end, pos, val));
    }
    
    int query(int node, int begin, int end, int l_pos, int r_pos) {
        if (end < l_pos || r_pos < begin) return IDENTITY;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        return min(query(l_node, begin, mid, l_pos, r_pos),
                   query(r_node, mid+1, end, l_pos, r_pos));
    }
};

class HLD: public SegmentTree {
public:
    vector<vector<int> > graph;
    vector<int> cnt, head, parent, f;
    int c = 0;
    HLD(int N) {
        graph = vector<vector<int> >(N+1);
        cnt = vector<int>(N+1);
        head = vector<int>(N+1);
        parent = vector<int>(N+1);
        f = vector<int>(N+1);
        tree.resize(N*4, 1);
    }
    
    int dfs(int root) {
        cnt[root] = 1;
        for (auto child : graph[root]) {
            cnt[root] += dfs(child);
        }
        return cnt[root];
    }
    
    void build(int root) {
        int first = 0;
        for (auto child : graph[root])
            if (cnt[first] < cnt[child])
                first = child;
        for (auto child : graph[root])
            if (first ^ child)
                build(child);
        if (!head[root]) head[root] = root;
        if (first) {
            head[first] = head[root];
            build(first);
        }
        f[root] = ++c;
    }
    
    void hld_update(int pos, int val) {
        update(1, 1, (int)graph.size()-1, f[pos], val);
    }
    
    int hld_query(int a, int b) {
        int ret = IDENTITY;
        
        while (head[a] ^ head[b]) {
            if (cnt[head[a]] > cnt[head[b]])
                swap(a, b);
            ret = min(ret, query(1, 1, (int)graph.size()-1, f[a], f[head[a]]));
            a = parent[head[a]];
        }
        if (cnt[a] > cnt[b])
            swap(a, b);
        ret = min(ret, query(1, 1, (int)graph.size()-1, f[a], f[b]-1));
        return ret;
    }
};

int main(void) {
    int N, Q;
    scanf("%d%d", &N, &Q);
    HLD hld(N);
    for (int i = 2; i <= N; ++i) {
        int parentofi;
        scanf("%d", &parentofi);
        hld.graph[parentofi].push_back(i);
        hld.parent[i] = parentofi;
        
    }
    hld.dfs(1);
    hld.build(1);
    
    
    for (int i = 0; i < N - 1 + Q; ++i) {
        int x;
        scanf("%d", &x);
        if (x == 0) { // update(delete)
            int b;
            scanf("%d", &b);
            hld.hld_update(b, 0);
        } else if (x == 1) { // query
            int c, d;
            scanf("%d%d", &c, &d);
            if (hld.hld_query(c, d)) {
                printf("YES\n");
            } else {
                printf("NO\n");
            }
        }
    }
    return 0;
}
