map使用文档

不支持erase(iter++) 操作

erase函数可能有点问题，出问题再找我

本来应该包含一个exception.hpp文件，但是我把抛出变成Backend里面了，懒得传两个文件

注：本map的operator[ ]当找不到时会新建一个T()进入map

若想要找不到时会报错（抛出out_of_bound）,请采用at函数

find函数返回iterator

erase函数参数为iterator//如果想要erase(iter++) 请找我

insert返回pair<iterator,bool> 指向插入后的元素，并返回是否插入成功，若插入失败，指向

受阻(即该Key已存在)的位置

其它大概同std

iterator大概同std

（没测试过，有问题找我）

