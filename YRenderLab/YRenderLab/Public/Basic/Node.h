#ifndef _YRENDER_BASIC_NODE_H_
#define _YRENDER_BASIC_NODE_H_

#include <Public/Basic/YHeapObject.h>

#include <Public/Basic/FunctionTraits.h>

#include <unordered_set>

template<typename T>
class Node : public YHeapObject {
protected:
	Node(std::shared_ptr<T> InParent = nullptr) : parent(InParent) {

	}
	virtual ~Node() = default;
	Node() = delete;

public:
	//原来的父子关系需要解除
	void AddChild(std::shared_ptr<T> child) {
		//return true when the child is invalid
		if (!child->parent.expired())
			child->parent.lock()->DelChild(child);
		child->parent = weak_this<T>();
		childrens.insert(child);
	}

	void DelChild(std::shared_ptr<T> child) {
		if (child->parent.lock() == shared_this()) {
			childrens.erase(child);
			child->parent.reset();
		}
	}

	//中序遍历的深搜
	template<typename LambdaExpr>
	void ForEachNode(LambdaExpr lambdaFunc) {
		//exec function
		lambdaFunc(shared_this<T>());

		for (auto Children : childrens) {
			Children->ForEachNode(lambdaFunc);
		}
	}


	const std::shared_ptr<T> GetParent() const { return parent.lock(); }
	const std::unordered_set<std::shared_ptr<T>>& GetChildrens() const { return childrens; }


protected:
	virtual void InitAfterNew() override {
		const auto CurParent = parent.lock();
		if (CurParent != nullptr)
			CurParent->AddChild(shared_this<T>());
	}

private:
	std::weak_ptr<T> parent;
	std::unordered_set<std::shared_ptr<T>> childrens;


};


#endif