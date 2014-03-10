#ifndef J_MAP_H
#define J_MAP_H

//
#include "J_RB_Tree.h"
//
#include "Pair_Key_Comparer.h"
namespace jomike{
#ifdef RB_TREE_TESTING
class RB_Tree_Tester;
#endif // RB_TREE_TESTING

#define J_MAP_TEMPLATE j_map<Key_t, Value_t, Comp_t, Alloc_t>
#define J_MAP_ITERATOR typename j_map<Key_t, Value_t, Comp_t, Alloc_t>::iterator
#define J_MAP_CONST_ITERATOR typename j_map<Key_t, Value_t, Comp_t, Alloc_t>::const_iterator

template<typename Key_t, typename Value_t, typename Comp_t = std::less<Key_t>
	, typename Alloc_t = std::allocator<std::pair<Key_t, Value_t>>>
class j_map{
private:
	typedef J_RB_Tree<
		std::pair<Key_t, Value_t>, Key_t
		, Pair_Key_Comparer<std::pair<Key_t, Value_t>, Comp_t>, Alloc_t> Base_Tree_t;
public:
	
	

	j_map();
	j_map(const std::initializer_list<std::pair<Key_t, Value_t>>& irk_list);
	template<typename Iter>
	j_map(Iter i_pos, Iter i_end):M_tree(i_pos, i_end){}

	j_map(j_map&& irr_src):M_tree(std::move(irr_src.M_tree)){}
	j_map(const j_map& irk_src);

	

	typedef PairIterator<typename Base_Tree_t::iterator> iterator;
	typedef PairIterator<typename Base_Tree_t::const_iterator> const_iterator;


	iterator begin();
	iterator end();

	const_iterator begin()const;
	const_iterator end()const;

	const_iterator cbegin()const;
	const_iterator cend()const;

	j_size_t count(const Key_t& irk_key)const;

	j_size_t erase(const Key_t& irk_key);
	iterator erase(iterator i_pos);
	iterator erase(const_iterator i_pos);

	typedef std::pair<iterator, bool> insert_result_t;

	insert_result_t insert(const Key_t& irk_key, const Value_t& irk_val);

	insert_result_t insert(Key_t&& irr_key, Value_t&& irr_val);

	insert_result_t insert(Key_t&& irr_key, const Value_t& irr_val);

	void clear();

	Value_t& operator[](const Key_t& irk_key);

	j_size_t size()const;

	bool empty()const;


	iterator find(const Key_t& irk_key);

	const_iterator find(const Key_t& irk_key)const;

	iterator lower_bound(const Key_t& irk_key);

	const_iterator lower_bound(const Key_t& irk_key)const;

	void swap(j_map& ir_src);
private:
	typedef std::pair<Key_t, Value_t> val_pair_t;
#ifdef RB_TREE_TESTING
	friend class RB_Tree_Tester;
#endif // RB_TREE_TESTING

	Base_Tree_t M_tree;


};


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
void j_map<Key_t, Value_t, Comp_t, Alloc_t>::swap(j_map<Key_t, Value_t, Comp_t, Alloc_t>& ir_src){
	M_tree.swap(ir_src.M_tree);
}


template<typename Key_t, typename Value_t, typename Comp_t, typename Alloc_t>
J_MAP_CONST_ITERATOR j_map<Key_t, Value_t, Comp_t, Alloc_t>::find(const Key_t& irk_key)const{
	return M_tree.find(irk_key);
}


template<typename Key_t, typename Value_t, typename Comp_t , typename Alloc_t>
J_MAP_ITERATOR j_map<Key_t, Value_t, Comp_t, Alloc_t>::find(const Key_t& irk_key){
		return M_tree.find(irk_key);
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
bool j_map<Key_t, Value_t, Comp_t, Alloc_t>::empty()const{
	return M_tree.empty();
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
j_size_t j_map<Key_t, Value_t, Comp_t, Alloc_t>::size()const{
	return M_tree.size();
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
Value_t& j_map<Key_t, Value_t, Comp_t, Alloc_t>::operator[](const Key_t& irk_key){
	auto found_pos = M_tree.find(irk_key);
	if(M_tree.end() != found_pos){
		return found_pos->second;
	}

	return M_tree.insert(std::make_pair(irk_key, Value_t())).first->second;
}

template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
typename j_map<Key_t, Value_t, Comp_t, Alloc_t>::insert_result_t 
	j_map<Key_t, Value_t, Comp_t, Alloc_t>::insert(Key_t&& irr_key, const Value_t& irr_val){
		auto found_pos = M_tree.find(irr_key);
		if(M_tree.end() != found_pos){
			return insert_result_t(found_pos, false);
		}

	return M_tree.insert(val_pair_t(std::move(irr_key), irr_val));
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
typename j_map<Key_t, Value_t, Comp_t, Alloc_t>::insert_result_t 
	j_map<Key_t, Value_t, Comp_t, Alloc_t>::insert(Key_t&& irr_key, Value_t&& irr_val){

	auto found_pos = M_tree.find(irr_key);
	if(M_tree.end() != found_pos){
		return insert_result_t(found_pos, false);
	}

	return M_tree.insert(val_pair_t(std::move(irr_key), std::move(irr_val)));
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
typename J_MAP_TEMPLATE::insert_result_t 
	j_map<Key_t, Value_t, Comp_t, Alloc_t>::insert(const Key_t& irk_key, const Value_t& irk_val){
	
	auto found_pos = M_tree.find(irr_key);
	if(M_tree.end() != found_pos){
		return insert_result_t(found_pos, false);
	}

	return M_tree.insert(val_pair_t(irr_key, irr_val));
}

template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
void j_map<Key_t, Value_t, Comp_t, Alloc_t>::clear(){
	return M_tree.clear();
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
J_MAP_ITERATOR j_map<Key_t, Value_t, Comp_t, Alloc_t>::erase(const_iterator i_pos){
	return M_tree.erase(i_pos.base());
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
typename j_map<Key_t, Value_t, Comp_t, Alloc_t>::iterator j_map<Key_t, Value_t, Comp_t, Alloc_t>::erase(iterator i_pos){
	return M_tree.erase(i_pos.base());
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
j_size_t j_map<Key_t, Value_t, Comp_t, Alloc_t>::erase(const Key_t& irk_key){
	return M_tree.erase(irk_key);
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
	j_size_t j_map<Key_t, Value_t, Comp_t, Alloc_t>::count(const Key_t& irk_key)const{
		return M_tree.count(irk_key);
	}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
J_MAP_CONST_ITERATOR j_map<Key_t, Value_t, Comp_t, Alloc_t>::cend()const{
	return M_tree.cend();
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
J_MAP_CONST_ITERATOR j_map<Key_t, Value_t, Comp_t, Alloc_t>::cbegin()const{
	return M_tree.cbegin();
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
J_MAP_CONST_ITERATOR j_map<Key_t, Value_t, Comp_t, Alloc_t>::end()const{
	return M_tree.end();
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
J_MAP_CONST_ITERATOR j_map<Key_t, Value_t, Comp_t, Alloc_t>::begin()const{
	return M_tree.begin();
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
J_MAP_ITERATOR j_map<Key_t, Value_t, Comp_t, Alloc_t>::end(){
	return M_tree.end();
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
J_MAP_ITERATOR j_map<Key_t, Value_t, Comp_t, Alloc_t>::begin(){
	return M_tree.begin();
}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
j_map<Key_t, Value_t, Comp_t, Alloc_t>::j_map(const j_map& irk_src):M_tree(irk_src.M_tree){

}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
j_map<Key_t, Value_t, Comp_t, Alloc_t>::j_map(const std::initializer_list<std::pair<Key_t, Value_t>>& irk_list): M_tree(irk_list){

}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
j_map<Key_t, Value_t, Comp_t, Alloc_t>::j_map(){

}


template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
J_MAP_ITERATOR J_MAP_TEMPLATE::lower_bound(const Key_t& irk_key){
	return M_tree.lower_bound(irk_key);
}

template<typename Key_t, typename Value_t, typename Comp_t /*= std::less<Key_t>*/
	, typename Alloc_t /*= std::allocator<std::pair<Key_t, Value_t>>*/>
J_MAP_CONST_ITERATOR J_MAP_TEMPLATE::lower_bound(const Key_t& irk_key)const{
		return M_tree.lower_bound(irk_key);
}

}

#endif //J_MAP_H