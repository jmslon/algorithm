#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

typedef int ll;

struct SegmentTree {
    vector<ll> arr, tree, cnt;
    void update(int node, int begin, int end, int l_pos, int r_pos, ll dif) {
        if (r_pos < begin || end < l_pos) return;
        if (l_pos <= begin && end <= r_pos) cnt[node] += dif;
        else {
            update(l_node, begin, mid, l_pos, r_pos, dif);
            update(r_node, mid+1, end, l_pos, r_pos, dif);
        }
        if (cnt[node]) tree[node] =  arr[end+1] - arr[begin];
        else if (begin == end) tree[node] = 0;
        else tree[node] = tree[l_node] + tree[r_node];
    }
};

struct Grid {
    int size;
    vector<ll> arr;
    SegmentTree st;
    void compress() {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        st.arr = arr;
        size = (int) arr.size();
        st.tree.resize(size<<2);
        st.cnt.resize(size<<2);
    }
    void update(int l_pos, int r_pos, ll dif) {
        l_pos = (int) (lower_bound(arr.begin(), arr.end(), l_pos) - arr.begin());
        r_pos = (int) (lower_bound(arr.begin(), arr.end(), r_pos) - arr.begin()); // IMPORTANT!
        st.update(1, 0, size-2, l_pos, r_pos-1, dif);
    }
};

struct Line {
    ll from, to, height, dif;
    bool operator < (const Line &r) const {
        return height == r.height ? dif > r.dif : height < r.height;
    }
};

struct Plane {
    vector<Line> lines;
    Grid grid;
    
    void sort_lines() {
        sort(lines.begin(), lines.end());
    }
    
    ll perimeter() {
        ll ret = 0;
        ll prev = 0;
        for (int i = 0; i < lines.size(); ++i) {
            grid.update(lines[i].from, lines[i].to, lines[i].dif);
            ll curr = grid.st.tree[1];
            ret += abs(curr - prev);
            prev = curr;
        }
        return ret;
    }
    
    ll area() {
        ll ret = 0;
        for (int i = 0; i < lines.size()-1; ++i) {
            grid.update(lines[i].from, lines[i].to, lines[i].dif);
            ret += (ll) grid.st.tree[1] * (lines[i+1].height - lines[i].height);
        }
        return ret;
    }
};

int main() {
    int N;
    scanf("%d", &N);
    Plane plane1, plane2;
    for (int i = 0; i < N; ++i) {
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        plane1.lines.push_back({x1, x2, y1, 1});
        plane1.lines.push_back({x1, x2, y2, -1});
        plane2.lines.push_back({y1, y2, x1, 1});
        plane2.lines.push_back({y1, y2, x2, -1});
        plane1.grid.arr.push_back(x1);
        plane1.grid.arr.push_back(x2);
        plane2.grid.arr.push_back(y1);
        plane2.grid.arr.push_back(y2);
    }
    plane1.sort_lines();
    plane2.sort_lines();
    plane1.grid.compress();
    plane2.grid.compress();
    printf("%d\n", plane1.perimeter() + plane2.perimeter());
    if (plane1.area() ^ plane2.area()) printf("error");
    
    return 0;
}
