#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SET '1'
#define UNSET '0'
#define EMPTY '.'

#define MIN_SIDE_LEN 3
#define MAX_SIDE_LEN 31

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

typedef struct __board_ {
    int side_len;
    int min_set;
    int max_set;
    int empty_count;
    char data[MAX_SIDE_LEN * MAX_SIDE_LEN];
} board_t;

static void print_board(const board_t * board) {
    for (int i = 0; i < board->side_len * board->side_len; ++i) {
        if (board->data[i] == EMPTY) {
            printf("  ");
        } else {
            printf(" %c", board->data[i]);
        }

        if (((i + 1) % board->side_len) == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

static int integer_sqrt(int size) {
    int side = 1;

    while (side < MAX_SIDE_LEN) {
        if (side * side == size) {
            return side;
        }
        if (side * side > size) {
            break;
        }
        side++;
    }

    return -1;
}

static int string_to_board(board_t * board, const char * buffer) {
    board->side_len = integer_sqrt(strlen(buffer) - 1);
    if (board->side_len < MIN_SIDE_LEN) {
        return 0;
    }

    memcpy(board->data, buffer, board->side_len * board->side_len);

    board->min_set = 1;
    board->max_set = board->side_len - 1;
    board->empty_count = 0;

    for (int i = 0; i < board->side_len * board->side_len; ++i) {
        if (board->data[i] == EMPTY) {
            board->empty_count++;
        }
    }

    return 1;
}

static int board_filled(const board_t * board) {
    return board->empty_count == 0;
}

static char inverse(char c) {
    return c == SET ? UNSET : SET;
}

static void fill_before_or_after_two_in_a_row(board_t * board) {
    int repeat = 1;

    while (repeat) {
        repeat = 0;

        for (int i = 0; i < board->side_len; ++i) {
            for (int j = 2; j < board->side_len; ++j) {
                if (board->data[i * board->side_len + j] == EMPTY) {
                    if (board->data[i * board->side_len + j - 1] != EMPTY && board->data[i * board->side_len + j - 1] == board->data[i * board->side_len + j - 2]) {
                        board->data[i * board->side_len + j] = inverse(board->data[i * board->side_len + j - 1]);
                        board->empty_count--;
                        repeat = 1;
                    }
                }

                if (board->data[(j) * board->side_len + i] == EMPTY) {
                    if (board->data[(j - 1) * board->side_len + i] != EMPTY && board->data[(j - 1) * board->side_len + i] == board->data[(j - 2) * board->side_len + i]) {
                        board->data[(j) * board->side_len + i] = inverse(board->data[(j - 1) * board->side_len + i]);
                        board->empty_count--;
                        repeat = 1;
                    }
                }

                if (board->data[i * board->side_len + (j - 2)] == EMPTY) {
                    if (board->data[i * board->side_len + (j - 2) + 1] != EMPTY && board->data[i * board->side_len + (j - 2) + 1] == board->data[i * board->side_len + (j - 2) + 2]) {
                        board->data[i * board->side_len + (j - 2)] = inverse(board->data[i * board->side_len + (j - 2) + 1]);
                        board->empty_count--;
                        repeat = 1;
                    }
                }

                if (board->data[(j - 2) * board->side_len + i] == EMPTY) {
                    if (board->data[(j - 2 + 1) * board->side_len + i] != EMPTY && board->data[(j - 2 + 1) * board->side_len + i] == board->data[(j - 2 + 2) * board->side_len + i]) {
                        board->data[(j - 2) * board->side_len + i] = inverse(board->data[(j - 2 + 1) * board->side_len + i]);
                        board->empty_count--;
                        repeat = 1;
                    }
                }
            }
        }
    }
}

static void fill_between_two(board_t * board) {
    int repeat = 1;

    while (repeat) {
        repeat = 0;

        for (int i = 0; i < board->side_len; ++i) {
            for (int j = 1; j < board->side_len - 1; ++j) {
                if (board->data[i * board->side_len + j] == EMPTY) {
                    if (board->data[i * board->side_len + j - 1] != EMPTY && board->data[i * board->side_len + j - 1] == board->data[i * board->side_len + j + 1]) {
                        board->data[i * board->side_len + j] = inverse(board->data[i * board->side_len + j - 1]);
                        board->empty_count--;
                        repeat = 1;
                    }
                }

                if (board->data[(j) * board->side_len + i] == EMPTY) {
                    if (board->data[(j - 1) * board->side_len + i] != EMPTY && board->data[(j - 1) * board->side_len + i] == board->data[(j + 1) * board->side_len + i]) {
                        board->data[(j) * board->side_len + i] = inverse(board->data[(j - 1) * board->side_len + i]);
                        board->empty_count--;
                        repeat = 1;
                    }
                }
            }
        }
    }
}

static void count_line_sets(board_t * board) {
    for (int i = 0; i < board->side_len; ++i) {
        int empty1 = 0;
        int set1 = 0;
        int empty2 = 0;
        int set2 = 0;

        for (int j = 0; j < board->side_len; ++j) {
            if (board->data[i * board->side_len + j] == EMPTY) {
                empty1++;
            } else if (board->data[i * board->side_len + j] == SET) {
                set1++;
            }

            if (board->data[j * board->side_len + i] == EMPTY) {
                empty2++;
            } else if (board->data[j * board->side_len + i] == SET) {
                set2++;
            }
        }

        board->min_set = MAX(board->min_set, set1);
        board->max_set = MIN(board->max_set, set1 + empty1);
        board->min_set = MAX(board->min_set, set2);
        board->max_set = MIN(board->max_set, set2 + empty2);
    }
}

static void fill_by_line_sets(board_t * board) {
    if (board->min_set != board->max_set) { return; }

    int repeat = 1;

    while (repeat) {
        repeat = 0;

        for (int i = 0; i < board->side_len; ++i) {
            int empty1 = 0;
            int set1 = 0;
            int empty2 = 0;
            int set2 = 0;

            for (int j = 0; j < board->side_len; ++j) {
                if (board->data[i * board->side_len + j] == EMPTY) {
                    empty1++;
                } else if (board->data[i * board->side_len + j] == SET) {
                    set1++;
                }

                if (board->data[j * board->side_len + i] == EMPTY) {
                    empty2++;
                } else if (board->data[j * board->side_len + i] == SET) {
                    set2++;
                }
            }

            if (empty1 > 0) {
                if (set1 == board->min_set) {
                    for (int j = 0; j < board->side_len; ++j) {
                        if (board->data[i * board->side_len + j] == EMPTY) {
                            board->data[i * board->side_len + j] = UNSET;
                            board->empty_count--;
                            repeat = 1;
                        }
                    }
                } else if (set1 + empty1 == board->min_set) {
                    for (int j = 0; j < board->side_len; ++j) {
                        if (board->data[i * board->side_len + j] == EMPTY) {
                            board->data[i * board->side_len + j] = SET;
                            board->empty_count--;
                            repeat = 1;
                        }
                    }
                }
            }


            if (empty2 > 0) {
                if (set2 == board->min_set) {
                    for (int j = 0; j < board->side_len; ++j) {
                        if (board->data[j * board->side_len + i] == EMPTY) {
                            board->data[j * board->side_len + i] = UNSET;
                            board->empty_count--;
                            repeat = 1;
                        }
                    }
                } else if (set1 + empty1 == board->min_set) {
                    for (int j = 0; j < board->side_len; ++j) {
                        if (board->data[j * board->side_len + i] == EMPTY) {
                            board->data[j * board->side_len + i] = SET;
                            board->empty_count--;
                            repeat = 1;
                        }
                    }
                }
            }
        }
    }
}

static void duplicate_board_with_guess(board_t * restrict board_dup, board_t * restrict board) {
    board->empty_count--;
    memcpy(board_dup, board, sizeof(board_t));

    for (int i = 0; i < board->side_len * board->side_len; ++i) {
        if (board->data[i] == EMPTY) {
            board->data[i] = SET;
            board_dup->data[i] = UNSET;
            return;
        }
    }
}

static board_t * solve(board_t * board) {
    int empty_before = -1;

    while (empty_before != board->empty_count) {
        empty_before = board->empty_count;

        fill_before_or_after_two_in_a_row(board);
        fill_between_two(board);
        count_line_sets(board);
        fill_by_line_sets(board);
    }

    if (board->empty_count == 0) {
        return board;
    }

    board_t board_dup;
    duplicate_board_with_guess(&board_dup, board);

    board_t * sol1 = solve(board);
    return sol1 == NULL ? solve(&board_dup) : sol1;
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("\nUSAGE\n\t%s puzzle_file\n\nARGUMENTS\n\tpuzzle_file - text file with Takuzu puzzles one by line, with missing positions as ASCII zeroes or dots. All other characters will be ignored.\n\nEXAMPLE\n\t%s puzzles.txt\n\n", argv[0], argv[0]);
        return 1;
    }
    unsigned int number = 0;
    unsigned int solved = 0;
    unsigned int impossible = 0;

    FILE * fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file for reading\n");
        return EXIT_FAILURE;
    }

    char buffer[501];

    while (fgets(buffer, 501, fp) != NULL) {
        buffer[500] = 0;
        if (buffer[0] == '#')
            continue;

        ++number;
        printf("Starting puzzle #%d:\n%s\n", number, buffer);

        board_t board;
        if (!string_to_board(&board, buffer)) {
            printf("Invalid puzzle string: must be a square\n");
            continue;
        }

        printf("Input:\n");
        print_board(&board);

        solve(&board);

        if (!board_filled(&board)) {
            impossible++;
            fprintf(stderr, "Impossible puzzle\n");
        } else {
            solved++;
            printf("Solution:\n");
        }
        print_board(&board);
    }

    printf("Solved=%u\nImpossible=%u\nTotal=%u\n", solved, impossible, number);
    return EXIT_SUCCESS;
}

