#Provider

Yadif contains several convenience provider templates to simplify the writing of providers.

##Provider<>

The variadic `Provider` template is used to inject bound instances into a constructor.

```c++
template <typename T, typename... Args>
struct Provider {
  std::shared_ptr<T> operator() (Injector& injector);
};
``` 

The first template argument is the type to be created, The following arguments specify the argument types of the T constructor used for creation. Each argument is defined as a binding key enclosed by a template that specifies the injection mode of the argument.

Example:

```c++
struct X {
  X(const A& a, B* b, std::shared_ptr<C> c);
  ...
};

using XProvider = Provider<X,
                           Ref<A>,
                           Ptr<B, Annotation<K>>,
                           SharedPtr<C>
                           >;
```

The following injection modes are implemented

* `Ref`: injects a reference to the injected instance.
* `Ptr`: injects a pointer to the injected instance.
* `SharedPtr`: injects a shared_ptr that to the injected instance.

The function call operator of the generated type uses the passed injector to create an instance of each argument, then calls the matching type constructor and returns the created instance wrapped in a shared_ptr. The injected argument instances are guaranteed to outlive the type instance. (They are actually squirrelled away in the deleter of the returned shared_ptr.)

##InstanceProvider<>

The `InstanceProvider` template is used to bind a constant instance.

```c++
template <typename T> 
class InstanceProvider {
public:
  InstanceProvider(T val);

  std::shared_ptr<T> operator() ()
};
```

It copies its constructor argument and always returns a shared_ptr to the copied value.


The helper function

```c++
template <typename T>
InstanceProvider<T> makeInstanceProvider(T val);
```

can be used to create an `InstanceProvider` and automatically deduce it's template type.

##ReferenceProvider<>

The `ReferenceProvider` template is used to bind a reference to an existing object.

```c++
template <typename T> 
class ReferenceProvider {
public:
  ReferenceProvider(T& ref);

  std::shared_ptr<T> operator() ()
};
```

It stores the reference argument and returns a shared_ptr containing the address of the referenced object. The custom deleter is empty.

:exclamation: It's your responsibility to ensure that the referenced object has a longer lifetime than any object that uses it as an injected instance. 


The helper function

```c++
template <typename T>
ReferenceProvider<T> makeReferenceProvider(T& ref);
```

can be used to create an `ReferenceProvider` and automatically deduce it's template type.

---

next: [Singleton](reference_singleton.md) 

