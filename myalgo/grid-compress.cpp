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

struct MaxSegmentTree { // Max Lazy
    vector<ll> arr, tree, lazy;
    MaxSegmentTree(int size) {
        arr.resize(size);
        tree.resize(size<<2);
        lazy.resize(size<<2);
    }

    ll init(int node, int begin, int end) {
        if (begin == end) return tree[node] = arr[begin];
        ll l = init(l_node, begin, mid);
        ll r = init(r_node, mid+1, end);
        return tree[node] = l+r;
    }
    
    ll update(int node, int begin, int end, int l_pos, int r_pos, ll dif) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (r_pos < begin || end < l_pos) return tree[node];
        if (l_pos <= begin && end <= r_pos) {
            propagate(node, begin, end, dif);
            return tree[node];
        }
        ll l = update(l_node, begin, mid, l_pos, r_pos, dif);
        ll r = update(r_node, mid+1, end, l_pos, r_pos, dif);
        return tree[node] = l>r?l:r;
    }

    void propagate(int node, int begin, int end, ll dif) {
        if (dif == 0) return;
        tree[node] += dif;
        if (begin ^ end) {
            lazy[l_node] += dif;
            lazy[r_node] += dif;
        }
    }

    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (r_pos < begin || end < l_pos) return 0;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l = query(l_node, begin, mid, l_pos, r_pos);
        ll r = query(r_node, mid+1, end, l_pos, r_pos);
        return l>r?l:r;
    }
};

struct SumSegmentTree { // Sum Lazy
    vector<ll> arr, tree, lazy;
    SumSegmentTree(int size) {
        arr.resize(size);
        tree.resize(size<<2);
        lazy.resize(size<<2);
    }
    
    ll init(int node, int begin, int end) {
        if (begin == end) return tree[node] = arr[begin];
        ll l = init(l_node, begin, mid);
        ll r = init(r_node, mid+1, end);
        return tree[node] = l+r;
    }
    
    ll update(int node, int begin, int end, int l_pos, int r_pos, ll dif) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (r_pos < begin || end < l_pos) return tree[node];
        if (l_pos <= begin && end <= r_pos) {
            propagate(node, begin, end, dif);
            return tree[node];
        }
        ll l = update(l_node, begin, mid, l_pos, r_pos, dif);
        ll r = update(r_node, mid+1, end, l_pos, r_pos, dif);
        return tree[node] = l+r;
    }
    
    void propagate(int node, int begin, int end, ll dif) {
        if (dif == 0) return;
        tree[node] += (ll)(end-begin+1)*dif;
        if (begin ^ end) {
            lazy[l_node] += dif;
            lazy[r_node] += dif;
        }
    }
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (r_pos < begin || end < l_pos) return 0;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l = query(l_node, begin, mid, l_pos, r_pos);
        ll r = query(r_node, mid+1, end, l_pos, r_pos);
        return l+r;
    }
};

struct BOJ10999 {
    BOJ10999() {
        int N, M, K; cin >> N >> M >> K;
        SumSegmentTree sum_tree(N+1);
        for (int i = 1; i <= N; ++i)
            cin >> sum_tree.arr[i];
        sum_tree.init(1, 1, N);
        for (int i = 0; i < M+K; ++i) {
            int a; cin >> a;
            if (a == 1) {
                int b, c; ll d; cin >> b >> c >> d;
                sum_tree.update(1, 1, N, b, c, d);
            } else if (a == 2) {
                int b, c; cin >> b >> c;
                cout << sum_tree.query(1, 1, N, b, c) << "\n";
            }
        }
    }
};

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
