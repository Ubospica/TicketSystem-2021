#ifndef BACKEND_MAP_HPP
#define BACKEND_MAP_HPP

#include <functional>
#include <cstddef>
#include<iostream>
#include "Utility.hpp"
namespace Backend {
    class NotFound{};
    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    > class map {
    public:
        typedef Ticket::pair<const Key, T> value_type;
    private:
        struct _AVLNode{
            value_type * _data;
            int _height;
            _AVLNode* _left_child;
            _AVLNode* _right_child;
            _AVLNode():_left_child(nullptr),_right_child(nullptr),_height(1){};
            _AVLNode(value_type __data){
                _left_child=nullptr;
                _right_child=nullptr;
                _height=1;
                _data=new value_type (__data);
            }
        };
        _AVLNode* _root_of_tree;
        _AVLNode* _end;
        int _size;
        template<class _T> inline
        const _T& max(const _T& _Left,const _T& _Right){
            return _Left>_Right? _Left:_Right;
        }
        inline int height(const _AVLNode* _Node){return _Node==nullptr? 0:_Node->_height;}
        void _Rotate_R(_AVLNode*& _pos){
            _AVLNode*  _new_root=_pos->_left_child;
            _pos->_left_child=_new_root->_right_child;
            _new_root->_right_child=_pos;
            _pos->_height=max(height(_pos->_left_child),height(_pos->_right_child))+1;
            _new_root->_height=max(height(_new_root->_left_child),height(_pos))+1;
            _pos=_new_root;
        }
        void _Rotate_L(_AVLNode*& _pos){
            _AVLNode* _new_root=_pos->_right_child;
            _pos->_right_child=_new_root->_left_child;
            _new_root->_left_child=_pos;
            _pos->_height=max(height(_pos->_left_child),height(_pos->_right_child))+1;
            _new_root->_height=max(height(_pos),height(_new_root->_right_child))+1;
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
        void _insert__AVLNode(const value_type& _data,_AVLNode*& _pos,bool& is_insert){
            if(_pos==nullptr){
                _pos=new _AVLNode(_data);
                is_insert=1;
                return ;
            }
            else if(Compare()(_data.first,_pos->_data->first)){//左子树
              _insert__AVLNode(_data,_pos->_left_child,is_insert);
                if(height(_pos->_left_child)-height(_pos->_right_child)==2){
                    if(Compare()(_data.first,_pos->_left_child->_data->first)) _Rotate_R(_pos);//LL 情况/右旋
                    else _Rotate_LR(_pos);//LR 情况/先左旋 再右旋
                }
            }
            else if(Compare()(_pos->_data->first,_data.first)){//右子树且右子树为大于等于
                _insert__AVLNode(_data,_pos->_right_child,is_insert);
                if(height(_pos->_right_child)-height(_pos->_left_child)==2){
                    if(Compare()(_pos->_right_child->_data->first,_data.first)) _Rotate_L(_pos);//RR 左旋
                    else _Rotate_RL(_pos);//RL 右旋
                }
            }
            else{ is_insert=0;return;}
            _pos->_height=max(height(_pos->_left_child),height(_pos->_right_child))+1;
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
                    _pos=(_pos->_left_child!=nullptr)? _pos->_left_child:_pos->_right_child;
                    delete _oldNode->_data;
                    delete _oldNode;
                    return false;//子树变矮了
                }
                else{
                    _AVLNode* _temp=_pos->_right_child;//保证一定有右子树
                    while(_temp->_left_child!=nullptr) _temp=_temp->_left_child;//相当于找最小值
                    delete _pos->_data;
                    _pos->_data=new value_type (*_temp->_data);
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
            return _Det;
        }
    public:
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
         //   std::cout<<"delete"<<'\n';
        }
        T & operator[](const Key &key) {
            bool is_insert;
            _AVLNode* _temp=_find(key);
            //value_type _new_temp(key,T());
            if(_temp==nullptr){
               // std::cout<<"?"<<'\n';
                throw NotFound();
            }
            else return _temp->_data->second;
        }
        bool empty() const {return _size==0?1:0;}
        size_t size() const {return _size;}
        void clear() {
            if(_size) _clear(_root_of_tree);
            _size=0;
            _root_of_tree=nullptr;
        }
        bool insert(const value_type &value) {
            bool is_insert;
            _insert__AVLNode(value,_root_of_tree,is_insert);
            if(is_insert) _size++;
            return is_insert;
        }
        bool erase(const Key& key) {
            _AVLNode* Node=_find(key);
            if(Node==nullptr) return false;
            else{
                _remove__AVLNode(key, _root_of_tree);
                _size--;
                return true;
            }
            //return false;
        }
        bool count(const Key &key) const {
            const _AVLNode* _judge=_find(key);
            return _judge==nullptr? 0:1;
        }
         void at(const Key & key,T & data){
            _AVLNode * tmp;
            tmp=_find(key);
            if(tmp!=nullptr) data=tmp->_data->second;
        }
    };
}
#endif
