#include <cstdio>
#include <vector>
#include <algorithm>

#define l_node (node<<1)
#define r_node (l_node+1)
#define mid ((begin+end)>>1)

using namespace std;

// boj 3392.cpp
// Plane Sweeping

class Line {
public:
    int x, y1, y2, z;
};

class PlaneSweeping {
public:
    vector<Line> lines;
    vector<long long> arr, tree, cnt;
    
    PlaneSweeping(int N) {
        lines.resize(N*2);
    }
    
    void init() {
        sort(lines.begin(), lines.end(), [] (Line a, Line b) {
            return a.x == b.x ? b.z < a.z : a.x < b.x;
        });
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        tree.resize(arr.size()*4);
        cnt.resize(arr.size()*4);
    }
    
    int posof(int val) {
        return (int) (lower_bound(arr.begin(), arr.end(), val) - arr.begin());
    }
    
    long long area() {
        long long ret = 0;
        for (int i = 0; i < lines.size() - 1; ++i) {
            update_range(1, 0, (int) arr.size()-2, posof(lines[i].y1), posof(lines[i].y2)-1, lines[i].z);
            ret += (long long) ((lines[i+1].x - lines[i].x) * tree[1]);
        }
        return ret;
    }
    
    void update_range(int node, int begin, int end, int l_pos, int r_pos, int val) {
        if (end < l_pos || r_pos < begin) return;
        if (l_pos <= begin && end <= r_pos) cnt[node] += val;
        else {
            update_range(l_node, begin, mid, l_pos, r_pos, val);
            update_range(r_node, mid+1, end, l_pos, r_pos, val);
        }
        if (cnt[node]) tree[node] = arr[end+1] - arr[begin];
        else {
            if (begin == end) tree[node] = 0;
            else tree[node] = tree[l_node] + tree[r_node];
        }
    }
};

int main() {
    int N;
    scanf("%d", &N);
    PlaneSweeping plane_sweeping(N);
    
    for (int i = 0, x1, x2, y1, y2; i < N; ++i) {
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        plane_sweeping.lines[i] = {x1, y1, y2, 1};
        plane_sweeping.lines[i+N] = {x2, y1, y2, -1};
        plane_sweeping.arr.push_back(y1);
        plane_sweeping.arr.push_back(y2);
    }
    
    plane_sweeping.init();
    printf("%lld\n", plane_sweeping.area());
    
    return 0;
}
