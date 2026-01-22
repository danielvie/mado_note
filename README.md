# mado_note

**mado_note** is a lightweight, single-header C library for managing string buffers. It allows you to safely format text, log arrays, and manage output to consoles or files using fixed or variable memory buffers.

Designed for embedded systems or small C projects, it gives you full control over memory allocation (stack or heap) while providing convenient, high-level formatting functions similar to `printf`.

## Features
* **Single-Header Library:** Easy to drop into any project.
* **Flexible Memory:** Bring your own buffer (stack or heap). No hidden `malloc` calls.
* **Safe Formatting:** Prevents buffer overflows with bounds checking.
* **Helper Functions:** Built-in support for logging arrays (`uint32_t`, `int32_t`) and formatting text.

---

## Getting Started

### 1. Integration
Because this is a **stb-style single-header library**, you must define the implementation macro in **exactly one** source file before including the header.

**In `main.c` (or your implementation file):**
```c
#define MADO_NOTE_IMPLEMENTATION
#include "mado_note.h"
```

**In other files (e.g., `helper.c`):**
```c
// Just include the header to access the functions
#include "mado_note.h"
```

### 2. Basic Usage (Example from `main.c`)
Here is how to initialize a buffer on the stack and write to it.

```c
// Create the container (holds the size/index variables)
NoteData note;

// Create the actual memory storage on the stack
char my_buffer[1024];

// Link them together
mado_note_init(&note, my_buffer, sizeof(my_buffer));
```


Here is an example of a program using the library.
```c
#include <stdio.h>
#define MADO_NOTE_IMPLEMENTATION
#include "mado_note.h"

int main() {
    // Create the buffer container
    NoteData note;
    
    // Allocate memory
    char my_buffer[1024];
    
    // Initialize the note with the buffer
    mado_note_init(&note, my_buffer, sizeof(my_buffer));

    // Open a file for output
    FILE *f;
    errno_t err = fopen_s(&f, "z_output.txt", "wb");

    // Add content
    mado_note_clear(&note);
    mado_note_add_text(&note, "Hello\n");
    mado_note_add_text(&note, ", %s!\n", "world");
    
    // Add an array formatted automatically
    uint32_t arr[] = {1, 2, 3};
    mado_note_add_array_u32(&note, "numbers", arr, 3);

    // Output results
    mado_note_print(&note);         // Print to stdout
    mado_note_print_buffer(&note);  // Print debug view (shows nulls/usage)

    // Write results to file
    if (err == 0) {
        mado_note_print_file(&note, f); // Write to file
        fclose(f);
    }

    return 0;
}
```

---

## How to Build and Run

Since you are using **Clang** on Windows, you can compile the project using the command line:

```bash
# Compile
clang main.c -o note_app.exe

# Run
.\note_app.exe
```

If you use **CMake**, your configuration might look like this:

```bash
cmake -G "Ninja" -DCMAKE_C_COMPILER=clang -B build
cmake --build build
```

---

## API Documentation

### Lifecycle

#### `void mado_note_init(NoteData *note, char *external_buffer, size_t size)`
Initializes the `NoteData` structure. You must provide a pointer to valid memory (`external_buffer`) and its size.
* **buffer:** Can be a stack array (local variable) or heap memory (`malloc`).

#### `void mado_note_clear(NoteData *note)`
Resets the buffer index to 0 and fills the memory with zeros. Does not free the memory.

### Adding Content

#### `void mado_note_add_text(NoteData *note, const char *format, ...)`
Adds formatted text to the buffer (works like `printf`).
* **Safety:** Checks for buffer overflow before writing.
* **Example:** `mado_note_add_text(&note, "Temp: %d", 25);`

#### `void mado_note_add_array_u32(..., const uint32_t *data, size_t len)`
Formats and appends an array of unsigned integers.
* **Output format:** `name: [1, 2, 3];\n`

#### `void mado_note_add_array_i32(..., const int32_t *data, size_t len)`
Formats and appends an array of signed integers.

### Output

#### `void mado_note_print(const NoteData *note)`
Prints the valid content of the buffer to the standard output (`stdout`).

#### `void mado_note_print_buffer(const NoteData *note)`
Prints a debug visualization of the entire buffer memory, including unused space (represented by `.`) and usage statistics (size, index, percentage used).

#### `bool mado_note_print_file(const NoteData *nb, FILE *f)`
Writes the valid content of the buffer to the specified file handle.
* **Returns:** `true` on success, `false` on write error.

---

## FAQ: What is `MADO_NOTE_IMPLEMENTATION`?

This library uses the **Single-Header** pattern. 

In C, you can **declare** a function (say it exists) in many files, but you can only **define** (write the code for) it in one place. If you put the code logic directly in a header file and include it in multiple `.c` files, the linker will see duplicate definitions and crash.

**The Solution:**
1.  **Header Mode (Default):** When you include `mado_note.h` normally, it only shows the function *declarations*. This is safe to include everywhere.
2.  **Implementation Mode:** When you `#define MADO_NOTE_IMPLEMENTATION` before including it, the preprocessor "unlocks" the actual code logic. You do this in **one file only** (usually `main.c` or a dedicated `impl.c`) so the compiler creates the function code exactly once.