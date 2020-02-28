namespace ntt {
	ll md = 998244353; // 469762049
	
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
	
	ll add(ll x, ll y) {
		x += y;
		if (x >= md) {
			x -= md;
		}
		return x;
	}
 
	ll sub(ll x, ll y) {
		x -= y;
		if (x < 0) {
			x += md;
		}
		return x;
	}
 
	ll mul(ll x, ll y) {
		return (long long)x * y % md;
	}
 
	ll power(ll x, ll y) {
		ll result = 1;
		for (; y; y >>= 1, x = mul(x, x)) {
			if (y & 1) {
				result = mul(result, x);
			}
		}
		return result;
	}
 
	ll base = 1, root = -1, max_base = -1;
	vector<ll> roots = { 0, 1 }, rev = { 0, 1 };
 
	void restore(ll _md) {
		md = _md;
		base = 1, root = -1, max_base = -1;
		roots = { 0, 1 }, rev = { 0, 1 };
	}
 
	void init() {
		ll temp = md - 1;
		max_base = 0;
		while (!(temp & 1)) {
			temp >>= 1;
			++max_base;
		}
		root = 2;
		while (true) {
			if (power(root, 1 << max_base) == 1 && power(root, 1 << max_base - 1) != 1) {
				break;
			}
			++root;
		}
	}
 
	void ensure_base(ll nbase) {
		if (max_base == -1) {
			init();
		}
		if (nbase <= base) {
			return;
		}
		assert(nbase <= max_base);
		rev.resize(1 << nbase);
		for (ll i = 0; i < (1 << nbase); ++i) {
			rev[i] = rev[i >> 1] >> 1 | ((i & 1) << nbase - 1);
		}
		roots.resize(1 << nbase);
		while (base < nbase) {
			ll z = power(root, 1 << max_base - 1 - base);
			for (ll i = 1 << base - 1; i < 1 << base; ++i) {
				roots[i << 1] = roots[i];
				roots[i << 1 | 1] = mul(roots[i], z);
			}
			++base;
		}
	}
 
	void dft(vector<ll>& a) {
		ll n = a.size(), zeros = __builtin_ctz(n);
		ensure_base(zeros);
		ll shift = base - zeros;
		for (ll i = 0; i < n; ++i) {
			if (i < rev[i] >> shift) {
				swap(a[i], a[rev[i] >> shift]);
			}
		}
		for (ll i = 1; i < n; i <<= 1) {
			for (ll j = 0; j < n; j += i << 1) {
				for (ll k = 0; k < i; ++k) {
					ll x = a[j + k], y = mul(a[j + k + i], roots[i + k]);
					a[j + k] = add(x, y);
					a[j + k + i] = sub(x, y);
				}
			}
		}
	}
 
	vector<ll> padd(vector<ll> a, vector<ll> b) {
		if (a.size() < b.size()) {
			a.resize(b.size());
		}
		for (ll i = 0; i < b.size(); ++i) {
			a[i] = add(a[i], b[i]);
		}
		return a;
	}
 
	vector<ll> psub(vector<ll> a, vector<ll> b) {
		if (a.size() < b.size()) {
			a.resize(b.size());
		}
		for (ll i = 0; i < b.size(); ++i) {
			a[i] = sub(a[i], b[i]);
		}
		return a;
	}
 
	vector<ll> pmul(vector<ll> a, vector<ll> b, bool equal = false) {
		ll need = a.size() + b.size() - 1, nbase = 0;
		while (1 << nbase < need) {
			++nbase;
		}
		ensure_base(nbase);
		ll size = 1 << nbase;
		a.resize(size);
		b.resize(size);
		dft(a);
		if (equal) {
			b = a;
		}
		else {
			dft(b);
		}
		ll inv = power(size, md - 2);
		for (ll i = 0; i < size; ++i) {
			a[i] = mul(mul(a[i], b[i]), inv);
		}
		reverse(a.begin() + 1, a.end());
		dft(a);
		a.resize(need);
		return a;
	}
 
	vector<ll> psqr(vector<ll> a) {
		return pmul(a, a, true);
	}
 
	vector<ll> pinv(vector<ll> a) {
		ll n = a.size(), m = n + 1 >> 1;
		if (n == 1) {
			return vector<ll>(1, power(a[0], md - 2));
		}
		else {
			vector<ll> b = pinv(vector<ll>(a.begin(), a.begin() + m));
			ll need = n << 1, nbase = 0;
			while (1 << nbase < need) {
				++nbase;
			}
			ensure_base(nbase);
			ll size = 1 << nbase;
			a.resize(size);
			b.resize(size);
			dft(a);
			dft(b);
			ll inv = power(size, md - 2);
			for (ll i = 0; i < size; ++i) {
				a[i] = mul(mul(sub(2, mul(a[i], b[i])), b[i]), inv);
			}
			reverse(a.begin() + 1, a.end());
			dft(a);
			a.resize(n);
			return a;
		}
	}
 
	vector<ll> pdiv(vector<ll> a, vector<ll> b) {
		ll n = a.size(), m = b.size();
		if (n < m) {
			return vector<ll>();
		}
		reverse(a.begin(), a.end());
		reverse(b.begin(), b.end());
		b.resize(n - m + 1);
		b = pmul(a, pinv(b));
		b.erase(b.begin() + n - m + 1, b.end());
		reverse(b.begin(), b.end());
		return b;
	}
 
	vector<ll> pmod(vector<ll> a, vector<ll> b) {
		ll n = a.size(), m = b.size();
		if (n < m) {
			return a;
		}
		vector<ll> c = pmul(pdiv(a, b), b);
		for (ll i = 0; i < m - 1; ++i) {
			c[i] = sub(a[i], c[i]);
		}
		c.resize(m - 1);
		return c;
	}
 
	vector<ll> pw(vector<ll> a, ll p) {
		vector<ll> res = { 1 };
		while (p) {
			if (p & 1) {
				res = pmul(res, a);
			}
			a = psqr(a);
			p >>= 1;
		}
		return res;
	}
 
}