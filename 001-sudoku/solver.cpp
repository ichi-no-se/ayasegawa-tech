#include <bit>
#include <iostream>

int main() {
    int d_board[81], d_ord[81][2], d_row[9], d_col[9], d_box[9], d_his[81],
        d_bit_box_id[81], d_num_cells_to_fill;
    int c_ord[81];
    unsigned int c_board[81], c_row_ap[9], c_col_ap[9], c_box_ap[9],
        c_num_cells_to_fill;
    int row_id[81], col_id[81], box_id[81];

    for (int i = 0; i < 9; i++) {
        c_row_ap[i] = 0u;
        c_col_ap[i] = 0u;
        c_box_ap[i] = 0u;
        d_row[i] = 0;
        d_col[i] = 0;
        d_box[i] = 0;
    }

    d_num_cells_to_fill = 0;
    c_num_cells_to_fill = 0;

    for (int i = 0; i < 9; i++) {
        char buf[10];
        scanf("%s", buf);
        for (int j = 0; j < 9; j++) {
            int k = i * 9 + j;
            row_id[k] = i;
            col_id[k] = j;
            box_id[k] = i / 3 * 3 + j / 3;
            d_bit_box_id[k] = 1 << box_id[k];
            if (buf[j] == '?') {
                d_board[k] = -1;
                c_board[k] = 1u;
                c_ord[c_num_cells_to_fill] = k;
                c_num_cells_to_fill++;
            } else {
                int d_t = buf[j] - '1';
                d_board[k] = d_t;
                d_row[d_t] |= 1 << i;
                d_col[d_t] |= 1 << j;
                d_box[d_t] |= d_bit_box_id[k];

                c_board[k] = 1u << (buf[j] - '0');
                c_row_ap[row_id[k]] |= c_board[k];
                c_col_ap[col_id[k]] |= c_board[k];
                c_box_ap[box_id[k]] |= c_board[k];
            }
        }
    }
    // d 側の前処理
    int d_row_appear_cnt[9];
    for (int i = 0; i < 9; i++) {
        d_row_appear_cnt[i] = std::popcount((unsigned int)d_row[i]);
    }
    for (int i = 0; i < 9; i++) {
        int max_appar_cnt = -1;
        int cand = -1;
        for (int j = 0; j < 9; j++) {
            if (d_row_appear_cnt[j] > max_appar_cnt) {
                max_appar_cnt = d_row_appear_cnt[j];
                cand = j;
            }
        }
        d_row_appear_cnt[cand] = -1;
        for (int j = 0; j < 9; j++) {
            if (~d_row[cand] & (1 << j)) {
                d_ord[d_num_cells_to_fill][0] = cand;
                d_ord[d_num_cells_to_fill][1] = j * 9;
                d_num_cells_to_fill++;
            }
        }
    }
    for (int i = 0; i < 81; i++) {
        d_his[i] = -1;
    }

    int d_p = 0;
    int c_p = 0;

    while (true) {
        for (int _ = 0; _ < 10000 && d_p != d_num_cells_to_fill; _++) {
            int num = d_ord[d_p][0];
            int row_idx = d_ord[d_p][1];
            int &c = d_col[num];
            int &b = d_box[num];
            if (d_his[d_p] != -1) {
                c &= ~(1 << d_his[d_p]);
                b &= ~d_bit_box_id[row_idx + d_his[d_p]];
                d_board[row_idx + d_his[d_p]] = -1;
            }
            while (true) {
                d_his[d_p]++;
                if (d_his[d_p] == 9) {
                    d_his[d_p] = -1;
                    d_p--;
                    break;
                }
                if (~c & (1 << d_his[d_p]) &&
                    ~b & d_bit_box_id[row_idx + d_his[d_p]] &&
                    d_board[row_idx + d_his[d_p]] == -1) {
                    c |= 1 << d_his[d_p];
                    b |= d_bit_box_id[row_idx + d_his[d_p]];
                    d_board[row_idx + d_his[d_p]] = num;
                    d_p++;
                    break;
                }
            }
        }
        if (d_p == d_num_cells_to_fill) {
            break;
        }
        for (int _ = 0; _ < 1000 && c_p != c_num_cells_to_fill; _++) {
            if (c_board[c_ord[c_p]] == 1u) {
                int best_poss = -1;  // 9 - (置ける可能性のある数)
                int best_imp = -1;   // 24 - (与える影響)
                int best_idx = -1;
                for (int i = c_p; i < c_num_cells_to_fill; i++) {
                    const auto t = c_ord[i];
                    const auto ri = row_id[t];
                    const auto ci = col_id[t];
                    const auto bi = box_id[t];

                    const int cur_poss = std::popcount(
                        c_row_ap[ri] | c_col_ap[ci] | c_box_ap[bi]);
                    if (cur_poss > best_poss) {
                        best_poss = cur_poss;
                        best_idx = i;
                        best_imp = std::popcount(c_row_ap[ri]) +
                                   std::popcount(c_col_ap[ci]) +
                                   std::popcount(c_box_ap[bi]);
                    } else if (cur_poss == best_poss) {
                        const auto cur_imp = std::popcount(c_row_ap[ri]) +
                                             std::popcount(c_col_ap[ci]) +
                                             std::popcount(c_box_ap[bi]);
                        if (cur_imp < best_imp) {
                            best_imp = cur_imp;
                            best_idx = i;
                        }
                    }
                }
                std::swap(c_ord[c_p], c_ord[best_idx]);
            }
            const auto id = c_ord[c_p];
            const auto ri = row_id[id];
            const auto ci = col_id[id];
            const auto bi = box_id[id];

            const auto board_not = ~c_board[id];
            c_row_ap[ri] &= board_not;
            c_col_ap[ci] &= board_not;
            c_box_ap[bi] &= board_not;

            const auto ap = c_row_ap[ri] | c_col_ap[ci] | c_box_ap[bi];

            do {
                c_board[id] <<= 1;
            } while (ap & c_board[id]);
            if (c_board[id] == 1u << 10) {
                c_board[id] = 1u;
                c_p--;
            } else {
                c_row_ap[ri] |= c_board[id];
                c_col_ap[ci] |= c_board[id];
                c_box_ap[bi] |= c_board[id];
                c_p++;
            }
        }
        if (c_p == c_num_cells_to_fill) {
            break;
        }
    }

    if (d_p == d_num_cells_to_fill) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                printf("%d", d_board[i * 9 + j] + 1);
            }
            printf("\n");
        }
    } else if (c_p == c_num_cells_to_fill) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                int k = i * 9 + j;
                int cnt = -1;
                while (c_board[k]) {
                    cnt++;
                    c_board[k] >>= 1;
                }
                printf("%d", cnt);
            }
            printf("\n");
        }
    }
}
