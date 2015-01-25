#Singleton

Singleton bindings are a special case of scope binding. The `SingletonScope` uses it's instance source to generate a single instance of the bound type. Afterwards it always returns that instance.

By default the instance is bound lazily. It's created when the injector gets the first 
request for the bound type. But it may be necessary to create the singleton eagerly (as soon as possible). I.e., the bound type may throw, and this should be detected as early as possible. In this case you should use the `.asEagerSingleton()` binding specification instead of `.asSingleton()`. This ensures the singleton instance is bound immediately after all modules have been processed.

During development, it may be preferred to use lazy binding for faster startup times. While release builds should use eager binding for early checking and minimizing latencies. This behaviour may be enabled by always using the `.asSingleton()` binding specification and defining the macro `YADIF_EAGER_SINGLETON` for release builds.

:exclamation: The singleton scopes and their instances are owned by the injector object. A new injector using the same modules will return a different singleton instance. Only the injectors cloning constructor will transfer the injectors state.

---

Next [Scope](reference_scope.md)