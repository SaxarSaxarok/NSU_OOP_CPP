#pragma once
#include <utility>
#include<iterator>
#include <type_traits>

template<typename T>
class Container{
private:
	struct Node{
		T data_ {};
		Node* next_;
		Node* prev_;

		Node(): next_( this ), prev_( this ){}
		Node( T data ): data_( data ), next_( this ), prev_( this ){}
		~Node(){ unlink(); }

		void push_back( Node* node ){
			node->next_ = this;
			node->prev_ = prev_;
			prev_->next_ = node;
			prev_ = node;
		}

		void push_front( Node* node ){
			node->prev_ = this;
			node->next_ = next_;
			next_->prev_ = node;
			next_ = node;
		}

		void unlink(){
			Node* next = next_;
			Node* prev = prev_;
			next->prev_ = prev;
			prev->next_ = next;
			next_ = this;
			prev_ = this;
		}

	};

	template<typename U>
	class Iterator{
		friend class Container;
		using node_ptr_type = std::conditional<std::is_const_v<U>, Node const*, Node*>::type;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = U*;
		using reference = U&;
	private:
		node_ptr_type node_;
		Iterator( node_ptr_type node ): node_( node ){}

	public:
		Iterator& operator++(){
			node_ = node_->next_;
			return *this;
		}
		Iterator& operator--(){
			node_ = node_->prev_;
			return *this;
		}

		Iterator operator++( int ){
			Iterator tmp = *this;
			++( *this );
			return tmp;
		}
		Iterator operator--( int ){
			Iterator tmp = *this;
			--( *this );
			return tmp;
		}

		bool operator==( Iterator other ) const{ return node_ == other.node_; };
		bool operator!=( Iterator other ) const{ return node_ != other.node_; };

		reference operator*() const{ return node_->data_; }
		pointer operator->() const{ return &( node_->data_ ); }
		operator Iterator<U const>() const{ return Iterator<U const>( node_ ); }
	};

public:
	using iterator = Iterator<T>;
	using const_iterator = Iterator<T const>;

	Container() = default;
	Container( std::initializer_list<T> init ){ for ( auto const& i : init ) { push_back( i ); } }
	~Container(){ clear(); }

	iterator begin(){ return iterator( fictiveNode_.next_ ); }
	iterator end(){ return iterator( &fictiveNode_ ); }

	const_iterator cbegin() const{ return const_iterator( fictiveNode_.next_ ); }
	const_iterator cend() const{ return const_iterator( &fictiveNode_ ); }

	void push_back( T data ){ fictiveNode_.push_back( new Node( data ) ); ++size_; }
	void push_front( T data ){ fictiveNode_.push_front( new Node( data ) ); ++size_; }

	T erase( const_iterator it ){
		if ( isEmpty() ) throw std::out_of_range( "Container is empty" );
		auto data = it.node_->data_;
		delete it.node_;
		--size_;
		return data;
	}
	T pop_back(){ return erase( begin() ); }
	T pop_front(){ return erase( --end() ); }

	T& front(){
		if ( isEmpty() )throw std::out_of_range( "Container is empty" );
		return *( --end() );
	}

	T& back(){
		if ( isEmpty() )throw std::out_of_range( "Container is empty" );
		return *( begin() );
	}

	T const& front() const{
		if ( isEmpty() )throw std::out_of_range( "Container is empty" );
		return *( --cend() );
	}

	T const& back() const{
		if ( isEmpty() )throw std::out_of_range( "Container is empty" );
		return *( cbegin() );
	}

	size_t size() const{ return size_; };
	bool isEmpty() const{ return size_ == 0; };

	void swap( Container& other ){
		other.fictiveNode_.next_->prev_ = &fictiveNode_;
		other.fictiveNode_.prev_->next_ = &fictiveNode_;
		fictiveNode_.next_->prev_ = &other.fictiveNode_;
		fictiveNode_.prev_->next_ = &other.fictiveNode_;
		std::swap( other.fictiveNode_.next_, fictiveNode_.next_ );
		std::swap( other.fictiveNode_.prev_, fictiveNode_.prev_ );
	}

	void reverse(){
		if ( isEmpty() ) return;
		auto left = begin();
		auto right = --end();
		while ( left != right && std::next( right ) != left )
		{
			std::swap( ( *left ), ( *right ) );
			++left;
			--right;
		}
	}

	void clear(){ while ( !isEmpty() ) erase( begin() ); }

private:
	Node fictiveNode_; //For iterator::end() and handling special cases (empty, front=back, etc...)
	size_t size_ = 0;
};