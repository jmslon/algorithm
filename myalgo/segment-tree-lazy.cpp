#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

typedef long long ll;

struct MaxSegmentTree { // Max Lazy
   vector<ll> arr, tree, lazy;
   MaxSegmentTree(int size) {
       arr.resize(size);
       tree.resize(size<<2);
       lazy.resize(size<<2);
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

   ll query(int node, int begin, int end, int l_pos, int r_pos) {
       propagate(node, begin, end, lazy[node]);
       lazy[node] = 0;
       if (r_pos < begin || end < l_pos) return 0;
       if (l_pos <= begin && end <= r_pos) return tree[node];
       ll l = query(l_node, begin, mid, l_pos, r_pos);
       ll r = query(r_node, mid+1, end, l_pos, r_pos);
       return l>r?l:r;
   }
   
private:
   void propagate(int node, int begin, int end, ll dif) {
       if (dif == 0) return;
       tree[node] += dif;
       if (begin ^ end) {
           lazy[l_node] += dif;
           lazy[r_node] += dif;
       }
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
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (r_pos < begin || end < l_pos) return 0;
        if (l_pos <= begin && end <= r_pos) return tree[node];
        ll l = query(l_node, begin, mid, l_pos, r_pos);
        ll r = query(r_node, mid+1, end, l_pos, r_pos);
        return l+r;
    }
   
private:
    void propagate(int node, int begin, int end, ll dif) {
        if (dif == 0) return;
        tree[node] += (ll)(end-begin+1)*dif;
        if (begin ^ end) {
            lazy[l_node] += dif;
            lazy[r_node] += dif;
        }
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
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    BOJ10999 p;
    return 0;
}

