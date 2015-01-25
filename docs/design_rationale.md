#Design Rationale

Let's state the design goals first, as they affect the design decisions.

###Design Goals

1. Non-intrusive - It must be possible to use any type for injection, not only types that have been derived from an library supplied base type.
2. Typesafe - Must fit into C++ type system.
3. RAII - must not leak resources.
4. Providers must be easy to generate
5. Try to use proven concepts from Guice library.

###Design Decisions

**Why must objects be created by providers?**

This follows directly from goal 1. If we cannot use a known interface to modify an object, the only common interface for object creation is the constructor. Therefore we must use a factory object that knows the constructor to use. Yadif calls these objects providers. (In fact, concept and name were shamelessly stolen from Guice.)

**Why return a shared pointer to injected instance?**

Goal 3 requires the use of a smart pointer. As it is allowed to bind singleton instances (multiple owners), `std::shared_ptr` is a perfect fit. Is's other features also come in handy. A `shared_ptr<void>` can be used for type erasure. And you can play interesting games with custom deleters. Like hiding a few injected instances.

**Why is it not allowed to redefine bindings?**

If a binding could be redefined, it would be possible to accidentally overwrite bindings from unrelated modules without warning. So it's not allowed to overwrite.

**Why are annotations types?**

Thats actually because of goal 4. I wrote a variadic template for the generation of injecting providers. As C++ doesn't have compile time strings yet, annotations have to be types. 


**Why does the library only provide the basic exception guarantee?**

Most of Yadifs functionality happens at compile time. The only function callable during runtime is the injectors get function. And if `get<T>()` throws, it means that the requested type(static) is not correctly handled by the bindings(static). As I do not see a way for dynamic recovery, Yadif doesn't provide the strong guarantee.