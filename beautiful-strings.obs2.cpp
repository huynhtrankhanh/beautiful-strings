#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for (int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

/**
 * Author: Simon Lindholm
 * Date: 2015-03-15
 * License: CC0
 * Source: own work
 * Description: Self-explanatory methods for string hashing.
 * Status: stress-tested
 */
#pragma once

// Arithmetic mod 2^64-1. 2x slower than mod 2^64 and more
// code, but works on evil test data (e.g. Thue-Morse, where
// ABBA... and BAAB... of length 2^10 hash the same mod 2^64).
// "typedef ull H;" instead if you think test data is random,
// or work mod 10^9+7 if the Birthday paradox is not a problem.
struct H
{
    typedef uint64_t ull;
    ull x;
    H(ull x = 0) : x(x) {}
#define OP(O, A, B)                         \
    H operator O(H o)                       \
    {                                       \
        ull r = x;                          \
        asm(A "addq %%rdx, %0\n adcq $0,%0" \
            : "+a"(r)                       \
            : B);                           \
        return r;                           \
    }
    OP(+, , "d"(o.x))
    OP(*, "mul %1\n", "r"(o.x)
       : "rdx")
    H operator-(H o)
    {
        return *this + ~o.x;
    }
    ull get() const { return x + !~x; }
    bool operator==(H o) const { return get() == o.get(); }
    bool operator<(H o) const { return get() < o.get(); }
};
static const H C = (ll)1e11 + 3; // (order ~ 3e9; random also ok)

struct HashInterval
{
    vector<H> ha, pw;
    HashInterval(string &str) : ha(sz(str) + 1), pw(ha)
    {
        pw[0] = 1;
        rep(i, 0, sz(str))
            ha[i + 1] = ha[i] * C + str[i],
                   pw[i + 1] = pw[i] * C;
    }
    H hashInterval(int a, int b)
    { // hash [a, b)
        return ha[b] - ha[a] * pw[b - a];
    }
};

vector<H> getHashes(string &str, int length)
{
    if (sz(str) < length)
        return {};
    H h = 0, pw = 1;
    rep(i, 0, length)
        h = h * C + str[i],
        pw = pw * C;
    vector<H> ret = {h};
    rep(i, length, sz(str))
    {
        ret.push_back(h = h * C + str[i] - pw * str[i - length]);
    }
    return ret;
}

H hashString(string &s)
{
    H h{};
    for (char c : s)
        h = h * C + c;
    return h;
}

int z[5001][5001], square_substring_count[5001][5001];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin.exceptions(cin.failbit);

    string s;
    cin >> s;

    HashInterval hashes(s);

    auto hash = [&](int left, int right)
    {
        return hashes.hashInterval(left, right + 1);
    };

    int n = s.size();

    for (int i = n - 1; i >= 0; i--)
        for (int j = n - 1; j >= 0; j--)
        {
            if (s[i] == s[j])
            {
                if (i != n - 1 && j != n - 1)
                    z[i][j] = 1 + z[i + 1][j + 1];
                else
                    z[i][j] = 1;
            }
            else
                z[i][j] = 0;
        }

    fill(square_substring_count[0], square_substring_count[0] + n + 1, 0);
    for (int i = 1; i < n; i++)
    {
        int counter = 0;
        fill(square_substring_count[i], square_substring_count[i] + n + 1, 0);
        for (int length = 1; length <= min(i, n - i); length++)
        {
            counter += z[i - length][i] >= length;
            square_substring_count[i][length] = counter;
        }

        for (int length = min(i, n - i) + 1; length <= n; length++)
            square_substring_count[i][length] = counter;
    }

    // (middle_part_left, middle_part_right, length_a, length_b)
    using information_t = tuple<int, int, int, int>;

    vector<information_t> partitions;

    for (int left = 0; left < n; left++)
        for (int right = left; right < n; right++)
        {
            int length = right - left + 1;
            for (int length_a = 1; length_a <= length; length_a++)
                for (int length_b = 1; length_b <= length; length_b++)
                {
                    int total = length_a * 3 + length_b * 2;
                    if (total >= length)
                        continue;
                    int length_c = length - total;

                    int cursor = left;

                    auto consume = [&](int length)
                    {
                        auto current = hash(cursor, cursor + length - 1);
                        cursor += length;
                        return current;
                    };

                    auto a1 = consume(length_a);
                    auto a2 = consume(length_a);
                    auto b1 = consume(length_b);
                    auto c = consume(length_c);
                    auto a3 = consume(length_a);
                    auto b2 = consume(length_b);

                    if (a1 == a2 && a2 == a3 && b1 == b2)
                    {
                        // record the partition
                        // A|ABC|AB
                        partitions.emplace_back(left + length_a, left + length_a + length_a + length_b + length_c - 1, length_a, length_b);
                    }
                }
        }

    sort(partitions.begin(), partitions.end());

    for (auto &[middle_part_left, middle_part_right, length_a, length_b] : partitions)
    {
        cout << "Middle part left = " << middle_part_left << ", middle part right = " << middle_part_right << ", length A = " << length_a << ", length B = " << length_b << "\n";

        int left = middle_part_left - length_a;
        int length_c = middle_part_right - middle_part_left + 1 - length_a - length_b;
        int right = left + length_a + length_a + length_b + length_c + length_a + length_b - 1;

        cout << "String in question: " << s << "\n";
        cout << "                    ";

        for (int i = 0; i < left; i++)
            cout << " ";
        for (int i = left; i <= right; i++)
            if (middle_part_left <= i && i <= middle_part_right)
            {
                cout << "^";
            }
            else
                cout << " ";
        cout << "\n";
        cout << "                    ";
        for (int i = 0; i < left; i++)
            cout << " ";
        for (int i = 0; i < length_a; i++)
            cout << "A";
        for (int i = 0; i < length_a; i++)
            cout << "A";
        for (int i = 0; i < length_b; i++)
            cout << "B";
        for (int i = 0; i < length_c; i++)
            cout << "C";
        for (int i = 0; i < length_a; i++)
            cout << "A";
        for (int i = 0; i < length_b; i++)
            cout << "B";
        cout << "\n";
        int predicted_count = min(z[middle_part_left][middle_part_right + 1], middle_part_right - middle_part_left) - length_a;
        cout << "Predicted count for this square substring: " << predicted_count << "\n";
        cout << "--------------------\n";
    }

    cout << "Total count: " << partitions.size() << "\n";
}