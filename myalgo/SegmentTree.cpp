/*
 Segment Tree by index
 */
#include <cstdio>
#include <vector>

#define mid ((begin+end)>>1)
#define l_node (node<<1)
#define r_node (l_node+1)

typedef long long ll;

using namespace std;

struct SegmentTree {
    int size;
    vector<ll> arr, tree;
    ll MEANINGLESS;
    
    SegmentTree(int size) {
        this->size = size;
        arr.resize(size);
        tree.resize(size<<2);
    }
    
    ll init(int node, int begin, int end) {
        if (begin == end) return tree[node] = begin;
        ll l_init = init(l_node, begin, mid);
        ll r_init = init(r_node, mid+1, end);
        return tree[node] = whichof(l_init, r_init);
    }
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        if (r_pos < begin || end < l_pos) return MEANINGLESS;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l_query = query(l_node, begin, mid, l_pos, r_pos);
        ll r_query = query(r_node, mid+1, end, l_pos, r_pos);
        return whichof(l_query, r_query);
    }
    
    virtual ll whichof(ll l, ll r) = 0;
};

struct MaxSegmentTree : public SegmentTree {
    MaxSegmentTree(int size) : SegmentTree(size) {
        MEANINGLESS = -1;
    }
    virtual ll whichof(ll l, ll r) {
        if (l == MEANINGLESS) return r;
        if (r == MEANINGLESS) return l;
        return arr[l] > arr[r] ? l : r;
    }
};

struct MinSegmentTree : public SegmentTree {
    MinSegmentTree(int size) : SegmentTree(size) {
        MEANINGLESS = 0x7fffffff;
    }
    virtual ll whichof(ll l, ll r) {
        if (l == MEANINGLESS) return r;
        if (r == MEANINGLESS) return l;
        return arr[l] < arr[r] ? l : r;
    }
};

int main() {
    
    int N;
    scanf("%d", &N);
    
    MinSegmentTree min_seg(N);
    MaxSegmentTree max_seg(N);
    
    for (int i = 0; i < N; ++i) {
        int tmp;
        scanf("%d", &tmp);
        min_seg.arr[i] = tmp;
        max_seg.arr[i] = tmp;
    }
    min_seg.init(1, 0, N-1);
    max_seg.init(1, 0, N-1);
    for (int i = 0; i < N-1; ++i) {
        for (int j = i+1; j < N; ++j) {
            printf("%d~%d : [%lld, %lld]\n", i, j, min_seg.query(1, 0, N-1, i, j), max_seg.query(1, 0, N-1, i, j));
        }
    }
    return 0;
}
