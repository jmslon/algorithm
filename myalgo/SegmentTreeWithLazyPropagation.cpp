/*
 boj.10999.cpp
 Sum Segment Tree by value with Lazy Propagation
*/

#include <cstdio>
#include <vector>
#include <algorithm>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

struct SegmentTree {
    vector<long long> arr;
    vector<long long> tree, lazy;
    int MEANINGLESS;
    
    SegmentTree(int size) {
        arr.resize(size+1);
        tree.resize(size<<2);
        lazy.resize(size<<2);
    }
    
    long long init(int node, int begin, int end) {
        if (begin == end) return tree[node] = arr[begin];
        long long l_init = init(l_node, begin, mid);
        long long r_init = init(r_node, mid+1, end);
        return tree[node] = whichof(l_init, r_init);
    }
    
    long long update_range(int node, int begin, int end, int l_pos, int r_pos, long long diff) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = MEANINGLESS;
        if (r_pos < begin || end < l_pos) return tree[node];
        if (l_pos <= begin && end <= r_pos) {
            propagate(node, begin, end, diff);
            return tree[node];
        }
        long long l_update = update_range(l_node, begin, mid, l_pos, r_pos, diff);
        long long r_update = update_range(r_node, mid+1, end, l_pos, r_pos, diff);
        return tree[node] = whichof(l_update, r_update);
    }
    
    long long query(int node, int begin, int end, int l_pos, int r_pos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = MEANINGLESS;
        if (r_pos < begin || end < l_pos) return MEANINGLESS;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        long long l_query = query(l_node, begin, mid, l_pos, r_pos);
        long long r_query = query(r_node, mid+1, end, l_pos, r_pos);
        return whichof(l_query, r_query);
    }
    
    virtual long long whichof(long long l, long long r) = 0;
    virtual void propagate(int node, int begin, int end, long long diff) = 0;
};

struct SumSegmentTree : public SegmentTree {
    SumSegmentTree(int size): SegmentTree(size) {
        MEANINGLESS = 0;
    }
    
    virtual long long whichof(long long l, long long r) {
        return (l + r);
    }
    
    virtual void propagate(int node, int begin, int end, long long dif) {
        tree[node] += (end - begin +1) * dif;
        if (begin ^ end) {
            lazy[l_node] += dif;
            lazy[r_node] += dif;
        }
    }
};

int main() {
    int N, M, K;
    scanf("%d%d%d", &N, &M, &K);
    SumSegmentTree segment_tree(N);
    for (int i = 1; i <= N; ++i)
        scanf("%lld", &segment_tree.arr[i]);
    
    segment_tree.init(1, 1, N);
    for (int i = 0; i < M+K; ++i) {
        int a, b, c; long long d;
        scanf("%d", &a);
        if (a == 1) { // update
            scanf("%d%d%lld", &b, &c, &d);
            segment_tree.update_range(1, 1, N, b, c, d);
        } else if (a == 2) { // query
            scanf("%d%d", &b, &c);
            printf("%lld\n", segment_tree.query(1, 1, N, b, c));
        }
    }
    
    return 0;
}
