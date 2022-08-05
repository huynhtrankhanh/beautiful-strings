#include <bits/stdc++.h>
using namespace std;
#define int long long
int z[5001][5001], square_substring_accumulated_length[5001][5001], square_substring_count[5001][5001];
signed main()
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

        fill(square_substring_accumulated_length[0], square_substring_accumulated_length[0] + n + 1, 0);
        for (int i = 1; i < n; i++)
        {
            int counter = 0;
            fill(square_substring_accumulated_length[i], square_substring_accumulated_length[i] + n + 1, 0);
            for (int length = 1; length <= min(i, n - i); length++)
            {
                if (z[i - length][i] >= length)
                    counter += length;
                square_substring_accumulated_length[i][length] = counter;
            }

            for (int length = min(i, n - i) + 1; length <= n; length++)
                square_substring_accumulated_length[i][length] = counter;
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

        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
            {
                int w = min(j - i, z[i][j]);

                if (w == 0)
                    continue;

                result += square_substring_count[i][w - 1] * min(z[i][j], j - i - 1) - square_substring_accumulated_length[i][w - 1];
            }

        cout << result << "\n";
    }
}
