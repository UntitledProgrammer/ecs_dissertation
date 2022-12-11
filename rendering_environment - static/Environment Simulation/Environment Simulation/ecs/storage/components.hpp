#ifndef NP_ECS_COMPONENTS_HPP
#define NP_ECS_COMPONENTS_HPP

//Includes:
#include "../allocators/components.hpp"
#include "../logic/bitmask.hpp"

#define BitmaskSize 10

namespace np_ecs
{
	static unsigned int flag_zero = 0;
	template<typename T> static unsigned int get_bit_flag()
	{
		static unsigned int flag = flag_zero++;
		return flag;
	}

	template<typename T> static Bitmask<BitmaskSize> get_bitmask()
	{
		Bitmask<BitmaskSize> b;
		b.set(get_bit_flag<T>(), true);
		return b;
	}

#define component_signature(type) 1 << bit_flag<type>()
#define hash_code(type) typeid(type).hash_code()

	template<typename type, typename... types> Bitmask<BitmaskSize> get_architype()
	{
		Bitmask<BitmaskSize> i;
		i.set(get_bit_flag<type>(), true);

		([&]
			{
				i.set(get_bit_flag<types>(), true);
			} (), ...);

		return i;
	}
}

#endif // ! NP_ECS_COMPONENTS_HPP