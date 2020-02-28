namespace fft {
	typedef double dbl;
 
	struct num {
		dbl x, y;
		num() { x = y = 0; }
		num(dbl x, dbl y) : x(x), y(y) { }
	};
 
	inline num operator+(num a, num b) { return num(a.x + b.x, a.y + b.y); }
	inline num operator-(num a, num b) { return num(a.x - b.x, a.y - b.y); }
	inline num operator*(num a, num b) { return num(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
	inline num conj(num a) { return num(a.x, -a.y); }
 
	ll base = 1;
	vector<num> roots = { {0, 0}, {1, 0} };
	vector<ll> rev = { 0, 1 };
 
	const dbl PI = acosl(-1.0);
	
	//можно комментить
	ll __builtin_ctz(ll n) {
		ll i = 0;
		ll ret = 0;
		while (((1LL << i) & n) == 0) {
			ret++;
			i++;
		}
		return ret;
	}
 
	void ensure_base(ll nbase) {
		if (nbase <= base) {
			return;
		}
		rev.resize(1 << nbase);
		for (ll i = 0; i < (1 << nbase); i++) {
			rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
		}
		roots.resize(1 << nbase);
		while (base < nbase) {
			dbl angle = 2 * PI / (1 << (base + 1));
			//      num z(cos(angle), sin(angle));
			for (ll i = 1 << (base - 1); i < (1 << base); i++) {
				roots[i << 1] = roots[i];
				//        roots[(i << 1) + 1] = roots[i] * z;
				dbl angle_i = angle * (2 * i + 1 - (1 << base));
				roots[(i << 1) + 1] = num(cos(angle_i), sin(angle_i));
			}
			base++;
		}
	}
 
	void fft(vector<num>& a, ll n = -1) {
		if (n == -1) {
			n = a.size();
		}
		assert((n & (n - 1)) == 0);
		ll zeros = __builtin_ctz(n);
		ensure_base(zeros);
		ll shift = base - zeros;
		for (ll i = 0; i < n; i++) {
			if (i < (rev[i] >> shift)) {
				swap(a[i], a[rev[i] >> shift]);
			}
		}

		for (ll len = 1; len < n; len <<= 1) {
			for (ll i = 0; i < n; i += 2 * len) {
				for (ll j = i, k = i + len; j < i + len; j++, k++) {
					num z = a[k] * roots[k - i];
					a[k] = a[j] - z;
					a[j] = a[j] + z;
				}
			}
		}
	}
 
	vector<num> fa, fb;
 
	vector<ll> multiply(vector<ll>& a, vector<ll>& b) {
		ll need = a.size() + b.size() - 1;
		ll nbase = 0;
		while ((1 << nbase) < need) nbase++;
		ensure_base(nbase);
		ll sz = 1 << nbase;
		if (sz > (ll)fa.size()) {
			fa.resize(sz);
		}
		for (ll i = 0; i < sz; i++) {
			ll x = (i < (ll)a.size() ? a[i] : 0);
			ll y = (i < (ll)b.size() ? b[i] : 0);
			fa[i] = num(x, y);
		}
		fft(fa, sz);
		num r(0, -0.25 / sz);
		for (ll i = 0; i <= (sz >> 1); i++) {
			ll j = (sz - i) & (sz - 1);
			num z = (fa[j] * fa[j] - conj(fa[i] * fa[i])) * r;
			if (i != j) {
				fa[j] = (fa[i] * fa[i] - conj(fa[j] * fa[j])) * r;
			}
			fa[i] = z;
		}
		fft(fa, sz);
		vector<ll> res(need);
		for (ll i = 0; i < need; i++) {
			res[i] = fa[i].x + 0.5;
		}
		return res;
	}
 
	// fft::multiply uses dbl, outputs vector<ll> of rounded values
	// fft::multiply_mod might work for res.size() up to 2^21
	// typedef long double dbl;          =>        up to 2^25 (but takes a lot of memory)
};