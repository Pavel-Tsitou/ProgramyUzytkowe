#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <array>
#include <algorithm>
#include <iterator>
using namespace std;

const int N = 10'000'003, HASH_PRIME = 109;

array<long long, 9> left_precalculated[10'000'228];
array<long long, 9> right_precalculated[10'000'228];

vector<int> left_side[N];
vector<int> right_side[N];

int n, digits[100];


void next() {
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
        // cout << "precalculating " << t << '\n';
        for (int d = 1; d < 10; d++) {
            long long sum = 0, prod = 1;
            for (int i = 0; i < n; i++) {
                sum += prod * digits[i];
                prod *= d * d;
            }
            left_precalculated[t][d - 1] = sum;
            right_precalculated[t][d - 1] = sum * d;
        }

        next();
    }
    cout << "Precalculating is done done done!\n";
}


bool eq(array<long long, 9> arr1, array<long long, 9> arr2, bool *mask) {
    for (int i = 0; i < 9; i++) {
        if (not mask[i]) continue;
        if (arr1[i] != arr2[i]) return false;
    }
    return true;
}


int calculate_with_mask(int mask_integer) {
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
    if (last_coeff > 9) return pow(-1, n_bits + 1);

    for (int i = 0; i < N; i++) {
        left_side[i].clear();
        right_side[i].clear();
    }


    const int pow_10 = pow(10, n);
    for (int t = 0; t < pow_10; t++) {
        long long hash = 0, prod = 1;
        for (int i = 0; i < 9; i++) {
            if (mask[i]) hash += (left_precalculated[t][i] + 1) % N * prod % N;
            prod = prod * HASH_PRIME % N;
        }
        hash %= N;
        left_side[hash].push_back(t);
        
        hash = 0; prod = 1;
        for (int i = 0; i < 9; i++) {
            if (mask[i]) hash += (right_precalculated[t][i] + 1) % N * prod % N;
            prod = prod * HASH_PRIME % N;
        }
        hash %= N;
        right_side[hash].push_back(t);
    }

    int ans = 0;
    for (int i = 0; i < N; i++) {
        if (left_side[i].size() == 0 or right_side[i].size() == 0) continue;
        
        // cout << i + 1 << ". " << left_side[i].size() << ' ' << right_side[i].size() << '\n';
        
        if (left_side[i].size() * right_side[i].size() < 50) {
            for (auto u : left_side[i]) {
                for (auto v : right_side[i]) {
                    if (eq(left_precalculated[u], right_precalculated[v], mask)) ans++;
                }
            }
        } else {
            map<array<long long, 9>, int> mp;
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
                    if (mask[j]) temp[j] = right_precalculated[v][j];
                    else temp[j] = -1;
                }
                ans += mp[temp];
            }
        }
    }

    return ans * pow(-1, n_bits + 1);
}


int main() {
    cin >> n;

    precalculate();

    int ans = 0;
    const int pow2 = pow(2, 9);
    for (int mask = 1; mask < pow2; mask++) {
        long long tmp = calculate_with_mask(mask);
        cout << "mask " << mask << " temp ans " << tmp << endl;
        ans += tmp;
    }

    cout << "ans = " << ans << endl;

    return 0;
}