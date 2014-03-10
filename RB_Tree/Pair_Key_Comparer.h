#ifndef PAIR_KEY_COMPARER
#define PAIR_KEY_COMPARER

#include <J_Fwd_Decl.h>


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
		return M_comp(irk_left, irk_right);
	}
private:
	Comp_t M_comp;
};
}

#endif //PAIR_KEY_COMPARER


