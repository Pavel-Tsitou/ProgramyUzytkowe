#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <map>
using namespace std;

const long long N = 10'000'003, HASH_PRIME = 109;

array<long long, 9> left_precalculated[100'000'228];

vector<int> left_side[N];
vector<int> right_side[N];

int n, digits[100];


void next_digit() {
    int cur = n - 1;
    digits[n - 1]++;
    while (digits[cur] == 10) {
        digits[cur] = 0;
        cur--;
        if (cur < 0) break;
        digits[cur]++;
    }
}


void precalculate() {
    const int pow_10 = pow(10, n);
    for (int t = 0; t < pow_10; t++) {
        for (int d = 1; d < 10; d++) {
            long long sum = 0, prod = 1;
            for (int i = 0; i < n; i++) {
                sum += prod * digits[i];
                prod *= d * d;
            }
            left_precalculated[t][d - 1] = sum;
        }

        next_digit();
    }
    cout << "Precalculating is done done done!\n";
}


bool eq(int left_ind, int right_ind, bool *mask) {
    for (int i = 0; i < 9; i++) {
        if (not mask[i]) continue;
        if (left_precalculated[left_ind][i] != (i + 1) * left_precalculated[right_ind][i]) return false;
    }
    return true;
}


long long my_pow(long long x, int n) {
    long long prod = 1;
    for (int i = 0; i < n; i++) prod *= x;
    return prod;
}


long long calculate_with_mask(int mask_integer) {
    int n_bits = 0, last_coeff = 1;
    bool mask[9];
    for (int i = 0; i < 9; i++) {
        mask[i] = mask_integer % 2;
        mask_integer /= 2;
        if (mask[i]) {
            n_bits++;
            last_coeff *= i + 1;
        }
    }
    if (last_coeff > 9) return 0;

    for (int i = 0; i < N; i++) {
        left_side[i].clear();
        right_side[i].clear();
    }


    const int pow_10 = my_pow(10, n);
    for (int t = 0; t < pow_10; t++) {
        long long hash = 0, prod = 1;
        for (int i = 0; i < 9; i++) {
            if (mask[i]) hash += (left_precalculated[t][i] * (i + 1) + 1) % N * prod % N;
            prod = prod * HASH_PRIME % N;
        }
        hash %= N;
        right_side[hash].push_back(t);
        
        if (t < pow_10 / 10) continue;

        hash = 0; prod = 1;
        for (int i = 0; i < 9; i++) {
            if (mask[i]) hash += (left_precalculated[t][i] + 1) % N * prod % N;
            prod = prod * HASH_PRIME % N;
        }
        hash %= N;
        left_side[hash].push_back(t);
    }

    long long ans = 0;
    for (int i = 0; i < N; i++) {
        if (left_side[i].size() == 0 or right_side[i].size() == 0) continue;
        
        if (left_side[i].size() * 1LL * right_side[i].size() < 50) {
            for (auto u : left_side[i]) {
                for (auto v : right_side[i]) {
                    if (eq(u, v, mask)) ans++;
                }
            }
        } else {
            map<array<long long, 9>, long long> mp;
            for (auto u : left_side[i]) {
                array<long long, 9> temp;
                for (int j = 0; j < 9; j++) {
                    if (mask[j]) temp[j] = left_precalculated[u][j];
                    else temp[j] = -1;
                }
                mp[temp] += 1;
            }
            for (auto v : right_side[i]) {
                array<long long, 9> temp;
                for (int j = 0; j < 9; j++) {
                    if (mask[j]) temp[j] = left_precalculated[v][j] * (j + 1);
                    else temp[j] = -1;
                }
                ans += mp[temp];
            }
        }
    }

    if (n_bits % 2 == 0) return -ans;
    return ans;
}


int main() {
    cin >> n;

    precalculate();

    long long ans = 0;
    const int pow2 = my_pow(2, 9);
    for (int mask = 1; mask < pow2; mask++) {
        long long tmp = calculate_with_mask(mask);
        cout << "mask " << mask << " temp ans " << tmp << endl;
        ans += tmp;
    }

    cout << "ans = " << ans + my_pow(10, 2 * n - 1) << endl;

    return 0;
}