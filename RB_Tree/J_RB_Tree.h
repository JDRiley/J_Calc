#ifndef J_RB_TREE_H
#define J_RB_TREE_H

#ifdef RB_TREE_DEBUG
#include "J_RB_Tree_Error.h"
//
#include <iostream>
//
#include <sstream>
//
#include <string>
#endif //RB_TREE_DEBUG

//
#include "j_iterator.h"




namespace jomike{

//Convenience Macros---------------------------------------------------------------------

//Declares the rb_tree iterator outside the class
#define RB_TREE_ITERATOR typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::iterator

//Declares the rb_tree const_iterator outside the class
#define RB_TREE_CONST_ITERATOR typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::const_iterator 

//Declares the rb_tree template
#define RB_TREE_TEMPLATE J_RB_Tree<St, Key, Comp_t, Alloc_t>

//type is declared if the types St, and Key are the same
#define ENABLE_IF_ST_OR_KEY(type_to_check, type_to_yield) \
	typename std::enable_if<std::is_same<type_to_check, St>::value \
|| std::is_same<Key, type_to_check>::value, type_to_yield>::type



///J_RB_Tree
/*				This class should NOT be used directly						*/
/*	Red Black Tree. Supports Duplicate types. St is the typed stored.		*/
/*	Key is the type to use to find elements in the tree. Comp_t must be a	*/
/*	which can compare two St's together and a St and a key.					*/
template<typename St, typename Key = St, class Comp_t = std::less<Key>
	, class Alloc_t = std::allocator<St>>
class J_RB_Tree{
private:
	class RB_Node__;
	typedef RB_Node__ Node_t;
public:
//Defined Types--------------------------------------------------------------------

	typedef	St				value_type;
	typedef	std::ptrdiff_t	difference_type;
	typedef	St& 			reference;
	typedef const St&		const_reference;
	typedef	St*  			pointer;
	typedef	const St*		const_pointer;
	typedef Alloc_t			allocator_type;
	typedef J_RB_Tree		this_type;

//Iterators----------------------------------------------------------------------------

	///iterator
	/*																			*/
	/*	Bidirectional iterator. Typedef of node_iterator class.					*/
	typedef node_iterator<typename this_type::Node_t*, St, this_type>
		iterator;

	///const_iterator
	/*																			*/
	/*	Bidirectional iterator. Typedef of node_iterator class.	Returns const	*/
	/*	items when dereferenced.													*/
	typedef node_iterator<typename this_type::Node_t*, const St, this_type>
		const_iterator;

//Constructors---------------------------------------------------------------------------

	///J_RB_Tree()
	/*																			*/
	/*		Constructs an empty J_RB_Tree of size zero.							*/
	J_RB_Tree();


	///J_RB_Tree(const J_RB_Tree&)
	/*																			*/
	/*		Constructs a J_RB_Tree which is a copy of irk_source. 				*/
	/*	All elements of irk_source are also copied.								*/
	J_RB_Tree(const J_RB_Tree&);


	///J_RB_Tree(J_RB_Tree&& irr_right)
	/*																			*/
	/*		Move constructor. Constructs a tree from irr_right.	irr_right is	*/
	/*	guaranteed to be in a valid but undetermined state after moving.		*/
	J_RB_Tree(J_RB_Tree&& irr_right);


	///template<typename iter>
	///J_RB_Tree(iter i_pos, iter i_last)
	/*																			*/
	/*		Range Constructor. Constructs a tree from elements in range			*/
	/*	[i_pos, i_last)															*/
	template<typename iter>
	J_RB_Tree(iter i_pos, iter i_last);


	///J_RB_Tree(const std::initializer_list<St>& irk_list)
	/*																			*/
	/*		Initializer list constructor. Copies elements in the initializer	*/
	/*	into the tree.															*/
	J_RB_Tree(const std::initializer_list<St>& irk_list);

//Assignment---------------------------------------------------------------------------

	///J_RB_Tree& operator=(const J_RB_Tree& irk_src)
	/*																			*/
	/*		Copy Assignment. Drops it's current values and creates fills itself	*/
	/*	with values of irk_src.													*/
	J_RB_Tree& operator=(const J_RB_Tree& irk_src);


	///J_RB_Tree& operator=(J_RB_Tree&& irr_src)
	/*																			*/
	/*		Move Assignment. Drops current values and moves contents of irr_src	*/
	/*	to the current container. Similar to swap. irr_src left in valid but	*/
	/*	undetermined state.														*/
	J_RB_Tree& operator=(J_RB_Tree&& irr_src);

	///template<typename Iter>
	///void assign(Iter i_pos, Iter i_end)
	/*																			*/
	/*		Current contents of the RB_Tree are disregarded and tree is filled	*/
	/*	with values from the range [i_pos, i_end).								*/
	template<typename Iter>
	void assign(Iter i_pos, Iter i_end);

	///void swap(J_RB_Tree& ir_src)
	/*																			*/
	/*	Swaps contents of this tree with ir_src									*/
	void swap(J_RB_Tree& ir_src);

//Insertion------------------------------------------------------------------------------
	
	//Type used as result of insertion
	typedef std::pair<iterator, bool> insert_result_t;
	
	///insert_result_t insert(St&&)
	/*																			*/
	/*		Inserts a copy of the element into the tree. If element already		*/
	/*	exists which equals irk_val then it is placed before the upper_bound	*/
	insert_result_t insert(const St&);

	///insert_result_t insert(St&& irr_val)
	/*																			*/
	/*		Moves the element into the tree. If element already					*/
	/*	exists which equals irk_val then it is placed before the upper_bound	*/
	insert_result_t insert(St&& irr_val);


//Deletion------------------------------------------------------------------------------


	///j_size_t erase(const Key& irk_key)
	/*																			*/
	/*		Removes all elements that are equal to irk_key.						*/
	/*	Returns the number of elements removed.									*/
	j_size_t erase(const Key& irk_key);


	///iterator erase(const_iterator i_pos)
	/*																			*/
	/*		Removes the element at i_pos. i_pos must be a valid iterator to the	*/
	/*	this tree. returns an iterator to the element following i_pos or end()	*/
	/*	if i_pos was the last element.											*/
	iterator erase(const_iterator i_pos);


	///iterator erase(const_iterator i_pos, const_iterator i_end)
	/*																			*/
	/*		Removes elements in the range [i_pos, i_end). Range must be valid	*/
	/*	range in this tree. Returns an iterator to the element following last	*/
	/*	element removed.														*/
	iterator erase(const_iterator i_pos, const_iterator i_end);


	///void clear()
	/*																			*/
	/*		Clears all elements in the tree and calls destructors on objects.	*/
	void clear();

	
//Object Manipulation--------------------------------------------------------------------
	///template<class Func_obj>
	///Func_obj apply(Func_obj i_func_obj)
	/*																			*/
	/*		Applies the () operator of i_func_obj on all the elements in the	*/
	/*	container in no particular order. returns the function object.			*/
	template<class Func_obj>
	Func_obj apply(Func_obj i_func_obj);

//Element Access-------------------------------------------------------------------------

	///iterator begin()
	/*																			*/
	/*		Returns an iterator to the beginning element in tree. equivalent to	*/
	/*	calling minimum()														*/
	iterator begin();



	///iterator end()
	/*																			*/
	/*		Returns an iterator to the beginning element in tree. equivalent to	*/
	/*	calling minimum()														*/
	iterator end();


	///const_iterator begin()const
	/*																			*/
	/*		Returns a const_iterator to the first element in tree. equivalent	*/
	/*	to calling minimum().													*/
	const_iterator begin()const;


	///const_iterator end()const
	/*																			*/
	/*		Returns a const_iterator to the first element in tree. equivalent	*/
	/*	to calling minimum().													*/
	const_iterator end()const;

	///const_iterator cbegin()const
	/*																			*/
	/*		Returns a const_iterator to the first element in tree. equivalent	*/
	/*	to calling minimum().													*/
	const_iterator cbegin()const;


	///const_iterator cend()const
	/*																			*/
	/*		Returns a const_iterator to the first element in tree. equivalent	*/
	/*	to calling minimum().													*/
	const_iterator cend()const;


	
	///iterator find(const Key& _ik_key)
	/*																			*/
	/*		Finds an element corresponding to the value of irk_key.	If multiple	*/
	/*	values match irk_key the one returned is undetermined.					*/
	iterator find(const Key& _ik_key);


	///const_iterator find(const Key& _ik_key)const
	/*																			*/
	/*		Finds an element corresponding to the value of irk_key.	If multiple	*/
	/*	values match irk_key the one returned is undetermined.	Cons			*/
	const_iterator find(const Key& _ik_key)const;

	//Type holding the beginning and end of a range in the tree
	typedef std::pair<iterator, iterator> range_t;

	//Type holding the beginning and end of a const range in the tree
	typedef std::pair<const_iterator, const_iterator> const_range_t;


	///range_t equal_range(const Key& irk_key)
	/*																			*/
	/*		Returns the range of elements equal to irk_key in the tree.			*/
	range_t equal_range(const Key& irk_key);

	///const_range_t equal_range(const Key& irk_key)const
	/*																			*/
	/*		Returns the const range of elements equal to irk_key in the tree.	*/
	const_range_t equal_range(const Key& irk_key)const;



	///iterator lower_bound(const Key& ik_key)
	/*																			*/
	/*		Returns the first element in the range to not compare less than		*/
	/*	irk_key.																*/
	iterator lower_bound(const Key& ik_key);
	
	///const_iterator lower_bound(const Key& ik_key)const
	/*																			*/
	/*		Returns the first element in the range to not compare less than		*/
	/*	irk_key.																*/
	const_iterator lower_bound(const Key& ik_key)const;


	///iterator upper_bound(const Key& ik_key)
	/*																			*/
	/*		Returns an iterator to the first element in the tree to compare less*/
	/*	than irk_key.															*/
	iterator upper_bound(const Key& ik_key);

	///const_iterator upper_bound(const Key& ik_key)const
	/*																			*/
	/*		Returns a const_iterator to the first element in the tree to compare*/
	/*	less than irk_key.														*/
	const_iterator upper_bound(const Key& ik_key)const;


	///iterator minimum()
	/*																			*/
	/*		Returns the minimum element in the tree.							*/
	iterator minimum();

	///const_iterator minimum()const
	/*																			*/
	/*		Returns the minimum element in the tree.							*/
	const_iterator minimum()const;


	///iterator maximum()
	/*																			*/
	/*		Returns the maximum element in the tree.							*/
	iterator maximum();

	///const_iterator maximum()const
	/*																			*/
	/*		Returns the maximum element in the tree.							*/
	const_iterator maximum()const;

	///iterator root()
	/*																			*/
	/*		Returns the root element of the tree.								*/
	iterator root();

	///const_iterator root()
	/*																			*/
	/*		Returns the root element of the tree.								*/
	const_iterator root()const;

	
//Status Functions-----------------------------------------------------------------------


	///const j_size_t size()const
	/*																			*/
	/*		Returns the number of elements in the tree.							*/
	j_size_t size()const;

	///bool empty()const
	/*																			*/
	/*		Returns true if the container is empty. False otherwise.			*/
	bool empty()const;
	
	///j_size_t count(const Key&)const
	/*																			*/
	/*		Returns the number of elements which match the value irk_key.		*/
	j_size_t count(const Key& irk_key)const;

//Destruction---------------------------------------------------------------------------
	~J_RB_Tree();

#ifdef RB_TREE_DEBUG
	void in_order_walk(std::ostream&);
	void set_messages(bool val);
	void rotate_right_pub(iterator pos);
	void rotate_left_pub(iterator pos);
	int proper_rb_tree()const;
	static void reset_rotations();
	static int get_rotations();
#endif //RB_TREE_DEBUG

private:
//Friends---------------------------------------------------------------------------------
	friend iterator;
	friend const_iterator;

//Members---------------------------------------------------------------------------------
	
	//Root element of tree.
	Node_t* M_root;

	//Number of elements in the tree.
	j_size_t M_size;

	typename allocator_type::template rebind<Node_t*>::other M_alloc;

	/*Compare Function Object*/
	Comp_t M_comp;
	
//Node Advancement. Used by node_iterator-------------------------------------------------

	/*static Node_t* previous_node(const Node_t*)*/
	static Node_t* previous_node(const Node_t* i_node);

	/*static Node_t* next_node(const Node_t*)*/
	static Node_t* next_node(const Node_t* i_node);

	/*insert_result_t insert_core(const St&& irr_val)*/
	insert_result_t insert_core(Node_t* irr_val);
	

//Tree Maintenance-----------------------------------------------------------------------


	void rotate_left(Node_t*);
	void rotate_right(Node_t*);
	void rb_transplant(Node_t* i_top_node, Node_t* i_bottom_node);


	void fix_up(Node_t* );
	void delete_fixup(Node_t* i_node);
	void delete_fixup(Node_t* i_node, bool left_branch);

//Internal find methods------------------------------------------------------------------

	Node_t* find_node(const Key& irk_key)const;

	static Node_t* minimum(Node_t*);
	static Node_t* maximum(Node_t*);

	/*	These (lower|upper)_bound_node functions are declared only if Key_t is one	*/
	/*of St or Key																	*/															
	template<typename Key_t>
	ENABLE_IF_ST_OR_KEY(Key_t, Node_t*) lower_bound_node(const Key_t& irk_key)const;
	template<typename Key_t>
	ENABLE_IF_ST_OR_KEY(Key_t, Node_t*) upper_bound_node(const Key_t& irk_key)const;
	template<typename Key_t>
	ENABLE_IF_ST_OR_KEY(Key_t, Node_t*) insert_pos_node(const Key_t& irk_key)const;

	/*	Properly Declared if Left_t and Right_t are both either St or Key			*/
	template<typename Left_t, typename Right_t>
	ENABLE_IF_ST_OR_KEY(ENABLE_IF_ST_OR_KEY(Left_t, Right_t), bool)
	 equal(const Left_t&, const Right_t&)const;
	
	/*void copy_node(Node_t*& dest_node, const Node_t* const src, Node_t*)*/
	void copy_node(Node_t** dest_node, const Node_t* src, Node_t* i_parent_node);

	void delete_nodes(Node_t*);

	template<class Func_obj>
	Func_obj apply_internal(Func_obj i_func_obj, Node_t* i_node);

#ifdef RB_TREE_DEBUG
	bool s_messages = false;
	std::ostringstream M_output_stream;
	void in_order_walk(std::ostream& oos, Node_t*, size_t& ir_pos);
	void print_output();
	static int num_rotations;
	void sub_proper_rb_tree(Node_t*, bool&, int&, int&)const;
#endif // RB_TREE_DEBUG


}; //J_RB_Tree

#define RB_NODE_T typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Node__

template<typename St, typename Key, typename Comp_t, class Alloc_t>
class J_RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Node__{
public:
	enum class COLOR{ RED, BLACK };

	bool is_leaf()const;

	COLOR color()const;

	void set_color(COLOR i_color);
	void set_parent(RB_Node__* i_parent);
	void set_right(RB_Node__* i_right);
	void set_left(RB_Node__* i_left);

	bool is_red()const;
	bool is_black()const;
	void color_black();
	void color_red();

	bool is_right()const;
	bool is_left()const;

	RB_Node__* left()const;
	RB_Node__* right()const;
	RB_Node__* parent()const;


	RB_Node__*& left();
	RB_Node__*& right();
	RB_Node__*& parent();


	RB_Node__(const RB_Node__& irk_src);
	RB_Node__(RB_Node__&& irk_src);
	RB_Node__(const St&  irk_data);
	RB_Node__(St&& irr_data);

	St& data();
	St& data()const;

private:
	St M_data;
	COLOR M_color;

	//friend typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::iterator;
	//friend typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::const_iterator;

	RB_Node__* M_left;
	RB_Node__* M_right;
	RB_Node__* M_parent;
};

template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>::J_RB_Tree():M_root(nullptr), M_size(0){}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>::J_RB_Tree(
	const J_RB_Tree<St, Key, Comp_t, Alloc_t>& irk_right)
	: M_size(irk_right.size()), M_comp(irk_right.M_comp){
	M_root = nullptr;
	
	copy_node(&M_root, irk_right.M_root, nullptr);

#ifdef RB_TREE_DEBUG
	s_messages = irk_right.s_messages;
#endif // RB_TREE_DEBUG

}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::copy_node(
	Node_t** i_dest_node, const Node_t*  irk_src, Node_t* i_parent_node){
	if(!irk_src){
		*i_dest_node = nullptr;
		return;
	}

	*i_dest_node = new Node_t(*irk_src);

	Node_t* dest_node = *i_dest_node;

	dest_node->set_parent(i_parent_node);

	copy_node(&dest_node->left(), irk_src->left(), *i_dest_node);
	copy_node(&dest_node->right(), irk_src->right(), *i_dest_node);
	
}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>::J_RB_Tree(J_RB_Tree<St, Key, Comp_t, Alloc_t>&& irr_right){
	M_size = 0; 
	M_root = nullptr;
#ifdef RB_TREE_DEBUG
	s_messages = false;
#endif // RB_TREE_DEBUG

	
	swap(irr_right);
}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>::J_RB_Tree(const std::initializer_list<St>& irk_list)
	:J_RB_Tree(irk_list.begin(), irk_list.end()){}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
template<typename iter>
J_RB_Tree<St, Key, Comp_t, Alloc_t>::J_RB_Tree(iter i_pos, iter i_last):J_RB_Tree(){
	assign(i_pos, i_last);
}

/*void assign(Iter, Iter)*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
template<typename Iter>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::assign(Iter i_pos, Iter i_end){
	clear();
	while(i_pos != i_end){
		insert(*i_pos++);
	}
}

/*J_RB_Tree& operator=(const J_RB_Tree&)*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>& J_RB_Tree<St, Key, Comp_t, Alloc_t>
	::operator=(
	const J_RB_Tree<St, Key, Comp_t, Alloc_t>& irk_src){
	
	J_RB_Tree<St, Key, Comp_t, Alloc_t> temp(irk_src);
	swap(temp);
	return *this;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>& J_RB_Tree<St, Key, Comp_t, Alloc_t>
	::operator=(
		J_RB_Tree<St, Key, Comp_t, Alloc_t>&& irr_src){
	swap(irr_src);
	return *this;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::swap(J_RB_Tree& ir_src){
		std::swap(M_root, ir_src.M_root);
		std::swap(M_size, ir_src.M_size);


#ifdef RB_TREE_DEBUG
		std::swap(s_messages, ir_src.s_messages);
#endif // RB_TREE_DEBUG
}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
std::pair<RB_TREE_ITERATOR, bool> 
	J_RB_Tree<St, Key, Comp_t, Alloc_t>::insert(const St& irk_val){

	return insert_core(new Node_t(irk_val));
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
std::pair<typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::iterator, bool> 
	J_RB_Tree<St, Key, Comp_t, Alloc_t>::insert(St&& irr_val){

	return insert_core(new Node_t(std::move(irr_val)));
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
std::pair<RB_TREE_ITERATOR, bool> J_RB_Tree<St, Key, Comp_t, Alloc_t>
	::insert_core(Node_t* i_new_node){
#ifdef RB_TREE_DEBUG
	if(s_messages){
		M_output_stream << "Inserting: " << i_new_node->data()

			<< " Search Cost Incurred" << std::endl;
	}
#endif // RB_TREE_DEBUG



	
	Node_t* prev_node = insert_pos_node(i_new_node->data());

	i_new_node->set_parent(prev_node);

	if(!prev_node){
#ifdef RB_TREE_DEBUG
		if(s_messages){
			M_output_stream << "New Root Node: " << i_new_node->data() << std::endl;
		}
#endif // RB_TREE_DEBUG
		M_root = i_new_node;
	}else if(M_comp(i_new_node->data(), prev_node->data())){
		prev_node->set_left(i_new_node);
	}else{
		prev_node->set_right(i_new_node);
	}


	i_new_node->right() = i_new_node->left() = nullptr;
	i_new_node->color_red();
	assert(M_root);
	fix_up(i_new_node);


	++M_size;

#ifdef RB_TREE_DEBUG
	print_output();
	assert(proper_rb_tree());
#endif // RB_TREE_DEBUG

	return insert_result_t(iterator(i_new_node), true);
	
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::fix_up(Node_t* i_node){
#ifdef RB_TREE_DEBUG
	M_output_stream << "Fixing up node: " << i_node->data() << std::endl;
#endif // RB_TREE_DEBUG

	assert(i_node->is_red());
	Node_t* trace_node = nullptr;
	while(i_node->parent()->is_red()){
		if(i_node->parent()->is_left()){
			trace_node = i_node->parent()->parent()->right();
			if(trace_node->is_red()){
				i_node->parent()->color_black();
				trace_node->color_black();
				i_node->parent()->parent()->color_red();
				i_node = i_node->parent()->parent();
			}else {
				if(i_node->is_right()){
					i_node = i_node->parent();
					rotate_left(i_node);
				}
				i_node->parent()->color_black();
				i_node->parent()->parent()->color_red();
				rotate_right(i_node->parent()->parent());
			}
		}else{
			trace_node = i_node->parent()->parent()->left();
			if(trace_node->is_red()){
				i_node->parent()->color_black();
				trace_node->color_black();
				i_node->parent()->parent()->color_red();
				i_node = i_node->parent()->parent();
			}else {
				if(i_node->is_left()){
					i_node = i_node->parent();
					rotate_right(i_node);
				}
				i_node->parent()->color_black();
				i_node->parent()->parent()->color_red();
				rotate_left(i_node->parent()->parent());
			}
		}
	}
	M_root->color_black();
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
j_size_t J_RB_Tree<St, Key, Comp_t, Alloc_t>::erase(const Key& i_key){
	auto key_range = equal_range(i_key);
	
	int return_val = 0;
	while(key_range.first != key_range.second){
		erase(key_range.first++);
		++return_val;
	}

	return return_val;
}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::iterator 
	J_RB_Tree<St, Key, Comp_t, Alloc_t>::erase(const_iterator i_pos){

	Node_t* cur_node = i_pos.base();
	assert(cur_node);
	iterator iterator_to_return = ++iterator(cur_node); 
	assert(iterator(next_node(cur_node)) == iterator_to_return);
	Node_t* node_to_delete = cur_node;
	Node_t* trailing_node = cur_node;
	assert(trailing_node);

#ifdef RB_TREE_DEBUG
	if(s_messages){
		M_output_stream << "Erasing " << trailing_node->data() <<
			" Search Cost Incurred" << std::endl;
	}
#endif // RB_TREE_DEBUG

	bool left_branch;
	bool is_y = false;
	bool right = false;
	Node_t* node_to_fixup;
	typename Node_t::COLOR original_color = trailing_node->color();
	Node_t* old_parent = nullptr;
	if(!cur_node->left()){
		node_to_fixup = cur_node->right();
		left_branch = cur_node->is_left();
		rb_transplant(cur_node, cur_node->right());
	}else if(!cur_node->right()){
		node_to_fixup = cur_node->left();
		left_branch = cur_node->is_left();
		rb_transplant(cur_node, cur_node->left());
	}else{
		is_y = true;
		trailing_node = minimum(cur_node->right());
		original_color = trailing_node->color();
		node_to_fixup = trailing_node->right();

		left_branch = trailing_node->is_left();
		if((trailing_node->parent() == cur_node)){
			if(node_to_fixup){
				node_to_fixup->parent() = trailing_node;
			}else{
				right = true;
			}
		}else{
			rb_transplant(trailing_node, trailing_node->right());
			trailing_node->right() = cur_node->right();
			trailing_node->right()->parent() = trailing_node;
		}

		if(!right)
			old_parent = trailing_node->parent();

		rb_transplant(cur_node, trailing_node);
		trailing_node->left() = cur_node->left();
		trailing_node->left()->parent() = trailing_node;
		trailing_node->set_color(cur_node->color());
	}

	if(Node_t::COLOR::BLACK == original_color){
		if(node_to_fixup)
			delete_fixup(node_to_fixup);
		else if(is_y){
			Node_t* node_to_use = right ? trailing_node : old_parent;
			delete_fixup(node_to_use, left_branch);
		}else{
			delete_fixup(cur_node->parent(), left_branch);
		}
	}

	delete node_to_delete;
	--M_size;

#ifdef RB_TREE_DEBUG
	if(Node_t::COLOR::RED == original_color && s_messages){
		M_output_stream << "###BYPASSED DELETE FIXUP### " << std::endl;
	}
	print_output();
	assert(proper_rb_tree());
#endif // RB_TREE_DEBUG

	return iterator_to_return;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::iterator 
	J_RB_Tree<St, Key, Comp_t, Alloc_t>::erase(
	const_iterator i_pos, const_iterator i_end_pos){

	iterator iterator_to_return
		= end() == i_end_pos ? end() : ++iterator(find_node(i_end_pos));

	while(i_pos != i_end_pos){
		i_pos = erase(i_pos);
	}


	return iterator_to_return;


}



template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::rb_transplant(Node_t* i_top_node, Node_t* i_bottom_node){
#ifdef RB_TREE_DEBUG
	M_output_stream << "Transplanting Nodes! Top Node: " << i_top_node->data();

	if(i_bottom_node){
		M_output_stream << " Bottom Node: " << i_bottom_node->data();
	}else{
		M_output_stream << " null bottom node...";
	}
	M_output_stream << std::endl;
#endif // RB_TREE_DEBUG
	if(!i_top_node->parent()){
#ifdef RB_TREE_DEBUG
		M_output_stream << "New Root Node: " ;
			if(i_bottom_node)
				M_output_stream << i_bottom_node->data() << std::endl;
			else {
				M_output_stream << "null node" << std::endl;
			}
#endif // RB_TREE_DEBUG
		M_root = i_bottom_node;
	}else if(i_top_node->is_left()){
		i_top_node->parent()->left() = i_bottom_node;
	}else{
		i_top_node->parent()->right() = i_bottom_node;
	}
	if(i_bottom_node){
		i_bottom_node->parent() = i_top_node->parent();
	}
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::delete_fixup(Node_t* i_node){
	Node_t* trailing_node;
	if(!i_node){
#ifdef RB_TREE_DEBUG
		if(s_messages){
			M_output_stream<< "Delete Fixing up null node" << std::endl;
		}
#endif // RB_TREE_DEBUG
		return;
	}
#ifdef RB_TREE_DEBUG
	if(s_messages){
		M_output_stream << "Delete Fixing up: " << i_node->data() << std::endl;
	}
#endif // RB_TREE_DEBUG
	while((i_node != M_root) && i_node->is_black()){
		if(i_node->is_left()){
			trailing_node = i_node->parent()->right();
			if(trailing_node->is_red()){
				trailing_node->color_black();
				i_node->parent()->color_red();
				rotate_left(i_node->parent());
				trailing_node = i_node->parent()->right();
			}

			if(!trailing_node->left()->is_red() && !trailing_node->right()->is_red()){
				trailing_node->color_red();
				i_node = i_node->parent();
			}else {
				if(!trailing_node->right()->is_red()){
					trailing_node->left()->color_black();
					trailing_node->color_red();
					rotate_right(trailing_node);
					trailing_node = i_node->parent()->right();
				}
				trailing_node->set_color(i_node->parent()->color());
				i_node->parent()->color_black();
				trailing_node->right()->color_black();
				rotate_left(i_node->parent());
				i_node = M_root;
			}
		}else{
			trailing_node = i_node->parent()->left();
			if(trailing_node->is_red()){
				trailing_node->color_black();
				i_node->parent()->color_red();
				rotate_right(i_node->parent());
				trailing_node = i_node->parent()->left();
			}
			if(!trailing_node->right()->is_red() && !trailing_node->left()->is_red()){
				trailing_node->color_red();
				i_node = i_node->parent();
			}else {
				if(!trailing_node->left()->is_red()){
					trailing_node->right()->color_black();
					trailing_node->color_red();
					rotate_left(trailing_node);
					trailing_node = i_node->parent()->left();
				}
				trailing_node->set_color(i_node->parent()->color());
				i_node->parent()->color_black();
				trailing_node->left()->color_black();
				rotate_right(i_node->parent());
				i_node = M_root;
			}
		}
	}
	i_node->color_black();
	
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::delete_fixup(Node_t* i_node, bool left_branch){
	Node_t* trailing_node;
	if(!i_node){

#ifdef RB_TREE_DEBUG
		if(s_messages){
			M_output_stream<< "Special First Iteration, Delete Fixing up null node" << std::endl;
		}
#endif // RB_TREE_DEBUG
		return;
	}
#ifdef RB_TREE_DEBUG
	if(s_messages){
		M_output_stream << "Special First Iteration Delete Fixing up: " << i_node->data() << std::endl;
	}
#endif // RB_TREE_DEBUG
	
		if(left_branch){
			trailing_node = i_node->right();
			if(trailing_node->is_red()){
				trailing_node->color_black();
				i_node->color_red();
				rotate_left(i_node);
				trailing_node = i_node->right();
			}
			if(!trailing_node->left()->is_red() && !trailing_node->right()->is_red()){
				trailing_node->color_red();
			}else {
				if(!trailing_node->right()->is_red()){
					trailing_node->left()->color_black();
					trailing_node->color_red();
					rotate_right(trailing_node);
					trailing_node = i_node->right();
				}
				trailing_node->set_color(i_node->color());
				i_node->color_black();
				trailing_node->right()->color_black();
				rotate_left(i_node);
				i_node = M_root;
			}
		}else{
			trailing_node = i_node->left();
			if(trailing_node->is_red()){
				trailing_node->color_black();
				i_node->color_red();
				rotate_right(i_node);
				trailing_node = i_node->left();
			}
			if(trailing_node->right()->is_black() && trailing_node->left()->is_black()){
				trailing_node->color_red();
			}else {
				if(trailing_node->left()->is_black()){
					trailing_node->right()->color_black();
					trailing_node->color_red();
					rotate_left(trailing_node);
					trailing_node = i_node->left();
				}
				trailing_node->set_color(i_node->color());
				i_node->color_black();
				trailing_node->left()->color_black();
				rotate_right(i_node);
				i_node = M_root;
			}
		}

	delete_fixup(i_node);
	
}

/*void clear()*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::clear(){
	delete_nodes(M_root);
	M_size = 0;
	M_root = nullptr;
}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_TREE_ITERATOR J_RB_Tree<St, Key, Comp_t, Alloc_t>::begin(){
	return iterator(minimum());
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_TREE_ITERATOR J_RB_Tree<St, Key, Comp_t, Alloc_t>::end(){
	return iterator(nullptr);
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_TREE_CONST_ITERATOR J_RB_Tree<St, Key, Comp_t, Alloc_t>::begin()const{
	return const_iterator(minimum());
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_TREE_CONST_ITERATOR J_RB_Tree<St, Key, Comp_t, Alloc_t>::end()const{
	return const_iterator(nullptr);
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_TREE_CONST_ITERATOR J_RB_Tree<St, Key, Comp_t, Alloc_t>::cbegin()const{
	return const_iterator(minimum());
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_TREE_CONST_ITERATOR J_RB_Tree<St, Key, Comp_t, Alloc_t>::cend()const{
	return const_iterator(nullptr);
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
template<class Func_obj>
Func_obj J_RB_Tree<St, Key, Comp_t, Alloc_t>::apply(Func_obj i_func_obj){
	return apply_internal(i_func_obj, M_root);
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
template<class Func_obj>
Func_obj J_RB_Tree<St, Key, Comp_t, Alloc_t>
	::apply_internal(Func_obj i_func_obj, Node_t* i_node){
	if(!i_node)
		return i_func_obj;
	i_func_obj(i_node->data());
	i_func_obj = apply_internal(i_func_obj, i_node->left());
	return apply_internal(i_func_obj, i_node->right());
}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::rotate_right(Node_t* i_node){
#ifdef RB_TREE_DEBUG
	if(s_messages){
		M_output_stream <<"Rotating Right Node: " << i_node->data() << std::endl;
	}
#endif // RB_TREE_DEBUG
	Node_t* trace_node = i_node->left();

#ifdef RB_TREE_DEBUG
	if(!trace_node){
		throw Bad_Tree_Op("No Left Node!");
	}
#else
	assert(trace_node);
#endif // RB_TREE_DEBUG

	i_node->left() = trace_node->right();
	if(trace_node->right()){
		trace_node->right()->parent() = i_node;
	}
	trace_node->parent() = i_node->parent();
	if(!i_node->parent()){

#ifdef RB_TREE_DEBUG
		if(s_messages){
			M_output_stream << "New Root Node: " << trace_node->data() << std::endl;
		}
#endif // RB_TREE_DEBUG
		M_root = trace_node;
	}else if(i_node->is_right()){
		i_node->parent()->right() = trace_node;
	}else{
		i_node->parent()->left() = trace_node;
	}
	trace_node->right() = i_node;
	i_node->parent() = trace_node;
#ifdef RB_TREE_DEBUG
	if(s_messages){
		M_output_stream << "Rotation " << num_rotations << std::endl;
	}
	++num_rotations;
#endif // RB_TREE_DEBUG
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::iterator J_RB_Tree<St, Key, Comp_t, Alloc_t>::find(const Key& irk_key){
	Node_t* found_node(find_node(irk_key));
	if(found_node){
		return iterator(find_node(irk_key));
	}else{
		return end();
	}
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::const_iterator J_RB_Tree<St, Key, Comp_t, Alloc_t>::find(const Key& irk_key)const{
	Node_t* found_node(find_node(irk_key));
	if(found_node){
		return const_iterator(find_node(irk_key));
	}else{
		return end();
	}
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t* J_RB_Tree<St, Key, Comp_t, Alloc_t>
	::find_node(const Key& irk_key)const{
	Node_t* cur_node = M_root;
	while(cur_node && !equal(cur_node->data(), irk_key)){
		if(M_comp(irk_key, cur_node->data())){
			cur_node = cur_node->left();
		}else{
			cur_node = cur_node->right();
		}
	}
	return cur_node;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
template<typename Key_t>
ENABLE_IF_ST_OR_KEY(Key_t, typename RB_TREE_TEMPLATE::Node_t*) J_RB_Tree<St, Key, Comp_t, Alloc_t>
	::lower_bound_node(const Key_t& irk_key)const{

	Node_t* cur_node = M_root;
	Node_t* trail_node = cur_node;
	while(cur_node){
		trail_node = cur_node;
		if(M_comp(cur_node->data(), irk_key)){
			cur_node = cur_node->right();
		} else{
			cur_node = cur_node->left();
		}

	}
	assert(trail_node || empty());
	
	return trail_node;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
template<typename Key_t>
ENABLE_IF_ST_OR_KEY(Key_t, typename RB_TREE_TEMPLATE::Node_t*)
RB_TREE_TEMPLATE::insert_pos_node(const Key_t& irk_key)const{

	Node_t* prev_node;
	Node_t* cur_node = prev_node = M_root;

	while(cur_node){
		//find upper_bound_node;
		prev_node = cur_node;
		if(M_comp(irk_key, cur_node->data())){
			cur_node = cur_node->left();
		} else{
			cur_node = cur_node->right();
		}
	}

	return prev_node;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::iterator 
	J_RB_Tree<St, Key, Comp_t, Alloc_t>::lower_bound(const Key& irk_key){



	return iterator(lower_bound_node(irk_key));
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::const_iterator
J_RB_Tree<St, Key, Comp_t, Alloc_t>::lower_bound(const Key& irk_key)const{



	return const_iterator(lower_bound_node(irk_key));
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_TREE_ITERATOR J_RB_Tree<St, Key, Comp_t, Alloc_t>::upper_bound(const Key& irk_key){
	return iterator(upper_bound_node(irk_key));
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_TREE_CONST_ITERATOR J_RB_Tree<St, Key, Comp_t, Alloc_t>::upper_bound(const Key& irk_key)const{
	return const_iterator(upper_bound_node(irk_key));
}

/*Minimum*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::iterator J_RB_Tree<St, Key, Comp_t, Alloc_t>::minimum(){
	if(empty()){
		return end();
	}
	return iterator(minimum(M_root));
}

/*Minimum*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::const_iterator J_RB_Tree<St, Key, Comp_t, Alloc_t>::minimum()const{
	if(empty()){
		return end();
	}
	return const_iterator(minimum(M_root));
}

/*Maximum*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::iterator J_RB_Tree<St, Key, Comp_t, Alloc_t>::maximum(){
	if(empty()){
		return end();
	}
	return iterator(maximum(M_root));
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t* J_RB_Tree<St, Key, Comp_t, Alloc_t>::minimum(Node_t* i_node){
	if(!i_node){
		return nullptr;
	}
	while( i_node->left()){
		i_node = i_node->left();
	}

	return i_node;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t* J_RB_Tree<St, Key, Comp_t, Alloc_t>::maximum(Node_t* i_node){
	if(!i_node){
		return nullptr;
	}
	while( i_node->right()){
		i_node = i_node->right();
	}

	return i_node;
}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_TREE_ITERATOR J_RB_Tree<St, Key, Comp_t, Alloc_t>::root(){
	return iterator(M_root);
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_TREE_CONST_ITERATOR J_RB_Tree<St, Key, Comp_t, Alloc_t>::root()const{
	return const_iterator(M_root);
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
template<typename Left_t, typename Right_t>
ENABLE_IF_ST_OR_KEY(ENABLE_IF_ST_OR_KEY(Left_t, Right_t), bool)
	J_RB_Tree<St, Key, Comp_t, Alloc_t>::equal(const Left_t& irk_data, const Right_t& irk_key)const{
	return (!M_comp(irk_data, irk_key) && !M_comp(irk_key, irk_data));
}
		
			
	
template<typename St, typename Key, typename Comp_t, class Alloc_t>
j_size_t J_RB_Tree<St, Key, Comp_t, Alloc_t>::size()const{
	return M_size;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
bool J_RB_Tree<St, Key, Comp_t, Alloc_t>::empty()const{
	return !M_root;
}

/*bool count(const Key&)const*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
j_size_t J_RB_Tree<St, Key, Comp_t, Alloc_t>::count(const Key& irk_key)const{
	j_size_t return_val = 0;
	auto range = equal_range(irk_key);

	while(range.first != range.second){
		++return_val;
		++range.first;
	}
	return return_val;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t* 
	J_RB_Tree<St, Key, Comp_t, Alloc_t>::next_node(const Node_t* i_node){
	assert(i_node);
	if(i_node->right()){
		return minimum(i_node->right());
	}

	while(i_node->is_right()){
		i_node = i_node->parent();
	}
	
	return i_node->parent();
}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t* 
	J_RB_Tree<St, Key, Comp_t, Alloc_t>::previous_node(const Node_t* i_node){
	assert(i_node);
	if(i_node->left()){
		return maximum(i_node->left());
	}

	while(i_node->is_left()){
		i_node = i_node->parent();
	}
	
	return i_node->parent();
}



template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>::~J_RB_Tree(){
	
	delete_nodes(M_root);
}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::delete_nodes(Node_t* i_node){
	if(!i_node)
		return;

	delete_nodes(i_node->left());
	delete_nodes(i_node->right());
	delete i_node;
		
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_TREE_TEMPLATE::range_t
	RB_TREE_TEMPLATE::equal_range(const Key& irk_key){
	return std::make_pair(lower_bound(irk_key), upper_bound(irk_key));
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_TREE_TEMPLATE::const_range_t
RB_TREE_TEMPLATE::equal_range(const Key& irk_key)const{
	return std::make_pair(lower_bound(irk_key), upper_bound(irk_key));
}

/*Rotate Left*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::rotate_left(Node_t* i_node){
#ifdef RB_TREE_DEBUG
	if(s_messages){
		M_output_stream <<"Rotating Left Node: " << i_node->data() << std::endl;
	}
#endif // RB_TREE_DEBUG
	Node_t* trace_node = i_node->right();

#ifdef RB_TREE_DEBUG
	if(!trace_node){
		throw Bad_Tree_Op("No Right Node!");
	}
#else
	assert(trace_node);
#endif // RB_TREE_DEBUG

	i_node->right() = trace_node->left();
	if(trace_node->left()){
		trace_node->left()->parent() = i_node;
	}
	trace_node->parent() = i_node->parent();
	if(!i_node->parent()){
#ifdef RB_TREE_DEBUG
		if(s_messages){
			M_output_stream << "New Root Node: " << trace_node->data() << std::endl;
		}
#endif // RB_TREE_DEBUG
		M_root = trace_node;
	}else if(i_node->is_left()){
		i_node->parent()->left() = trace_node;
	}else{
		i_node->parent()->right() = trace_node;
	}
	trace_node->left() = i_node;
	i_node->parent() = trace_node;
#ifdef RB_TREE_DEBUG
	if(s_messages){
		M_output_stream << "Rotation " << num_rotations << std::endl;
	}
	++num_rotations;
#endif // RB_TREE_DEBUG
	
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
template<typename Key_t>
ENABLE_IF_ST_OR_KEY(Key_t, typename RB_TREE_TEMPLATE::Node_t*)
	J_RB_Tree<St, Key, Comp_t, Alloc_t>::upper_bound_node(const Key_t& irk_key)const{

	Node_t* node = insert_pos_node(irk_key);
	
	if(!node){
		return nullptr;
	}

	if(!M_comp(irk_key, node->data()) && !M_comp(node->data(), irk_key)){
		return next_node(node);
	}

	return node;
}




#ifdef RB_TREE_DEBUG //************************************************************

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::set_messages(bool val){
	s_messages = val;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
static void J_RB_Tree<St, Key, Comp_t, Alloc_t>::reset_rotations(){
	num_rotations = 0;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
static int J_RB_Tree<St, Key, Comp_t, Alloc_t>::get_rotations(){
	return num_rotations;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::rotate_right_pub(iterator pos){
		try{
			rotate_right(pos.M_pos);
		} catch(Bad_Tree_Op& e){
			std::cerr << e.what() << std::endl;
		}
	}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::rotate_left_pub(iterator pos){
	try{
		rotate_left(pos.M_pos);
	} catch(Bad_Tree_Op& e){
		std::cerr << e.what();
	}
}

/*void print_output()*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::print_output(){
	if(s_messages){
		std::cout << M_output_stream.str() << std::endl;
	}
	M_output_stream.str(std::string());
}

/*Proper RB Tree*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
int J_RB_Tree<St, Key, Comp_t, Alloc_t>::proper_rb_tree()const{
	bool return_val = true;
	int black_height = 0;
	if(!M_root)
		return true;

	if(M_root->is_red()){
		std::cerr << "Red Root! egad!" << std::endl;
		return false;
	}

	int found_black_height = -1;
	 sub_proper_rb_tree(M_root, return_val, black_height, found_black_height);
	 if(return_val)
		 return found_black_height;
	 return 0;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::sub_proper_rb_tree(Node_t* i_node, bool& valid, int& ir_black_height, int& found_black_height)const{
	if(!valid)
		return;

	if(!i_node->is_red()){
		++ir_black_height;
	}else{
		if(i_node->left()->is_red() || i_node->right()->is_red()){
			std::cerr << "Red on Red Action!!!" << std::endl;
			valid = false;
			return;
		}
	}

	if(!i_node){
		valid = ((found_black_height == -1) || (found_black_height == (ir_black_height)));
		if(!valid){
			assert(found_black_height != ir_black_height);
			std::cerr << "Differing Black Heights Calculated ! " << found_black_height << ' ' << ir_black_height << std::endl;
		}
		found_black_height = ir_black_height;
	}else{
		sub_proper_rb_tree(i_node->left(), valid, ir_black_height, found_black_height);
		sub_proper_rb_tree(i_node->right(), valid, ir_black_height, found_black_height);
	}
	if(!i_node->is_red()){
		--ir_black_height;
	}

}


/*in order_walk*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::in_order_walk(std::ostream& oos){
	size_t pos = 0;
	if(empty()){
		return;
	}
	if(M_root->left())
		in_order_walk(oos, M_root->left(), pos);

	oos << M_root->data() << ' ';
	++pos;
	if(pos == size()){
		oos << std::endl;
		return;
	}
	if(M_root->right())
		in_order_walk(oos, M_root->right(), pos);
}

/*in order walk helper*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::in_order_walk(std::ostream& oos, Node_t* i_node, size_t& ir_pos){
	if(i_node->left())
		in_order_walk(oos, i_node->left(), ir_pos);
	oos << i_node->data() << ' ';
	++ir_pos;
	if(ir_pos == size()){
		oos << std::endl;
		return;
	}
	if(i_node->right())
		in_order_walk(oos, i_node->right(), ir_pos);
}

/*Num rotations*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
int J_RB_Tree<St, Key, Comp_t, Alloc_t>::num_rotations = 0;


#endif // RB_TREE_DEBUG

//RB_Tree_Node_Definitions------------------------------------------------------------

template<typename St, typename Key, typename Comp_t, class Alloc_t>
bool J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::is_leaf()const{
	return (!left() && !right());
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename J_RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Node__::COLOR
J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::color()const{
	return M_color;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::set_color(RB_NODE_T::COLOR i_color){
	M_color = i_color;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::set_parent(RB_Node__* i_parent){
	M_parent = i_parent;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::set_right(RB_Node__* i_right){
	M_right = i_right;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::set_left(RB_Node__* i_left){
	M_left = i_left;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
bool J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::is_red()const{
	return this ? (M_color == COLOR::RED) : false;
}
template<typename St, typename Key, typename Comp_t, class Alloc_t>
bool J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::is_black()const{
	return !is_red();
}
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::color_black(){ M_color = COLOR::BLACK; }

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::color_red(){ M_color = COLOR::RED; }

template<typename St, typename Key, typename Comp_t, class Alloc_t>
bool J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::is_right()const{ return (M_parent && (this == M_parent->right())); }

template<typename St, typename Key, typename Comp_t, class Alloc_t>
bool J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::is_left()const{ return (M_parent && (this == M_parent->left())); }

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_NODE_T* J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::left()const{
	return M_left;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_NODE_T* J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::right()const{
	return M_right;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_NODE_T* J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::parent()const{
	return M_parent;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_NODE_T*& J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::left(){
	return M_left;
}


template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_NODE_T*& J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::right(){
	return M_right;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_NODE_T*& J_RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t::parent(){
	return M_parent;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Node__::RB_Node__(const RB_NODE_T& irk_src):M_data(irk_src.M_data)
, M_color(irk_src.M_color){
	M_left = M_right = M_parent = nullptr;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Node__::RB_Node__(RB_NODE_T&& irk_src):M_data(std::move(irk_src.M_data))
, M_color(irk_src.M_color){
	M_left = M_right = M_parent = nullptr;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Node__::RB_Node__(const St&  irk_data)
:M_data(irk_data), M_color(COLOR::RED){
	M_left = M_right = M_parent = nullptr;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
J_RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Node__::RB_Node__(St&& irr_data) : M_data(std::move(irr_data)){
	M_left = M_right = M_parent = nullptr;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
St& J_RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Node__::data(){
	return M_data;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
St& J_RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Node__::data()const{
	return M_data;
}

}


#endif //J_RB_TREE