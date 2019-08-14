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
    double size  (void)              const {return sqrt(x*x+y*y);}
    ll size2     (void)              const {return x*x+y*y;}
    
    void print_info() {cout<<"("<<x<<","<<y<<")\n";}
};

struct Vector3: Vector {
    ll z;
    Vector3(){}
    Vector3(ll _x, ll _y, ll _z):Vector(_x, _y),z(_z){}
    
    bool    operator == (const Vector3 &a)  const {return x==a.x&&y==a.y&&z==a.z;}
    bool    operator <  (const Vector3 &a)  const {return z==a.z?y==a.y?x<a.x:y<a.y:z<a.z;}
    Vector3 operator +  (const Vector3 &a)  const {return {x+a.x,y+a.y,z+a.z};}
    Vector3 operator -  (const Vector3 &a)  const {return {x-a.x,y-a.y,z-a.z};}
    Vector3 operator -  (void)              const {return {-x,-y,-z};}
    Vector3 operator *  (const ll a)        const {return {x*a,y*a,z*a};}
    
    ll dot        (const Vector3 &a)  const {return x*a.x+y*a.y+z*a.z;}
    Vector3 cross (const Vector3 &a)  const {return {y*a.z-z*a.y,z*a.x-x-a.z,x*a.y-y*a.x};}
    double size   (void)              const {return sqrt(x*x+y*y+z*z);}
    ll size2      (void)              const {return x*x+y*y+z*z;}
    
    void print_info() {cout<<"("<<x<<","<<y<<","<<z<<"\n";}
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
            //return P.first + p * t1;
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

struct FarestPair: PointSet {
    vector<Vector> hull;
    vector<pair<Vector, Vector> > pairs;
    
    FarestPair(int size): PointSet(size) {}
    
    virtual void sort_points() {
        swap(P[0], *min_element(P.begin(), P.end()));
        for (int i = 1; i < P.size(); ++i) P[i] = P[i] - P[0];
        sort(P.begin()+1, P.end(), [] (const Vector &a, const Vector &b) {
            ll cr = a.cross(b);
            if (cr) return cr > 0;
            return a.y + abs(a.x) < b.y + abs(b.x);
        });
        for (int i = 1; i < P.size(); ++i) P[i] = P[i] + P[0];
    }
    
    void graham_scan() {
        //sort_points();
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
    
    void rotating_calipers() {
        //graham_scan();
        pairs = {{hull[0], hull[1]}};
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
                pairs = {{I, J}};
                max = tmp;
            } else if (max == tmp) {
                pairs.push_back({I, J});
            }
        }
        for (auto pair: pairs) if (pair.first < pair.second) swap(pair.first, pair.second);
        sort(pairs.begin(), pairs.end());
        pairs.erase(unique(pairs.begin(), pairs.end()), pairs.end());
    }
    
    void brute_force() {
        pairs = {{P[0], P[1]}};
        ll max = (P[0]-P[1]).size2();
        for (int i = 0; i < P.size(); ++i) {
            Vector I = P[i];
            for (int j = i + 1; j < P.size(); ++j) {
                Vector J = P[j];
                ll tmp = (I-J).size2();
                if (max < tmp) {
                    pairs = {{I, J}};
                    max = tmp;
                } else if (max == tmp) {
                    pairs.push_back({I, J});
                }
            }
        }
    }
};

struct ClosestPair: PointSet {
    vector<pair<Vector, Vector> > pairs;
    
    ClosestPair(int size): PointSet(size) {}
    
    void sort_points() {
        sort(P.begin(), P.end(), [] (Vector &a, Vector &b) {
            return a.x == b.x ? a.y < b.y : a.x < b.x;
        });
        P.erase(unique(P.begin(), P.end()), P.end());
    }
    void sweeping() {
        //sort_points();
        set<Vector> SET = {P[0], P[1]};
        
        pairs = {{P[0], P[1]}};
        ll min = (P[0]-P[1]).size2();
        for (int i = 2, j = 0; i < P.size(); ++i) {
            Vector I = P[i];
            for (; j < i; ++j) {
                Vector J = P[j];
                Vector tmp = I-J;
                if (tmp.x * tmp.x < min || tmp.y * tmp.y < min) break;
                SET.erase(J);
            }
            
            ll d = sqrt(min) + 1;
            Vector D = {d, d};
            auto end = SET.upper_bound(I+D);
            for (auto it = SET.lower_bound(I-D); it != end; ++it) {
                Vector J = *it;
                ll tmp = (I-J).size2();
                
                if (tmp < min) {
                    pairs = {{I,J}};
                    min = tmp;
                } else if (tmp == min) {
                    pairs.push_back({I,J});
                }
            }
            SET.insert(I);
        }
    }
    
    void brute_force() {
        pairs = {{P[0], P[1]}};
        ll min = (P[0]-P[1]).size2();
        for (int i = 0; i < P.size(); ++i) {
            Vector I = P[i];
            for (int j = i + 1; j < P.size(); ++j) {
                Vector J = P[j];
                ll tmp = (I-J).size2();
                if (min > tmp) {
                    pairs = {{I, J}};
                    min = tmp;
                } else if (min == tmp) {
                    pairs.push_back({I, J});
                }
            }
        }
    }
};
