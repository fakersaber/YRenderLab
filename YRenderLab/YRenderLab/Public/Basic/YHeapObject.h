#ifndef _YRENDER_BASIC_YHEAPOBJECT_H_
#define _YRENDER_BASIC_YHEAPOBJECT_H_

#include <memory>
#include <typeinfo>

namespace YRender {


	template<typename FromType, typename ToType, bool isUp, bool isDown>
	struct CastWrapper;

	template<typename FromType, typename ToType>
	struct CastWrapper<FromType, ToType, true, false> {
		static std::shared_ptr<ToType> Cast(const std::shared_ptr<FromType>& ptr) {
			return std::dynamic_pointer_cast<ToType>(ptr);
		}

		static std::weak_ptr<ToType> Cast(const std::weak_ptr<FromType>& ptr) {
			return std::dynamic_pointer_cast<ToType>(ptr.lock());
		}
	};

	template<typename FromType, typename ToType>
	struct CastWrapper<FromType, ToType, false, true> {
		static std::shared_ptr<ToType> Cast(const std::shared_ptr<FromType>& ptr) {
			return std::static_pointer_cast<ToType>(ptr);
		}

		static std::weak_ptr<ToType> Cast(const std::weak_ptr<FromType>& ptr) {
			return std::static_pointer_cast<ToType>(ptr.lock());
		}
	};

	template<typename FromType, typename ToType>
	struct CastWrapper<FromType, ToType, true, true> {
		static std::shared_ptr<ToType> Cast(const std::shared_ptr<FromType>& ptr) {
			return ptr;
		}

		static std::weak_ptr<ToType> Cast(const std::weak_ptr<FromType>& ptr) {
			return ptr;
		}
	};

	template<typename FromType,typename ToType>
	struct CastWrapper<FromType, ToType, false, false> {
		static std::shared_ptr<ToType> Cast(const std::shared_ptr<FromType>& ptr) {
			return nullptr;
		}		

		static std::weak_ptr<FromType> Cast(const std::weak_ptr<ToType>& ptr) {
			return nullptr;
		}
	};

	template<typename ToType, typename FromType>
	std::shared_ptr<ToType> Cast(const std::shared_ptr<FromType>& ptr) {
		return CastWrapper<FromType, ToType, std::is_base_of_v<FromType, ToType>, std::is_base_of_v<ToType, FromType>>::Cast(ptr);
	}

	template<typename ToType,typename FromType>
	std::weak_ptr<ToType> Cast(const std::weak_ptr<FromType>& ptr) {
		return CastWrapper<FromType, ToType, std::is_base_of_v<FromType, ToType>, std::is_base_of_v<ToType, FromType>>::Cast(ptr);
	}


	//若不继承于enable_shared_from_this，只能每次返回一个新的sharedPtr造成多次释放
	class YHeapObject : public std::enable_shared_from_this<YHeapObject> {
	private:
		template<typename Type,typename ...Args>
		friend std::shared_ptr<Type> New(Args&& ... args);

	protected:
		template<typename T = YHeapObject>
		std::shared_ptr<T> shared_this(){
			return Cast<T>(shared_from_this());
		} 

		template<typename T = YHeapObject>
		std::weak_ptr<T> weak_this(){
			return Cast<T>(weak_from_this());
		}

	protected:
		virtual void InitAfterNew() {
		}

	protected:
		YHeapObject() = default;
		virtual ~YHeapObject() = default;

	private:
		static void Delete(YHeapObject* obj) {
			delete obj;
		}

		void* operator new(size_t size) {
			if (void* mem = malloc(size))
				return mem;
			throw std::bad_alloc();
		}

		void operator delete(void* mem) noexcept{
			free(mem);
		}

	private:
		using std::enable_shared_from_this<YHeapObject>::shared_from_this;
		using std::enable_shared_from_this<YHeapObject>::weak_from_this;
	};

	template<typename Type, typename ...Args>
	std::shared_ptr<Type> New(Args&& ... args) {
		auto NewObject = std::shared_ptr<Type>(new Type(std::forward<Args>(args)...),YHeapObject::Delete);
		(static_cast<std::shared_ptr<YHeapObject>>(NewObject))->InitAfterNew();
		return NewObject;
	}
}


#endif