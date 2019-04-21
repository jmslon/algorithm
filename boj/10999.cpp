#include <cstdio>
#include <vector>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

// boj 10999.cpp
// segment tree with lazy propagation

class SegmentTree {
public:
    vector<long long> arr, tree, lazy;
    int IDENTITY = 0;
    
    SegmentTree(int N) {
        arr.resize(N+1);
        tree.resize(N*4);
        lazy.resize(N*4);
    }
    
    long long init(int node, int begin, int end) {
        if (begin == end) return tree[node] = arr[begin];
        return tree[node] = init(l_node, begin, mid) + init(r_node, mid+1, end);
    }
    
    long long update_range(int node, int begin, int end, int l_pos, int r_pos, long long dif) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = IDENTITY;
        
        if (end < l_pos || r_pos < begin) return tree[node];
        if (l_pos <= begin && end <= r_pos) {
            propagate(node, begin, end, dif);
            return tree[node];
        }
        return tree[node] = update_range(l_node, begin, mid, l_pos, r_pos, dif) + update_range(r_node, mid+1, end, l_pos, r_pos, dif);
    }
    
    long long query(int node, int begin, int end, int l_pos, int r_pos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = IDENTITY;
        
        if (end < l_pos || r_pos < begin) return IDENTITY;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        return query(l_node, begin, mid, l_pos, r_pos) + query(r_node, mid+1, end, l_pos, r_pos);
    }
    
    void propagate(int node, int begin, int end, long long dif) {
        if (dif == IDENTITY) return;
        tree[node] += (end - begin + 1) * dif;
        if (begin ^ end) {
            lazy[l_node] += dif;
            lazy[r_node] += dif;
        }
    }
    
};

int main() {
    int N, M, K;
    scanf("%d%d%d", &N, &M, &K);
    SegmentTree segment_tree(N);
    for (int i = 1; i <= N; ++i) {
        scanf("%lld", &segment_tree.arr[i]);
    }
    segment_tree.init(1, 1, N);
    for (int i = 0; i < M+K; ++i) {
        int a;
        scanf("%d", &a);
        if (a == 1) { // update_range
            int b, c;
            long long d;
            scanf("%d%d%lld", &b, &c, &d);
            segment_tree.update_range(1, 1, N, b, c, d);
        } else if (a == 2) { // query
            int b, c;
            scanf("%d%d", &b, &c);
            printf("%lld\n", segment_tree.query(1, 1, N, b, c));
        }
    }
    
    return 0;
}
