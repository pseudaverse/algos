struct segtree {
	// 4Head
	struct node {
		node(ll x) : val_mx(x) {};
		node() : val_mx(LLONG_MIN) {};
		ll val_mx = LLONG_MIN;
		ll lazy = 0;
		void apply(int tl, int tr, ll add) {
			val_mx += add;
			lazy += add;
		}
	};
	node merge(const node& a, const node& b) const {
		return node(max(a.val_mx, b.val_mx));
	}
	void push(int u, int tl, int tr) {
		t[u * 2].apply(1, 1, t[u].lazy);
		t[u * 2 + 1].apply(1, 1, t[u].lazy);
		t[u].lazy = 0;
	}
	// 4Head
	node get(int u, int tl, int tr, int l, int r) {
		if (r < l || tl > r || tr < l) {
			return neutral;
		}
		if (l <= tl && tr <= r) {
			return t[u];
		} else {
			push(u, tl, tr);
			int tm = (tl + tr) / 2;
			return merge(get(u * 2, tl, tm, l, r), get(u * 2 + 1, tm + 1, tr, l, r));
		}
	}
	template<typename... M>
	void upd(int u, int tl, int tr, int l, int r, const M&... m) {
		if (r < l || tl > r || tr < l) {
			return;
		}
		if (l <= tl && tr <= r) {
			t[u].apply(tl, tr, m...);
		} else {
			push(u, tl, tr);
			int tm = (tl + tr) / 2;
			upd(u * 2, tl, tm, l, r, m...);
			upd(u * 2 + 1, tm + 1, tr, l, r, m...);
			t[u] = merge(t[u * 2], t[u * 2 + 1]);
		}
	}
	template<class T>
	void build(int u, int tl, int tr, vector<T>& v) {
		if (tl == tr) {
			node tmp(v[tl]);
			t[u] = tmp;
		} else {
			int tm = (tl + tr) / 2;
			build(u * 2, tl, tm, v);
			build(u * 2 + 1, tm + 1, tr, v);
			t[u] = merge(t[u * 2], t[u * 2 + 1]);
		}
	}
	segtree(ll n) : n(n) {
		t.resize(4 * n);
	}
	template<class T>
	segtree(vector<T>& v) : n(v.size()) {
		t.resize(4 * n);
		build(1, 0, n - 1, v);
	}
	node get(int l, int r) {
		return get(1, 0, n - 1, l, r);
	}
	template<typename... M>
	void upd(int l, int r, const M&... m) {
		upd(1, 0, n - 1, l, r, m...);
	}
	int n;
	vector<node>t;
	node neutral;
};
