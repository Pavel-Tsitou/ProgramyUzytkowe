#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <map>
using namespace std;

int n, digits[100];


void next_digits() {
    int cur = n - 1;
    digits[n - 1]++;
    while (digits[cur] == 10) {
        digits[cur] = 0;
        cur--;
        if (cur < 0) break;
        digits[cur]++;
    }
}

long long calc_pol(int x) {
    long long sum = 0, prod = 1;
    for (int i = 0; i < n; i++) {
        sum += digits[i] * prod;
        prod = prod * x;
    }
    return sum;
}

bool check_pol() {
    if (calc_pol(0) == 0LL) return true;
    if (calc_pol(-1) == 0LL) return true;
    if (calc_pol(-2) == 0LL) return true;
    if (calc_pol(-3) == 0LL) return true;
    if (calc_pol(-4) == 0LL) return true;
    if (calc_pol(-5) == 0LL) return true;
    if (calc_pol(-6) == 0LL) return true;
    if (calc_pol(-7) == 0LL) return true;
    if (calc_pol(-8) == 0LL) return true;
    if (calc_pol(-9) == 0LL) return true;
    return false;
}


int main() {
    cin >> n;

    int ans = 0;
    int pow_10 = pow(10, n);
    for (int i = 0; i < pow_10; i++) {
        if (check_pol()) {
            ans++;
        }
        next_digits();
    }
    
    cout << "bruteforce ans = " << ans << endl;

    return 0;
}