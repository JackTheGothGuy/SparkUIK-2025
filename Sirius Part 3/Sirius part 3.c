#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ENTRIES 16
#define NAME_LEN 32

typedef struct chunk_t {
    size_t size;
    char *data;
    char name[NAME_LEN];
    int in_use;
} chunk_t;

chunk_t *table[MAX_ENTRIES];

void print_flag(void) {
    FILE *f = fopen("flag.txt", "r");
    if (!f) {
        puts("The fragment hides beyond the cosmic veil...");
        return;
    }

    char buf[256];
    fgets(buf, sizeof(buf), f);
    puts("[The Final Fragment Descends]");
    printf("%s\n", buf);
    fclose(f);
}

/* victim function that is intended to be called only when auth is satisfied.
   The function pointer will be the target of heap corruption in the challenge. */
void (*victim)(void) = NULL;

/* The poem (user-provided content) */
void sirius_poem(void) {
    puts("sirius part 3");
    puts("Verse VII");
    puts("O Sirius, I have waited long for thee,");
    puts("Where hast thou been? I longed thy face to see.");
    puts("Fast bound unto the root of my temporal tree,");
    puts("Now thou behold’st, and I am thee; all that I see is me.\n");

    puts("Verse VIII");
    puts("She hath found me, and I her, and thou hast found us here,");
    puts("Into our lives thou cam’st ere thou didst disappear.");
    puts("As all my former mem’ry blurred and smeared,");
    puts("Yet hers alone endured, ne’er disappeared.\n");

    puts("Verse IX");
    puts("Hast thou e’er asked thyself what ’tis like");
    puts("To stand upon a mountain, or in forest alike,");
    puts("Just one more moment ere the fates strike,");
    puts("I love thee more than the sun in the skies doth spike.\n");
}

/* Helper - safe read (no bounds guarantees intentionally omitted for challenge edges) */
ssize_t read_all(int fd, void *buf, size_t count) {
    ssize_t r = 0;
    while ((size_t)r < count) {
        ssize_t n = read(fd, (char*)buf + r, count - r);
        if (n <= 0) break;
        r += n;
    }
    return r;
}

void menu(void) {
    puts("=====================================================");
    puts("1) Allocate  chunk");
    puts("2) Free      chunk");
    puts("3) Edit      chunk");
    puts("4) Show      chunk");
    puts("5) List      chunks");
    puts("6) Invoke    ritual");
    puts("7) Exit");
    puts("----------------------------------------");
    printf("Choice: ");
    fflush(stdout);
}



int find_free_slot(void) {
    for (int i = 0; i < MAX_ENTRIES; ++i) {
        if (!table[i]) return i;
    }
    return -1;
}

void do_allocate(void) {
    int idx = find_free_slot();
    if (idx < 0) {
        puts("No free slots.");
        return;
    }
    printf("Index %d selected. size (decimal): ", idx);
    fflush(stdout);
    char line[32];
    if (!fgets(line, sizeof(line), stdin)) return;
    size_t sz = strtoul(line, NULL, 10);
    if (sz == 0 || sz > 0x1000) {
        puts("Invalid size.");
        return;
    }

    chunk_t *c = malloc(sizeof(chunk_t));
    if (!c) { puts("malloc fail"); exit(1); }
    c->size = sz;
    c->data = malloc(sz);
    if (!c->data) { puts("malloc fail"); exit(1); }
    memset(c->data, 0, sz);
    c->in_use = 1;
    snprintf(c->name, NAME_LEN, "chunk_%d", idx);
    table[idx] = c;

    printf("Allocated chunk %d of size %zu\n", idx, sz);
}

void do_free(void) {
    printf("index: ");
    fflush(stdout);
    char line[8];
    if (!fgets(line, sizeof(line), stdin)) return;
    int idx = atoi(line);
    if (idx < 0 || idx >= MAX_ENTRIES || !table[idx]) {
        puts("Invalid index.");
        return;
    }
    chunk_t *c = table[idx];
    if (c->data) {
        free(c->data);
    }
    c->in_use = 0;
    printf("Freed chunk %d (data pointer still referenced in metadata structure)\n", idx);
}

void do_edit(void) {
    printf("index: ");
    fflush(stdout);
    char line[8];
    if (!fgets(line, sizeof(line), stdin)) return;
    int idx = atoi(line);
    if (idx < 0 || idx >= MAX_ENTRIES || !table[idx]) {
        puts("Invalid index.");
        return;
    }
    chunk_t *c = table[idx];

    printf("bytes to write (decimal): ");
    fflush(stdout);
    if (!fgets(line, sizeof(line), stdin)) return;
    size_t n = strtoul(line, NULL, 10);
    if (n == 0 || n > 0x2000) { puts("Invalid len."); return; }

    printf("Enter data (raw, read %zu bytes):\n", n);
    fflush(stdout);

    ssize_t r = read_all(STDIN_FILENO, c->data, n);
    (void)r;
    puts("write complete.");
}

void do_show(void) {
    printf("index: ");
    fflush(stdout);
    char line[8];
    if (!fgets(line, sizeof(line), stdin)) return;
    int idx = atoi(line);
    if (idx < 0 || idx >= MAX_ENTRIES || !table[idx]) {
        puts("Invalid index.");
        return;
    }
    chunk_t *c = table[idx];

    puts("---- chunk meta ----");
    printf("name: %s\n", c->name);
    printf("size: %zu\n", c->size);
    printf("in_use: %d\n", c->in_use);
    puts("---- data ----");
    if (c->data) puts(c->data);
    else puts("(null)");
}

void do_list(void) {
    puts("Index | name            | size    | in_use");
    puts("------+-----------------+---------+--------");
    for (int i = 0; i < MAX_ENTRIES; ++i) {
        if (table[i]) {
            printf("%5d | %-15s | %7zu | %6d\n", i, table[i]->name, table[i]->size, table[i]->in_use);
        } else {
            printf("%5d | %-15s | %7s | %6s\n", i, "(empty)", "-", "-");
        }
    }
}

void invoke_ritual(void) {
    puts("You stand before the astral gate...");
    if (victim == print_flag) {
        puts("The heavens part before thy will...");
        sirius_poem();
        print_flag();
    } else if (victim) {
        puts("Something stirred, but not the true spark.");
        victim();
    } else {
        puts("The sky rejects thine offering.");
        puts("Sirius withholds her radiant truth...");
    }
}

int main(void) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    setbuf(stderr, NULL);

    puts("=====================================================");
    puts(
"  *    .  *       .             *\n"
"                         *\n"
" *   .        *       .       .       *\n"
"   .     *\n"
    );
    puts("          S I R I U S   -   P A R T   III");
    puts(
"  *    .  *       .             *\n"
"                         *\n"
" *   .        *       .       .       *\n"
"   .     *\n"
    );
    puts("=====================================================");
    puts("The Cavern Echoes secrets anew...");
    puts("Yet only those who bend fate itself may hear them.");
    puts("Cast thy message toward the star:\n");

    
    for (int i = 0; i < MAX_ENTRIES; ++i) table[i] = NULL;

    chunk_t *init = malloc(sizeof(chunk_t));
    init->size = 0x30;
    init->data = malloc(0x30);
    strcpy(init->name, "init_guard");
    init->in_use = 1;
    memset(init->data, 'A', 0x30);
    table[0] = init;

    chunk_t *control = malloc(sizeof(chunk_t));
    control->size = 0x40;
    control->data = malloc(0x40);
    strcpy(control->name, "control");
    control->in_use = 1;
    memset(control->data, 'C', 0x40);
    table[1] = control;

    // Main loop
    while (1) {
        menu();
        char choice_line[8];
        if (!fgets(choice_line, sizeof(choice_line), stdin)) break;
        int choice = atoi(choice_line);
        switch (choice) {
            case 1: do_allocate(); break;
            case 2: do_free(); break;
            case 3: do_edit(); break;
            case 4: do_show(); break;
            case 5: do_list(); break;
            case 6: invoke_ritual(); break;
            case 7: puts("Farewell, stargazer."); goto cleanup;
            default: puts("Invalid."); break;
        }
    }

cleanup:
    // tidy: free allocated metadata pointers (not necessary for challenge)
    for (int i = 0; i < MAX_ENTRIES; ++i) {
        if (table[i]) {
            if (table[i]->data) free(table[i]->data);
            free(table[i]);
            table[i] = NULL;
        }
    }
    return 0;
}
