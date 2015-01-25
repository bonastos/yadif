#Scope

A scope controls how instances are returned for each injectors `get<>()` request. It has access to an internal instance source. Depending on the external context, it must decide if it needs to generate a new instance or return a cached instances. For example, if your application had some session context, it could return the same instance for all requests within the same session.

Yadif supplies only one scope, the `SingletonScope`. This scope generates a single instance and returns it on each request.

You may write your own scope by implementing the `Scope` interface. Check the implementation
of `SingletonScope` for details.

---

Next : [Design Rationale](design_rationale.md)