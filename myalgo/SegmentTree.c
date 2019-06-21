// Segment Tree

#include <cstdio>
#include <vector>

#define mid ((begin+end)>>1)
#define l_node (node<<1)
#define r_node (l_node+1)

using namespace std;

struct SegmentTree {
    vector<int> arr, tree;
    int MEANINGLESS;
    
    SegmentTree(int size) {
        arr.resize(size);
        tree.resize(size<<2);
    }
    
    int init(int node, int begin, int end) {
        if (begin == end) return tree[node] = begin;
        int l_init = init(l_node, begin, mid);
        int r_init = init(r_node, mid+1, end);
        return tree[node] = whichof(l_init, r_init);
    }
    
    int query(int node, int begin, int end, int l_pos, int r_pos) {
        if (end < l_pos || r_pos < begin) return MEANINGLESS;
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
        if (l == MEANINGLESS) return r;
        if (r == MEANINGLESS) return l;
        return arr[l] > arr[r] ? l : r;
    }
};

struct MinSegmentTree: public SegmentTree {
    MinSegmentTree(int size) : SegmentTree(size) {
        MEANINGLESS = 0x7fffffff;
    }
    virtual int whichof(int l, int r) {
        if (l == MEANINGLESS) return r;
        if (r == MEANINGLESS) return l;
        return arr[l] < arr[r] ? l : r;
    }
};
