#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sirius_poem(void) {
    puts("\n[Verse IV]");
    puts("  The Sirius, warmth within mine heart,");
    puts("  A love that ne’er shall tear us apart.");
    puts("  A weapon forged for aye from the start,");
    puts("  No rightful place in the quartet’s part.\n");

    puts("[Verse V]");
    puts("  If Sirius I may no longer see,");
    puts("  My life shall sink in deep misery.");
    puts("  If Sirius I may no longer love,");
    puts("  Shall it e’er grant me freedom from above?\n");

    puts("[Verse VI]");
    puts("  Her love doth serve as fuel to my life,");
    puts("  For it I shall ever strive through strife.");
    puts("  Though challenged by mine earthly bounds and blight,");
    puts("  I shall behold Sirius e’en beyond the night.\n");
}

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


int main(void) {
    puts("=====================================================");
    puts(
"  *    .  *       .             *\n"
"                         *\n"
" *   .        *       .       .       *\n"
"   .     *\n"
    );
    puts("          S I R I U S   -   P A R T   II");
    puts(
"  *    .  *       .             *\n"
"                         *\n"
" *   .        *       .       .       *\n"
"   .     *\n"
    );
    puts("=====================================================");
    puts("The astral winds whisper secrets anew...");
    puts("Yet only those who bend fate itself may hear them.");
    puts("Cast thy message toward the star:\n");

    char buf[64];
    int auth = 0;  

    
    gets(buf);    

    printf("\nThy voice echoes: %s\n", buf);

    if (auth == 0x1337) {
        puts("\nThe heavens part before thy will...");
        sirius_poem();
        print_flag();
    } else {
        puts("\nThe sky rejects thine offering.");
        puts("Sirius withholds her radiant truth...\n");
    }

    puts("The constellations fade into silence.\n");
    return 0;
}
