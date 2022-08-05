#include <bits/stdc++.h>
using namespace std;
int main()
{
    cin.tie(0)->sync_with_stdio(0);

    int n = 10;

    ofstream input("beautiful-strings.in");
    ofstream output("beautiful-strings.out");

    input << (1 << n) << "\n";

    for (int mask = 0; mask < (1 << n); mask++)
    {
        auto hash = [&](int left, int right)
        {
            return (mask >> left) & ((1 << (right - left + 1)) - 1);
        };

        for (int i = 0; i < n; i++)
            if (mask >> i & 1)
                input << "1";
            else
                input << "0";
        input << "\n";

        int counter = 0;

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
                            int current = hash(cursor, cursor + length - 1);
                            cursor += length;
                            return current;
                        };

                        int a1 = consume(length_a);
                        int a2 = consume(length_a);
                        int b1 = consume(length_b);
                        int c = consume(length_c);
                        int a3 = consume(length_a);
                        int b2 = consume(length_b);

                        if (a1 == a2 && a2 == a3 && b1 == b2)
                            counter++;
                    }
            }
        output << counter << "\n";
    }
}