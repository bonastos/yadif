*v1.0 build 1 (master branch)*

#Yadif
Yadif is a lightweight, nonintrusive C++11 dependency injection framework. It is distributed as a single header. Concepts are (loosely) modelled after the Java Guice framework. The name actually stands for **Y**et **A**nother **D**ependency **I**njection **F**ramework.

###Introduction

Dependency injection is an object oriented design pattern that uses inversion of control to reduce coupling between objects.

Although widely used in the Java community, in C++ dependency injection is mostly noted by its absence. This probably stems from the fact that C++ doesn't have programmable annotations or a class loader that assists with object creation. In C++ every object must be created by calling a constructor. This used to mean that a lot of factory objects had to be written, and dependency injection was just to tedious to be helpful.

But C++11 changed the picture. Variadic templates allow to write functions that overload on a variable number of arguments. A good example (and also used for object creation) is *std::make_shared<>*. Yadif uses similar techniques to simplify the generation of factory objects.

###Documentation

* [Tutorial](docs/tutorial.md)
* [Reference](docs/reference.md)
* [Design Rationale](docs/design_rationale.md)

###Compilers
Currently, Yadif compiles with clang 3.5 and gcc 4.9.

###Acknowledgements

* Thanks to the [Guice](https://github.com/google/guice) community for providing a good role model.
* Thanks to [philsquared](https://github.com/philsquared) for the single header idea. And, of course, for [Catch](https://github.com/philsquared/Catch).

