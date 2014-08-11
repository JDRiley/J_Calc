#ifndef J_TREE_H
#define J_TREE_H

//
#include "J_RB_Tree.h"
//
#include "Pair_Key_Comparer.h"
namespace jomike{
#ifdef j_tree_TESTING
class j_tree_Tester;
#endif // j_tree_TESTING

#define J_TREE_TEMPLATE j_tree<St, Key, Comp_t, Alloc_t>
#define J_TREE_ITERATOR typename j_tree<St, Key, Comp_t, Alloc_t>::iterator
#define J_TREE_CONST_ITERATOR typename j_tree<St, Key, Comp_t, Alloc_t>::const_iterator

template < typename St, typename Key = St
	, typename Comp_t = std::less<St>, typename Alloc_t = std::allocator < St >>
class j_tree;

template<typename St>
using j_shared_ptr_tree
= j_tree<std::shared_ptr<St>, std::shared_ptr<St>, std::owner_less<std::shared_ptr<St>>>;


template<typename St, typename Key, typename Comp_t, typename Alloc_t>
class j_tree{
private:
	typedef J_RB_Tree<St, Key, Comp_t, Alloc_t> Base_Tree_t;
public:
	
	

	j_tree();

	template<typename Iter>
	j_tree(Iter i_pos, Iter i_end):M_tree(i_pos, i_end){}
	
	j_tree(const std::initializer_list<St>&);

	j_tree(j_tree&& irr_src):M_tree(std::move(irr_src.M_tree)){}
	j_tree(const j_tree& irk_src);

	j_tree& operator=(const j_tree& irr_tree);
	j_tree& operator=(j_tree&& irr_tree);
	

	typedef typename Base_Tree_t::iterator iterator;
	typedef typename Base_Tree_t::const_iterator const_iterator;


	iterator begin();
	iterator end();

	const_iterator begin()const;
	const_iterator end()const;

	const_iterator cbegin()const;
	const_iterator cend()const;

	j_size_t count(const St& irk_key)const;

	j_size_t erase(const St& irk_key);
	iterator erase(iterator i_pos);
	iterator erase(const_iterator i_pos);

	typedef std::pair<iterator, bool> insert_result_t;


	insert_result_t insert(const St& irk_val);

	insert_result_t insert(St&& irr_key);


	void clear();


	j_size_t size()const;

	bool empty()const;


	iterator find(const Key& irk_key);

	const_iterator find(const Key& irk_key)const;

	iterator lower_bound(const St& irk_key);

	const_iterator lower_bound(const St& irk_key)const;

	void swap(j_tree& ir_src);


	template<class Func_obj>
	Func_obj apply(Func_obj i_func_obj);

private:
	typedef std::pair<St, Key> val_pair_t;
#ifdef j_tree_TESTING
	friend class j_tree_Tester;
#endif // j_tree_TESTING

	Base_Tree_t M_tree;


};

template<typename St, typename Key, typename Comp_t, typename Alloc_t>
j_tree<St, Key, Comp_t, Alloc_t>& j_tree<St, Key, Comp_t, Alloc_t>::operator=(j_tree&& irr_tree){
	swap(irr_tree);
	return *this;
}

template<typename St, typename Key, typename Comp_t, typename Alloc_t>
j_tree<St, Key, Comp_t, Alloc_t>& j_tree<St, Key, Comp_t, Alloc_t>::operator=(const j_tree& irk_tree){
	j_tree temp(irk_tree);
	swap(temp);
	return *this;
}

template<typename St, typename Key, typename Comp_t, typename Alloc_t>
template<class Func_obj>
Func_obj j_tree<St, Key, Comp_t, Alloc_t>::apply(Func_obj i_func_obj){
	return M_tree.apply(i_func_obj);
}



template<typename St, typename Key, typename Comp_t, typename Alloc_t>
void j_tree<St, Key, Comp_t, Alloc_t>::swap(j_tree<St, Key, Comp_t, Alloc_t>& ir_src){
	M_tree.swap(ir_src.M_tree);
}


template<typename St, typename Key, typename Comp_t, typename Alloc_t>
J_TREE_CONST_ITERATOR j_tree<St, Key, Comp_t, Alloc_t>::find(const Key& irk_key)const{
	return M_tree.find(irk_key);
}


template<typename St, typename Key, typename Comp_t , typename Alloc_t>
typename j_tree<St, Key, Comp_t, Alloc_t>::iterator 
j_tree<St, Key, Comp_t, Alloc_t>::find(const Key& irk_key){
	return M_tree.find(irk_key);
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
bool j_tree<St, Key, Comp_t, Alloc_t>::empty()const{
	return M_tree.empty();
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
j_size_t j_tree<St, Key, Comp_t, Alloc_t>::size()const{
	return M_tree.size();
}

template<typename St, typename Key, typename Comp_t /*= std::less<St>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
typename j_tree<St, Key, Comp_t, Alloc_t>::insert_result_t 
	j_tree<St, Key, Comp_t, Alloc_t>::insert(St&& irr_val){

		auto iter_pos = M_tree.find(irr_val);
		if(M_tree.end() != iter_pos){
			return insert_result_t(iter_pos, false);
		}

		return M_tree.insert(std::move(irr_val));
}

template<typename St, typename Key, typename Comp_t /*= std::less<St>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
typename j_tree<St, Key, Comp_t, Alloc_t>::insert_result_t
	j_tree<St, Key, Comp_t, Alloc_t>::insert(const St& irr_val){

		auto iter_pos = M_tree.find(irr_val);
		if(M_tree.end() != iter_pos){
			return insert_result_t(iter_pos, false);
		}


		auto pair = M_tree.insert(irr_val);
		return insert_result_t(pair.first, pair.second);
}



template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
void j_tree<St, Key, Comp_t, Alloc_t>::clear(){
	return M_tree.clear();
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
J_TREE_ITERATOR j_tree<St, Key, Comp_t, Alloc_t>::erase(const_iterator i_pos){
	return M_tree.erase(i_pos);
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
J_TREE_ITERATOR j_tree<St, Key, Comp_t, Alloc_t>::erase(iterator i_pos){
	return M_tree.erase(i_pos);
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
j_size_t j_tree<St, Key, Comp_t, Alloc_t>::erase(const St& irk_key){
	return M_tree.erase(irk_key);
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
j_size_t j_tree<St, Key, Comp_t, Alloc_t>::count(const St& irk_key)const{
	return M_tree.count(irk_key);
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
J_TREE_CONST_ITERATOR j_tree<St, Key, Comp_t, Alloc_t>::cend()const{
	return M_tree.cend();
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
J_TREE_CONST_ITERATOR j_tree<St, Key, Comp_t, Alloc_t>::cbegin()const{
	return M_tree.cbegin();
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
	J_TREE_CONST_ITERATOR j_tree<St, Key, Comp_t, Alloc_t>::end()const{
		return M_tree.end();
	}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
J_TREE_CONST_ITERATOR j_tree<St, Key, Comp_t, Alloc_t>::begin()const{
	return M_tree.begin();
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
J_TREE_ITERATOR j_tree<St, Key, Comp_t, Alloc_t>::end(){
	return M_tree.end();
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
J_TREE_ITERATOR j_tree<St, Key, Comp_t, Alloc_t>::begin(){
	return M_tree.begin();
}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
j_tree<St, Key, Comp_t, Alloc_t>::j_tree(const j_tree& irk_src):M_tree(irk_src.M_tree){

}


template<typename St, typename Key, typename Comp_t /*= std::less\<St\>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
j_tree<St, Key, Comp_t, Alloc_t>::j_tree(const std::initializer_list<St>& irk_list): M_tree(irk_list){

}


template<typename St, typename Key, typename Comp_t /*= std::less<St>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
j_tree<St, Key, Comp_t, Alloc_t>::j_tree(){

}


template<typename St, typename Key, typename Comp_t /*= std::less<St>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
J_TREE_ITERATOR	j_tree<St, Key, Comp_t, Alloc_t>::lower_bound(const St& irk_key){
		return M_tree.lower_bound(irk_key);
}

template<typename St, typename Key, typename Comp_t /*= std::less<St>*/
	, typename Alloc_t /*= std::allocator<std::pair<St, Key>>*/>
J_TREE_CONST_ITERATOR J_TREE_TEMPLATE::lower_bound(const St& irk_key)const{
		return M_tree.lower_bound(irk_key);
}

}

#endif //J_TREE_H