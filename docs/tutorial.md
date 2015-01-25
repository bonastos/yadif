#Tutorial

First, take the `yadif.hpp` from the `single_header` directory and put it in your include path.

###Example

In Yadif the rules for object creation are called bindings. There are two kinds of bindings. Type bindings are used to bind an implementation to an interface. The second kind, provider bindings, define how an object of a specified type is created. This is done by registering a factory object that creates an instance of the bound type. These factories are called providers.


Here's a complete example. 

```c++
#include <yadif.hpp>
#include <string>
#include <iostream>

struct Named
{
  virtual std::string name() const = 0;
  virtual ~Named() = default;
};

struct World : Named
{
  std::string name() const override { return "World"; }
};

class Hello
{
public:
  Hello(Named& named) : named_(named) {}
  std::string msg() { return "Hello " + named_.name() + "!"; }
private:
  Named& named_;
};

using WorldProvider = yadif::Provider<World>;
using HelloProvider = yadif::Provider<Hello, yadif::Ref<Named>>;

struct ExampleModule : public yadif::Module
{
  void configure() const override
  {
    bind<Named>().to<World>();
    bind<World>().toProvider(WorldProvider{});
    bind<Hello>().toProvider(HelloProvider{});
  }
};

int main()
{
  auto injector = yadif::Injector{ExampleModule{}};
  auto helloPtr = injector.get<Hello>();
  std::cout << helloPtr->msg() << "\n";
}
```

Lets look at what's going on here.


```
#include <yadif.hpp>

```

You have to include the Yadif header to use Yadif. Obvious, but should be mentioned anyway.

Some std headers that are needed for for an compiling example. Followed by the actual classes we want to use in our example:

* An interface called `Named`.
* A class `World` that implements the `Named`interface.
* A class `Hello` that accepts a reference to a `Named` and uses it in it's definition. 

Next we define two type aliases for later use in our binding.

```
using WorldProvider = yadif::Provider<World>;
using HelloProvider = yadif::Provider<Hello, yadif::Ref<Named>>;
```

Remember, providers are the objects used for creating instances of bound types. The supplied `Provider` template takes the type to be created as its first argument. This is followed by a list of constructor arguments, each wrapped in a specification how the argument is to be injected.

In the case of the `WorldProvider`, we just have to specify the type to be created, as the `World` constructor is defaulted and takes no arguments. For `HelloProvider`, we specify an argument of type `Named` that will be injected by reference.

Now we are ready to configure our bindings. For this we must implement a module.

```c++
struct ExampleModule : public yadif::Module
{
  void configure() const override
  {
    ...
  }
};

```

Modules are used by Yadif to collect bindings for later use. Each module must implement the `Module` interface and overwrite the `configure()` method. All bindings are defined within the `configure()` method.

```c++
bind<Named>().to<World>();
```

The first binding is a type binding. It binds the interface `Named`to the implementation `World`. As a binding operates on types, the template parameter has to be supplied explicitly.

```c++
bind<World>().toProvider(WorldProvider{});
bind<Hello>().toProvider(HelloProvider{});
```  

The other bindings are provider bindings. They register a provider object that will be used to produce an instance of a type. Here we use the type aliases we have defined earlier to create Provider instances.

Now we are ready to use the module.

```c++
auto injector = yadif::Injector{ExampleModule{}};
```

An Injector collects bindings from a list of modules. After it is constructed, you may ask for a an instance of a bound type.

```c++
auto helloPtr = injector.get<Hello>();
	
```

The injectors `get()` method always returns a shared pointer to the requested type. So here the type of `helloPtr` is `std::shared_ptr<Hello>`.

And finally we use the injected object.

```c++
std::cout << helloPtr->msg() << "\n";
```

Let's just say that it will print something familiar ...

###Singletons

Sometimes you want to bind a type to a single instance created by the injector. You want it, you get it. Just append `.asSingleton()` to the binding.

###Annotations

What happens if you want to use two different implementations of an interface in your bindings? As it turns out that can be done using annotations. In fact, each function template in Yadif that takes a type for binding has an optional annotation parameter. The annotation is a type wrapped in by the `Annotation` template. 

Let's look at an example, it's not as complicated as it sounds.

Assume we have an interface `Guest` that has been implemented by the classes `Friend` and `Celebrity`. We could use the following module.

```c++
using yadif::Annotation;
struct Special {};

struct GuestModule : yadif::Module
  {
  void configure() const override
  {
    bind<Guest>().to<Friend>();
    bind<Guest, Annotation<Special>>.to<Celebrity>;
  }
};

int main()
{
  auto injector = yadif::Injector{GuestModule{}};
  
  guest1Ptr = injector.get<Guest>();                      // injects Friend instance
  guest2Ptr = injector.get<Guest, Annotation<Special>>(); // injects Celebrity instance
}
```

I we had a `Host` class that takes references to two Guest objects in its constructor, we could use the following provider:

```c++
using HostProvider = Provider<Host,
                             Ref<Guest>,
                             Ref<Guest, Annotation<Special>>
                             >;
```

###A Word About Providers

As providers are central to Yadif, they should be easy to write. Therefore Yadif has a rather flexible notion of providers.

```c++
bind<interface>().toProvider(P);
``` 

will accept any callable entity `P` that is

1. either callable as `P()` or by `P(Injector&)`
2. and returns something that can be used to construct a `std::shared_ptr<interface>`.

So, if you want to adapt your existing factory function/object or the yadif supplied provider generators do not meet your needs, just write your own. 


And speaking of supplied provider generators, what do we have?

* `Provider`- You've seen it. It's the Yadif workhorse for creating instances with injected arguments.
* `InstanceProvider<T>` - creates a copy of it's constructor argument and returns that instance.
* `ReferenceProvider<T>` - References an existing object.


---

Next: [Reference](reference.md)
