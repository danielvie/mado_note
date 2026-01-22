#include <corecrt.h>
#include <stdio.h>

#define MADO_NOTE_IMPLEMENTATION
#include "mado_note.h"

int main() {

    // creating buffer
    NoteData note;
    char my_buffer[1024];
    mado_note_init(&note, my_buffer, sizeof(my_buffer));

    // open file to write
    FILE *f;
    errno_t err = fopen_s(&f, "z_output.txt", "wb");

    mado_note_clear(&note);
    mado_note_add_text(&note, "Hello");
    mado_note_add_text(&note, ", %s!\n", "world");
    uint32_t arr[] = {1,2,3};
    mado_note_add_array_u32(&note, "numbers", arr, 3);

    printf("\n\n-----mado_note_print(&note) results:\n\n");
    mado_note_print(&note);

    printf("\n-----mado_note_print_buffer(&note) results:\n\n");
    mado_note_print_buffer(&note);

    // write to file
    if (err == 0) {
        mado_note_print_file(&note, f);
        fclose(f);
    }

    return 0;
}
