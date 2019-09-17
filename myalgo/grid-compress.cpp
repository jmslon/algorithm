#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)
#define INF 0x6fffffffffffffff

using namespace std;

typedef long long ll;

struct CompressedSegmentTree { // min, grid compress
    vector<ll> arr, tree;
    
    void init() {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        tree.resize(arr.size()*4, INF);
    }
    
    void update(int l_val, ll val) {
        int l_pos = (int)(lower_bound(arr.begin(), arr.end(), l_val)-arr.begin());
        update(1, 0, (int)arr.size()-1, l_pos, val);
    }
    
    ll query(int l_val, int r_val) {
        int l_pos = (int)(lower_bound(arr.begin(), arr.end(), l_val)-arr.begin());
        int r_pos = (int)(lower_bound(arr.begin(), arr.end(), r_val)-arr.begin());
        return query(1, 0, (int)arr.size()-1, l_pos, r_pos);
    }
    
private:
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

struct BOJ5480 {
    struct Ship {
        int x1, y1, x2, y2; ll w;
    };
    
    struct Laser {
        int a; bool b;
    };
    
    BOJ5480() {
        int N, K, L; cin >> N >> K >> L;
        vector<Ship> ships;
        vector<Laser> lasers;
        CompressedSegmentTree treex, treey;
        
        for (int i = 0; i < K; ++i) {
            int x1, y1, x2, y2, w; cin >> x1 >> y1 >> x2 >> y2 >> w;
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            ships.push_back({x1, y1, x2, y2, w});
            treex.arr.push_back(x1); treex.arr.push_back(x2);
            treey.arr.push_back(y1); treey.arr.push_back(y2);
        }
        for (int i = 0; i < L; ++i) {
            int a; bool b; cin >> a >> b;
            lasers.push_back({a, b});
            if (b) treex.arr.push_back(lasers[i].a);
            else treey.arr.push_back(lasers[i].a);
        }
        
        treex.init(); treey.init();
        
        for (int i = L-1; i >= 0; --i) {
            if (lasers[i].b) treex.update(lasers[i].a, i);
            else treey.update(lasers[i].a, i);
        }
        
        vector<ll> answer(L);
        for (int i = 0; i < K; ++i) {
            ll laserx = treex.query(ships[i].x1, ships[i].x2);
            ll lasery = treey.query(ships[i].y1, ships[i].y2);
            ll laser = min(laserx, lasery);
            if (laser == INF) continue;
            answer[laser] = max(answer[laser], ships[i].w);
        }
        for (auto val: answer)
            cout << (val == INF ? 0 : val) << "\n";
        
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T; cin >> T; while (T--) BOJ5480 p;
    return 0;
}
