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
