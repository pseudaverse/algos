template<typename T>
struct sparse {
	vector<vector<T>> sp;
	vector<T>lg;
	int n, k;
	T f(T& x, T& y) {
		return min(x, y);
	}
	sparse(vector<T>& v) {
		n = v.size();
		lg.resize(n + 1);
		lg[1] = 0; for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;
		k = lg[n];
		sp.resize(n);
		for (int i = 0; i < n; i++) {
			sp[i].resize(k + 1);
			sp[i][0] = v[i];
		}
		for (int j = 1; j <= k; j++) {
			for (int i = 0; i + (1 << j) <= n; i++) {
				sp[i][j] = f(sp[i][j - 1], sp[i + (1 << (j - 1))][j - 1]);
			}
		}
	}
	T get(int l, int r) {
		int j = lg[r - l + 1];
		return f(sp[l][j], sp[r - (1 << j) + 1][j]);
	}
};