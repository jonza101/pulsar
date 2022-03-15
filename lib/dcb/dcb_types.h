#pragma once

#include <DirectXMath.h>


#define DCB_RAW_DATA_TYPES					\
	DCB_RDT_MACRO(Float)					\
	DCB_RDT_MACRO(Float2)					\
	DCB_RDT_MACRO(Float3)					\
	DCB_RDT_MACRO(Float4)					\
	DCB_RDT_MACRO(Matrix)					\
	DCB_RDT_MACRO(Bool)

namespace PULSAR
{
	namespace DCB
	{
		enum TYPE
		{
			Empty,
		#define DCB_RDT_MACRO(t) t,
			DCB_RAW_DATA_TYPES
		#undef DCB_RDT_MACRO
			Struct,
			Array	//	IMPLEMENT LATER
		};


		template<TYPE type>
		struct type_attr
		{
			static const bool is_valid = false;
			static const size_t hlsl_size = 0ull;
		};
		template<> struct type_attr<Float>
		{
			static const bool is_valid = true;

			using sys_type = float;
			static const size_t hlsl_size = sizeof(sys_type);
		};
		template<> struct type_attr<Float2>
		{
			static const bool is_valid = true;

			using sys_type = DirectX::XMFLOAT2;
			static const size_t hlsl_size = sizeof(sys_type);
		};
		template<> struct type_attr<Float3>
		{
			static const bool is_valid = true;

			using sys_type = DirectX::XMFLOAT3;
			static const size_t hlsl_size = sizeof(sys_type);
		};
		template<> struct type_attr<Float4>
		{
			static const bool is_valid = true;

			using sys_type = DirectX::XMFLOAT4;
			static const size_t hlsl_size = sizeof(sys_type);
		};
		template<> struct type_attr<Matrix>
		{
			static const bool is_valid = true;

			using sys_type = DirectX::XMMATRIX;
			static const size_t hlsl_size = sizeof(sys_type);
		};
		template<> struct type_attr<Bool>
		{
			static const bool is_valid = true;

			using sys_type = bool;
			static const size_t hlsl_size = 4ull;
		};

	#define DCB_RDT_MACRO(t) static_assert(type_attr<t>::is_valid, "Missing DCB raw type attributes implementation");
		DCB_RAW_DATA_TYPES
	#undef DCB_RDT_MACRO

		template<typename T>
		struct reverse_attr_type
		{
			static const bool is_valid = false;
		};

	#define DCB_RDT_MACRO(t)												\
		template<> struct reverse_attr_type<type_attr<t>::sys_type>			\
		{																	\
			static const TYPE type = t;										\
			static const bool is_valid = true;								\
		};
		DCB_RAW_DATA_TYPES
	#undef DCB_RDT_MACRO
	}
}
