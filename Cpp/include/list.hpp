#ifndef YSL_LIST_HPP
#define YSL_LIST_HPP
#pragma once
#include <iostream>
#include <vector>
using namespace std;

namespace YSL{
template <class _TYPE>
class list {
	struct _node{
		_TYPE value = NULL;
		_node* next = nullptr;
		_node* prev = nullptr;
	};
	private:
		_node* _head = nullptr;
		_node* _tail = nullptr;
		bool _isRandomAccessable = false;
		std::size_t _size = 0;
		_node** _randomAccessPtr = nullptr;
		std::size_t _randomAccessPtrSize = 0;
		auto get_ptr(std::size_t idx);
	public:
		list(){}
		list(const std::vector<_TYPE>& v){ for(const _TYPE& e : v) push_back(e); }
		list(std::initializer_list<_TYPE> init_list){ for(auto p=init_list.begin(); p!=init_list.end(); ++p) push_back(*p); }
		list(std::pair<std::size_t, _TYPE> init_pair){ for(std::size_t i=0; i<init_pair.first; ++i) push_back(init_pair.second); }
		_TYPE front();
		_TYPE back();
		_TYPE get(std::size_t idx);
		void set(std::size_t idx, _TYPE input);
		void push_back(_TYPE input);
		void push_front(_TYPE input);
		void insert(std::size_t idx, _TYPE input);
		void pop_back();
		void pop_front();
		void erase(std::size_t idx);
		void swap(std::size_t idx_1, std::size_t idx_2);
		void clear();
		void reverse();
		void toRandomAccessable();
		void view();
		void rview();
		void memView();
		bool isRandomAccessable(){ return _isRandomAccessable; }
		bool empty(){ return (_size==0? true : false); }
		std::size_t size(){ return _size; }
		std::vector<_TYPE> to_vector();
		void convert(std::vector<_TYPE>& v);
		void add(std::vector<_TYPE>& v);
		void load(std::vector<_TYPE>& v);
};
}

template <typename _TYPE> auto  YSL::list<_TYPE>::get_ptr(std::size_t idx){
	if(_isRandomAccessable){
		return _randomAccessPtr[idx];
	}else{
		if(_size/2 > idx){
			_node* p = _head;
			for(std::size_t i=0; i<idx; ++i) p = p->next;
			return p;
		}else{
			_node* p = _tail;
			for(std::size_t i=0; i<_size-idx-1; ++i) p = p->prev;
			return p;
		}
	}
}
template <typename _TYPE> _TYPE YSL::list<_TYPE>::front(){
	if(_size==0){
		std::cerr << "[ERROR] : In list.front(), list has no data." << std::endl;
		exit(1);
	}else{
		return _head->value;
	}
}
template <typename _TYPE> _TYPE YSL::list<_TYPE>::back(){
	if(_size==0){
		std::cerr << "[ERROR] : In list.back(), list has no data." << std::endl;
		exit(1);
	}else if(_size==1){
		return _head->value;
	}else{
		return _tail->value;
	}
}
template <typename _TYPE> _TYPE YSL::list<_TYPE>::get(std::size_t idx){
	if(_size==0){
		std::cerr << "[ERROR] : In list.get(idx), list has no data." << std::endl;
		exit(1);
	}else if(_size-1<idx){
		std::cerr << "[ERROR] : In list.get(idx), list doesn't have idx=" << idx << " number data." << std::endl;
		exit(1);
	}else{
		return get_ptr(idx)->value;
	}
}
template <typename _TYPE> void YSL::list<_TYPE>::set(std::size_t idx, _TYPE input){
	if(_size==0){
		std::cerr << "[ERROR] : In list.set(idx, input), list has no data." << std::endl;
		exit(1);
	}else if(_size-1<idx){
		std::cerr << "[ERROR] : In list.set(idx, input), list doesn't have idx=" << idx << " number data." << std::endl;
		exit(1);
	}else{
		get_ptr(idx)->value = input;
	}
}
template <typename _TYPE> void  YSL::list<_TYPE>::push_back(_TYPE input){
	if(_size == 0){
		_head = new _node;
		(*_head) = {input, nullptr, nullptr};
	}else{
		if(_size == 1){
			_tail = new _node;
			(*_tail) = {input, nullptr, _head};
			_head->next = _tail;
		}else{
			_node* p_new = new _node;
			(*p_new) = {input, nullptr, _tail};
			_tail->next = p_new;
			_tail = p_new;
		}
	}
	_isRandomAccessable = false;
	_size++;
}
template <typename _TYPE> void  YSL::list<_TYPE>::push_front(_TYPE input){
	if(_size == 0){
		_head = new _node;
		_head->value = input;
		_head->next = nullptr;
		_head->prev = nullptr;
	}else{
		if(_size == 1){
			_node* p_new = new _node;
			(*p_new) = {input, _head, nullptr};
			_tail = _head;
			_tail->next = nullptr;
			_tail->prev = p_new;
			_head = p_new;
		}else{
			_node* p_new = new _node;
			(*p_new) = {input, _head, nullptr};
			_head->prev = p_new;
			_head = p_new;
		}
	}
	_isRandomAccessable = false;
	_size++;
}
template <typename _TYPE> void  YSL::list<_TYPE>::insert(std::size_t idx, _TYPE input){
	if(_size < idx){
		std::cerr << "[ERROR] : In list.insert(idx, input), list doesn't have idx=" << idx << " number data." << std::endl;
		exit(1);
	}else if(idx == 0){
		std::cerr << "[ERROR] :In list.insert(idx, input), you want to use idx=0, please use list.push_front(input)" << std::endl;
		exit(1);
	}else if(idx == _size){
		std::cerr << "[ERROR] :In list.insert(idx, input), if you use idx=list.size(), please use list.push_back(input)" << std::endl;
		exit(1);
	}else if(idx == _size-1){
		_node* p_new = new _node;
		(*p_new) = {input, _tail, _tail->prev};
		_tail->prev->next = p_new;
		_tail->prev = p_new;
	}else{
		_node* p_idx = get_ptr(idx);
		_node* p_new = new _node;
		(*p_new) = {input, p_idx, p_idx->prev};
		p_idx->prev->next = p_new;
		p_idx->prev = p_new;
	}
	_isRandomAccessable = false;
	_size++;
}
template <typename _TYPE> void  YSL::list<_TYPE>::erase(std::size_t idx){
	if(idx < 0){
		std::cerr << "[ERROR] : In list.erase(idx), Please idx>=0" << std::endl;
		exit(1);
	}
	if(_size-1 < idx){
		std::cerr << "[ERROR] : In list.erase(idx), list doesn't have idx=" << idx << " data." << std::endl;
		exit(1);
	}else{
		if(idx==0){
			if(_size == 1){
				delete _head;
			}else{
				_head = _head->next;
				delete _head->prev;
				_head->prev = nullptr;
			}
		}else if(idx==_size-1){
			_tail = _tail->prev;
			delete _tail->next;
			_tail->next = nullptr;
		}else{
			_node* p_idx = get_ptr(idx);
			p_idx->prev->next = p_idx->next;
			p_idx->next->prev = p_idx->prev;
			delete p_idx;
		}
		_isRandomAccessable = false;
		_size--;
	}
}
template <typename _TYPE> void  YSL::list<_TYPE>::pop_back(){
	if(_size == 0){
		std::cerr << "[ERROR] : In list.pop_back(), list has no data." << std::endl;
		exit(1);
	}else{
		if(_size == 1){
			delete _head;
		}else{
			_tail = _tail->prev;
			delete _tail->next;
			_tail->next = nullptr;
		}
		_isRandomAccessable = false;
		_size--;
	}
}
template <typename _TYPE> void  YSL::list<_TYPE>::pop_front(){
	if(_size == 0){
		std::cerr << "[ERROR] : In list.pop_front(), list has no data." << std::endl;
		exit(1);
	}else if(_size == 1){
		delete _head;
	}else{
		_head = _head->next;
		delete _head->prev;
		_head->prev = nullptr;
		if(_size==1){
			_head->next = nullptr;
			delete _tail;
		}
	}
	_isRandomAccessable = false;
	_size--;
}
template <typename _TYPE> void  YSL::list<_TYPE>::clear(){
	_node* p_itr = _head;
	delete[] _randomAccessPtr;
	while(p_itr != _tail){
		p_itr = p_itr->next;
		delete p_itr->prev;
	}
	delete p_itr;
	_isRandomAccessable = false;
	_size = 0;
}
template <typename _TYPE> void  YSL::list<_TYPE>::swap(std::size_t idx_1, std::size_t idx_2){
	if(idx_1 > idx_2) std::swap(idx_1, idx_2);
	_node* p_1;
	_node* p_2 = _head;
	for(std::size_t i=0; i<idx_2; ++i){
		if(i==idx_1) p_1 = p_2;
		p_2 = p_2->next;
	}
	std::swap(p_1->value, p_2->value);
	_isRandomAccessable = false;
}
template <typename _TYPE> void  YSL::list<_TYPE>::reverse(){
	_node* p_itr = _head;
	while(p_itr != nullptr){
		std::swap(p_itr->next, p_itr->prev);
		p_itr = p_itr->prev;
	}
	std::swap(_head, _tail);
	_isRandomAccessable = false;
}
template <typename _TYPE> void  YSL::list<_TYPE>::view(){
	if(_size==0){
		std::cerr << "[WARNING] : In list.view(), list has no data." << std::endl;
		return;
	}else{
		if(_isRandomAccessable){
			for(std::size_t i=0; i<_size; ++i){
				std::cout << _randomAccessPtr[i]->value << ' ';
			}
			std::cout << std::endl;
		}else{
			_node* p_itr = _head;
			while(p_itr!=nullptr){
				std::cout << p_itr->value << ' ';
				p_itr = p_itr->next;
			}
			std::cout << std::endl;
		}
	}
	return;
}
template <typename _TYPE> void  YSL::list<_TYPE>::rview(){
	if(_size==0){
		std::cerr << "[WARNING] : In list.rview(), list has no data." << std::endl;
		return;
	}else{
		if(_isRandomAccessable){
			for(size_t i=_size-1; i<_size; --i){
				std::cout << _randomAccessPtr[i]->value << ' ';
			}
			std::cout << std::endl;
		}else{
			_node* p_itr = _tail;
			while(p_itr!=nullptr){
				std::cout << p_itr->value << ' ';
				p_itr = p_itr->prev;
			}
			std::cout << std::endl;
		}
	}
	return;
}
template <typename _TYPE> void  YSL::list<_TYPE>::memView(){
	if(_size==0){
		std::cerr << "[WARNING] : In list.view(), list has no data." << std::endl;
		return;
	}else{
		if(_isRandomAccessable){
			for(std::size_t i=0; i<_size; ++i){
				std::cout << _randomAccessPtr[i] << ' ';
			}
			std::cout << std::endl;
		}else{
			_node* p_itr = _head;
			while(p_itr!=nullptr){
				std::cout << p_itr << ' ';
				p_itr = p_itr->next;
			}
			std::cout << std::endl;
		}
	}
	return;
}
template <typename _TYPE> void  YSL::list<_TYPE>::toRandomAccessable(){
	if(_isRandomAccessable){
		return;
	}else{
		if(_randomAccessPtr != nullptr) delete[] _randomAccessPtr;
		_node* p_itr = _head;
		_randomAccessPtr = new _node*[_size];
		for(std::size_t i=0; i<_size; ++i){
			_randomAccessPtr[i] = p_itr;
			p_itr = p_itr->next;
		}
		if(_randomAccessPtrSize != _size) _randomAccessPtrSize = _size;
		_isRandomAccessable = true;
	}
}
template <typename _TYPE> std::vector<_TYPE> YSL::list<_TYPE>::to_vector(){
	std::vector<_TYPE> v(_size);
	_node* p = _head;
	for(std::size_t i=0; i<_size; ++i){
		v[i] = p->value;
		p = p->next;
	}
	return v;
}
template <typename _TYPE> void YSL::list<_TYPE>::convert(std::vector<_TYPE>& v){
	v.resize(_size);
	_node* p = _head;
	for(std::size_t i=0; i<_size; ++i){
		v[i] = p->value;
		p = p->next;
	}
}
template <typename _TYPE> void YSL::list<_TYPE>::add(std::vector<_TYPE>& v){
	for(std::size_t i=0; i<v.size(); ++i){
		push_back(v[i]);
	}
}
template <typename _TYPE> void YSL::list<_TYPE>::load(std::vector<_TYPE>& v){
	clear();
	for(std::size_t i=0; i<v.size(); ++i){
		push_back(v[i]);
	}
}
#endif
