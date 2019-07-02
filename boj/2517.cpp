#include <cstdio>
#include <vector>
#include <algorithm>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

typedef long long ll;

using namespace std;

struct Query {
    int idx, val;
    
    bool operator < (const Query &a) const {
        return this->val > a.val;
    }
};

struct SegmentTree {
    int size;
    vector<ll> tree;
    
    SegmentTree(int size) : size(size) {
        tree.resize(size<<2);
    }
    
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

int main() {
    
    int N;
    scanf("%d", &N);
    vector<Query> queries(N+1);
    SegmentTree seg_tree(N);
    for (int i = 1; i <= N; ++i) {
        int val;
        scanf("%d", &val);
        queries[i] = {i, val};
    }
    
    sort(queries.begin()+1, queries.end());
    vector<ll> answer(N+1);
    for (int i = 1; i <= N; ++i) {
        answer[queries[i].idx] = seg_tree.query(1, 1, N, 1, queries[i].idx) + 1;
        seg_tree.update(1, 1, N, queries[i].idx, 1);
    }
    for (int i = 1; i <= N; ++i) {
        printf("%lld\n", answer[i]);
    }
    
    return 0;
}

