/**
 * @file rlbox_type_traits.hpp
 * @copyright Copyright (c) 2022 UCSD PLSysSec. This project is released under
 * the MIT License. You can obtain a copy of the License at
 * https://raw.githubusercontent.com/PLSysSec/rlbox/master/LICENSE
 * @brief This header contains misc type traits useful for rlbox
 */

#pragma once

#include <array>
#include <type_traits>

namespace rlbox::detail {

/**
 * @brief An expression that always evaluates to false. Useful for
 * static_assert(false) that is evaluated inside a constexpr.
 * @details See https://quuxplusone.github.io/blog/2018/04/02/false-v/ for more
 * details
 * @tparam T is a template parameter to ensure that the expression is evaluated
 * lazily
 */
template <typename T>
inline constexpr bool false_v = true;

namespace copy_cvref_detail {
/**
 * @brief This trait is the internal helper to implement copy_cvref_t
 * @tparam TSource is the type from which we copy the cvref qualifiers
 * @tparam TDest is the type to which we copy the cvref qualifiers
 */
template <typename TSource, typename TDest>
struct copy_cvref {
  // Implementation adapted from https://stackoverflow.com/a/31173086
 private:
  using R = std::remove_reference_t<TSource>;
  using U1 = std::conditional_t<std::is_const<R>::value,
                                std::add_const_t<TDest>, TDest>;
  using U2 = std::conditional_t<std::is_volatile<R>::value,
                                std::add_volatile_t<U1>, U1>;
  using U3 = std::conditional_t<std::is_lvalue_reference<TSource>::value,
                                std::add_lvalue_reference_t<U2>, U2>;
  using U4 = std::conditional_t<std::is_rvalue_reference<TSource>::value,
                                std::add_rvalue_reference_t<U3>, U3>;

 public:
  using type = U4;
};
}  // namespace copy_cvref_detail

/**
 * @brief Copies the cvref qualifiers from `TSource` to `TDest`.
 * @code
 * static_assert(std::is_same_t<copy_cvref_t<const int&, short>, const short&>);
 * @endcode
 * @tparam TSource is the type from which we copy the cvref qualifiers
 * @tparam TDest is the type to which we copy the cvref qualifiers
 */
template <typename TSource, typename TDest>
using copy_cvref_t =
    typename copy_cvref_detail::copy_cvref<TSource, TDest>::type;

/**
 * @brief Polyfill for C++20's std::remove_cvref_t
 * @tparam T is the type for which we are removing cvref qualifiers
 */
template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

/**
 * @brief A trait that returns true if the type `T` has any cvref qualifiers
 * @tparam T is the type for which is checked for cvref qualifiers
 */
template <typename T>
inline constexpr bool is_cvref_t = !std::is_same_v<remove_cvref_t<T>, T>;

///////////////////////////////////////////////////////////////////////////////

/**
 * @brief If T is a C array, this trait converts it to the std::array
 * type for this, else T is returned.
 * @tparam T is the type to convert
 * @code
 * static_assert(std::is_same_t<c_array_to_std_array_t<int[3]>, std::array<int,
 * 3>>); static_assert(std::is_same_t<c_array_to_std_array_t<int>, int>);
 * @endcode
 */
template <typename T>
using c_array_to_std_array_t =
    std::conditional_t<std::is_array_v<T>,
                       std::array<std::remove_extent_t<T>, std::extent_v<T>>,
                       T>;

/**
 * @brief Returns the value-type equivalent of the given type. Currently this is
 * only needed to convert c style arrays to std::arrays
 * @tparam T is the type to convert
 */
template <typename T>
using value_type_t = c_array_to_std_array_t<T>;

}  // namespace rlbox::detail
