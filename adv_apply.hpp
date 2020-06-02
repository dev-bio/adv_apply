#ifndef ADV_APPLY_HPP
#define ADV_APPLY_HPP

#include <utility>
#include <tuple>
 
namespace adv
{
	namespace type
	{
		using index_t = unsigned;
	}
 
	namespace utility
	{
		template <typename ... Types>
		struct type_carrier
		{
			using carrier_type = type_carrier<Types ... >;
 
			static constexpr type::index_t size()
			{
				return static_cast<type::index_t>(sizeof ... (Types));
			}
		};
 
		template <unsigned ... Indices>
		struct index_carrier
		{
			using carrier_type = index_carrier<Indices ... >;
 
			static constexpr type::index_t size()
			{
				return static_cast<type::index_t>(sizeof ... (Indices));
			}
		};
 
		namespace detail
		{
			template <typename, typename>
			struct type_carrier_contains;
 
			template <typename Type, typename Current>
			struct type_carrier_contains<type_carrier<Current>, Type>
			{
				enum : bool { value = std::is_same<Current, Type>::value };
			};
 
			template <typename Type, typename Current, typename Next, typename ... Rest>
			struct type_carrier_contains<type_carrier<Current, Next, Rest ... >, Type>
			{
				enum : bool { value = type_carrier_contains<type_carrier<Current>, Type>::value || type_carrier_contains<type_carrier<Next, Rest ... >, Type>::value };
			};
 
			template <typename, unsigned>
			struct index_carrier_contains;
 
			template <unsigned Index, unsigned Current>
			struct index_carrier_contains<index_carrier<Current>, Index>
			{
				enum : bool { value = Current == Index };
			};
 
			template <unsigned Index, unsigned Current, unsigned Next, unsigned ... Rest>
			struct index_carrier_contains<index_carrier<Current, Next, Rest ... >, Index>
			{
				enum : bool { value = index_carrier_contains<index_carrier<Current>, Index>::value || index_carrier_contains<index_carrier<Next, Rest ... >, Index>::value };
			};
		}
 
		template <typename, typename>
		struct carrier_contains;
 
		template <typename ... Types, typename Current>
		struct carrier_contains<type_carrier<Types ... >, type_carrier<Current>>
		{
			enum : bool { value = detail::type_carrier_contains<type_carrier<Types ... >, Current>::value };
		};
 
		template <typename ... Types, typename Current, typename Next, typename ... Rest>
		struct carrier_contains<type_carrier<Types ... >, type_carrier<Current, Next, Rest ... >>
		{
			enum : bool { value = detail::type_carrier_contains<type_carrier<Types ... >, Current>::value&& carrier_contains<type_carrier<Types ... >, type_carrier<Next, Rest ... >>::value };
		};
 
		template <unsigned ... Indices, unsigned Current>
		struct carrier_contains<index_carrier<Indices ... >, index_carrier<Current>>
		{
			enum : bool { value = detail::index_carrier_contains<index_carrier<Indices ... >, Current>::value };
		};
 
		template <unsigned ... Indices, unsigned Current, unsigned Next, unsigned ... Rest>
		struct carrier_contains<index_carrier<Indices ... >, index_carrier<Current, Next, Rest ... >>
		{
			enum : bool { value = detail::index_carrier_contains<index_carrier<Indices ... >, Current>::value&& carrier_contains<index_carrier<Indices ... >, index_carrier<Next, Rest ... >>::value };
		};
 
		// ----------------------------------------->
 
		namespace detail {
			template <typename, typename>
			struct type_carrier_count;
 
			template <typename Type>
			struct type_carrier_count<type_carrier<>, Type>
			{
				enum : type::index_t { value = 0 };
			};
 
			template <typename Type, typename Current>
			struct type_carrier_count<type_carrier<Current>, Type>
			{
				enum : type::index_t { value = std::is_same<Current, Type>::value ? 1 : 0 };
			};
 
			template <typename Type, typename Current, typename ... Rest>
			struct type_carrier_count<type_carrier<Current, Rest ... >, Type>
			{
				enum : type::index_t { value = type_carrier_count<type_carrier<Current>, Type>::value + type_carrier_count<type_carrier<Rest ... >, Type>::value };
			};
 
			template <typename, unsigned>
			struct index_carrier_count;
 
			template <unsigned Index>
			struct index_carrier_count<index_carrier<>, Index>
			{
				enum : type::index_t { value = 0 };
			};
 
			template <unsigned Index, unsigned Current>
			struct index_carrier_count<index_carrier<Current>, Index>
			{
				enum : type::index_t { value = (Current == Index) ? 1 : 0 };
			};
 
			template <unsigned Index, unsigned Current, unsigned ... Rest>
			struct index_carrier_count<index_carrier<Current, Rest ... >, Index>
			{
				enum : type::index_t { value = index_carrier_count<index_carrier<Current>, Index>::value + index_carrier_count<index_carrier<Rest ... >, Index>::value };
			};
		}
 
		template <typename, typename>
		struct carrier_count;
 
		template <typename ... Types, typename Current>
		struct carrier_count<type_carrier<Types ... >, type_carrier<Current>>
		{
			enum : type::index_t { value = detail::type_carrier_count<type_carrier<Types ... >, Current>::value };
		};
 
		template <unsigned ... Indices, unsigned Current>
		struct carrier_count<index_carrier<Indices ... >, index_carrier<Current>>
		{
			enum : type::index_t { value = detail::index_carrier_contains<index_carrier<Indices ... >, Current>::value };
		};
 
		// ----------------------------------------->
 
		namespace detail {
			template <typename, typename, unsigned, unsigned, unsigned>
			struct type_carrier_nth_index;
 
			template <typename Type, unsigned Index, unsigned Count, unsigned Target>
			struct type_carrier_nth_index<type_carrier<>, Type, Index, Count, Target>
			{
				enum : type::index_t { value = 0 };
			};
 
			template <typename Type, typename Current, unsigned Index, unsigned Count, unsigned Target>
			struct type_carrier_nth_index<type_carrier<Current>, Type, Index, Count, Target>
			{
				enum : type::index_t { value = ((Count == Target) && std::is_same<Current, Type>::value) ? Index : 0 };
			};
 
			template <typename Type, typename Current, typename ... Rest, unsigned Index, unsigned Count, unsigned Target>
			struct type_carrier_nth_index<type_carrier<Current, Rest ... >, Type, Index, Count, Target>
			{
				enum : type::index_t
				{
					value = type_carrier_nth_index<type_carrier<Current>, Type, Index, Count, Target>::value + type_carrier_nth_index<type_carrier<Rest ... >, Type, Index + 1, Count + (std::is_same<Current, Type>::value ? 1 : 0), Target>::value
				};
			};
 
			template <typename, unsigned, unsigned, unsigned, unsigned>
			struct index_carrier_nth_index;
 
			template <unsigned Other, unsigned Index, unsigned Count, unsigned Target>
			struct index_carrier_nth_index<index_carrier<>, Other, Index, Count, Target>
			{
				enum : type::index_t { value = 0 };
			};
 
			template <unsigned Other, unsigned Current, unsigned Index, unsigned Count, unsigned Target>
			struct index_carrier_nth_index<index_carrier<Current>, Other, Index, Count, Target>
			{
				enum : type::index_t { value = ((Count == Target) && (Current == Other)) ? Index : 0 };
			};
 
			template <unsigned Other, unsigned Current, unsigned ... Rest, unsigned Index, unsigned Count, unsigned Target>
			struct index_carrier_nth_index<index_carrier<Current, Rest ... >, Other, Index, Count, Target>
			{
				enum : type::index_t { value = index_carrier_nth_index<index_carrier<Current>, Other, Index, Count, Target>::value + index_carrier_nth_index<index_carrier<Rest ... >, Other, Index + 1, Count + (Current == Other ? 1 : 0), Target>::value };
			};
		}
 
		template <typename, typename, unsigned>
		struct carrier_nth_index;
 
		template <typename ... Types, typename Current, unsigned Number>
		struct carrier_nth_index<type_carrier<Types ... >, Current, Number>
		{
			static_assert(carrier_count<type_carrier<Types ... >, type_carrier<Current>>::value > Number, "Type carrier does not contain type with this index!");
			enum : type::index_t { value = detail::type_carrier_nth_index<type_carrier<Types ... >, Current, 0, 0, Number>::value };
		};
 
		template <unsigned ... Indices, unsigned Current, unsigned Number>
		struct carrier_nth_index<index_carrier<Indices ... >, index_carrier<Current>, Number>
		{
			static_assert(carrier_count<index_carrier<Indices ... >, index_carrier<Current>>::value > Number, "Index carrier does not contain index with this index!");
			enum : type::index_t { value = detail::index_carrier_nth_index<index_carrier<Indices ... >, Current, 0, 0, Number>::value };
		};
 
		namespace detail
		{
			template <typename>
			struct parameters;
 
			template <typename Return, typename Type, typename ... Arguments>
			struct parameters<Return(Type::*)(Arguments ...)>
			{
				using type = type_carrier<std::decay_t<Arguments> ... >;
			};
 
			template <typename Return, typename Type, typename ... Arguments>
			struct parameters<Return(Type::*)(Arguments ...) const>
			{
				using type = type_carrier<std::decay_t<Arguments> ... >;
			};
		}
 
		template <typename Type>
		using parameters_t = typename detail::parameters<decltype(&Type::operator ())>::type;
 
		namespace detail
		{
			template <typename, typename, typename, typename>
			struct map_parameters_to_carrier;
 
			template <typename Parameter, typename ... Types>
			struct map_parameters_to_carrier<type_carrier<Parameter>, type_carrier<Types ... >, index_carrier<>, type_carrier<>>
			{
				using type = index_carrier<carrier_nth_index<type_carrier<Types ... >, Parameter, 0>::value>;
			};
 
			template <typename Parameter, typename ... Types, unsigned ... Indices, typename ... Visited>
			struct map_parameters_to_carrier<type_carrier<Parameter>, type_carrier<Types ... >, index_carrier<Indices ... >, type_carrier<Visited ... >>
			{
				using type = index_carrier<Indices ... , carrier_nth_index<type_carrier<Types ... >, Parameter, carrier_count<type_carrier<Visited ... >, type_carrier<Parameter>>::value>::value>;
			};
 
			template <typename Current, typename ... Parameters, typename ... Types>
			struct map_parameters_to_carrier<type_carrier<Current, Parameters ... >, type_carrier<Types ... >, index_carrier<>, type_carrier<>>
			{
				using type = typename map_parameters_to_carrier<type_carrier<Parameters ... >, type_carrier<Types ... >, index_carrier<carrier_nth_index<type_carrier<Types ... >, Current, 0>::value>, type_carrier<Current>>::type;
			};
 
			template <typename Current, typename ... Parameters, typename ... Types, unsigned ... Indices, typename ... Visited>
			struct map_parameters_to_carrier<type_carrier<Current, Parameters ... >, type_carrier<Types ... >, index_carrier<Indices ... >, type_carrier<Visited ... >>
			{
				using type = typename map_parameters_to_carrier<type_carrier<Parameters ... >, type_carrier<Types ... >, index_carrier<Indices ... , carrier_nth_index<type_carrier<Types ... >, Current, carrier_count<type_carrier<Visited ... >, type_carrier<Current>>::value>::value>, type_carrier<Visited ... , Current>>::type;
			};
		}
 
		template <typename Parameters, typename Carrier>
		using map_parameters_to_carrier_t = typename detail::map_parameters_to_carrier<Parameters, Carrier, index_carrier<>, type_carrier<>>::type;
	}
 
	namespace detail
	{
		template <typename ... Types, unsigned ... Indices, typename Functor>
		inline constexpr decltype(auto) this_with(std::tuple<Types ... > & object, utility::index_carrier<Indices ... >, Functor && functor)
		{
			return functor(std::get<Indices>(object) ...);
		}

		template <typename ... Types, unsigned ... Indices, typename Functor>
		inline constexpr decltype(auto) this_with(std::tuple<Types ... > const & object, utility::index_carrier<Indices ... >, Functor && functor)
		{
			return functor(std::get<Indices>(object) ...);
		}

		template <typename ... Types, unsigned ... Indices, typename Functor>
		inline constexpr decltype(auto) this_with(std::pair<Types ... > & object, utility::index_carrier<Indices ... >, Functor && functor)
		{
			return functor(std::get<Indices>(object) ...);
		}

		template <typename ... Types, unsigned ... Indices, typename Functor>
		inline constexpr decltype(auto) this_with(std::pair<Types ... > const & object, utility::index_carrier<Indices ... >, Functor && functor)
		{
			return functor(std::get<Indices>(object) ...);
		}
	}
 
	template <typename ... Types, typename Functor>
	inline constexpr decltype(auto) apply(std::tuple<Types ... > & object, Functor && functor)
	{
		static_assert(utility::carrier_contains<utility::type_carrier<Types ... >, utility::parameters_t<Functor>>::value, "This tuple does not contain one or more of these types!");
 
		constexpr auto parameters = utility::map_parameters_to_carrier_t<utility::parameters_t<Functor>, utility::type_carrier<Types ... >>
		{
			// ...
		};
 
		return detail::this_with(object, parameters, std::forward<Functor>(functor));
	}

	template <typename ... Types, typename Functor>
	inline constexpr decltype(auto) apply(std::tuple<Types ... > const & object, Functor && functor)
	{
		static_assert(utility::carrier_contains<utility::type_carrier<Types ... >, utility::parameters_t<Functor>>::value, "This tuple does not contain one or more of these types!");
 
		constexpr auto parameters = utility::map_parameters_to_carrier_t<utility::parameters_t<Functor>, utility::type_carrier<Types ... >>
		{
			// ...
		};
 
		return detail::this_with(object, parameters, std::forward<Functor>(functor));
	}

	template <typename ... Types, typename Functor>
	inline constexpr decltype(auto) apply(std::pair<Types ... > & object, Functor && functor)
	{
		static_assert(utility::carrier_contains<utility::type_carrier<Types ... >, utility::parameters_t<Functor>>::value, "This pair does not contain one or more of these types!");
 
		constexpr auto parameters = utility::map_parameters_to_carrier_t<utility::parameters_t<Functor>, utility::type_carrier<Types ... >>
		{
			// ...
		};
 
		return detail::this_with(object, parameters, std::forward<Functor>(functor));
	}

	template <typename ... Types, typename Functor>
	inline constexpr decltype(auto) apply(std::pair<Types ... > const & object, Functor && functor)
	{
		static_assert(utility::carrier_contains<utility::type_carrier<Types ... >, utility::parameters_t<Functor>>::value, "This pair does not contain one or more of these types!");
 
		constexpr auto parameters = utility::map_parameters_to_carrier_t<utility::parameters_t<Functor>, utility::type_carrier<Types ... >>
		{
			// ...
		};
 
		return detail::this_with(object, parameters, std::forward<Functor>(functor));
	}
}
 
#endif