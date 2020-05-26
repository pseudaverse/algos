struct point {
    ld x, y;

    point(ld _x = 0, ld _y = 0) : x(_x), y(_y) {}

    point operator + (point a) {
        return point(x + a.x, y + a.y);
    }
    point operator - (point a) {
        return point(x - a.x, y - a.y);
    }
    ld operator * (point a) {
        return x * a.y - a.x * y;
    }
    ld operator ^ (point a) {
        return x * a.x + y * a.y;
    }
    bool operator < (point a) const {
        return x < a.x or (x == a.x and y < a.y);
    }
    bool operator == (point a) {
        return x == a.x and y == a.y;
    }

    ld len2() {
        return ((*this) ^ (*this));
    }
};

istream& operator>>(istream& in, point& a) {
    cin >> a.x >> a.y;
    return in;
}

ld dist(point a, point b) {
    return sqrtl((a - b).len2());
}
ld dist(point a, point b, point c) {
    return abs((a - b) * (a - c)) / dist(b, c);
}
ld area(point a, point b, point c) {
    return abs((b - a) * (c - a)) / 2.0;
}

ld const pi = acosl(-1.0);