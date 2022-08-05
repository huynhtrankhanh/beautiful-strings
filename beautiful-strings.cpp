#include <bits/stdc++.h>
using namespace std;
int z[5001][5001], square_substring_count[5001][5001];
int main()
{
    cin.tie(0)->sync_with_stdio(0);

    int t;
    cin >> t;

    while (t--)
    {
        string s;
        cin >> s;

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

        int result = 0;

        // int unofficial = 0;

        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
            {
                int w = min(j - i, z[i][j]);

                if (w == 0)
                    continue;

                // if (square_substring_count[i][w - 1] != 0)
                // {
                //     for (int length_a = 1; length_a <= w - 1; length_a++)
                //     {
                //         int exists = square_substring_count[i][length_a] - square_substring_count[i][length_a - 1];

                //         if (!exists)
                //             continue;

                //         // AABCAB

                //         int length_b = w - length_a;

                //         int left = i - length_a;
                //         int right = j + w - 1;

                //         int length = right - left + 1;

                //         cout << "Recorded: left = " << left << ", right = " << right << ", length_a = " << length_a << ", length_b = " << length_b << "\n";
                //         cout << "Middle interval: " << i << " " << j - 1 << "\n";
                //         if (3 * length_a + 2 * length_b < length)
                //         {
                //             unofficial++;
                //         }
                //     }
                // }

                result += square_substring_count[i][w - 1];
            }

        cout << result << "\n";
        // cout << "Unofficial: " << unofficial << "\n";
    }
}
