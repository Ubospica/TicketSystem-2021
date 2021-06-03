#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP
#include <functional>
#include <cstddef>
#include <iostream>

//#include "utility.hpp"
//#include "exceptions.hpp"
#include "Utility.hpp"
namespace Ticket {
	template<
			class Key,
			class T,
			class Compare = std::less<Key>
	> class map {
	public:
		typedef Ticket::pair<Key, T> value_type;
	private:
		struct _AVLNode{
			value_type * _data;
			int _height;
			_AVLNode* _left_child;
			_AVLNode* _right_child;
			_AVLNode* _father;
			_AVLNode():_left_child(nullptr),_right_child(nullptr),_father(nullptr),_height(1){};
			_AVLNode(value_type __data){
				_left_child=nullptr;
				_right_child=nullptr;
				_father=nullptr;
				_height=1;
				_data=new value_type (__data);
			}
		};
		_AVLNode* _root_of_tree;
		_AVLNode* _end;
		int _size;
		_AVLNode* _get_sub_min(_AVLNode* _root){
			_AVLNode* _temp=_root;
			if(_root==nullptr) return nullptr;
			while(_temp->_left_child!=nullptr) _temp=_temp->_left_child;
			return _temp;
		}
		const _AVLNode* _get_sub_min(const _AVLNode* _root)const {
			const _AVLNode* _temp=_root;
			if(_root==nullptr) return nullptr;;
			while(_temp->_left_child!=nullptr) _temp=_temp->_left_child;
			return _temp;
		}
		_AVLNode* _get_sub_max(_AVLNode* _root) {
			_AVLNode *_temp = _root;
			if(_root==nullptr) return nullptr;
			while (_temp->_right_child != nullptr) _temp = _temp->_right_child;
			return _temp;
		}
		const _AVLNode* _get_sub_max(const _AVLNode* _root)const {
			const _AVLNode *_temp = _root;
			if(_root==nullptr) return nullptr;
			while (_temp->_right_child != nullptr) _temp = _temp->_right_child;
			return _temp;
		}
		_AVLNode* _get_precursor(_AVLNode* _start){
			if(_start->_left_child!=nullptr) return _get_sub_max(_start->_left_child);
			else if(_start==_end) return _get_sub_max(_root_of_tree);
			else if(_start==_root_of_tree) return nullptr;
			else if(_start==_start->_father->_right_child) return _start->_father;
			else{
				_AVLNode* _temp_father=_start->_father;
				while(_temp_father->_father!=nullptr){
					// std::cout<<'?'<<'\n';
					if(_temp_father->_father->_right_child==_temp_father) return _temp_father->_father;
					_temp_father=_temp_father->_father;
				}
				return nullptr;
			}
		}
		const _AVLNode* _get_precursor(const _AVLNode* _start)const{
			if(_start->_left_child!=nullptr) return _get_sub_max(_start->_left_child);
			else if(_start==_end) return _get_sub_max(_root_of_tree);
			else if(_start==_root_of_tree) return nullptr;
			else if(_start==_start->_father->_right_child) return _start->_father;
			else{
				_AVLNode* _temp_father=_start->_father;
				while(_temp_father->_father!=nullptr){
					if(_temp_father->_father->_right_child==_temp_father) return _temp_father->_father;
					_temp_father=_temp_father->_father;
				}
				return nullptr;
			}
		}
		_AVLNode* _get_successor(_AVLNode* _start){
			if(_start->_right_child!=nullptr) return _get_sub_min(_start->_right_child);
			else if(_start==_root_of_tree) return _end;
			else if(_start->_father->_left_child==_start) return _start->_father;
			else{
				_AVLNode* _temp_father=_start->_father;
				while(_temp_father->_father!=nullptr){
					if(_temp_father->_father->_left_child==_temp_father) return _temp_father->_father;
					_temp_father=_temp_father->_father;
				}
				return _end;
			}
		}
		const _AVLNode* _get_successor(const _AVLNode* _start)const {
			if(_start->_right_child!=nullptr) return _get_sub_min(_start->_right_child);
			else if(_start==_root_of_tree) return _end;
			else if(_start->_father->_left_child==_start) return _start->_father;
			else{
				_AVLNode* _temp_father=_start->_father;
				while(_temp_father->_father!=nullptr){
					if(_temp_father->_father->_left_child==_temp_father) return _temp_father->_father;
					_temp_father=_temp_father->_father;
				}
				return _end;
			}
		}
		template<class _T> inline
		const _T& max(const _T& _Left,const _T& _Right){
			return _Left>_Right? _Left:_Right;
		}
		inline int height(const _AVLNode* _Node){return _Node==nullptr? 0:_Node->_height;}
		void _Rotate_R(_AVLNode*& _pos){
			_AVLNode*  _new_root=_pos->_left_child;
			_pos->_left_child=_new_root->_right_child;
			if(_pos->_left_child!=nullptr) _pos->_left_child->_father=_pos;
			_new_root->_right_child=_pos;
			_new_root->_father=_pos->_father;
			_pos->_father=_new_root;
			_pos->_height=max(height(_pos->_left_child),height(_pos->_right_child))+1;
			_new_root->_height=max(height(_new_root->_left_child),height(_new_root->_right_child))+1;
			_pos=_new_root;
		}
		void _Rotate_L(_AVLNode*& _pos){
			_AVLNode* _new_root=_pos->_right_child;
			_pos->_right_child=_new_root->_left_child;
			if(_pos->_right_child!=nullptr) _pos->_right_child->_father=_pos;
			_new_root->_left_child=_pos;
			_new_root->_father=_pos->_father;
			_pos->_father=_new_root;
			_pos->_height=max(height(_pos->_left_child),height(_pos->_right_child))+1;
			_new_root->_height=max(height(_new_root->_left_child),height(_new_root->_right_child))+1;
			_pos=_new_root;
		}
		void _Rotate_LR(_AVLNode*& _pos){
			_Rotate_L(_pos->_left_child);
			_Rotate_R(_pos);
		}
		void _Rotate_RL(_AVLNode*& _pos){
			_Rotate_R(_pos->_right_child);
			_Rotate_L(_pos);
		}
		_AVLNode* _insert__AVLNode(const value_type& _data,_AVLNode*& _pos,_AVLNode* _father,bool& is_insert){
			_AVLNode* _target=nullptr;
			if(_pos==nullptr){
				_pos=new _AVLNode(_data);
				_pos->_father=_father;
				is_insert=1;
				return _pos;
			}
			else if(Compare()(_data.first,_pos->_data->first)){//左子树
				_target=_insert__AVLNode(_data,_pos->_left_child,_pos,is_insert);
				if(height(_pos->_left_child)-height(_pos->_right_child)==2){
					if(Compare()(_data.first,_pos->_left_child->_data->first)) _Rotate_R(_pos);//LL 情况/右旋
					else _Rotate_LR(_pos);//LR 情况/先左旋 再右旋
				}
			}
			else if(Compare()(_pos->_data->first,_data.first)){//右子树且右子树为大于等于
				_target=_insert__AVLNode(_data,_pos->_right_child,_pos,is_insert);
				if(height(_pos->_right_child)-height(_pos->_left_child)==2){
					if(Compare()(_pos->_right_child->_data->first,_data.first)) _Rotate_L(_pos);//RR 左旋
					else _Rotate_RL(_pos);//RL 右旋
				}
			}
			else{ is_insert=0;return _pos;}
			_pos->_height=max(height(_pos->_left_child),height(_pos->_right_child))+1;
			return _target;
		}
		bool adjust(_AVLNode* & _pos,int _sub_tree){//_sub_tree=0 为左子树变矮 1为右子树变矮
			if(_sub_tree){//右子树上删除情况
				if(height(_pos->_left_child)-height(_pos->_right_child)==1) return true;
				if(height(_pos->_right_child)==height(_pos->_left_child)){--(_pos->_height);return false;}
				if(height(_pos->_left_child->_right_child)>height(_pos->_left_child->_left_child)){
					_Rotate_LR(_pos);
					return false;
				}
				_Rotate_R(_pos);
				if(height(_pos->_right_child)==height(_pos->_left_child)) return false;
				else return true;
			}
			else{//左子树变矮
				if(height(_pos->_right_child)-height(_pos->_left_child)==1) return true;
				if(height(_pos->_right_child)==height(_pos->_left_child)) {--(_pos->_height);return false;}
				if(height(_pos->_right_child->_left_child)>height(_pos->_right_child->_right_child)){
					_Rotate_RL(_pos);
					return false;
				}
				_Rotate_L(_pos);
				if(height(_pos->_right_child)==height(_pos->_left_child)) return false;
				else return true;
			}
		}
		bool _remove__AVLNode(const Key& key,_AVLNode* & _pos){
			if(_pos==nullptr) return true;
			if(Compare()(key,_pos->_data->first)){//左子树上删除
				if(_remove__AVLNode(key,_pos->_left_child)) return true;
				return adjust(_pos,0);//左子树变矮
			}
			else if(Compare()(_pos->_data->first,key)){//右子树上删除
				if(_remove__AVLNode(key,_pos->_right_child)) return true;
				return adjust(_pos,1);//右子树变矮
			}
			else{//删除根节点
				if(_pos->_left_child==nullptr||_pos->_right_child==nullptr){//小于两个节点
					_AVLNode* _oldNode=_pos;
					_AVLNode* _old_father=_pos->_father;
					bool _is_left ;
					if(_old_father!=nullptr) _is_left= _pos == _old_father->_left_child ? 1 : 0;
					_pos=(_pos->_left_child!=nullptr)? _pos->_left_child:_pos->_right_child;
					if(_old_father!=nullptr) {if (_is_left) _old_father->_left_child = _pos; else _old_father->_right_child = _pos;}
					if(_pos!=nullptr) _pos->_father=_old_father;
					delete _oldNode->_data;
					delete _oldNode;
					return false;//子树变矮了
				}
				else{
					_AVLNode* _temp=_pos->_right_child;//保证一定有右子树
					while(_temp->_left_child!=nullptr) _temp=_temp->_left_child;//相当于找最小值
					delete _pos->_data;
					_pos->_data=new value_type(*(_temp->_data));
					if(_remove__AVLNode(_temp->_data->first,_pos->_right_child)) return true;//右子树没有变矮
					return adjust(_pos,1);
				}
			}
			
		}
		_AVLNode* _find(const Key& _key){
			_AVLNode* _temp=_root_of_tree;
			while(_temp!=nullptr){
				if(Compare()(_key,_temp->_data->first)) _temp=_temp->_left_child;
				else if(Compare()(_temp->_data->first,_key))_temp=_temp->_right_child;
				else break;
			}
			return _temp;
		}
		const _AVLNode* _find(const Key& _key)const{
			const _AVLNode* _temp=_root_of_tree;
			while(_temp!=nullptr){
				if(Compare()(_key,_temp->_data->first)) _temp=_temp->_left_child;
				else if(Compare()(_temp->_data->first,_key))_temp=_temp->_right_child;
				else break;
			}
			return _temp;
		}
		void _clear(_AVLNode*& _root){
			if(_root==nullptr) return;
			_clear(_root->_left_child);
			_clear(_root->_right_child);
			delete _root->_data;
			delete _root;
			return;
		}
		_AVLNode* _copy_build(_AVLNode*& _Det,_AVLNode* _Src){
			if(_Src==nullptr) return nullptr;
			_Det=new _AVLNode(*(_Src->_data));
			_Det->_height=_Src->_height;
			_Det->_left_child=_copy_build(_Det->_left_child,_Src->_left_child);
			_Det->_right_child=_copy_build(_Det->_right_child,_Src->_right_child);
			if(_Det->_left_child!=nullptr) _Det->_left_child->_father=_Det;
			if(_Det->_right_child!=nullptr) _Det->_right_child->_father=_Det;
			return _Det;
		}
		_AVLNode* _copy_sub_tree(_AVLNode*& _Det,_AVLNode* _Src){
			if(_Det==nullptr&&_Src==nullptr) return nullptr;
			else if(_Det==nullptr&&_Src!=nullptr) _Det=new _AVLNode(*(_Src->_data));
			else if(_Det!=nullptr&&_Src!=nullptr){
				_Det->_data->first=_Src->_data->first;
				_Det->_data->second=_Src->_data->second;
			}
			else {
				_copy_sub_tree(_Det->_left_child, nullptr);
				_copy_sub_tree(_Det->_right_child, nullptr);
				delete _Det->_data;
				delete _Det;
				return nullptr;
			}
			_Det->_height=_Src->_height;
			_Det->_left_child=_copy_build(_Det->_left_child,_Src->_left_child);
			_Det->_right_child=_copy_sub_tree(_Det->_right_child,_Src->_right_child);
			if(_Det->_left_child!=nullptr) _Det->_left_child->_father=_Det;
			if(_Det->_right_child!=nullptr) _Det->_right_child->_father=_Det;
			return _Det;
		}
	public:
		class const_iterator;
		class iterator {
			friend const_iterator;
			friend class map;
		private:
			map<Key,T,Compare>* _master_map;
			_AVLNode* _pointer;
		public:
			iterator() {
				_master_map=nullptr;
				_pointer=nullptr;
			}
			iterator(const iterator &other) {
				_master_map=other._master_map;
				_pointer=other._pointer;
			}
			iterator(map<Key,T,Compare>* __master_map,_AVLNode* __pointer){
				_master_map=__master_map;
				_pointer=__pointer;
			}
			iterator operator++(int) {
				if(_pointer==_master_map->_end) throw Ticket::invalid_iterator();
				iterator temp(*this);
				_pointer=_master_map->_get_successor(_pointer);
				return temp;
			}
			iterator & operator++() {
				if(_pointer==_master_map->_end) throw Ticket::invalid_iterator();
				_pointer=_master_map->_get_successor(_pointer);
				return *this;
			}
			iterator operator--(int) {
				iterator temp(*this);
				_pointer=_master_map->_get_precursor(_pointer);
				if(_pointer==nullptr) throw Ticket::invalid_iterator();
				return temp;
			}
			iterator & operator--() {
				_pointer=_master_map->_get_precursor(_pointer);
				if(_pointer==nullptr) throw Ticket::invalid_iterator();
				return *this;
			}
			value_type & operator*() const {return *(_pointer->_data);}
			bool operator==(const iterator &rhs) const {return _pointer==rhs._pointer;}
			bool operator==(const const_iterator &rhs) const {return _pointer==rhs._pointer;}
			bool operator!=(const iterator &rhs) const {return _pointer!=rhs._pointer;}
			bool operator!=(const const_iterator &rhs) const {return _pointer!=rhs._pointer;}
			value_type* operator->() const noexcept {return _pointer->_data;}
		};
		class const_iterator {
			friend iterator;
			friend class map;
		private:
			const map<Key,T,Compare>* _master_map;
			const _AVLNode* _pointer;
		public:
			const_iterator() {
				_master_map=nullptr;
				_pointer=nullptr;
			}
			const_iterator(const const_iterator &other) {
				// TODO
				_master_map=other._master_map;
				_pointer=other._pointer;
			}
			const_iterator(const iterator &other) {
				_master_map=other._master_map;
				_pointer=other._pointer;
			}
			const_iterator(const map<Key,T,Compare>* __master_map,const _AVLNode* __pointer){
				_master_map=__master_map;
				_pointer=__pointer;
			}
			const_iterator operator++(int){
				if(_pointer==_master_map->_end) throw Ticket::invalid_iterator();
				const_iterator temp(*this);
				_pointer=_master_map->_get_successor(_pointer);
				return temp;
			}
			const_iterator & operator++(){
				if(_pointer==_master_map->_end) throw Ticket::invalid_iterator();
				_pointer=_master_map->_get_successor(_pointer);
				return *this;
			}
			const_iterator operator--(int){
				const_iterator temp(*this);
				_pointer=_master_map->_get_precursor(_pointer);
				if(_pointer==nullptr) throw Ticket::invalid_iterator();
				return temp;
			}
			const_iterator & operator--(){
				_pointer=_master_map->_get_precursor(_pointer);
				if(_pointer==nullptr) throw Ticket::invalid_iterator();
				return *this;
			}
			value_type & operator*() const {return *(_pointer->_data);}
			bool operator==(const iterator &rhs) const {return _pointer==rhs._pointer;}
			bool operator==(const const_iterator &rhs) const {return _pointer==rhs._pointer;}
			bool operator!=(const iterator &rhs) const {return _pointer!=rhs._pointer;}
			bool operator!=(const const_iterator &rhs) const {return _pointer!=rhs._pointer;}
			value_type* operator->() const noexcept {return _pointer->_data;}
		};
		map() {
			_end=new _AVLNode;
			_root_of_tree=nullptr;
			_size=0;
		}
		map(const map &other) {
			_end=new _AVLNode;
			_root_of_tree=nullptr;
			_copy_build(_root_of_tree,other._root_of_tree);
			_size=other._size;
			if(_root_of_tree!=nullptr)  _root_of_tree->_father=nullptr;
		}
		map & operator=(const map &other) {
			if(this==&other) return* this;
			clear();
			_copy_build(_root_of_tree,other._root_of_tree);
			_size=other._size;
			return *this;
		}
		~map() {
			if(_size) _clear(_root_of_tree);
			delete _end;
		}
		T & at(const Key &key) {
			_AVLNode* _temp=_find(key);
			if(_temp==nullptr) throw Ticket::invalid_iterator();
			else return _temp->_data->second;
		}
		const T & at(const Key &key) const {
			const _AVLNode* _temp=_find(key);
			if(_temp== nullptr) throw Ticket::invalid_iterator();
			else return _temp->_data->second;
		}
		T & operator[](const Key &key) {
			bool is_insert;
			_AVLNode* _temp=_find(key);
			value_type _new_temp(key,T());
			if(_temp==nullptr){ _AVLNode* ans=_insert__AVLNode(_new_temp,_root_of_tree,nullptr,is_insert);
				_size++;
				return ans->_data->second;
			}
			else return _temp->_data->second;
		}
		const T & operator[](const Key &key) const {
			const _AVLNode* _temp=_find(key);
			if(_temp== nullptr) throw Ticket::invalid_iterator();
			else return _temp->_data->second;
		}
		iterator begin() {
			if(!_size) return iterator(this,_end);
			return iterator(this,_get_sub_min(_root_of_tree));
		}
		const_iterator cbegin() const {
			if(!_size) return const_iterator(this,_end);
			return const_iterator(this,_get_sub_min(_root_of_tree));
		}
		iterator end() {return iterator(this, _end);}
		const_iterator cend() const {return const_iterator(this,_end);}
		bool empty() const {return _size==0?1:0;}
		size_t size() const {return _size;}
		void clear() {
			if(_size) _clear(_root_of_tree);
			_size=0;
			_root_of_tree=nullptr;
		}
		Ticket::pair<iterator, bool> insert(const value_type &value) {
			bool is_insert;
			_AVLNode* _ans=_insert__AVLNode(value,_root_of_tree,nullptr,is_insert);
			iterator _res(this,_ans);
			if(is_insert) _size++;
			return Ticket::pair<iterator,bool>(_res,is_insert);
		}
		void erase(iterator pos) {
			if(pos._master_map!=this) throw Ticket::invalid_iterator();
			if(pos==this->end()) throw Ticket::invalid_iterator();
			_remove__AVLNode((pos._pointer)->_data->first,_root_of_tree);
			_size--;
		}
		size_t count(const Key &key) const {
			const _AVLNode* _judge=_find(key);
			return _judge==nullptr? 0:1;
		}
		iterator find(const Key &key) {
			_AVLNode* _res=_find(key);
			if(_res==nullptr) return end();
			else return iterator(this,_res);
		}
		const_iterator find(const Key &key) const {
			const _AVLNode* _res=_find(key);
			if(_res==nullptr) return cend();
			else return const_iterator(this,_res);
		}
	};
}
#endif
