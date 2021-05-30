//
// Created by Ubospica on 2021/5/14.
//

#ifndef TICKETSYSTEM_2021_FILEIO_HPP
#define TICKETSYSTEM_2021_FILEIO_HPP


#include <iostream>
#include <fstream>
#include <string>
//#include <map>

#include "Exception.hpp"
#include "List.hpp"
#include "map_for_cache/map_cache.hpp"

namespace Ticket {
	
	/**
	 * file IO class
	 *
	 * maintains three things : a filestream, a read&write pointer, and a flag showing whether the file is opened for the first time
	 */
	class FileIONoCache {
	protected:
		std::fstream fs;
		bool firstOpen = false;
		
		
	public:
		/**
		 * @brief position constants
		 */
		enum Pos {
			END = -1, CUR = -2, BEG = 0,
		};
		
		FileIONoCache() = default;
		
		/**
		 * constructor
		 * @param name file name
		 */
		explicit FileIONoCache(const std::string &name) {
			open(name);
//			std::vector<int> a;
		}
		
		FileIONoCache& operator=(FileIONoCache &&another) = default;
		
		void open(const std::string &name) {
			fs.close();
			fs.clear();
			constexpr auto fl = std::ios::in | std::ios::out | std::ios::binary;
			fs.open(name, fl);
			if (!fs) {
				std::ofstream tmp(name);
				tmp.close();
				fs.open(name, fl);
				if (!fs) {
					throw RuntimeError("File system error");
				}
				firstOpen = true;
			}
		}
		
		/**
		 * check if the file is opened for the first time
		 * @return true when is opened first; false otherwise
		 */
		bool isFirstOpen() const {
			return firstOpen;
		}
		
		/**
		 * read value from fstream
		 * @tparam T
		 * @param pos Pos::CUR (-2) or >= 0
		 * @param cur value
		 */
		template <typename T>
		void read(int pos, T &cur) {
			if(pos >= 0){ //pos == CUR == -2
				fs.seekg(pos);
			}
			fs.read(reinterpret_cast<char*>(&cur), sizeof(cur));
		}
		
		/**
		 * write value to fs
		 * @tparam T
		 * @param pos >=0 or Pos::END(-1) or Pos::CUR (-2)
		 * @param cur value
		 */
		template <typename T>
		void write(int pos, const T &cur) {
			if(pos >= 0){
				fs.seekp(pos);
			}
			else if(pos == Pos::END) {
				fs.seekp(0, std::ios::end);
			}
			fs.write(reinterpret_cast<const char*>(&cur), sizeof(cur));
		}
		
		void write(int pos, void *val, int size) {
			if(pos >= 0){
				fs.seekp(pos);
			}
			else if(pos == Pos::END) {
				fs.seekp(0, std::ios::end);
			}
			fs.write(reinterpret_cast<const char*>(val), size);
		}
		
		/**
		 * move position pointer
		 * @param pos >=0 or Pos::END
		 */
		void movePos(int pos) {
			if (pos >= 0) {
				fs.seekp(pos, std::ios::beg);
			}
			else if (pos == Pos::END) {
				fs.seekp(0, std::ios::end);
			}
		}
		
		/**
		 * move pointer forward
		 * @param pos the distance that the pointer is moved forward for
		 */
		void moveForward(int pos) {
			fs.seekp(pos, std::ios::cur);
		}
		
		/**
		 * show current pointer
		 * @return
		 */
		size_t tellPos() {
			return fs.tellp();
		}
		
		/**
		 * close the fstream
		 */
		void close() {
			fs.close();
		}
		
		~FileIONoCache() {
			fs.close();
		}
		
	};
	
	/**
	 * FileIO with LRU cache
	 */
	using Backend::map;
	class FileIO : public FileIONoCache {
//	public:
//		using FileIONoCache::FileIONoCache;
//	};
//
	protected:
		struct CacheNode {
			void *value = nullptr;
			int pos = 0, size = 0;
			bool isModifyed = false;
			CacheNode() = default;
			explicit CacheNode(void *value, int pos, int size) : value(value), pos(pos), size(size) {}
			~CacheNode() {
//				if (value != nullptr) {
//					operator delete(value);
//					value = nullptr;
//				}
			}
			friend std::ostream& operator << (std::ostream &os, CacheNode one) {
				os << one.value;
				if (one.value != nullptr) os << ' ' << *(int*)one.value;
				os << " pos = " << one.pos << " sz = " << one.size << " ismo = " << one.isModifyed;
				return os;
			}
		};
		List<CacheNode> cache;
		map<int, List<CacheNode>::Node*> cacheIndex;
		int cacheCnt = 0;
		static const int cacheSize = 4;

		template<typename T>
		void _insertCache(int pos, T *value) {
			auto res = cache.insert(cache.head, CacheNode(static_cast<void*>(value), pos, sizeof(T)));
			cacheIndex.insert(make_pair(pos, res));
			++cacheCnt;
			if (cacheCnt > cacheSize) {
				auto endNode = (cache.end) -> prev;
				_popNode(endNode);
				--cacheCnt;
			}
		}

		void _popNode(List<CacheNode>::Node *oneNode) {
			if ((oneNode -> value).isModifyed) {
				FileIONoCache::write((oneNode -> value).pos, oneNode -> value.value, oneNode -> value.size);
			}
			cacheIndex.erase(cacheIndex.find(oneNode -> value.pos));
			operator delete((oneNode -> value).value);
			cache.erase(oneNode);
		}
		
		void _moveCache(List<CacheNode>::Node *oneNode) {
			cache.eraseNoDelete(oneNode);
			cache.insertNode(cache.head, oneNode);
		}

	public:
		FileIO() : FileIONoCache(), cache(), cacheIndex() { }
		explicit FileIO (const std::string &name) : FileIONoCache(name), cache(), cacheIndex() { }
		~FileIO() {
			auto current = cache.head -> next;
			while (current != cache.end) {
				auto tmp = current;
				current = current -> next;
				_popNode(tmp);
			}
		}


		template <typename T>
		void read(int pos, T &cur)  {
			int realPos = pos;
			if (pos < 0) realPos = fs.tellg();
			auto it = cacheIndex.find(realPos);
			if (it == cacheIndex.end()) {
				//not found
				T *ptr = static_cast<T*>(operator new(sizeof(T)));
				FileIONoCache::read(pos, *ptr);
				cur = *ptr;
				_insertCache(pos, ptr);
			}
			else {
				cur = *static_cast<T*>((it -> second -> value).value);
				_moveCache(it -> second);
			}
//			cache.print();
		}

		/**
		 * write value to fs
		 * @tparam T
		 * @param pos >=0 or Pos::END(-1) or Pos::CUR (-2)
		 * @param cur value
		 */
		template <typename T>
		void write(int pos, const T &cur) {
			int realPos = pos;
			if (pos == Pos::CUR) realPos = fs.tellg();
			else if (pos == Pos::END) {
				FileIONoCache::write(pos, cur);
				return;
			}
			auto it = cacheIndex.find(realPos);
			if (it == cacheIndex.end()) {
				FileIONoCache::write(pos, cur);
				return;
			}
			else {
				auto &cacheNode = it -> second -> value;
				*static_cast<T*>(cacheNode.value) = cur;
				cacheNode.isModifyed = true;
			}
		}
	};
}


#endif //TICKETSYSTEM_2021_FILEIO_HPP
