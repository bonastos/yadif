#Injector

The `Injector` class collects bindings and uses them to create instances.

###Construction

```
template <typename... Modules>
Injector(Modules... modules);
```

The variadic constructor takes a list of modules and collects their bindings.


```
template <typename... Modules>
Injector(const Injector& injector, Modules... modules);
```

The second constructor takes another injector and a list of modules. It clones the binding state from the other injector and then adds the bindings from the module list.

Both constructors throw `DuplicateBinding` if a key is bound twice.

Both constructors throw `IllegalBinding` if a binding is incomplete or contains more than one  binding specification for the interface type.


###Instance Creation

```
std::shared_ptr<Interface> Injector::get<interface_key>();
```

Returns an bound instance of the requested type.

Throws `NoBindingFound` if the key wasn't bound in a module.


###Exceptions

All exceptions are derived from `yadif::Exception`, which in turn is derived from `std::exception`.

---

Next: [Provider](reference_provider.md)