#pragma once
#include "Types.h"

namespace Bolt
{

	namespace lua_help
	{

		template<typename T>
		inline T _check_get(lua_State* l, int index)
		{
			BLT_ASSERT(false, "Unable to get type {}", typeid(T).name());
			return T();
		}

		template<>
		inline int _check_get(lua_State* l, int index)
		{
			if (!lua_isinteger(l, index))
			{
				BLT_ASSERT(false, "Value at index {} was not an int", index);
				return 0;
			}
			return (int)lua_tointeger(l, index);
		}

		template<>
		inline float _check_get(lua_State* l, int index)
		{
			if (!lua_isnumber(l, index))
			{
				BLT_ASSERT(false, "Value at index {} was not a float", index);
				return 0;
			}
			return (float)lua_tonumber(l, index);
		}

		template<>
		inline blt::string _check_get(lua_State* l, int index)
		{
			if (!lua_isstring(l, index))
			{
				BLT_ASSERT(false, "Value at index {} was not a string", index);
				return "";
			}
			return blt::string(lua_tostring(l, index));
		}

		template<typename T>
		inline void _push(lua_State* l, const T& value)
		{
			BLT_ASSERT(false, "Unable to push type {}", typeid(T).name());
		}

		template<>
		inline void _push(lua_State* l, const int& value)
		{
			lua_pushinteger(l, value);
		}

		template<>
		inline void _push(lua_State* l, const float& value)
		{
			lua_pushnumber(l, value);
		}

		template<>
		inline void _push(lua_State* l, const blt::string& value)
		{
			lua_pushstring(l, value.c_str());
		}

		template<size_t... Is>
		struct _indices {};

		template<size_t N, size_t... Is>
		struct _indices_builder : public _indices_builder<N - 1, N - 1, Is...> {};

		template<size_t... Is>
		struct _indices_builder<0, Is...>
		{
			using type = _indices<Is...>;
		};

		template<typename... T, size_t... N>
		inline std::tuple<T...> _get_args(lua_State* l, _indices<N...>)
		{
			return std::make_tuple<T...>(_check_get<T>(l, N + 1)...);
		}

		template<typename... T>
		inline std::tuple<T...> _get_args(lua_State * l)
		{
			constexpr size_t argCount = sizeof...(T);
			return _get_args<T...>(l, typename _indices_builder<argCount>::type());
		}

		template<typename Ret, typename... Args, size_t... N>
		inline Ret _lift(std::function<Ret(Args...)> func, std::tuple<Args...> args, _indices<N...>)
		{
			return func(std::get<N>(args)...);
		}

		template<typename Ret, typename... Args>
		inline Ret _lift(std::function<Ret(Args...)> func, std::tuple<Args...> args)
		{
			return _lift(std::move(func), std::move(args), typename _indices_builder<sizeof...(Args)>::type());
		}

	}

}