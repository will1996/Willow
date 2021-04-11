#pragma once
#include <cstddef>
#include<string>
#include<vector>
#include<iostream>
#include<sstream>
#include<optional>
#include<map>
#include<functional>
#include<assert.h>
#include<algorithm>
#include<memory>
#include<atomic>
#include<set>
#include<glm/glm.hpp>
#include<glm/gtx/string_cast.hpp>

#include "willow/root/Logger.hpp"
#include "willow/root/WiloConfig.h"
#include "PlatformCodes.hpp"

#define WILO_ASSERT(result) assert(result==true)


namespace wlo {


	template<typename T>
	using BorrowedPointer = std::weak_ptr<T>;

	template<typename T>
	using UniquePointer = std::unique_ptr<T>;

	 template<typename T, typename ... Args>
	constexpr UniquePointer<T> CreateUniquePointer(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)... );
	}

	template<typename T>
	using SharedPointer = std::shared_ptr<T>;

	 template<typename T, typename ... Args>
	constexpr SharedPointer<T> CreateSharedPointer(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)... );
	}
    template<typename T>
    using Ref = std::reference_wrapper<T>;


	using byte = std::byte;

	using std::max;
	using std::min;
	using std::cout;
	using std::endl;
	using std::vector;
	using std::set;
	using std::map;

	using ID_type = size_t;

    using Vec2 = glm::vec2;
    using Vec3 = glm::vec3;
    using Vec4 = glm::vec4;

    using Mat3 = glm::mat3;
    using Mat4 = glm::mat4;
}
