#include <cstdio>
#include <vector>

#define mid ((begin+end)>>1)
#define l_node (node<<1)
#define r_node (l_node+1)
#define INF 0x7fffffff

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
        if (begin == end) return tree[node] = arr[begin];
        ll l_init = init(l_node, begin, mid);
        ll r_init = init(r_node, mid+1, end);
        return tree[node] = whichof(l_init, r_init);
    }
    
    ll update(int node, int begin, int end, int pos, ll val) {
        if (pos < begin || end < pos) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l_update = update(l_node, begin, mid, pos, val);
        ll r_update = update(r_node, mid+1, end, pos, val);
        return tree[node] = whichof(l_update, r_update);
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

struct MaxSegmentTree {
    vector<ll> arr, tree;
    
    MaxSegmentTree(int size) {
        arr.resize(size);
        tree.resize(size<<2);
    }
    
    ll init(int node, int begin, int end) {
        if (begin == end) return tree[node] = arr[begin];
        ll l = init(l_node, begin, mid);
        ll r = init(r_node, mid+1, end);
        return tree[node] = l>r?l:r;
    }
    
    ll update(int node, int begin, int end, int pos, ll val) {
        if (pos < begin || end < pos) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l = update(l_node, begin, mid, pos, val);
        ll r = update(r_node, mid+1, end, pos, val);
        return tree[node] = l>r?l:r;
    }
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        if (r_pos < begin || end < l_pos) return 0;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l = query(l_node, begin, mid, l_pos, r_pos);
        ll r = query(r_node, mid+1, end, l_pos, r_pos);
        return l>r?l:r;
    }
};

struct MinSegmentTree {
    vector<ll> arr, tree;
    
    MinSegmentTree(int size) {
        arr.resize(size);
        tree.resize(size<<2);
    }
    
    ll init(int node, int begin, int end) {
        if (begin == end) return tree[node] = arr[begin];
        ll l = init(l_node, begin, mid);
        ll r = init(r_node, mid+1, end);
        return tree[node] = l<r?l:r;
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

struct SumSegmentTree {
    vector<ll> arr, tree;
    
    SumSegmentTree(int size) {
        arr.resize(size);
        tree.resize(size<<2);
    }
    
    ll init(int node, int begin, int end) {
        if (begin == end) return tree[node] = arr[begin];
        ll l = init(l_node, begin, mid);
        ll r = init(r_node, mid+1, end);
        return tree[node] = l+r;
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

int main() {
    
    int N;
    scanf("%d", &N);
    
    MinSegmentTree min_seg(N);
    MaxSegmentTree max_seg(N);
    SumSegmentTree sum_seg(N);
    
    for (int i = 0; i < N; ++i) {
        int tmp;
        scanf("%d", &tmp);
        min_seg.arr[i] = tmp;
        max_seg.arr[i] = tmp;
        sum_seg.arr[i] = tmp;
    }
    min_seg.init(1, 0, N-1);
    max_seg.init(1, 0, N-1);
    sum_seg.init(1, 0, N-1);
    for (int i = 0; i < N-1; ++i) {
        for (int j = i+1; j < N; ++j) {
            printf("%d~%d : [%lld, %lld, %lld]\n", i, j, min_seg.query(1, 0, N-1, i, j), max_seg.query(1, 0, N-1, i, j), sum_seg.query(1, 0, N-1, i, j));
        }
    }
    return 0;
}
