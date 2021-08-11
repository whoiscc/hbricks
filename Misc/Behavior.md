**Dynamical dispatching.** Most of the function callings in this project are polymorphic, i.e. the callings are indirectly applied to (mostly constant) function pointers. For some components like the `Game` itself, they require flexibility which can be provided by such indirect callings. For example, game mode can be selected at runtime, and the kind of bricks can be randomized. By switching between different the pointers of functions to be called all of these goals could be achieved.

There is also some other components that do not require dynamically switching at runtime, but their functionalities varies across compilation targets. For example, the `View` for different platforms are dramatically different, but `Game` must be able to draw game situation on all of them, although it only needs to interact with one single kind of `View` for each compilation target. For this problem there are certain more *statical* solutions, but in this project it is also solved with polymorphic callings as the previous problem. This helps keep the project more consistency, and the unnecessary performance overhead is acceptable.

**State and behavior.** Each of the component that can be polymorphic, e.g. `Game`, `View` and `Brick`, is decoupled into state and behavior. State is plain old data, and behavior is a set of function pointers. In this project state does not hold a reference to its behavior (like an actual vtable), instead they are passing around together.

**Behavior header for `X`.** The path should be `Lib/Behavior/X.h`. The header should look like:

```C
// pragma once, include stuff, etc

// only forward declaration
// each implementation should have its own (private) data layout
struct XState;

struct XBehavior {
  // should take state as its first argument, like `self` in Python
  void (*SomeMethod)(struct XState *, /*...*/);
  // getter-like method
  void (*PureMethod)(const struct XState *, /*...*/);
  // stateless (static) method, should be rare
  void (*StatelessMethod)(/*...*/);
  // extra behaviors, see sections below
  struct ExtraBehavior Ex;
};
```

Notes:
* The memory layout of state is unrelated, all interfaces interact with state data through pointer. In fact, according to the memory management convention of this project, most structs have private memory layouts.
* Behavior struct does not carry any state, i.e. data field. Every field is a pointer to a function, except `Ex`, which is a nested struct that contains more function pointers. See below for more detail.

**User side: polymorphic calling.** Suppose a user wants to interact with a polymorphic `X` defined as above, her code should look like this:

```C
#include "Lib/Behavior/X.h"
// ...
void UseX(/*args*/, struct XState *x, const struct XBehavior *X, /*...*/) {
  // ...
  X->SomeMethod(x, /*...*/);
}
```

Notes:
* Behavior struct should always be constant. State struct may also be constant if only pure methods are used inside.
* Behavior struct should be passed after state struct. In some cases behavior argument can be omitted.
* Argument names are corresponded to behavior name. If there are multiple states in the same scope, use prefix, e.g. `another_x`, to distinguish.

**Host side: behavior implementation.** Suppose a host wants to provide an implementation of `X`, first she should give her implementation a name. For an implementation called `MyX`, the header file should look like:

```C
#include "Lib/Behavior/X.h"
// ...
// state size, required by memory management convention
// no state struct declaration in header file
#define SIZEOF_MyXState 32

extern const struct XBehavior MyX;
// other functions that manipulate state struct but not included in behavior
// i.e. non-polymorphic functions
// the most common one that follows memory management convention
struct XState *InitMyX(uint8_t *mem);
```

And in source file:

```C
// do not include behavior header, include associate header instead
#include "PathToHeaderAbove.h"
// ...
// notice the name: `XState` instead of `MyXState`
struct XState {
  // ...
};
// prevent memory overflow when adding more fields in the future
STATIC_ASSERT(sizeof(struct MyXState) <= SIZEOF_MyXState);

void SomeMethod(struct XState *this, /*...*/) {
  // ...
}
// implementation of other behavior methods go here

const struct XBehavior MyX = {
    .SomeMethod = SomeMethod;
    .PureMethod = /*...*/;
    .StatelessMethod = /*...*/;
    .Ex = {
        // ...
    };
};
```

Notes:
* No struct type named `MyXState` or `MyXBehavior`.
  * All implementations of `X` have same set of behavior methods as defined in `XBehavior`, so `MyX` only need to instantiate a `XBehavior` (which called `MyX`) instead of creating a new type for it.
  * Each implementation has its own state struct type, but all of those struct types are called `XState`. Every declaration of those `XState` is in source file rather than header file, so there is no declaration conflicting.
    * If that still seems confusing, you can try to `typedef struct XState MyXState;` in source file and use `MyXState` afterward. I do not recommend this way though.
* The downside of this solution: cannot define simple method inline in the header, like in C++. However, the missing feature does not help user anyway, so it's ok to me.

**User side: using implementation.** See memory management convention section for the example.

**Extra behaviors.** There are some common functions that should be implemented by various instances, no matter what they are, i.e. what behavior they are implementing. Example of these functions are stringify, cloning, comparing, etc. These functions are grouped into a special behavior called `ExtraBehavior`. It has some difference to a normal behavior (a.k.a. behavior from now on):
* It does not have a forward declaration for state struct, because it may be called with states of various types. The first argument of behavior methods should be `void *` (or `const void *`).
* Implementation is not required to provide all methods of the extra behavior, only the subset that makes sense. The function pointers that are not initialized will be automatically zeroed (because of C's global variable spec). The whole `Ex` field can be omitted as well if none extra behavior method is defined.
* Polymorphic function should check whether an extra behavior method exists because calling it. In most situation a polymorphic function should know the method it calls exists and assert instead of checking.

In some cases, a function may expect polymorphic argument of an extra behavior instead of a behavior. Then it should take a pair of arguments typed `void *` and `const struct ExtraBehavior *`. Whenever it knows that its argument always implements a behavior, the arguments should be typed after that behavior, even only extra behavior methods are used inside.

**Behavior vs. extra behavior.**
* Behavior is exclusive, i.e. an implementation cannot implement two behaviors at the same time. Extra behavior is a set of multiple unrelated methods, which could be treated as multiple behaviors.
* Extra behavior is globally-defined. 
  * One cannot tell what is the implementing type from `void *`. For example, consider a simple comparator `int (*Cmp)(void *this, void *)`, the implementation has to relay on external way to check whether `this` is compared with an instance whose type is the same.
  * One cannot tell what method is expected from `const struct ExtraBehavior *`. A polymorphic function cannot use type system to constraint.

In conclusion, although all behaviors can be converted into extra behavior, because of the weak typing system of C, it is still preferred to utilize behaviors whenever it is reasonable.

**Behavior-like component.** Some components, e.g. string buffer, may not implement any common behavior, but instead provide a set of it own functions. (It could also be treated as it is the only implementation of a behavior.) For these components, although it is not necessary to access its methods through struct (since we don't need to switch the struct), a struct could still act like a namespace, which is a good feature that C unfortunately lacks of. See `Lib/StrBuf.h` for a simple example of such components.