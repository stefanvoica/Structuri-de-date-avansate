#include <bits/stdc++.h>
using namespace std;
ofstream f("heap.in");

void generate_input()
{
    int N = rand() % 100 + 1;
    int Q = rand() % 200000 + 1;

    f << N << " " << Q << "\n";

    int weights[] = {50, 25, 25};
    int weightSum = weights[0] + weights[1] + weights[2];

    for (int i = 0; i < Q; i++)
    {
        int op_type = 0;
        int randNum = rand() % weightSum;
        if (randNum < weights[0])
            op_type = 1;
        else if (randNum < weights[0] + weights[1])
            op_type = 2;
        else
            op_type = 3;

        if (op_type == 2)
        {
            int my_set = rand() % N + 1;
            f << op_type << " " << my_set << "\n";
        }
        else if (op_type == 1)
        {
            int my_set = rand() % N + 1;
            int x = rand() % 2000000000 + 1;
            f << op_type << " " << my_set << " " << x << "\n";
        }
        else
        {
            int my_set1 = rand() % N + 1;
            int my_set2 = rand() % N + 1;
            f << op_type << " " << my_set1 << " " << my_set2 << "\n";
        }
    }

    f.close();
}

int main()
{
    generate_input();
    return 0;
}
