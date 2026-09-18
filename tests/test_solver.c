#include "grid.h"
#include "wordlist.h"
#include "solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define CHECK(cond, msg)                                                \
    do {                                                                \
        if (!(cond)) {                                                  \
            printf("FAIL: %s\n", msg);                                  \
            failures++;                                                 \
        } else {                                                        \
            printf("ok  : %s\n", msg);                                  \
        }                                                               \
    } while (0)

static void test_horizontal(void)
{
    Grid *g = grid_create(3, 5);
    const char *row0 = "HELLO";
    const char *row1 = "WORLD";
    const char *row2 = "ABCDE";
    for (int c = 0; c < 5; c++) {
        grid_set(g, 0, c, row0[c]);
        grid_set(g, 1, c, row1[c]);
        grid_set(g, 2, c, row2[c]);
    }

    WordPosition p = solver_find_word(g, "HELLO");
    CHECK(p.found && p.x0 == 0 && p.y0 == 0
          && p.x1 == 4 && p.y1 == 0, "horizontal HELLO");

    p = solver_find_word(g, "ABCDE");
    CHECK(p.found && p.x0 == 0 && p.y0 == 2
          && p.x1 == 4 && p.y1 == 2, "horizontal ABCDE");

    grid_free(g);
}

static void test_vertical(void)
{
    /* CATS is placed vertically in column 0. */
    Grid *g = grid_create(4, 4);
    const char *rows[4] = { "CXXX", "AXXX", "TXXX", "SXXX" };
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            grid_set(g, r, c, rows[r][c]);
        }
    }

    WordPosition p = solver_find_word(g, "CATS");
    CHECK(p.found && p.x0 == 0 && p.y0 == 0
          && p.x1 == 0 && p.y1 == 3, "vertical CATS");

    grid_free(g);
}

static void test_diagonal(void)
{
    /* SNAK is placed on the SW diagonal from (3,0) to (0,3). */
    Grid *g = grid_create(4, 4);
    const char *rows[4] = { "ABCS", "DENF", "GAHI", "KJLM" };
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            grid_set(g, r, c, rows[r][c]);
        }
    }

    WordPosition p = solver_find_word(g, "SNAK");
    CHECK(p.found && p.x0 == 3 && p.y0 == 0
          && p.x1 == 0 && p.y1 == 3, "diagonal SNAK");

    grid_free(g);
}

static void test_not_found(void)
{
    Grid *g = grid_create(3, 3);
    grid_set(g, 0, 0, 'A');
    grid_set(g, 0, 1, 'B');
    grid_set(g, 0, 2, 'C');
    grid_set(g, 1, 0, 'D');
    grid_set(g, 1, 1, 'E');
    grid_set(g, 1, 2, 'F');
    grid_set(g, 2, 0, 'G');
    grid_set(g, 2, 1, 'H');
    grid_set(g, 2, 2, 'I');

    WordPosition p = solver_find_word(g, "XYZ");
    CHECK(!p.found, "not found XYZ");

    p = solver_find_word(g, "ABCDEFGHI");
    CHECK(!p.found, "too long ABCDEFGHI");

    grid_free(g);
}

int main(void)
{
    test_horizontal();
    test_vertical();
    test_diagonal();
    test_not_found();

    if (failures == 0) {
        printf("\nAll tests passed.\n");
        return 0;
    }
    printf("\n%d test(s) failed.\n", failures);
    return 1;
}
