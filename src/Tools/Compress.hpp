//
// Created by Ubospica on 2021/6/20.
//

#ifndef CODE_COMPRESS_HPP
#define CODE_COMPRESS_HPP

#include <unistd.h>

namespace Ticket {
	/**
	 * compress inputname to targetdir/targetname.tar.bz2
	 * @param targetdir
	 * @param targetname
	 * @param inputname
	 */
	void compress(const std::string &targetdir, const std::string &targetname, const std::string inputname) {
		char command[200];
		sprintf(command, "mkdir -p %s", targetdir.c_str());
		system(command);
		sprintf(command, "tar -cjf %s/%s.tar.bz2 %s", targetdir.c_str(), targetname.c_str(), inputname.c_str());
		system(command);
	}
	/**
	 * extract targetdir/targetname.tar.bz2 to directory outputdir
	 * @param targetdir
	 * @param targetname
	 * @param inputname
	 */
	void decompress(const std::string &targetdir, const std::string &targetname, const std::string outputdir) {
		char command[200];
		sprintf(command, "mkdir -p %s", outputdir.c_str());
		system(command);
		sprintf(command, "tar -xjf %s/%s.tar.bz2 %s -C %s", targetdir.c_str(), targetname.c_str(), outputdir);
		system(command);
	}
}



#endif //CODE_COMPRESS_HPP
