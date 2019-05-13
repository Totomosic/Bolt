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
		inline void _check_get(lua_State* l, int index)
		{
			
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
		inline int64_t _check_get(lua_State* l, int index)
		{
			if (!lua_isinteger(l, index))
			{
				BLT_ASSERT(false, "Value at index {} was not an int", index);
				return 0;
			}
			return (int64_t)lua_tointeger(l, index);
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
		inline double _check_get(lua_State* l, int index)
		{
			if (!lua_isnumber(l, index))
			{
				BLT_ASSERT(false, "Value at index {} was not a float", index);
				return 0;
			}
			return (double)lua_tonumber(l, index);
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

		template<>
		inline const char* _check_get(lua_State* l, int index)
		{
			if (!lua_isstring(l, index))
			{
				BLT_ASSERT(false, "Value at index {} was not a string", index);
				return "";
			}
			return lua_tostring(l, index);
		}

		template<typename T>
		inline void _push(lua_State* l, T value)
		{
			BLT_ASSERT(false, "Unable to push type {}", typeid(T).name());
		}

		template<>
		inline void _push(lua_State* l, int value)
		{
			lua_pushinteger(l, value);
		}

		template<>
		inline void _push(lua_State* l, int64_t value)
		{
			lua_pushinteger(l, value);
		}

		template<>
		inline void _push(lua_State* l, float value)
		{
			lua_pushnumber(l, value);
		}

		template<>
		inline void _push(lua_State* l, double value)
		{
			lua_pushnumber(l, value);
		}

		template<>
		inline void _push(lua_State* l, const char* value)
		{
			lua_pushstring(l, value);
		}

		template<>
		inline void _push(lua_State* l, blt::string value)
		{
			_push(l, value.c_str());
		}

		template<uint32_t... Is>
		struct _indices {};

		template<uint32_t N, uint32_t... Is>
		struct _indices_builder : public _indices_builder<N - 1, N - 1, Is...> {};

		template<uint32_t... Is>
		struct _indices_builder<0, Is...>
		{
			using type = _indices<Is...>;
		};

		template<typename... T, uint32_t... N>
		inline std::tuple<T...> _get_args(lua_State* l, _indices<N...>)
		{
			return std::make_tuple<T...>(_check_get<T>(l, N + 1)...);
		}

		template<typename... T>
		inline std::tuple<T...> _get_args(lua_State * l)
		{
			return _get_args<T...>(l, typename _indices_builder<sizeof...(T)>::type());
		}

		template<typename Ret, typename... Args, uint32_t... N>
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