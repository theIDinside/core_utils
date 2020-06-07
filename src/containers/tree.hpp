#pragma once
#include <functional>
#include <memory>
#include <cassert>

#ifdef DEBUG
	#include <fmt/core.h>
#endif


namespace cx::containers::skog 
{
	using u64 = unsigned long;
	
	template <typename NodeRef, typename TravelMapFn>
	void in_order_map(NodeRef node, TravelMapFn fn) {
		if(!node) return;
		in_order_map(node->m_left.get(), fn);
		fn(node);
		in_order_map(node->m_right.get(), fn);
	}

	// TODO(implement): Balancing of tree
	// TODO(implement): Rotation of sub-trees


	template <typename T, typename Key = u64>
	struct Tree {
		using TreeNode	= std::unique_ptr<Tree<T, Key>>;
		using NodeRef	= Tree<T, Key>*;
		T data;
		Key k;
		TreeNode m_left;
		TreeNode m_right;
		Tree* p_parent;
		// TODO(optimization) add concept requires-clauses here, so that we can check that both T and Key are move constructible, then default to a signature of T&& and Key&&,
		// otherwise do pass-by-value-then-move as defined here (Sean gave a great talk about pass-by-value-then move). The compiler also gets to do a lot of assumptions,
		// since it can treat everything as pure values, thus possibly provide optimizations. Which is why this is added as a todo, so it can be benchmarked and checked
		// later.
		Tree(T data, Key key) : data(std::move(data)), k(std::move(key)), m_left(nullptr), m_right(nullptr), p_parent(nullptr) { }
		~Tree() {
			#ifdef DEBUG
				fmt::print("Destroying Tree Node ~ Data: {}\t\tKey: {}\n", this->data, this->k);
			#endif
		}

		// TODO(fix?): Can these be used somehow?
		Tree(const Tree&)				= delete;
		Tree(Tree&&)					= delete;
		Tree& operator=(const Tree&)	= delete;
		Tree& operator=(Tree&&)			= delete;

		// TODO(fix): fix so that this tree can have equal key values, or at least handle it in some way. For now we hack-n-slash
		void insert(T data, Key key) {
			assert(key != k && "Keys in this tree can _not_ be of equal values");
			if(key < k) {
				if(m_left) {
					m_left->insert(std::move(data), std::move(key));
				} else {
					m_left = std::make_unique<Tree<T, Key>>(data, key);
					m_left->p_parent = this;
				}
			} else {
				if(m_right) {
					m_right->insert(std::move(data), std::move(key));
				} else {
					m_right = std::make_unique<Tree<T, Key>>(data, key);
					m_right->p_parent = this;
				}
			}
		}
	
		
		template <typename NodeRef, typename TravelMapFn>
		friend void in_order_map(NodeRef node, TravelMapFn fn);
	
		template <typename NodeRef, typename TravelMapFn>
		friend void pre_order_map(NodeRef node, TravelMapFn fn);
		
		template <typename NodeRef, typename TravelMapFn>
		friend void post_order_map(NodeRef node, TravelMapFn fn);

		bool is_root() const {
			return p_parent == this;
		}

		
	};

	template <typename T, typename Key>
	static auto make_root(T data, Key k) {
		auto root = std::make_unique<Tree<T, Key>>(data, k);
		root->p_parent = root.get();
		return root;
	}



}
