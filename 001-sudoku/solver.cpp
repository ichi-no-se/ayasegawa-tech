#include <iostream>
#include <bit>

int main()
{
    int row_id[81], col_id[81], box_id[81], num_cells_to_fill, ord[81];
    unsigned int board[81], row_ap[9], col_ap[9], box_ap[9];

    for (int i = 0; i < 81; i++)
    {
        row_id[i] = i / 9;
        col_id[i] = i % 9;
        box_id[i] = i / 27 * 3 + i % 9 / 3;
    }

    for (int i = 0; i < 9; i++)
    {
        row_ap[i] = 0u;
        col_ap[i] = 0u;
        box_ap[i] = 0u;
    }

    num_cells_to_fill = 0;

    for (int i = 0; i < 9; i++)
    {
        char buf[10];
        scanf("%s", buf);
        for (int j = 0; j < 9; j++)
        {
            const auto k = i * 9 + j;
            if (buf[j] == '?')
            {
                board[k] = 1u;
                ord[num_cells_to_fill] = k;
                num_cells_to_fill++;
            }
            else
            {
                board[k] = 1u << (buf[j] - '0');
                row_ap[row_id[k]] |= board[k];
                col_ap[col_id[k]] |= board[k];
                box_ap[box_id[k]] |= board[k];
            }
        }
    }

    int p = 0;
    while (p != num_cells_to_fill)
    {
        if (board[ord[p]] == 1u)
        {
            int best_poss = -1; // 9 - (置ける可能性のある数)
            int best_imp = -1;  // 24 - (与える影響)
            int best_idx = -1;
            for (int i = p; i < num_cells_to_fill; i++)
            {
                const auto t = ord[i];
                const auto ri = row_id[t];
                const auto ci = col_id[t];
                const auto bi = box_id[t];

                const int cur_poss = std::popcount(row_ap[ri] | col_ap[ci] | box_ap[bi]);
                if (cur_poss > best_poss)
                {
                    best_poss = cur_poss;
                    best_idx = i;
                    best_imp = std::popcount(row_ap[ri]) + std::popcount(col_ap[ci]) + std::popcount(box_ap[bi]);
                }
                else if (cur_poss == best_poss)
                {
                    const auto cur_imp = std::popcount(row_ap[ri]) + std::popcount(col_ap[ci]) + std::popcount(box_ap[bi]);
                    if (cur_imp < best_imp)
                    {
                        best_imp = cur_imp;
                        best_idx = i;
                    }
                }
            }
            std::swap(ord[p], ord[best_idx]);
        }
        const auto id = ord[p];
        const auto ri = row_id[id];
        const auto ci = col_id[id];
        const auto bi = box_id[id];

        const auto board_not = ~board[id];
        row_ap[ri] &= board_not;
        col_ap[ci] &= board_not;
        box_ap[bi] &= board_not;

        const auto ap = row_ap[ri] | col_ap[ci] | box_ap[bi];

        do
        {
            board[id] <<= 1;
        } while (ap & board[id]);
        if (board[id] == 1u << 10)
        {
            board[id] = 1u;
            p--;
        }
        else
        {
            row_ap[ri] |= board[id];
            col_ap[ci] |= board[id];
            box_ap[bi] |= board[id];
            p++;
        }
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int k = i * 9 + j;
            int cnt = 0;
            while (board[k])
            {
                cnt++;
                board[k] >>= 1;
            }
            printf("%d", cnt);
        }
        printf("\n");
    }
}
