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
    int x1, x2, y, dif;
};

struct SegmentTree {
    vector<ll> x, tree, cnt;
    void update(int node, int begin, int end, int l_pos, int r_pos, int dif) {
        if (r_pos < begin || end < l_pos) return;
        if (l_pos <= begin && end <= r_pos) cnt[node] += dif;
        else {
            update(l_node, begin, mid, l_pos, r_pos, dif);
            update(r_node, mid+1, end, l_pos, r_pos, dif);
        }
        if (cnt[node]) tree[node] = x[end+1]-x[begin];
        else if (begin == end) tree[node] = 0;
        else tree[node] = tree[l_node] + tree[r_node];
    }
};

struct Plane: SegmentTree {
    int size; // size after compress
    vector<Line> lines;
    Plane(int size) {
        lines.resize(size<<1);
        x.resize(size<<1);
    }
    
    void sort_lines() {
        sort(lines.begin(), lines.end(), [] (Line &a, Line &b) {
            return a.y == b.y ? b.dif < a.dif : a.y < b.y;
        });
    }
    
    void grid_compress() {
        
        // 1. build arr
        // 입력시 했으므로 이 단계는 생략.
        
        // 2. sort and erase duplicated elements
        sort(x.begin(), x.end());
        x.erase(unique(x.begin(), x.end()), x.end());
        
        // 3. copy
        for (int i = 0; i < lines.size(); ++i) {
            lines[i].x1 = (int)(lower_bound(x.begin(),x.end(),lines[i].x1)-x.begin());
            lines[i].x2 = (int)(lower_bound(x.begin(),x.end(),lines[i].x2)-x.begin());
        }
        
        // 4. resize tree
        size = (int) x.size();
        tree.resize(size<<2);
        cnt.resize(size<<2);
    }
    
    ll area() {
        ll ret = 0;
        for (int i = 0; i < lines.size()-1; ++i) {
            update(1, 0, size-2, lines[i].x1, lines[i].x2-1, lines[i].dif);
            ret += (ll) tree[1] * (lines[i+1].y - lines[i].y);
        }
        return ret;
    }
};

int main() {
    int N;
    scanf("%d", &N);
    Plane plane(N);
    for (int i = 0; i < N; ++i) {
        int x1, x2, y1, y2;
        scanf("%d%d%d%d", &x1, &x2, &y1, &y2);
        plane.lines[i] = {x1, x2, y1, 1};
        plane.lines[i+N] = {x1, x2, y2, -1};
        plane.x[i] = x1;
        plane.x[i+N] = x2;
    }
    plane.sort_lines();
    plane.grid_compress();
    printf("%lld\n", plane.area());
    
    return 0;
}

