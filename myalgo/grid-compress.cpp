#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#define INF 0x6fffffffffffffff
#define lnode (node<<1)
#define rnode (lnode+1)
#define mid ((begin+end)>>1)

using namespace std;

typedef unsigned long long ull;
typedef unsigned long ul;
typedef long long ll;

struct SegmentTree { // Min, Compressed
    vector<ll> arr, tree;
    
    void init() {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        tree.resize(arr.size()<<2, INF);
    }
    
    void update(ll pos, ll val) {
        ul idx = lower_bound(arr.begin(), arr.end(), pos) - arr.begin();
        update(1, 0, arr.size()-1, idx, val);
    }
    
    ll query(ll lpos, ll rpos) {
        ul lidx = lower_bound(arr.begin(), arr.end(), lpos) - arr.begin();
        ul ridx = lower_bound(arr.begin(), arr.end(), rpos) - arr.begin();
        return query(1, 0, arr.size()-1, lidx, ridx);
    }
    
private:
    ll update(ul node, ul begin, ul end, ul idx, ll val) {
        if (idx < begin || end < idx) return tree[node];
        if (begin == end) return tree[node] = val;
        ll l = update(lnode, begin, mid, idx, val);
        ll r = update(rnode, mid+1, end, idx, val);
        return tree[node] = l < r ? l : r;
    }
    
    ll query(ul node, ul begin, ul end, ul lidx, ul ridx) {
        if (ridx < begin || end < lidx) return INF;
        if (lidx <= begin && end <= ridx) return tree[node];
        ll l = query(lnode, begin, mid, lidx, ridx);
        ll r = query(rnode, mid+1, end, lidx, ridx);
        return l < r ? l : r;
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
        SegmentTree treex, treey;
        
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




struct SegmentTree { // Max Lazy Compressed (Plane)
    vector<ll> arr, tree, lazy;
    
    void init() {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        tree.resize(arr.size()<<2);
        lazy.resize(arr.size()<<2);
    }
    
    void update(ll lpos, ll rpos, ll dif) {
        ul lidx = lower_bound(arr.begin(), arr.end(), lpos) - arr.begin();
        ul ridx = lower_bound(arr.begin(), arr.end(), rpos) - arr.begin();
        update(1, 0, arr.size()-2, lidx, ridx-1, dif);
    }
    
    ll query(ll lpos, ll rpos) {
        ul lidx = lower_bound(arr.begin(), arr.end(), lpos) - arr.begin();
        ul ridx = lower_bound(arr.begin(), arr.end(), rpos) - arr.begin();
        return query(1, 0, arr.size()-2, lidx, ridx-1);
    }
    
private:
    ll update(ul node, ul begin, ul end, ul lidx, ul ridx, ll dif) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (ridx < begin || end < lidx) return tree[node];
        if (lidx <= begin && end <= ridx) {
            propagate(node, begin, end, dif);
            return tree[node];
        }
        ll l = update(lnode, begin, mid, lidx, ridx, dif);
        ll r = update(rnode, mid+1, end, lidx, ridx, dif);
        return tree[node] = l > r ? l : r;
    }
    
    ll query(ul node, ul begin, ul end, ul lidx, ul ridx) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (ridx < begin || end < lidx) return 0;
        if (lidx <= begin && end <= ridx) return tree[node];
        ll l = query(lnode, begin, mid, lidx, ridx);
        ll r = query(rnode, mid+1, end, lidx, ridx);
        return l > r ? l : r;
    }
    
    void propagate(ul node, ul begin, ul end, ll dif) {
        if (dif == 0) return;
        tree[node] += dif;
        if (begin ^ end) {
            lazy[lnode] += dif;
            lazy[rnode] += dif;
        }
    }
};

struct BOJ8889 {
    struct Vector { ll x, y; };
    struct Polygon {
        vector<Vector> p;
        Polygon(int size) {
            p.resize(size);
        }
    };
    
    struct Line {
        ll x1, x2, y, dif;
    };
    
    BOJ8889() {
        int N; cin >> N;
        vector<Line> lines;
        SegmentTree seg_tree;
        ll min_val = INF, max_val = 0;
        while (N--) {
            int n; cin >> n;
            Polygon polygons = n;
            for (int i = 0; i < n; ++i)
                cin >> polygons.p[i].x >> polygons.p[i].y;
            for (int i = 0; i < n; ++i) {
                int j = (i+1)%n;
                auto vi = polygons.p[i], vj = polygons.p[j];
                if (vi.x < vj.x) lines.push_back({vi.x, vj.x, vi.y, 1});
                if (vi.x > vj.x) lines.push_back({vj.x, vi.x, vi.y, -1});
                seg_tree.arr.push_back(vi.x);
                min_val = min(min_val, vi.x);
                max_val = max(max_val, vi.x);
            }
        }
        sort(lines.begin(), lines.end(), [] (Line &a, Line &b) {
            return a.y == b.y ? a.dif > b.dif : a.y < b.y;
        });
        seg_tree.init();
        ll answer = 0;
        for (auto line: lines) {
            seg_tree.update(line.x1, line.x2, line.dif);
            answer = max(answer, seg_tree.query(min_val, max_val));
        }
        cout << answer << "\n";
    }
};



struct SegmentTree { // Max Lazy Compressed (Not Plane)
    vector<ll> arr, tree, lazy;
    
    void init() {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        tree.resize(arr.size()<<2);
        lazy.resize(arr.size()<<2);
    }
    
    void update(ll lval, ll rval, ll dif) {
        ul lpos = lower_bound(arr.begin(), arr.end(), lval) - arr.begin();
        ul rpos = lower_bound(arr.begin(), arr.end(), rval) - arr.begin();
        update(1, 0, arr.size()-1, lpos, rpos, dif);
    }
    
    ll query(ll lval, ll rval) {
        ul lpos = lower_bound(arr.begin(), arr.end(), lval) - arr.begin();
        ul rpos = lower_bound(arr.begin(), arr.end(), rval) - arr.begin();
        return query(1, 0, arr.size()-1, lpos, rpos);
    }
    
private:
    ll update(ul node, ul begin, ul end, ul lpos, ul rpos, ll dif) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (rpos < begin || end < lpos) return tree[node];
        if (lpos <= begin && end <= rpos) {
            propagate(node, begin, end, dif);
            return tree[node];
        }
        ll l = update(lnode, begin, mid, lpos, rpos, dif);
        ll r = update(rnode, mid+1, end, lpos, rpos, dif);
        return tree[node] = l > r ? l : r;
    }
    
    ll query(ul node, ul begin, ul end, ul lpos, ul rpos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (rpos < begin || end < lpos) return 0;
        if (lpos <= begin && end <= rpos) return tree[node];
        ll l = query(lnode, begin, mid, lpos, rpos);
        ll r = query(rnode, mid+1, end, lpos, rpos);
        return l > r ? l : r;
    }
    
    void propagate(ul node, ul begin, ul end, ll dif) {
        if (dif == 0) return;
        tree[node] += dif;
        if (begin ^ end) {
            lazy[lnode] += dif;
            lazy[rnode] += dif;
        }
    }
};

struct BOJ16357 {
    BOJ16357() {
        struct Pair {
            int first, second;
            bool operator < (Pair a) const { // for priority_queue
                // second가 작은게 가장 먼저 pop
                return second == a.second ? first > a.first : second > a.second;
            }
        };
        int N; cin >> N;
        vector<Pair> pairs(N);
        SegmentTree segment_tree;
        int first = 10000000, last = -10000000;
        for (int i = 0; i < N; ++i) {
            int ux, uy, vx, vy; cin >> ux >> uy >> vx >> vy;
            pairs[i] = {vy, uy};
            segment_tree.arr.push_back(vy);
            segment_tree.arr.push_back(uy);
            first = min(first, vy);
            last = max(last, uy);
        }
        sort(pairs.begin(), pairs.end(), [] (Pair &a, Pair &b) {
            return a.first == b.first ? a.second < b.second : a.first < b.first;
        });
        
        segment_tree.init();
        for (auto node: pairs) segment_tree.update(node.first, node.second, 1);
        
        ll answer = segment_tree.query(first, last);
        priority_queue<Pair> pq;
        for (auto node: pairs) {
            while (!pq.empty()) {
                if (pq.top().second < node.first) pq.pop();
                else break;
            }
            pq.push(node);
            segment_tree.update(node.first, node.second, -1);
            answer = max(answer, (ll)pq.size()+segment_tree.query(first, last));
        }
        cout << answer << "\n";
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T; cin >> T; while (T--) BOJ5480 p;
    return 0;
}
