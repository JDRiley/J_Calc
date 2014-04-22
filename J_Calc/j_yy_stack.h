#ifndef J_YY_STACK_H
#define J_YY_STACK_H

#ifndef YY_YY_STACK_HH_INCLUDED
# define YY_YY_STACK_HH_INCLUDED

# include <deque>


namespace yy {

template <class T, class S = std::deque<T>>
class stack{
public:
	// Hide our reversed order.
	typedef typename S::reverse_iterator iterator;
	typedef typename S::const_reverse_iterator const_iterator;

	stack():seq_(){}

	stack(unsigned int n): seq_(n){}

	inline
		T& operator [] (unsigned int i){
			return seq_[i];
	}

	inline
		const T& operator [] (unsigned int i)const{
			return seq_[i];
	}

	inline
		void push(T& t){
			seq_.push_front(std::move(t));
	}



	inline
		void pop(unsigned int n = 1){
			for(; n; --n)
				seq_.pop_front();
	}

	inline
		unsigned int height()const{
			return seq_.size();
	}

	inline const_iterator begin() const { return seq_.rbegin(); }
	inline const_iterator end() const { return seq_.rend(); }

private:
	S seq_;
};

/// Present a slice of the top of a stack.
template <class T, class S = stack<T>>
class slice
{
public:
	slice(const S& stack, unsigned int range)
		: stack_(stack), range_(range){
	}

	inline
		const T& operator [] (unsigned int i)const{
			return stack_[range_ - i];
	}

private:
	const S& stack_;
	unsigned int range_;
};

} // yy

#endif /* !YY_YY_STACK_HH_INCLUDED  */

#endif //J_YY_STACK_H


