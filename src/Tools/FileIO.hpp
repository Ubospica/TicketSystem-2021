//
// Created by Ubospica on 2021/5/14.
//

#ifndef TICKETSYSTEM_2021_FILEIO_HPP
#define TICKETSYSTEM_2021_FILEIO_HPP

#include "Exception.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <map>

namespace Ticket {
	
	/**
	 * file IO class
	 *
	 * maintains three things : a filestream, a read&write pointer, and a flag showing whether the file is opened for the first time
	 */
	class FileIO {
	private:
		std::fstream fs;
		bool firstOpen = false;
		
	public:
		/**
		 * @brief position constants
		 */
		enum Pos {
			END = -1, CUR = -2, BEG = 0,
		};
		
		/**
		 * constructor
		 * @param name file name
		 */
		explicit FileIO(const std::string &name) {
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
		
		~FileIO() {
			fs.close();
		}
	};
}


#endif //TICKETSYSTEM_2021_FILEIO_HPP
