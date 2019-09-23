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

struct SegmentTree { // Min, Compressed
    vector<ll> arr, tree;
    
    void init() {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        tree.resize(arr.size()*4, INF);
    }
    
    void update(int l_pos, ll val) {
        l_pos = (int) (lower_bound(arr.begin(), arr.end(), l_pos)-arr.begin());
        update(1, 0, (int)arr.size()-1, l_pos, val);
    }
    
    ll query(int l_pos, int r_pos) {
        l_pos = (int)(lower_bound(arr.begin(), arr.end(), l_pos)-arr.begin());
        r_pos = (int)(lower_bound(arr.begin(), arr.end(), r_pos)-arr.begin());
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
        tree.resize(arr.size()*4);
        lazy.resize(arr.size()*4);
    }
    
    void update(int l_pos, int r_pos, ll dif) {
        l_pos = (int) (lower_bound(arr.begin(), arr.end(), l_pos)-arr.begin());
        r_pos = (int) (lower_bound(arr.begin(), arr.end(), r_pos)-arr.begin());
        update(1, 0, (int) arr.size()-2, l_pos, r_pos-1, dif);
    }
    
    ll query(int l_pos, int r_pos) {
        l_pos = (int) (lower_bound(arr.begin(), arr.end(), l_pos)-arr.begin());
        r_pos = (int) (lower_bound(arr.begin(), arr.end(), r_pos)-arr.begin());
        return query(1, 0, (int) arr.size()-2, l_pos, r_pos-1);
    }
    
private:
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
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (r_pos < begin || end < l_pos) return 0;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l = query(l_node, begin, mid, l_pos, r_pos);
        ll r = query(r_node, mid+1, end, l_pos, r_pos);
        return l>r?l:r;
    }
    
    void propagate(int node, int begin, int end, ll dif) {
        if (dif == 0) return;
        tree[node] += dif;
        if (begin ^ end) {
            lazy[l_node] += dif;
            lazy[r_node] += dif;
        }
    }
};

struct BOJ8889 {
    struct Vector {
        ll x, y;
    };
    
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
        sort(lines.begin(), lines.end(), [] (auto a, auto b) {
            return a.y == b.y ? a.dif > b.dif : a.y < b.y;
        });
        seg_tree.init();
        ll answer = 0;
        for (auto line: lines) {
            seg_tree.update(line.x1, line.x2, line.dif);
            answer = max(answer, seg_tree.query(min_val, max_val));
        }
        cout << answer << endl;
    }
};

struct SegmentTree { // Max Lazy Compressed (Not Plane)
    vector<ll> arr, tree, lazy;
    
    void init() {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        tree.resize(arr.size()*4);
        lazy.resize(arr.size()*4);
    }
    
    void update(int l_pos, int r_pos, ll dif) {
        l_pos = (int) (lower_bound(arr.begin(), arr.end(), l_pos)-arr.begin());
        r_pos = (int) (lower_bound(arr.begin(), arr.end(), r_pos)-arr.begin());
        update(1, 0, (int) arr.size()-1, l_pos, r_pos, dif);
    }
    
    ll query(int l_pos, int r_pos) {
        l_pos = (int) (lower_bound(arr.begin(), arr.end(), l_pos)-arr.begin());
        r_pos = (int) (lower_bound(arr.begin(), arr.end(), r_pos)-arr.begin());
        return query(1, 0, (int) arr.size()-1, l_pos, r_pos);
    }
    
private:
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
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (r_pos < begin || end < l_pos) return 0;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l = query(l_node, begin, mid, l_pos, r_pos);
        ll r = query(r_node, mid+1, end, l_pos, r_pos);
        return l>r?l:r;
    }
    
    void propagate(int node, int begin, int end, ll dif) {
        if (dif == 0) return;
        tree[node] += dif;
        if (begin ^ end) {
            lazy[l_node] += dif;
            lazy[r_node] += dif;
        }
    }
};

struct BOJ16357 {
    BOJ16357() {
        struct Node {
            int src, dst;
            bool operator < (Node a) const { // for priority_queue
                // dst가 작은게 가장 먼저 pop
                return dst == a.dst ? src > a.src : dst > a.dst;
            }
        };
        int N; cin >> N;
        vector<Node> nodes(N);
        SegmentTree seg_tree;
        int first = 10000000, last = -10000000;
        for (int i = 0; i < N; ++i) {
            int ux, uy, vx, vy; cin >> ux >> uy >> vx >> vy;
            nodes[i] = {vy, uy};
            seg_tree.arr.push_back(vy);
            seg_tree.arr.push_back(uy);
            first = min(first, vy);
            last = max(last, uy);
        }
        sort(nodes.begin(), nodes.end(), [] (Node a, Node b) {
            return a.src == b.src ? a.dst < b.dst : a.src < b.src;
        });
        
        seg_tree.init();
        for (auto node: nodes)
            seg_tree.update(node.src, node.dst, 1);
        
        ll answer = seg_tree.query(first, last);
        priority_queue<Node> pq;
        for (auto node: nodes) {
            while (!pq.empty()&&pq.top().dst<node.src) pq.pop();
            pq.push(node);
            seg_tree.update(node.src, node.dst, -1);
            answer = max(answer, (ll)pq.size()+seg_tree.query(first, last));
        }
        printf("%lld\n", answer);
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T; cin >> T; while (T--) BOJ5480 p;
    return 0;
}
