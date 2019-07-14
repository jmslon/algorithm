/*
 boj.5480.cpp
 Segment Tree
 Grid Compress
 Offline Query
 */
#include <cstdio>
#include <vector>
#include <algorithm>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)
#define INF 0x7fffffff

using namespace std;

typedef int ll;

struct SegmentTree { // min tree value
    int size;
    vector<ll> tree;
    ll update(int node, int begin, int end, int pos, int val) {
        if (pos < begin || end < pos) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l = update(l_node, begin, mid, pos, val);
        ll r = update(r_node, mid+1, end, pos, val);
        return tree[node] = l < r ? l : r;
    }
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        if (r_pos < begin || end < l_pos) return 0x7fffffff;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l = query(l_node, begin, mid, l_pos, r_pos);
        ll r = query(r_node, mid+1, end, l_pos, r_pos);
        return l < r ? l : r;
    }
};

struct Ship {
    int x1, y1, x2, y2, w;
};

struct Laser {
    int a, b;
};

struct Grid {
    int size;
    SegmentTree st;
    vector<ll> arr;
    void compress() {
        // arr must be built, before compress
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        size = (int) arr.size();
        st.tree.resize(size<<2, INF);
    }
    void update(int pos, ll val) {
        pos = (int) (lower_bound(arr.begin(), arr.end(), pos) - arr.begin());
        st.update(1, 0, size-1, pos, val);
    }
    ll query(int l_pos, int r_pos) {
        l_pos = (int) (lower_bound(arr.begin(), arr.end(), l_pos) - arr.begin());
        r_pos = (int) (lower_bound(arr.begin(), arr.end(), r_pos) - arr.begin());
        return st.query(1, 0, size-1, l_pos, r_pos);
    }
};

int main() {
    
    int T;
    scanf("%d", &T);
    for (;T--;) {
        int N, K, L;
        scanf("%d%d%d", &N, &K, &L);
        vector<Ship> ships;
        vector<Laser> lasers;
        for (int i = 0; i < K; ++i) {
            int x1, y1, x2, y2, w;
            scanf("%d%d%d%d%d", &x1, &y1, &x2, &y2, &w);
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            ships.push_back({x1, y1, x2, y2, w});
        }
        
        for (int i = 0; i < L; ++i) {
            int a, b;
            scanf("%d%d", &a, &b);
            lasers.push_back({a, b});
        }
        
        Grid xgrid, ygrid;
        // offline query
        for (int i = 0; i < K; ++i) {
            xgrid.arr.push_back(ships[i].x1);
            xgrid.arr.push_back(ships[i].x2);
            ygrid.arr.push_back(ships[i].y1);
            ygrid.arr.push_back(ships[i].y2);
        }
        for (int i = 0; i < L; ++i) {
            if (lasers[i].b == 0)  // y축 평행
                ygrid.arr.push_back(lasers[i].a);
            else if (lasers[i].b == 1) // x축 평행
                xgrid.arr.push_back(lasers[i].a);
        }
        
        xgrid.compress();
        ygrid.compress();
        
        for (int i = L-1; i >= 0; --i) {
            if (lasers[i].b == 0)
                ygrid.update(lasers[i].a, i);
            else if (lasers[i].b == 1)
                xgrid.update(lasers[i].a, i);
        }
        vector<ll> answer(L);
        for (int i = 0; i < K; ++i) {
            ll xlaser = xgrid.query(ships[i].x1, ships[i].x2);
            ll ylaser = ygrid.query(ships[i].y1, ships[i].y2);
            ll laser = xlaser < ylaser ? xlaser : ylaser;
            if (laser == INF) continue;
            answer[laser] = answer[laser] > ships[i].w ? answer[laser] : ships[i].w;
        }
        for (int i = 0; i < L; ++i)
            printf("%d\n", answer[i] == INF ? 0 : answer[i]);
    }
    
    return 0;
}
