#ifndef _YRENDER_BASIC_WEAKPTR_HASH_H_
#define _YRENDER_BASIC_WEAKPTR_HASH_H

#include <memory>

//shared直接使用std::hash

template<typename T>
struct WeakHasher {
	size_t operator()(const std::weak_ptr<T>& _Keyval) const noexcept{
		return std::hash<std::shared_ptr<T>>()(_Keyval.lock());
		
		//等价
		//_NODISCARD size_t operator()(const shared_ptr<_Ty>& _Keyval) const noexcept
		//{	// hash _Keyval to size_t value by pseudorandomizing transform
		//	return (hash<typename shared_ptr<_Ty>::element_type *>()(_Keyval.get()));
		//}
	}
};

template<typename T>
struct WeakEqualTo {
	bool operator()(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>& rhs) const {
		return !lhs.owner_before(rhs) && !rhs.owner_before(lhs);
	}
};

#endif