#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void big_verse(void) {
    puts("\n[Verse I]");
    puts("  The stars doth shine most fair this night,");
    puts("  Mine eyes beheld her, yet I lacked the right.");
    puts("  Against the light I strove with all my might,");
    puts("  To pierce the veil and see what lies in sight.\n");

    puts("[Verse II]");
    puts("  O star of Dog, do guide me true,");
    puts("  Lead me unto the land where visions view.");
    puts("  I’ll strive and strive till I her form do plight,");
    puts("  O Sirius, ne’er shall I match thy light.\n");

    puts("[Verse III]");
    puts("  O Sirius, star that rules the night,");
    puts("  Thou art the flame I ne’er did seek to blight.");
    puts("  For in the darkest hours of yonder sky,");
    puts("  E’en the dimmest corners of my sphere do lie");
    puts("  And shine with brilliance that shall never die.\n");
}

void print_flag(void) {
    FILE *f = fopen("flag.txt", "r");
    if (!f) {
        puts("The fragment drifts beyond your reach...");
        return;
    }

    char buf[256];
    fgets(buf, sizeof(buf), f);
    puts("\n[The Fragment Reveals Itself]");
    printf("%s\n", buf);
    fclose(f);
}

int main(void) {
    char buffer[64];
    char bigbuf[512]; 

    puts("=====================================================");
    puts(
"  *    .  *       .             *\n"
"                         *\n"
" *   .        *       .       .       *\n"
"   .     *\n"
    );
    puts("          S I R I U S   -   P A R T   I");
    puts(
"  *    .  *       .             *\n"
"                         *\n"
" *   .        *       .       .       *\n"
"   .     *\n"
    );
    puts("=====================================================");
    puts("A voyage inscribed 'mongst the constellations...");
    puts("Entreat the stars to bestow their wisdom upon thee:");


    int n = read(STDIN_FILENO, bigbuf, sizeof(bigbuf) - 1);
    if (n < 0) return 1;
    bigbuf[n] = '\0';

    strncpy(buffer, bigbuf, sizeof(buffer) - 1);
    buffer[63] = '\0';

    printf("\nThou Shoutest unto the sky:\n%s\n", buffer);


    if (n > 64) {
        big_verse();
        print_flag();
    } else {
        puts("\nThou dost step into the sable gloom...");
        puts("Yet the light of Sirius doth grant thee no reply.\n");

    }

        puts("\nThe firmament of night doth wane.\n");
    return 0;
}
