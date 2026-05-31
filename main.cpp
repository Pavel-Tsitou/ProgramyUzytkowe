#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
#include <iterator>
using namespace std;

const int N = 10'000'019, hash_prime = 1019;

vector<vector<long long>> left_side[N];
vector<vector<long long>> right_side[N];

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


int calculate_with_mask(int mask_integer) {
    for (int i = 0; i < N; i++) {
        left_side[i].clear();
        right_side[i].clear();
    }

    int n_bits = 0;
    bool mask[9];
    for (int i = 0; i < 9; i++) {
        mask[i] = mask_integer % 2;
        mask_integer /= 2;
        if (mask[i]) n_bits++;
    }

    const int pow_10 = pow(10, n);
    for (int t = 0; t < pow_10; t++) {
        vector<long long> temp; temp.resize(9, -1);

        for (int d = 1; d < 10; d++) {
            if (not mask[d - 1]) continue;

            long long sum = 0, prod = 1;
            for (int i = 0; i < n; i++) {
                sum += prod * digits[i];
                prod *= d * d;
            }
            temp[d - 1] = sum;
        }

        long long hash = 0, prod = 1;
        for (int i = 0; i < 9; i++) {
            hash += (temp[i] + 1) % N * prod;
            prod = prod * hash_prime % N;
        }
        hash %= N;
        left_side[hash].push_back(temp);

        for (int d = 1; d < 10; d++) {
            if (not mask[d - 1]) continue;
            temp[d - 1] *= d;
        }
        
        hash = 0; prod = 1;
        for (int i = 0; i < 9; i++) {
            hash += (temp[i] + 1) % N * prod;
            prod = prod * hash_prime % N;
        }
        hash %= N;
        right_side[hash].push_back(temp);

        next();
    }

    int ans = 0;
    for (int i = 0; i < N; i++) {
        if (left_side[i].size() == 0 or right_side[i].size() == 0) continue;
        std::map<vector<long long>, int> mp;
        for (auto &u : left_side[i]) {
            mp[u] += 1;
        }
        for (auto &u : right_side[i]) {
            ans += mp[u];
        }
    }

    return ans * pow(-1, n_bits + 1);
}


int main() {
    cin >> n;

    int ans = 0;

    const int pow2 = pow(2, 9);
    for (int mask = 1; mask < pow2; mask++) {
        cout << mask << endl;
        ans = ans + calculate_with_mask(mask);
    }

    cout << "ans = " << ans << endl;

    return 0;
}