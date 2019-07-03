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
    int size;
    vector<ll> tree;
    vector<Vector> P;
    SegmentTree(int size) {
        P.resize(size);
    }
    
    ll update(int node, int begin, int end, int pos, int dif) {
        if (pos < begin || end < pos) return tree[node];
        if (begin == end) return tree[node] += dif;
        ll l_update = update(l_node, begin, mid, pos, dif);
        ll r_update = update(r_node, mid+1, end, pos, dif);
        return tree[node] = l_update + r_update;
    }
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        if (r_pos < begin || end < l_pos) return 0;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l_query = query(l_node, begin, mid, l_pos, r_pos);
        ll r_query = query(r_node, mid+1, end, l_pos, r_pos);
        return l_query + r_query;
    }
    
    void grid_compress() {
        vector<int> x(P.size());
        for (int i = 0; i < x.size(); ++i)
            x[i] = P[i].x;
        sort(x.begin(), x.end());
        x.erase(unique(x.begin(), x.end()), x.end());
        for (int i = 0; i < P.size(); ++i)
            P[i].x = (int)(lower_bound(x.begin(),x.end(),P[i].x)-x.begin());
        size = (int) x.size();
        tree.resize(size<<2);
    }
    
    void sort_points() { // for sweeping
        sort(P.begin(), P.end(), [] (Vector &a, Vector &b) {
            return a.y == b.y ? a.x < b.x : a.y > b.y;
        });
    }
    
    ll get_answer() {
        ll ret = 0;
        for (int i = 0; i < P.size(); ++i) {
            ret += query(1, 0, size-1, 0, P[i].x);
            update(1, 0, size-1, P[i].x, 1); // dif == 1
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
        SegmentTree st(N);
        for (int i = 0; i < N; ++i)
            scanf("%d%d", &st.P[i].x, &st.P[i].y);
        st.grid_compress();
        st.sort_points();
        printf("%lld\n", st.get_answer());
    }
    return 0;
}


