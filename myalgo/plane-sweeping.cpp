#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

typedef long long ll;

struct PlaneSweeping {
    int size;
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
    
    void compress() {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        size = (int) arr.size();
        tree.resize(size<<2);
        cnt.resize(size<<2);
    }
    
    void update(int l_pos, int r_pos, ll dif) {
        l_pos = (int) (lower_bound(arr.begin(), arr.end(), l_pos) - arr.begin());
        r_pos = (int) (lower_bound(arr.begin(), arr.end(), r_pos) - arr.begin()); // IMPORTANT!
        update(1, 0, size-2, l_pos, r_pos-1, dif); // r_pos-1 !!
    }
};

struct Plane {
    struct Line {
        ll from, to, height, dif;
        bool operator < (const Line &r) const {
            return height == r.height ? dif > r.dif : height < r.height;
        }
    };
    
    vector<Line> lines;
    PlaneSweeping grid;
    
    void sort_lines() {
        sort(lines.begin(), lines.end());
    }
    
    ll perimeter() {
        ll ret = 0, prev = 0;
        for (int i = 0; i < lines.size(); ++i) {
            grid.update(lines[i].from, lines[i].to, lines[i].dif);
            ll curr = grid.tree[1];
            ret += abs(curr - prev);
            prev = curr;
        }
        return ret;
    }
    
    ll area() {
        ll ret = 0;
        for (int i = 0; i < lines.size()-1; ++i) {
            grid.update(lines[i].from, lines[i].to, lines[i].dif);
            ret += grid.tree[1] * (lines[i+1].height - lines[i].height);
            cout << grid.cnt[1] << endl;
        }
        return ret;
    }
};

struct BOJ2185 {
    BOJ2185() {
        int N; cin >> N;
        Plane plane1, plane2;
        for (int i = 0; i < N; ++i) {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
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
        cout << plane1.perimeter() + plane2.perimeter() << endl;
    }
};

struct BOJ7626 {
    BOJ7626() {
        int N; cin >> N;
        Plane plane;
        for (int i = 0; i < N; ++i) {
            ll x1, x2, y1, y2;
            cin >> x1 >> x2 >> y1 >> y2;
            plane.lines.push_back({x1, x2, y1, 1});
            plane.lines.push_back({x1, x2, y2, -1});
            plane.grid.arr.push_back(x1);
            plane.grid.arr.push_back(x2);
        }
        plane.sort_lines();
        plane.grid.compress();
        cout << plane.area() << endl;
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
//    BOJ2185 p;
//    BOJ7626 p;
    return 0;
}
