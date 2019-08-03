#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <iostream>

#define NULL_VECTOR {0,0}
#define VOID_VECTOR {-987654321, 987654321}

using namespace std;

typedef double ll;

struct Vector {
    ll x, y;
    Vector(){}
    Vector(ll _x, ll _y) : x(_x), y(_y) {}
    
    bool    operator == (const Vector& a)   const {return x==a.x&&y==a.y;}
    bool    operator <  (const Vector& a)   const {return y==a.y?x<a.x:y<a.y;}
    Vector  operator +  (const Vector& a)   const {return {x+a.x,y+a.y};}
    Vector  operator -  (const Vector& a)   const {return {x-a.x,y-a.y};}
    Vector  operator -  (void)              const {return {-x,-y};}
    Vector  operator *  (const ll a)        const {return {x*a,y*a};}
    
    ll dot      (const Vector& a)   const {return x*a.x+y*a.y;}
    ll cross    (const Vector& a)   const {return x*a.y-y*a.x;}
    double size (void)              const {return sqrt(x*x+y*y);}
    ll size2    (void)              const {return x*x+y*y;}
    
    void print_info() {printf("(%.9lf, %.9lf)\n", x, y);}
};

struct LineSegment {
    pair<Vector, Vector> P;
    ll a, b, c;
    LineSegment(){}
    LineSegment(pair<Vector, Vector> _P) : P(_P) {
        Vector p = _P.second - _P.first;
        a = p.y;
        b = -p.x;
        c = p.cross(_P.first);
    }
    
    int intersect(LineSegment a) {
        Vector p = P.second - P.first;
        Vector q = a.P.second - a.P.first;
        Vector r = a.P.first - P.first;
        ll det = p.cross(q);
        ll t1 = r.cross(q);
        ll t2 = r.cross(p);
        if (det == 0) {
            if (t1 != 0 || t2 != 0) return 0;
            if (P.first < P.second) swap(P.first, P.second);
            if (a.P.first < a.P.second) swap(a.P.first, a.P.second);
            if (a.P.first < P.second) return 0;
            if (P.first < a.P.second) return 0;
            return 1;
        }
        t1 /= det;
        t2 /= det;
        return 0<=t1&&t1<=1&&0<=t2&&t2<=1;
    }
    
    Vector intersection_point(LineSegment a) {
        Vector p = P.second - P.first;
        Vector q = a.P.second - a.P.first;
        Vector r = a.P.first - P.first;
        ll det = p.cross(q);
        ll t1 = r.cross(q);
        ll t2 = r.cross(p);
        if (det == 0) {
            if (t1 != 0 || t2 != 0) return VOID_VECTOR;
            if (P.first < P.second) swap(P.first, P.second);
            if (a.P.first < a.P.second) swap(a.P.first, a.P.second);
            if (a.P.first < P.second) return VOID_VECTOR;
            if (P.first < a.P.second) return VOID_VECTOR;
            if (P.first < a.P.first) return P.first;
            return a.P.first;
        }
        t1 /= det;
        t2 /= det;
        if (0<=t1&&t1<=1&&0<=t2&&t2<=1) {
//            return P.first + p * t1;
            return a.P.first + q * t2;
        } else return VOID_VECTOR;
    }
};

struct PointSet {
    vector<Vector> P;
    
    PointSet(int size) {
        P.resize(size);
    }
    
    virtual void sort_points() = 0;
};

struct FarestPair : public PointSet {
    vector<Vector> hull;
    vector<pair<Vector, Vector> > farest_pairs;
    
    FarestPair(int size) : PointSet(size) {
        P.resize(size);
    }
    
    virtual void sort_points() {
        swap(P[0], *min_element(P.begin(), P.end()));
        for (int i = 1; i < P.size(); ++i) P[i] = P[i] - P[0];
        sort(P.begin()+1, P.end(), [] (const Vector &a, const Vector &b) {
            ll cr = a.cross(b);
            if(cr) return cr > 0;
            return a.y + abs(a.x) < b.y + abs(b.x);
        });
        for (int i = 1; i < P.size(); ++i) P[i] = P[i] + P[0];
    }
    
    void graham_scan() {
        hull = {P[0]};
        for (int i = 1; i < P.size(); ++i) {
            while (1) {
                if (hull.size() <= 1) break;
                Vector back1 = hull.back(), back2 = hull[hull.size()-2];
                if ((back1-back2).cross(P[i]-back2) > 0) break;
                hull.pop_back();
            }
            hull.push_back(P[i]);
        }
    }
    
    ll rotating_calipers() {
        ll max = (hull[0]-hull[1]).size2();
        for (int i = 0, j = 1; i < hull.size(); ++i) {
            int ni = (i+1) % hull.size();
            Vector NI = hull[ni], I = hull[i], NJ, J;
            while (1) {
                int nj = (j+1) % hull.size();
                NJ = hull[nj]; J = hull[j];
                if ((NI-I).cross(NJ-J) > 0) j = nj;
                else break;
            }
            ll tmp = (I-J).size2();
            if(max < tmp) {
                farest_pairs = {{I, J}};
                max = tmp;
            } else if (max == tmp) {
                farest_pairs.push_back({I, J});
            }
        }
        return max;
    }
};
int main() {
    ll x[4], y[4];
    scanf("%lf %lf %lf %lf %lf %lf %lf %lf", x, y, x+1, y+1, x+2, y+2, x+3, y+3);
    LineSegment L1({{x[0], y[0]}, {x[1], y[1]}});
    LineSegment L2({{x[2], y[2]}, {x[3], y[3]}});
    
    if (L1.intersect(L2)) {
        L1.intersection_point(L2).print_info();
        printf("%lfx+%lfy+%lf=0\n", L1.a, L1.b, L1.c);
        printf("%lfx+%lfy+%lf=0\n", L2.a, L2.b, L2.c);
    } else {
        printf("no intersection point or too many points");
    }
    
    return 0;
}
