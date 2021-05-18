//
// Created by Ubospica on 2021/5/14.
//

#ifndef TICKETSYSTEM_2021_FILEIO_HPP
#define TICKETSYSTEM_2021_FILEIO_HPP

#include "Exception.hpp"

#include <iostream>
#include <fstream>
#include <string>

namespace Ticket {
	
	class FileIO {
	private:
		std::fstream fs;
		bool firstOpen = false;
		
	public:
		enum Pos {
			END = -1, CUR = -2, BEG = 0,
		};
		
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
		
		bool isFirstOpen() {
			return firstOpen;
		}
		
		/**
		 * read value from fs
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
		
		void movePos(int pos) {
			if (pos >= 0) {
				fs.seekp(pos, std::ios::beg);
			}
			else if (pos == Pos::END) {
				fs.seekp(0, std::ios::end);
			}
		}
		
		void moveForward(int pos) {
			fs.seekp(pos, std::ios::cur);
		}
		
		size_t tellPos() {
			return fs.tellp();
		}
		
		void close() {
			fs.close();
		}
		
		~FileIO() {
			fs.close();
		}
	};
}


#endif //TICKETSYSTEM_2021_FILEIO_HPP
