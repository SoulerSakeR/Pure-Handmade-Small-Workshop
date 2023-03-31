#pragma once
#include <type_traits>

// primary template handles types that have no nested ::type member:
template< class, class = std::void_t<> >
struct has_type_member : std::false_type { };

// specialization recognizes types that do have a nested ::type member:
template< class T >
struct has_type_member<T, std::void_t<typename T::customType>> : std::true_type { };
