***Meta.*** The meaning for convention is not to fix things forever, but to help memorize. Everything below could be changed if it is proved to be wrong or improvable.
* 2021.8.7: First draft.
* 2021.8.8: 
  * Update `SIZEOF` assertion.
  * Add `#include` order convention.
* 2021.8.11:
  * Remove behavior.
  * Update file structure.

**File structure.** Almost every file in this project is self-documented source file, so source root is project root rather than a dedicated `src` folder. There is also no `include`, each associated header file, if exists, should colocate with implementation.

Brief project structure:
* `Game` core implementation.
* `Lib` cross platform code which does not describe game logic.
  * `Polyfill.h` complement patterns for language and platforms.
  * `Behavior` behavior headers.
* `Platform` platform-dependent code. "*"-ed are future works.
  * `GDI+` win32 application.
  * `Text` interactive-fiction-style command line program.
  * `Test` mock implementation for testing
  * `Cocoa`*
  * `KDE`*
  * `curses`*
  * `WebView`* (compiled to wasm)
* `Test` GoogleTest suites.
* `Misc` non-code files.

**Language choice.** The code should be written in C as long as possible, and only use C++ when unavoidable. Because the test cases use GoogleTest, every tested header file has to be compatible to C++. The `#include` will be wrapped with `extern "C"` at the user side, so most of the time header files need not be C++-aware.

The C standard is C11 for `*_s` functions, and C++ standard is C++17 for various modern features.

All core code (`Game` and `Lib`) should be written in C. When interacting with those code from C++, the user code should look like C as much as possible.

The project utilizes a behavior model, to achieve polymorphism in C. See [behavior document] for detail.

[behavior document]: /Misc/Behavior.md

**Memory management.** The user of an instance decides where to allocate memory for it, i.e. the state struct. The convention at the same time wants to keep state struct layout private to the user. So, for behavior `X`, the implementation `My`:
* Provides a `SIZEOF_MyXState` macro, which expands to a `size_t` that not less than `sizeof(struct XState)` that defines in the implementation code file.
  * The size has to be manually decided, and may waste some memory. The recommended size is 32 bytes which fits an L1 cache line.
  * After state struct declaration add `STATIC_ASSERT` to look out for growing struct size in the future.
* Provides an `InitMyX` function, which takes a `uint8_t *` and other custom initialization configs as arguments, and returns `struct XState *`. In the function:
  * Cast `uint8_t *` into private `struct XState *`.
  * Assign initial values for the struct.
    * May allocate memory for sub-instances.
  * Return the struct pointer.
* Optionally provides a `DropMyX` function to release any resource.

The user code of the implementation should be like this:

```C
uint8_t x_mem[SIZEOF_MyXState];  // or
uint8_t *x_mem = malloc(SIZEOF_MyXState);  // or any other preferred way
struct XState *x = InitMyX(x_mem, /*...*/);
// directly call behavior method
MyX.SomeMethod(x, /*...*/);
// call some polymorphic function
UseX(x, &MyX, /*...*/);
DropMyX(x);  // if there is one
```

**General naming convention.** Most convention is shown in above code snippets. In addition to those:
* Macro includes:
  * Conventional macros:
    * `SIZEOF_*` for state size.
    * `IMPL_*` in source file (without value).
  * All capitalized procedure macro.
* Enumerate name is WIP.
* File name should be in camel case.
* There is no plural(-s/es), past(-ed) or present(-ing) tense anywhere.
  * Except `CMakeLists.txt`

**`#include` convention.** All project header `#include` should be relative to project root. Follow Google C++ code style, the `#include`s are listed in the following order:
* Associated header.
* A blank line.
* System headers, for C++ only headers end with `.h`.
* A blank line.
* For C++, system headers without trailing `.h`
* For C++, a blank line.
* Headers of other libraries, e.g. `<gdiplus.h>`.
* A blank line.
* Headers of this project.
