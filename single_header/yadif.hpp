/*
 *  YADIF v0.9 build 7 (master branch)
 *  Generated: 2015-01-21 13:30:54.244261
 *  ----------------------------------------------------------
 *  This file has been merged from multiple headers. Please don't edit it directly
 *  Copyright (c) 2015 Dirk Bonekamper. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef YADIF_SINGLE_HEADER_HPP
#define YADIF_SINGLE_HEADER_HPP

#include <memory>

namespace yadif {

  class Injector;

  namespace detail {

    using InstancePtr = std::shared_ptr<void>;

    struct InstanceSource
    {
      virtual InstancePtr get(Injector& injector) = 0;

      virtual ~InstanceSource() = default;
    };

    using SourcePtr = std::shared_ptr<InstanceSource>;
  }
}

namespace yadif {

  struct Scope : public detail::InstanceSource
  {
    using SourcePtr   = detail::SourcePtr;
    using InstancePtr = detail::InstancePtr;

    virtual void wrap(SourcePtr sourcePtr) = 0;
  };

  using ScopePtr = std::shared_ptr<Scope>;
}

namespace yadif {

  namespace detail {
    struct None {};
  }

  template<typename T = detail::None>
  struct Annotation
  {
    using type = T;
  };

  template<typename T>
  struct is_annotation : std::false_type {};

  template<typename T>
  struct is_annotation<Annotation<T>> : std::true_type {};
}

#include <typeindex>
#include <string>
#include <utility>

namespace yadif {
  namespace detail {

    template <typename T>
    std::type_index
    typeIndex()
    {
      return std::type_index(typeid(T));
    }

    class Key
    {
    public:
      Key(std::type_index typeId, std::type_index annotation)
        : key_{typeId, annotation}
      {}

      bool
      operator<(const Key& rhs) const
      {
        return key_ < rhs.key_;
      }

      std::string
      name() const
      {
        auto n = std::string("<") + key_.first.name();
        if (key_.second != typeIndex<Annotation<>>())
          n += std::string(", ") + key_.second.name();
        n += ">";
        return n;
      }

    private:
      // first contains typId, second annotation.
      std::pair<std::type_index, std::type_index> key_;
    };

    template <typename T>
    struct TypeWrap {};

    template <typename T, typename A = Annotation<>>
    Key key()
    {
      static_assert(is_annotation<A>::value, "second key() parameter must be an annotation");
      return Key(typeIndex<TypeWrap<T>>(), typeIndex<A>());
    }
  }
}

#include <stdexcept>

namespace yadif {

  struct Exception : public std::logic_error
  {
    using std::logic_error::logic_error;
  };

  struct NoBindingFound : public Exception
  {
    NoBindingFound(detail::Key key)
      : Exception("no binding found for " + key.name())
    {}
  };

  struct DuplicateBinding : public Exception
  {
    DuplicateBinding(detail::Key key)
      : Exception("duplicate binding for " + key.name())
    {}
  };

  struct IllegalBinding : public Exception
  {
    IllegalBinding(const std::string& msg)
      : Exception(msg)
    {}
  };
}

#include <cassert>

namespace yadif {
  namespace detail {
    class Binding;

    class Binding
    {
    public:

      void
      setSource(SourcePtr sourcePtr)
      {
        if (sourcePtr_)
          throw IllegalBinding("binding interface twice");

        sourcePtr_ = std::move(sourcePtr);
      }

      void
      setScope(ScopePtr scopePtr)
      {
        if (scopePtr_)
          throw IllegalBinding("binding scope twice");

        scopePtr_ = std::move(scopePtr);
      }

      void
      setEager(bool value)
      {
        isEager_ = value;
      }

      void
      finish()
      {
        if (!sourcePtr_)
          throw IllegalBinding("interface not bound");

        if (scopePtr_)
        {
          scopePtr_->wrap(std::move(sourcePtr_));
          sourcePtr_ = scopePtr_;
        }
      }

      InstanceSource&
      source()
      {
        assert(sourcePtr_);
        return *sourcePtr_;
      }

      bool
      isEager() const
      {
        return isEager_;
      }

    private:
      SourcePtr sourcePtr_;
      ScopePtr  scopePtr_;
      bool      isEager_ = false;
    };

  }
}

namespace yadif {
  namespace detail {
    class BindingStore;

    struct BindingCreator
    {
      virtual void createBindings(BindingStore& bindings) const = 0;
    };

  }
}

#include <map>

namespace yadif {
  namespace detail {

    class BindingStore
    {
    public:

      void
      add(Key key, Binding binding)
      {
        if (bindings_.count(key) != 0)
          throw DuplicateBinding(key);

        bindings_.insert(std::make_pair(key, binding));
      }

      void
      bindEagerSources(Injector& injector)
      {
        for (auto& e : bindings_)
        {
          auto& binding = e.second;

          if (binding.isEager())
            binding.source().get(injector);
        }
      }

      InstanceSource&
      source(Key key)
      {
        if (bindings_.count(key) == 0)
          throw NoBindingFound(key);

        return bindings_.at(key).source();
      }

    private:
      std::map<Key, Binding> bindings_;
    };

  }
}

namespace yadif {

  class Injector {
  public:
    template <typename... Modules>
    Injector(const Modules&... modules)
    {
      createBindings(modules...);
      bindings_.bindEagerSources(*this);
    }

    template <typename... Modules>
    Injector(const Injector& other, Modules... modules)
      : bindings_{other.bindings_}
    {
      createBindings(modules...);
      bindings_.bindEagerSources(*this);
    }

    template<typename T, typename A = Annotation<>>
    std::shared_ptr<T>
    get()
    {
      auto key = detail::key<T, A>();
      return std::static_pointer_cast<T>(bindings_.source(key).get(*this));
    }

  private:
    template <typename Module, typename... Modules>
    void
    createBindings(const Module& module, const Modules&... modules)
    {
      module.createBindings(bindings_);
      createBindings(modules...);
    }

    void
    createBindings()
    {}

  public:
    detail::BindingStore bindings_;
  };

}

#include <type_traits>

namespace yadif {
  class Injector;

  namespace detail {

    // --------------------------------------------------------------------------
    // Some type alias for std::traits (Actually available in C++14)
    // --------------------------------------------------------------------------

    template <typename T>
    using result_of_t = typename std::result_of<T>::type;

    template <bool Condition, typename TrueType, typename FalseType>
    using conditional_t = typename std::conditional<Condition, TrueType, FalseType>::type;

    // --------------------------------------------------------------------------
    // Nifty trick taken from n3911
    // --------------------------------------------------------------------------

    template< class... > struct voider { using type = void; };
    template< class... Ts> using void_t = typename voider<Ts...>::type;

    // --------------------------------------------------------------------------
    // Predicates that detect if a provider is callable with a specific interface
    // --------------------------------------------------------------------------

    template <typename F, typename Check = void>
    struct is_default_callable : std::false_type {};

    template <typename F>
    struct is_default_callable<F, void_t<result_of_t<F()>>> : std::true_type {};

    template <typename F, typename  = void>
    struct is_callable_with_injector : std::false_type {};

    template <typename F>
    struct is_callable_with_injector<F, void_t<result_of_t<F(Injector&)>>> : std::true_type {};
  }
}

#include <memory>

namespace yadif {
  namespace detail {

    template<typename Interface, typename Implementation, typename A>
    struct ConvertingSource : public InstanceSource
    {
      static_assert(std::is_convertible<Implementation*, Interface*>::value, "illegal binding");

      InstancePtr get(Injector& injector_) override
      {
        return std::shared_ptr<Interface>{injector_.get<Implementation, A>()};
      }
    };

  }
}

#include <memory>

namespace yadif {
  namespace detail {

    // --------------------------------------------------------------------------
    // helper template alias ProviderTag<F>
    // --------------------------------------------------------------------------

    namespace tag {
      struct DefaultCallable {};
      struct CallableWithInjector {};
      struct Illegal {};
    }

    template <typename F>
    using ProviderTag = conditional_t<is_default_callable<F>::value,
                                      tag::DefaultCallable,
                                      conditional_t<is_callable_with_injector<F>::value,
                                                    tag::CallableWithInjector,
                                                    tag::Illegal>>;
    // --------------------------------------------------------------------------
    // helper template alias ProviderResult<F>
    // --------------------------------------------------------------------------

    template <typename F, typename Tag>
    struct ProviderResultImpl
    {};

    template <typename F>
    struct ProviderResultImpl<F, tag::DefaultCallable>
    {
      using type = result_of_t<F()>;
    };

    template <typename F>
    struct ProviderResultImpl<F, tag::CallableWithInjector>
    {
      using type = result_of_t<F(Injector&)>;
    };

    template <typename F>
    using ProviderResult = typename ProviderResultImpl<F, ProviderTag<F>>::type;

    // --------------------------------------------------------------------------
    // template ProvidingSource<F>
    // --------------------------------------------------------------------------

    template <typename Interface, typename F>
    struct ProvidingSource : public InstanceSource
    {
      using SharedPtr = std::shared_ptr<Interface>;

      ProvidingSource(F&& f)
        : f_(std::forward<F>(f))
      {
        static_assert(!std::is_same<ProviderTag<F>, tag::Illegal>::value, "provider not callable");
        static_assert(std::is_constructible<std::shared_ptr<Interface>, ProviderResult<F>>::value,
                      "illegal provider return type");
      }

      template <typename FF>
      SharedPtr
      getInstance(FF f, Injector& injector, tag::DefaultCallable)
      {
        return SharedPtr{f()};
      }

      template <typename FF>
      SharedPtr
      getInstance(FF f, Injector& injector, tag::CallableWithInjector)
      {
        return SharedPtr{f(injector)};
      }

      InstancePtr
      get(Injector& injector) override
      {
        return getInstance(f_, injector, ProviderTag<F>{});
      }

      F f_;
  };

  }
}

namespace yadif {

  struct SingletonScope;

  namespace detail {

    template <typename T>
    struct EagerBinding
    {
      static constexpr bool value = std::is_same<T, SingletonScope>::value ?
#ifdef YADIF_EAGER_SINGLETON
        true
#else
        false
#endif
        : false;
    };

  }

  struct SingletonScope : public Scope
  {
    void
    wrap(SourcePtr sourcePtr) override
    {
      sourcePtr_ = sourcePtr;
    }

    InstancePtr
    get(Injector& injector) override
    {
      if (!instancePtr_)
        instancePtr_ = sourcePtr_->get(injector);

      return instancePtr_;
    }

    SourcePtr   sourcePtr_;
    InstancePtr instancePtr_;
  };

}

#include <memory>

namespace yadif {
  namespace detail {

    template <typename Interface>
    class Binder {
    public:
      Binder(Binding& binding)
        : binding_{binding}
      {}

      template <typename Implementation, typename A = Annotation<>>
      Binder&
      to()
      {
        static_assert(is_annotation<A>::value, "second to() parameter must be an annotation");
        binding_.setSource(std::make_shared<ConvertingSource<Interface, Implementation, A>>());
        return *this;
      }

      template <typename TypeProvider>
      Binder&
      toProvider(TypeProvider&& provider)
      {
        binding_.setSource(std::make_shared<ProvidingSource<Interface, TypeProvider>>(std::forward<TypeProvider>(provider)));
        return *this;
      }

      template <typename ScopeType>
      Binder&
      in(ScopeType&& scope)
      {
        setScope(std::forward<ScopeType>(scope), EagerBinding<ScopeType>::value);
        return *this;
      }

      Binder&
      asSingleton()
      {
        return in(SingletonScope{});
      }

      Binder&
      asEagerSingleton()
      {
        setScope(SingletonScope{}, true);
        return *this;
      }

    private:

      template <typename ScopeType>
      void
      setScope(ScopeType&& scope, bool eager)
      {
        binding_.setEager(eager);
        binding_.setScope(std::make_shared<ScopeType>(std::forward<ScopeType>(scope)));
      }

      Binding& binding_;
    };
  }
}

#include <vector>

namespace yadif {

  class Module : public detail::BindingCreator
  {
  private:
    virtual void configure() const = 0;

  public:

    void
    createBindings(detail::BindingStore& bindings) const override
    {
      cache_.clear();
      configure();

      for (auto& e : cache_)
      {
        e.binding.finish();
        bindings.add(e.key, e.binding);
      }
    }

  protected:

    template <typename Interface, typename A = Annotation<>>
    detail::Binder<Interface> bind() const
    {
      static_assert(is_annotation<A>::value, "second bind() parameter must be an annotation");

      cache_.emplace_back(detail::key<Interface, A>());
      return detail::Binder<Interface>{cache_.back().binding};
    }

  private:
    struct CachedBinding
    {
      CachedBinding(detail::Key k)
        : key(k)
      {}

      detail::Key key;
      detail::Binding binding;
    };

    mutable std::vector<CachedBinding> cache_;
  };

}

#include <cstddef>

namespace yadif {
  namespace detail {
    template<std::size_t ...I>
    struct index_sequence
    {
      static constexpr std::size_t size() {
        return sizeof...(I);
      }
    };

    template<std::size_t N, std::size_t ...S>
    struct gen_index_sequence : gen_index_sequence<N - 1, N - 1, S...>
    {};

    template<std::size_t ...S>
    struct gen_index_sequence<0, S...>
    {
      using type = index_sequence<S...>;
    };

    template<std::size_t N>
    using make_index_sequence = typename gen_index_sequence<N>::type;
  }
}

#include <memory>
#include <tuple>
#include <utility>

namespace yadif {
  namespace detail {

    // -------------------------------------------------------------------
    // tie lifetime of injected objects by embedding them in deleter
    // -------------------------------------------------------------------

    template <typename T, typename Others>
    class TieDeleter {
    public:
      TieDeleter(Others& others) : others_(others) {}

      void operator () (T* p) const { delete p; }
    private:
      Others others_;
    };

  }

  // ---------------------------------------------------------------------
  // helper templates representing argument bindings
  // ---------------------------------------------------------------------

  template<typename T, typename A = Annotation<>>
  struct Ref
  {
    static_assert(is_annotation<A>::value, "second Ref parameter must be an annotation");

    using ValueType      = T;
    using AnnotationType = A;
    using ParamType      = T&;

    static ParamType transform(std::shared_ptr<T>& sp) { return *sp; }
  };

  template<typename T, typename A = Annotation<>>
  struct Ptr
  {
    static_assert(is_annotation<A>::value, "second Ptr parameter must be an annotation");

    using ValueType      = T;
    using AnnotationType = A;
    using ParamType      = T*;

    static ParamType transform(std::shared_ptr<T>& sp) { return sp.get(); }
  };

  template<typename T, typename A = Annotation<>>
  struct SharedPtr
  {
    static_assert(is_annotation<A>::value, "second SharedPtr parameter must be an annotation");

    using ValueType      = T;
    using AnnotationType = A;
    using ParamType      = std::shared_ptr<T>;

    static ParamType transform(std::shared_ptr<T>& sp) { return sp; }
  };

  // ---------------------------------------------------------------------
  // provider generator template
  // ---------------------------------------------------------------------

  template <typename T, typename... Args>
  struct Provider
  {
    using Instances = std::tuple<std::shared_ptr<typename Args::ValueType>...>;
    using Parameters = std::tuple<typename Args::ParamType...>;
    using Deleter = detail::TieDeleter<T, Instances>;
    using Indices = detail::make_index_sequence<sizeof...(Args)>;

    static_assert(std::is_constructible<T, typename Args::ParamType...>::value, "no matching ctor found");

    Instances
    getInstances(Injector& injector)
    {
      return Instances(injector.get<typename Args::ValueType, typename Args::AnnotationType>()...);
    }

    template <size_t... I>
    Parameters
    getParametersImpl(Instances& instances, detail::index_sequence<I...>)
    {
      return Parameters(Args::transform(std::get<I>(instances))...);
    }

    Parameters
    getParameters(Instances& instances)
    {
      return getParametersImpl(instances, Indices{});
    }

    template <size_t... I>
    T*
    constructImpl(const Parameters& p, detail::index_sequence<I...>)
    {
      return new T(std::get<I>(p)...);
    }

    T*
    construct(const Parameters& p)
    {
      return constructImpl(p, Indices{});
    }

    std::shared_ptr<T>
    operator() (Injector& injector)
    {
      auto instances = getInstances(injector);
      return std::shared_ptr<T>(construct(getParameters(instances)),
                                Deleter(instances));
    }
  };

}

#include <memory>

namespace yadif {

  template <typename T>
  class InstanceProvider
  {
  public:
    InstanceProvider(T val)
      : instancePtr_{std::make_shared<T>(val)}
    {}

    std::shared_ptr<T>
    operator() ()
    {
      return instancePtr_;
    }

  private:
    std::shared_ptr<T> instancePtr_;
  };

  template <typename T>
  InstanceProvider<T>
  makeInstanceProvider(T val)
  {
    return InstanceProvider<T>(std::move(val));
  }
}

#include <memory>

namespace yadif {

  namespace detail {
    struct NoDeleter
    {
      void operator () (void* p) const {}
    };
  }

  template <typename T>
  class ReferenceProvider
  {
  public:

    ReferenceProvider(T& ref)
      : instancePtr_{&ref, detail::NoDeleter{}}
    {}

    std::shared_ptr<T>
    operator() ()
    {
      return instancePtr_;
    }

  private:
    std::shared_ptr<T> instancePtr_;
  };

  template <typename T>
  ReferenceProvider<T>
  makeReferenceProvider(T& ref)
  {
    return ReferenceProvider<T>(ref);
  }

}

#endif//ndef YADIF_SINGLE_HEADER_HPP
