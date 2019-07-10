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

struct Line {
    int from, to, height;
    ll dif;
    bool operator < (const Line &r) const {
        return height == r.height ? dif > r.dif : height < r.height;
    }
};

struct Plane {
    int size;
    SegmentTree st;
    vector<Line> lines;
    vector<int> arr;

    void sort_lines() {
        sort(lines.begin(), lines.end());
    }

    void compress() {
        // 1. build (done)
        // 2. sort & erase
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());

        // 3. copy
        for (int i = 0; i < lines.size(); ++i) {
            lines[i].from = (int) (lower_bound(arr.begin(), arr.end(), lines[i].from) - arr.begin());
            lines[i].to = (int) (lower_bound(arr.begin(), arr.end(), lines[i].to) - arr.begin());
        }
        st.arr = arr;
        
        // 4. resize
        size = (int) arr.size();
        st.tree.resize(size<<2);
        st.cnt.resize(size<<2);
    }
    
    ll perimeter() {
        ll ret = 0;
        ll prev = 0;
        for (int i = 0; i < lines.size(); ++i) {
            st.update(1, 0, size-2, lines[i].from, lines[i].to-1, lines[i].dif);
            ll curr = st.tree[1];
            ret += abs(curr - prev);
            prev = curr;
        }
        return ret;
    }
    
    ll area() {
        ll ret = 0;
        for (int i = 0; i < lines.size()-1; ++i) {
            st.update(1, 0, size-2, lines[i].from, lines[i].to-1, lines[i].dif);
            ret += (ll) st.tree[1] * (lines[i+1].height - lines[i].height);
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
        plane1.arr.push_back(x1);
        plane1.arr.push_back(x2);
        plane2.arr.push_back(y1);
        plane2.arr.push_back(y2);
    }
    plane1.sort_lines();
    plane2.sort_lines();
    plane1.compress();
    plane2.compress();
    printf("%d\n", plane1.perimeter() + plane2.perimeter());
    if (plane1.area() ^ plane2.area()) printf("error");
    
    return 0;
}
