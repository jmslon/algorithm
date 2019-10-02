#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

typedef long long ll;

struct Vector {
    ll x, y;
    Vector(){}
    Vector(ll _x,ll _y):x(_x),y(_y){}
    
    bool    operator == (const Vector &a)   const {return x==a.x&&y==a.y;}
    bool    operator <  (const Vector &a)   const {return y==a.y?x<a.x:y<a.y;}
    Vector  operator +  (const Vector &a)   const {return {x+a.x,y+a.y};}
    Vector  operator -  (const Vector &a)   const {return {x-a.x,y-a.y};}
    Vector  operator -  (void)              const {return {-x,-y};}
    Vector  operator *  (const ll a)        const {return {x*a,y*a};}
    
    ll dot       (const Vector &a)   const {return x*a.x+y*a.y;}
    ll cross     (const Vector &a)   const {return x*a.y-y*a.x;}
    //double size  (void)              const {return sqrt(x*x+y*y);}
    ll size2     (void)              const {return x*x+y*y;}
    
    void print_info() {cout<<"("<<x<<","<<y<<")\n";}
};

struct PointSet {
    vector<Vector> P;
    
    PointSet(int size) {
        P.resize(size);
    }
    PointSet(Vector A, Vector B) {
        P.push_back(A);
        P.push_back(B);
    }
};

struct LineSegment: PointSet {
    LineSegment():PointSet(2){}
    LineSegment(Vector A, Vector B):PointSet(A, B){}
};

struct Geometry {
    static bool intersect(LineSegment a, LineSegment b) {
        Vector p = a.P[1] - a.P[0];
        Vector q = b.P[1] - b.P[0];
        Vector r = b.P[0] - a.P[0];
        ll det = p.cross(q);
        ll t1 = r.cross(q);
        ll t2 = r.cross(p);
        if (det == 0) {
            if (t1 != 0 || t2 != 0) return 0; // 평행
            if (a.P[0] < a.P[1]) swap(a.P[0], a.P[1]);
            if (b.P[0] < b.P[1]) swap(b.P[0], b.P[1]);
            if (b.P[0] < a.P[1]) return 0; // 안 겹침
            if (a.P[0] < b.P[1]) return 0; // 안 겹침
            return 1; // 일부 겹침
        }
        if (det<0) {
            t1*=-1;
            t2*=-1;
            det*=-1;
        }
        return 0<=t1&&t1<=det&&0<=t2&&t2<=det; // 한점에서 만남
    }
};

struct Graph {
    struct Edge {
        int src, dst; unsigned long key;
    };
    
    vector<vector<unsigned long>> adj;
    vector<Edge> edges;
    
    Graph(int size) {
        adj.resize(size);
    }
    
    void push(int src, int dst) {
        push(src, dst, edges.size());
    }
    
    void push(int src, int dst, unsigned long key) {
        adj[src].push_back(key);
        edges.push_back({src, dst, key});
    }
};

struct StronglyConnectedComponent: Graph {
    vector<int> f, g, g_id;
    vector<vector<int>> SCC;
    
    StronglyConnectedComponent(int size): Graph(size) {
        f.resize(size, -1);
        g_id.resize(size);
    }
    
    vector<vector<int>> strongly_connected_component() {
        vector<int> stack;
        vector<bool> visit(adj.size());
        for (int i = 0; i < adj.size(); ++i) if (f[i] == -1)
            strongly_connected_component(i, stack, visit);
        return SCC;
    }
    
    int strongly_connected_component(int src, vector<int> &stack, vector<bool> &visit) {
        if (f[src] != -1) return f[src];
        
        f[src] = (int)g.size();
        g.push_back(src);
        stack.push_back(src);
        int ret = f[src];
        for (auto idx: adj[src]) {
            auto dst = edges[idx].dst;
            if (!visit[dst])
                ret = min(ret, strongly_connected_component(dst, stack, visit));
        }
        
        if (ret == f[src]) {
            vector<int> scc;
            while (1) {
                auto t = stack.back(); stack.pop_back();
                visit[t] = 1;
                scc.push_back(t);
                g_id[t] = (int) SCC.size();
                if (t == src) break;
            }
            SCC.push_back(scc);
        }
        return ret;
    }
};

struct SAT: StronglyConnectedComponent {
    SAT(int size): StronglyConnectedComponent(size*2) {}
    void push(int a, int b) {
        Graph::push(f(-a), f(b));
        Graph::push(f(-b), f(a));
    }
    vector<int> is_valid() {
        vector<int> ret;
        for (int i = 1; i < adj.size()/2; ++i) {
            if (g_id[i*2]==g_id[i*2+1]) return vector<int>();
            if (g_id[i*2]<g_id[i*2+1]) ret.push_back(i);
        }
        return ret;
    }
    inline int f(int a) {return a>0?2*a:-2*a+1;} // DO NOT USE ZERO INDEX!
    inline int g(int a) {return a%2?-a/2:a/2;}
};

struct BOJ2519 {
    int N;
    BOJ2519() {
        cin >> N;
        SAT sat = N*3+1;
        vector<LineSegment> lines(N*3+1);
        for (int i = 1; i <= N*3; ++i) {
            ll x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
            lines[i] = {{x1, y1}, {x2, y2}};
        }
        for (int i = 1; i <= N*3; ++i) for (int j = i+1; j <= N*3; ++j) {
            if (Geometry::intersect(lines[i], lines[j]))
                sat.push(i, j);
        }
        for (int i = 1; i <= N; ++i) {
            sat.push(-(i*3-2), -(i*3-1));
            sat.push(-(i*3-2), -(i*3));
            sat.push(-(i*3-1), -(i*3));
        }
        sat.strongly_connected_component();
        auto answer = sat.is_valid();
        if (answer.size() == 0) cout << "-1\n";
        else {
            sort(answer.begin(), answer.end());
            cout << answer.size() << endl;
            for (auto a: answer) cout << a << " "; cout << endl;
        }
    }
};

int main(){
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    BOJ2519 p;
}
