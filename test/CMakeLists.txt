add_executable(yadif_test
  annotation_test.cpp
  binder_test.cpp
  binder_with_eager_singleton_test.cpp  binding_test.cpp
  binding_store_test.cpp
  index_sequence_test.cpp
  injector_test.cpp
  instance_provider_test.cpp
  key_test.cpp
  module_test.cpp
  provider_test.cpp
  providing_source_test.cpp
  reference_provider_test.cpp
  type_traits_test.cpp
  yadif_basic_binding_test.cpp
  yadif_eager_singleton_binding_test.cpp
  yadif_mi_binding_test.cpp
  yadif_singleton_binding_test.cpp
  catch_main.cpp
)

add_custom_command(
  TARGET yadif_test
  POST_BUILD
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/yadif_test
)

add_test(yadif_test yadif_test)
