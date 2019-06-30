#include <cstdio>
#include <vector>
#include <algorithm>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

typedef long long ll;

struct Vector {
    int x, y;
};

struct SegmentTree {
    int size, MEANINGLESS = 0;
    vector<int> arr, tree;
    SegmentTree(int size) : size(size) {
        tree.resize(size<<2);
    }
    
    int update(int node, int begin, int end, int pos, int val) {
        if (pos < begin || end < pos) return tree[node];
        if (begin == end) return tree[node] = val;
        int l_update = update(l_node, begin, mid, pos, val);
        int r_update = update(r_node, mid+1, end, pos, val);
        return tree[node] = l_update + r_update;
    }
    
    int query(int node, int begin, int end, int l_pos, int r_pos) {
        if (r_pos < begin || end < l_pos) return MEANINGLESS;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        int l_query = query(l_node, begin, mid, l_pos, r_pos);
        int r_query = query(r_node, mid+1, end, l_pos, r_pos);
        return l_query + r_query;
    }
    
    void grid_compress() {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
    }
    
    int posof(int val) {
        return (int) (lower_bound(arr.begin(),arr.end(),val)-arr.begin());
    }
    
};

struct Plane : SegmentTree {
    vector<Vector> P;
    vector<int> val;
    Plane(int size) : SegmentTree(size) {
        P.resize(size);
        val.resize(size);
    }
    
    void sort_points() {
        sort(P.begin(), P.end(), [] (Vector &a, Vector &b) {
            return a.x == b.x ? b.y < a.y : a.x < b.x;
        });
    }
    
    ll get_answer() {
        ll ret = 0;
        for (int i = 0; i < size; ++i) {
            int pos = posof(P[i].y);
            ret += (ll) query(1, 0, size-1, pos, size-1);
            val[pos]++;
            update(1, 0, size-1, pos, val[pos]);
        }
        return ret;
    }
    
};

int main() {
    int T;
    scanf("%d", &T);
    for (int i = 0; i < T; ++i) {
        int N;
        scanf("%d", &N);
        Plane plane(N);
        for (int i = 0; i < N; ++i) {
            int x, y;
            scanf("%d%d", &x, &y);
            plane.P[i] = {x, y};
            plane.arr.push_back(y);
        }
        plane.sort_points();
        plane.grid_compress();
        
        printf("%lld\n", plane.get_answer());
        
    }
    
    return 0;
}
