/*
 boj.7626.cpp
 Plane Sweeping
 Segment Tree
 Grid Compression
 */

#include <cstdio>
#include <vector>
#include <algorithm>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

typedef long long ll;

struct Line {
    int x, y1, y2, dif;
};

struct Plane {
    int size;
    vector<Line> lines;
    vector<ll> arr, tree, lazy;
    Plane(int size) {
        lines.resize(size<<1);
        arr.resize(size<<1);
    }
    
    void sort_lines() {
        sort(lines.begin(), lines.end(), [] (Line &a, Line &b) {
            return a.x == b.x ? b.dif < a.dif : a.x < b.x;
        });
    }
    
    void grid_compress() {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
    }
    
    void init() {
        size = (int) arr.size();
        tree.resize(size<<2);
        lazy.resize(size<<2);
    }
    
    void update_range(int node, int begin, int end, int l_pos, int r_pos, int dif) {
        if (end < l_pos || r_pos < begin) return;
        if (l_pos <= begin && end <= r_pos) lazy[node] += dif;
        else {
            update_range(l_node, begin, mid, l_pos, r_pos, dif);
            update_range(r_node, mid+1, end, l_pos, r_pos, dif);
        }
        if (lazy[node]) tree[node] = arr[end+1] - arr[begin];
        else if (begin == end) tree[node] = 0;
        else tree[node] = tree[l_node] + tree[r_node];
    }
    
    ll area() {
        ll ret = 0;
        
        for (int i = 0; i < lines.size()-1; ++i) {
            int l_pos = posof(lines[i].y1);
            int r_pos = posof(lines[i].y2)-1;
            int dif = lines[i].dif;
            update_range(1, 0, size-2, l_pos, r_pos, dif);
            ret += (ll) tree[1] * (lines[i+1].x - lines[i].x);
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
    Plane plane(N);
    for (int i = 0; i < N; ++i) {
        int x1, x2, y1, y2;
        scanf("%d%d%d%d", &x1, &x2, &y1, &y2);
        plane.lines[i] = {x1, y1, y2, 1};
        plane.lines[i+N] = {x2, y1, y2, -1};
        plane.arr[i] = y1;
        plane.arr[i+N] = y2;
    }
    
    plane.sort_lines();
    plane.grid_compress();
    plane.init();
    
    printf("%lld\n", plane.area());
    
    return 0;
}

