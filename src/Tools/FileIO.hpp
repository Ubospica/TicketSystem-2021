#ifndef TICKETSYSTEM_2021_FILEIO_HPP
#define TICKETSYSTEM_2021_FILEIO_HPP


#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
//#include <map>

#include "Exception.hpp"
#include "List.hpp"
#include "map_cache.hpp"

namespace Ticket {

    /**
     * file IO class
     *
     * maintains three things : a FILE*, a read&write pointer, and a flag showing whether the file is opened for the first time
     */
    class FileIONoCache {
        //protected:
    public:
        std::FILE *fs = nullptr;
        bool firstOpen = false;
        std::string name;

    public:
        /**
         * @brief position constants
         */
        enum Pos {
            END = -1, BEG = 0,
        }; //CUR = -2,

        FileIONoCache() = default;

        /**
         * constructor
         * @param name file name
         */
        explicit FileIONoCache(const std::string &name) {
            open(name);
        }

        FileIONoCache& operator=(FileIONoCache &&another) = default;

        void open(const std::string &name) {
            if (fs != nullptr) {
                fclose(fs);
                clearerr(fs);
            }
            this -> name = name;
            fs = fopen(name.c_str(), "rb+");
            if (fs == nullptr) {
                fclose(fopen(name.c_str(), "a"));
                fs = fopen(name.c_str(), "rb+");
                if (fs == nullptr) {
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
                fseek(fs, pos, SEEK_SET);
            }
            fread(reinterpret_cast<void*>(&cur), sizeof(cur), 1, fs);
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
                fseek(fs, pos, SEEK_SET);
            }
            else if(pos == Pos::END) {
                fseek(fs, 0, SEEK_END);
            }
//			int tmp = sizeof(cur);
//			std::cerr << "write " << name  << ' ' << cur << ' ' << sizeof(cur) << '\n';
//			std::cerr << "before = " << fs.tellp();
            fwrite(reinterpret_cast<const void*>(&cur), sizeof(cur), 1, fs);
//			std::cerr << " after = " << fs.tellp() << '\n';
        }

        void write(int pos, void *val, int size) {
            if(pos >= 0){
                fseek(fs, pos, SEEK_SET);
            }
            else if(pos == Pos::END) {
                fseek(fs, 0, SEEK_END);
            }
            fwrite(val, size, 1, fs);
        }

        /**
         * move position pointer
         * @param pos >=0 or Pos::END
         */
        void movePos(int pos) {
            if (pos >= 0) {
                fseek(fs, pos, SEEK_CUR);
            }
            else if (pos == Pos::END) {
                fseek(fs, 0, SEEK_END);
            }
        }

        /**
         * move pointer forward
         * @param pos the distance that the pointer is moved forward for
         */
        void moveFromCurrent(int pos) {
            fseek(fs, pos, SEEK_CUR);
        }

        /**
         * show current pointer
         * @return
         */
        size_t tellPos() {
            return ftell(fs);
        }

        /**
         * close the fstream
         */
        void close() {
            if (fs != nullptr) {
                fclose(fs);
                fs = nullptr;
            }
        }

        void clear() {
//			std::cerr << "clear\n";
            fclose(fs);
            fs = fopen(name.c_str(), "w+");
            firstOpen = true;
//			bool tmp = fs.fail();
        }

        ~FileIONoCache() {
            close();
        }

    };

    /**
     * FileIO with LRU cache
     */
    class FileIO : public FileIONoCache {
    public:
        using FileIONoCache::FileIONoCache;
    };

   /* protected:
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
        static const int cacheSize = 1000;

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
            //			if (pos < 0) realPos = fs.tellg();
            auto it = cacheIndex.find(realPos);
            if (it == cacheIndex.end()) {
                //not found
                T *ptr = static_cast<T*>(operator new(sizeof(T)));
                FileIONoCache::read(realPos, *ptr);
                cur = *ptr;
                _insertCache(realPos, ptr);
            }
            else {
                cur = *static_cast<T*>((it -> second -> value).value);
                _moveCache(it -> second);
            }
            //			cache.print();
        }

        *
        * write value to fs
        * @tparam T
        * @param pos >=0 or Pos::END(-1) or Pos::CUR (-2)
        * @param cur value

        template <typename T>
        void write(int pos, const T &cur) {
            int realPos = pos;
//			if (pos == Pos::CUR) realPos = fs.tellg();
            if (pos == Pos::END) {
                FileIONoCache::write(pos, cur);
                return;
            }
            auto it = cacheIndex.find(realPos);
            if (it == cacheIndex.end()) {
                FileIONoCache::write(realPos, cur);
                return;
            }
            else {
                auto &cacheNode = it -> second -> value;
                *static_cast<T*>(cacheNode.value) = cur;
                cacheNode.isModifyed = true;
            }
        }
        void clear() {
            auto current = cache.head -> next;
            while (current != cache.end) {
                auto tmp = current;
                current = current -> next;
                _popNode(tmp);
            }
            FileIONoCache::clear();
            cacheCnt = 0;
        }
    };*/
}


#endif //TICKETSYSTEM_2021_FILEIO_HPP
