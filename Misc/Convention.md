***Meta.*** The meaning for convention is not to fix things forever, but to help memorize. Everything below could be changed if it is proved to be wrong or improvable.
* 2021.8.7: First draft.
* 2021.8.8: 
  * Update `SIZEOF` assertion.
  * Add `#include` order convention.

**File structure.** Almost every file in this project is self-documented source file, so source root is project root rather than a dedicated `src` folder. There is also no `include`, each associated header file, if exists, should colocate with implementation. There is a global config to include project root in header path, so every `#include` with quotes should use path relative to project root, even when referencing associated header.

**Platform-related code structure.** All platform-dependent code goes into `Platform/<platform>` directories. Grouping-by-platform makes it easy to perform platform-specific tuning on all affected code. Each platform directory should at least have an `App.c[xx]`, which contains the entry of executable. Every platform-independent code file should be outside `Platform` directory.

**Language choice.** The code should be written in C as long as possible, and only use C++ when unavoidable. Because the test cases use GoogleTest, every tested header file has to be compatible to C++. The `#include` will be wrapped with `extern "C"` at the user side, so most of the time header files need not be C++-aware.

The C standard is C11 for `sscanf_s`, and C++ standard is C++17 for various modern features. When interact with C-side interfaces like `struct` and casting, the C++ code should *look like* C as much as possible to be consistent. The rest part of C++ code should follow clang-tidy's advices.

**Polymorphism.** The platform-independent `Game` needs to interact with multiple platform-dependent components like `View` and `Emitter`. Although the implementation of these components should be unique for each platform target, the project still go with a vtable-like dynamically-dispatching strategy, rather than directly provide functions with identical declaration for each target, for reasons:
* The negligible performance overhead is acceptable.
* Some components like `Game` itself and `Brick` may require flexibility. It is more consistency to apply only one kind of polymorphism for all usage.

To apply such polymorphism, each component is decouple into a `State` struct and a constant `Behavior` struct, where the prior one is plain-old-data, and the latter one is a group of function pointers, and each of the function probably takes a maybe-constant `State` as its first argument. 

Each polymorphic component is defined in a behavior header. The header contains a forward declaration for state struct, and the declaration for behavior struct. It is called behavior header because it actually only defines behavior struct, and the forward declaration for state struct is only for so that behavior methods can reference the pointer to the state. Optionally the header can also provide default implementation for some behavior methods to help backward compatibility.

For any user that wants to interact with some polymorphic `X`, she should include the behavior header for `X`, the code should look like this:

```C
T UseX(struct XState *x, const struct XBehavior *X, /*...*/) {
  // ...
  X->SomeMethod(x, /*...*/);
}
```

A real example would be the implementation of `Draw` method of `GameBahavior`, which needs to work with a polymorphic `View`.

Some keynotes:
* The state does not keep a pointer to behavior by default like an actual vtable.
* When passing into a function, the state goes first.
  * In some case only state needs to be passed.
* Behavior is always constant.
* Both structs are not `typedef`ed.

For any host that want to provide a polymorphic implementation, she should:
* Include behavior header in the header file.
* Declare an external constant global instance of the behavior in the header, and instantiate it in the code file.
  * The instance is named `<Impl><Behavior>`, e.g. `GDIPlusView` is a `ViewBehavior` instance for `GDIPlus` implementation. There is no trailing `Behavior` in the name.
* Declare state struct privately in the code file.
* Provide some other utility function that not required by the behavior.

Notice that instead of exposing the detail layout of state struct, the header file may only contain a `SIZEOF` macro to achieve isolated accessing. See *memory management* for more detail.

**Behavior-related code structure.** All behavior headers go into `Behavior` directory, to keep their path not prefix-matching any other headers. Platform-dependent implementations are stored in previously-mentioned `Platform` directory, and other cross-platform implementations can be simply put into project root. Some many-to-implement behavior like `Brick` may get its own directory.

**Memory management.** The user of an instance decides where to allocate memory for it, i.e. the state struct. The convention at the same time wants to keep state struct layout private to the user. So, for behavior `X`, the implementation `My`:
* Provides a `SIZEOF_MyXState` macro, which expands to a `size_t` that not less than `sizeof(struct XState)` that defines in the implementation code file.
  * The size has to be manually decided, and may waste some memory. The recommended size is 32 bytes which fits an L1 cache line.
  * After state struct declaration add `STATIC_ASSERT_SIZEOF(My, X);` to look out for growing struct size in the future.
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

**Keynotes for polymorphic naming.**
* There is `struct XState`, `struct XBehavior`, `MyX`, which is an instance of `struct XBehavior`, and is named capitalized because it can act like a namespace.
* There is no `MyXState` or `MyXBehavior`. The latter one is simply as `MyX`.
* For state struct, the name `struct XState` is used everywhere. Each implementation has its own version of `struct XState`.
  * Since struct name is eliminated after compilation, this kind of duplication should be fine.
  * The user of `MyX` may name the instance `my_x`, if there is `your_x` presenting at the same time.
  * The user of polymorphic `X` should name the instance simply `x`.

**General naming convention.** Most convention is shown in above code snippets. In addition to those:
* Macro should be rarely used, either value or procedure. A common usage is `SIZEOF_*` macros. The name of other macros should be all capitalized and as short as possible.
* Enumerate name is WIP.
* File name should be in camel case.
* There is no plural(-s/es), past(-ed) or present(-ing) tense anywhere.
  * Except `CMakeLists.txt`

**`#include` order.** Follow Google C++ code style, the `#include`s are listed as:
* Associated header.
* A blank line.
* System headers, for C++ only headers end with `.h`.
* A blank line.
* For C++, system headers without trailing `.h`
* For C++, a blank line.
* Headers of other libraries, e.g. `<gdiplus.h>`.
* A blank line.
* Headers of this project.
