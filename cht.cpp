struct Line {
	ll k, b;
	mutable const Line* nx;
	bool operator<(ll x) const {
		if (!nx) return 0;
		return b - nx->b < (nx->k - k) * x;
	}
	bool operator<(const Line& rhs) const {
		return k < rhs.k;
	}
};
// will maintain upper hull for maximum
struct HullDynamic : multiset<Line, less<>> {
	bool bad(iterator y) {
		auto z = next(y);
		if (y == begin()) {
			if (z == end()) return 0;
			return y->k == z->k && y->b <= z->b;
		}
		auto x = prev(y);
		if (z == end()) return y->k == x->k && y->b <= x->b;
		return (x->b - y->b) * (int128)(z->k - y->k) >= (y->b - z->b) * (int128)(y->k - x->k);
	}
	void insert_line(ll k, ll b) {
		auto y = insert({ k, b, 0 });
		if (bad(y)) { erase(y); return; }
		auto z = next(y);
		while (z != end() && bad(z)) z = erase(z);
		if (z != end()) y->nx = &*z;
		while (y != begin() && bad(z = prev(y))) erase(z);
		if (y != begin()) z->nx = &*y;
	}
	ll eval(ll x) {
		auto l = *lower_bound(x);
		return l.k * x + l.b;
	}
};
