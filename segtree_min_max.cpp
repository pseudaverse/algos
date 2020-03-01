struct segtree {
	//  меняем
	struct node {
		node(ll x): val_mn(x), val_mx(x) {};
		node(ll mn, ll mx) : val_mn(mn), val_mx(mx) {};
		node() : val_mn(INT_MAX), val_mx(INT_MIN) {};
		ll val_mn = INT_MAX;
		ll val_mx = INT_MIN;
		ll lazy = 0;
		bool is_lazy = 0;
		void apply(int tl, int tr, ll add) {
			//val += (tr - tl + 1) * add;
			val_mn = add;
			val_mx = add;
			lazy = add;
			is_lazy = 1;
		}
	};
	node neutral;
	node merge(const node& a, const node& b) const {
		return node(min(a.val_mn, b.val_mn), max(a.val_mx, b.val_mx));
		//return node(a.val + b.val);
	}
	// дальше не трогай
	void push(int u, int tl, int tr) {
		if (t[u].is_lazy) {
			int tm = (tl + tr) / 2;
			if (tl != tr) {
				t[u * 2].apply(tl, tm, t[u].lazy);
				t[u * 2 + 1].apply(tm + 1, tr, t[u].lazy);
			}
			t[u].is_lazy = 0;
		}
	}
	node get(int u, int tl, int tr, int l, int r) {
		if (r < l || tr < tl || tl > r || tr < l) {
			return neutral;
		}
		push(u, tl, tr);
		if (l <= tl && tr <= r) {
			return t[u];
		} else {
			int tm = (tl + tr) / 2;
			return merge(get(u * 2, tl, tm, l, r), get(u * 2 + 1, tm + 1, tr, l, r));
		}
	}
	template<typename... M>
	void upd(int u, int tl, int tr, int l, int r, const M&... m) {
		if (r < l || tr < tl || tl > r || tr < l) {
			return;
		}
		push(u, tl, tr);
		if (l <= tl && tr <= r) {
			t[u].apply(tl, tr, m...);
		} else {
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
		}
		else {
			int tm = (tl + tr) / 2;
			build(u * 2, tl, tm, v);
			build(u * 2 + 1, tm + 1, tr, v);
			t[u] = merge(t[u * 2], t[u * 2 + 1]);
		}
	}
 
	template<class T>
	segtree(T n) : n(n) {
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
};
