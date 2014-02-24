#ifndef J_MAP_H
#define J_MAP_H

//
#include <RB_Tree.h>

namespace jomike{

template<typename Pair_t, typename Comp_t>
class Pair_Key_Comparer{
public:
	typedef typename Pair_t::first_type Key_t;
	bool operator()(const Pair_t& irk_left, const Pair_t& irk_right)const{
		return M_comp(irk_left.first, irk_right.first);
	}

	bool operator()(const Pair_t& irk_left, const Key_t& irk_right)const{
		return M_comp(irk_left.first, irk_right);
	}

	bool operator()(const Key_t& irk_left, const Pair_t& irk_right)const{
		return M_comp(irk_left, irk_right.first);
	}

	bool operator()(const Key_t& irk_left, const Key_t& irk_right)const{
		return M_comp(irk_left, , irk_right);
	}
private:
	Comp_t M_comp;
};

template<
	typename Key_t, typename Value_t, typename Comp_t = std::less<Key_t>
	, typename Alloc_t = std::allocator<std::pair<Key_t, Value_t>>>
class j_map{
public:
	typedef RB_Tree<
		std::pair<Key_t, Value_t>, Key_t
		, Pair_Key_Comparer<std::pair<Key_t, Value_t>, Comp_t>, Alloc_t> Base_Tree_t;
	

	typedef PairIterator<typename Base_Tree_t::iterator> iterator;
	typedef PairIterator<typename Base_Tree_t::const_iterator> const_iterator;


	iterator begin(){ return M_tree.begin(); }
	iterator end(){ return M_tree.end(); }

	const_iterator begin()const{ return M_tree.begin(); }
	const_iterator end()const{ return M_tree.end(); }

	const_iterator cbegin()const{ return M_tree.cbegin(); }
	const_iterator cend()const { return M_tree.cend(); }

	j_size_t count(const Key_t& irk_key)const{ return M_tree.count(irk_key); }

	j_size_t erase(const Key_t& irk_key){
		return M_tree.erase(irk_key);
	}

	void clear(){
		return M_tree.clear();
	}

	Value_t& operator[](const Key_t& irk_key){
		auto found_pos = M_tree.find(irk_key);
		if(M_tree.end() != found_pos){
			return found_pos->second;
		}

		return M_tree.insert(std::make_pair(irk_key, Value_t())).first->second;
	}

	j_size_t size()const{
		return M_tree.size();
	}

	bool empty()const{
		return M_tree.empty();
	}
private:

	Base_Tree_t M_tree;


};


}

#endif //J_MAP_H