#pragma once

#include "rlbox_tainted_relocatable.hpp"
#include "rlbox_tainted_volatile_standard.hpp"
#include "rlbox_wrapper_traits.hpp"

/**
 * @file rlbox_sandbox.hpp
 * @brief This header implements the rlbox_sandbox class.
 */

namespace rlbox {

/**
 * @brief An rlbox sandbox type tha provides APIs to invoke functions in the
 * sandboxed component, expose callbacks, retrieve information about the sandbox
 * memory etc.
 * @tparam TSbx is the type of the sandbox plugin that represents the underlying
 * sandbox implementation. For the "null" sandbox, the value is
 * rlbox_noop_sandbox.
 */
template<typename TSbx>
class rlbox_sandbox
{

private:
  /**
   * @brief The default implementation of tainted unless overridden by TSbx
   */
  template<typename T>
  using TDefaultTainted = tainted_relocatable<T, TSbx>;

  /**
   * @brief The default implementation of tainted_volatile unless overridden by
   * TSbx
   */
  template<typename T>
  using TDefaultTaintedVolatile = tainted_volatile_standard<T, TSbx>;

public:
  /**
   * @brief The tainted type used by the underlying TSbx specification. If
   * unspecified this is tainted_relocatable by default.
   *
   * @tparam T is the type of the data that is wrapped.
   * @tparam TSbx is the type of the sandbox plugin that represents the
   * underlying sandbox implementation.
   */
  template<typename T>
  using tainted =
    detail::get_typemember_tainted_or_default_t<TSbx, TDefaultTainted, T>;

  /**
   * @brief The tainted_volatile type used by the underlying TSbx specification.
   * If unspecified this is tainted_volatile_standard by default.
   *
   * @tparam T is the type of the data that is wrapped.
   * @tparam TSbx is the type of the sandbox plugin that represents the
   * underlying sandbox implementation.
   */
  template<typename T>
  using tainted_volatile = detail::get_typemember_tainted_volatile_or_default_t<
    TSbx,
    TDefaultTaintedVolatile,
    T>;
};

}