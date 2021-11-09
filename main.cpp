#include <iostream>
#include <ctime>
#include <vector>
#include <array>
#include <algorithm>
#include <cmath>
#include <numeric>

constexpr int MAX_N = 200;

using dist_t = std::array<std::array<int, MAX_N>, MAX_N>;
dist_t d;
struct edge { int v, u; };
int n, m, k;
clock_t start;

bool improved[MAX_N];
double max_delta = 0;
double max_score = 0;
int max_denom = 0;
std::vector<struct edge> best_solution;

constexpr int BRANCHING_LIST[] = {500, 100, 50, 40, 30};

double max_diff = 0;
dist_t a, b;

void f(int k, int branching, double sum = 0, const dist_t &d = d, std::vector<struct edge> solution = {}) {
	if (double(clock() - start) / CLOCKS_PER_SEC  > 3.9) return;
	k--;

	std::vector<std::pair<double, struct edge>> edges;
	edges.reserve(MAX_N * MAX_N);
	for (int x = 0; x < n; ++x)
		for (int y = x+1; y < n; ++y)
			if (d[x][y] > 1) {
				double dt = 0;
				for (int i = 0; i < n; ++i) {
					if (d[i][x] > d[i][y] + 1 || d[i][y] > d[i][x] + 1) {
						for (int j = 0; j < n; ++j) {
							int dist = std::min(d[i][x] + d[y][j], d[i][y] + d[x][j]) + 1;
							if (dist < d[i][j]) {
								dt -= 1. / d[i][j];
								dt += 1. / dist;
							}
						}
					}
				}
				max_denom = std::max(max_denom, denom);
				edges.push_back({dt, {x, y}});
			}

	std::sort(edges.begin(), edges.end(), [](const auto &a, const auto &b) {
		return a.first > b.first;
	});

	if (edges.size() >= branching)
		edges.resize(branching);

	for (int branch = 0; branch < edges.size(); ++branch) {
		auto new_d = d;
		auto new_edge = edges[branch].second;
		int x = new_edge.v, y = new_edge.u;
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j) {
				int dist = std::min(d[i][x] + d[y][j], d[i][y] + d[x][j]) + 1;
				new_d[i][j] = std::min(d[i][j], dist);
			}
		auto new_sol = solution;
		new_sol.push_back(new_edge);
		auto new_sum = sum + edges[branch].first;
		if (!k) {
			double score = 0;
			for (int i = 0; i < n; ++i)
				for (int j = 0; j < n; ++j)
					if (i != j)
						score += 1. / new_d[i][j];
			if (score > max_score) {
				max_delta = new_sum;
				best_solution = new_sol;
				max_score = score;
			}
			return;
		} else {
			f(k, std::max(branching / 10, 1), new_sum, new_d, new_sol);
		}
	}
}

int main() {
	start = clock();
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::cin >> n >> m >> k;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			d[i][j] = MAX_N;
		}
		d[i][i] = 0;
	}

	for (int i = m; i--; ) {
		int v, u;
		std::cin >> v >> u, --v, --u;
		d[v][u] = d[u][v] = 1;
	}

	for (int k = 0; k < n; ++k)
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);

	int branching = 10;
	if (k <= 5) {
		branching = BRANCHING_LIST[k-1] * pow(double(200) / n, 1);
	}
	std::cerr << "branching: " <<  branching << "\n";
	f(k, branching);
	auto end = clock();
	std::cerr << "time: " << double(end - start) / CLOCKS_PER_SEC << "\n";
  
	for (auto edge: best_solution) {
		std::cout << edge.v+1 << " " << edge.u+1 << "\n";
	}
  
	std::cerr << "delta: " << max_delta << "\n";
	std::cerr << "diff: " << max_diff << "\n";
	std::cerr << "denom: " << max_denom << "\n";
	std::cerr << "score: " << max_score << "\n";
}
