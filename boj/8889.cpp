#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)
#define INF 0x6fffffffffffffff

using namespace std;

typedef long long ll;

struct MaxSegmentTree { // Max Lazy
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
        MaxSegmentTree seg_tree;
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

int main() {
    int T; cin >> T; while (T--) BOJ8889 p;
    return 0;
}
