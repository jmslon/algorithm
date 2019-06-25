#include <cstdio>
#include <vector>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

struct SegmentTree {
    vector<long long> arr;
    vector<int> tree;
    int MEANINGLESS;
    
    SegmentTree(int size) {
        arr.resize(size + 1);
        tree.resize(size * 4);
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
    
    long long max_area(int l_pos, int r_pos) {
        if (l_pos > r_pos) return -1;
        int min_pos = query(1, 1, (int)arr.size()-1, l_pos, r_pos);
        long long ret = arr[min_pos] * (r_pos - l_pos + 1);
        ret = max(ret, max_area(l_pos, min_pos-1));
        ret = max(ret, max_area(min_pos+1, r_pos));
        return ret;
    }
    
    virtual int whichof(int l, int r) = 0;
};

struct MaxSegmentTree : public SegmentTree {
    MaxSegmentTree(int size) : SegmentTree(size) {
        MEANINGLESS = -1;
    }
    int whichof(int l, int r) {
        if (l == MEANINGLESS) return r;
        if (r == MEANINGLESS) return l;
        return arr[l] < arr[r] ? l : r;
    }
};

int main(int argc, const char * argv[]) {
    
    int N;
    while (1) {
        scanf("%d", &N);
        if (N == 0) break;
        MaxSegmentTree segment_tree(N);
        for (int i = 1; i <= N; ++i)
            scanf("%lld", &segment_tree.arr[i]);
        segment_tree.init(1, 1, N);
        printf("%lld\n", segment_tree.max_area(1, N));
    }
    
    return 0;
}

