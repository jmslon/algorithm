#include <cstdio>
#include <vector>
#include <algorithm>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

typedef long long ll;

struct Line {
    int x1, x2, y, dif;
};

struct MinSegmentTree {
    int size;
    vector<Line> lines;
    vector<ll> arr, tree, lazy;
    MinSegmentTree(int size) {
        lines.resize(size<<1);
    }
    
    ll update_range(int node, int begin, int end, int l_pos, int r_pos, int dif) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (end < l_pos || r_pos < begin) return tree[node];
        if (l_pos <= begin && end <= r_pos) {
            propagate(node, begin, end, dif);
            return tree[node];
        }
        ll l_update = update_range(l_node, begin, mid, l_pos, r_pos, dif);
        ll r_update = update_range(r_node, mid+1, end, l_pos, r_pos, dif);
        return tree[node] = l_update < r_update ? l_update : r_update;
    }
    
    ll query(int node, int begin, int end, int l_pos, int r_pos) {
        propagate(node, begin, end, lazy[node]);
        lazy[node] = 0;
        if (tree[node]) return arr[end+1] - arr[begin];
        if (begin == end) return 0;
        ll l_query = query(l_node, begin, mid, l_pos, r_pos);
        ll r_query = query(r_node, mid+1, end, l_pos, r_pos);
        return l_query + r_query;
    }
    
    void propagate(int node, int begin, int end, ll dif) {
        if (dif == 0) return;
        tree[node] += dif;
        if (begin ^ end) {
            lazy[l_node] += dif;
            lazy[r_node] += dif;
        }
    }
    
    void sort_lines() {
        sort(lines.begin(), lines.end(), [] (Line &a, Line &b) {
            return a.y == b.y ? b.dif < a.dif : a.y < b.y;
        });
    }
    
    void grid_compress() {
        // 1. build arr.
        for (int i = 0; i < lines.size(); ++i) {
            arr.push_back(lines[i].x1);
            arr.push_back(lines[i].x2);
        }
        
        // 2. sort arr and erase duplicated elements.
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        
        // 3. resize tree and lazy.
        size = (int) arr.size();
        tree.resize(size<<2);
        lazy.resize(size<<2);
    }
    
    ll area() {
        ll ret = 0;
        for (int i = 0; i < lines.size()-1; ++i) {
            int l_pos = posof(lines[i].x1);
            int r_pos = posof(lines[i].x2)-1;
            int dif = lines[i].dif;
            update_range(1, 0, size-2, l_pos, r_pos, dif);
            ret += (ll) query(1, 0, size-2, l_pos, r_pos)*(lines[i+1].y - lines[i].y);
        }
        return ret;
    }
    
    int posof(int val) {
        return (int)(lower_bound(arr.begin(),arr.end(),val)-arr.begin());
    }
};

int main() {
    int N;
    scanf("%d", &N);
    MinSegmentTree plane(N);
    for (int i = 0; i < N; ++i) {
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        plane.lines[i] = {x1, x2, y1, 1};
        plane.lines[i+N] = {x1, x2, y2, -1};
    }
    plane.sort_lines();
    plane.grid_compress();
    printf("%lld\n", plane.area());
    
    return 0;
}

