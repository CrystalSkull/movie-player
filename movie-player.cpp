#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "movie-player.h"

int main(int argc, char *argv[]) {
	//no arguments, lets assume we just want to play the latest movie with the default path
	if(argc == 1) {
		std::string path = findMovie(findLatestDirectory(DEFAULT_PATH));
		int retVal = startMovie(path);
		if(retVal)
			reportError();
	}
	else if(argc >= 2) {
		std::string path = findMovie(findLatestDirectory(getFlag("--path", argc, argv)));	
		int retVal = startMovie(path);
		if(retVal)
			reportError();
	}
	return 0;
}

std::string findLatestDirectory(std::string path) {
	struct stat st;
	DIR *dir;
	struct dirent *ent;
	
	time_t lastModified = 0;
	std::string nameOfLatest;
	if((dir = opendir(path.c_str())) != NULL) {
		std::string filename;
		while((ent = readdir(dir)) != NULL) {
			filename = std::string(ent->d_name);
			if(filename == "." || filename == "..")
				continue;
			filename = path + "/" + filename;
			if(stat(filename.c_str(), &st) == -1) {
				std::cout << "Could not read stat for file " << filename << std::endl;
				closedir(dir);
				return std::string();
			}
			else {
				//if it's either not set or if the new time is larger => it's more newly modified
				if(!lastModified || st.st_mtime > lastModified) {
					lastModified = st.st_mtime;
					nameOfLatest = filename;
				}
			}
		}
	}
	closedir(dir);
	return std::string(nameOfLatest);
}

std::string findMovie(std::string dirPath) {
	struct stat st;
	DIR *dir;
	struct dirent *ent;

	std::string moviePath;
	off_t sizeOfLargest = 0;

	if((dir = opendir(dirPath.c_str())) != NULL) {
		std::string filename;
		while((ent = readdir(dir)) != NULL) {
			filename = dirPath + "/" + std::string(ent->d_name);
			if(stat(filename.c_str(), &st) == -1) {
				std::cout << "Could not read stat for file " << filename << std::endl;
				closedir(dir);
				return std::string();
			}
			else {
				if(!sizeOfLargest || st.st_size > sizeOfLargest) {
					size_t length = filename.size();
					if(length > 3) {
						std::string filetype = filename.substr(filename.size()-3);
						for(int i = 0; i < AMOUNT_VALID_TYPES; i++) {
							if(filetype == VALID_TYPES[i]) {
								moviePath = filename;
								sizeOfLargest = st.st_size;
							}
						}
					}
				}
			}
		}
	}
	closedir(dir);
	return moviePath;
}

int startMovie(std::string path) {
	std::string fullCommand(COMMAND_TEXT);
	fullCommand.append(" \"" + path + "\"");
	std::cout << "Trying to start movie: " << path << std::endl;
	return system(fullCommand.c_str());
}

void reportError() {
	std::cout << "Could not play this file" << std::endl;
}

std::string getFlag(std::string flag, int argc, char *argv[]) {
	for(int i = 1; i < argc; i++) {
		if(flag.compare(argv[i]) == 0 && i+1 < argc)
			return std::string(argv[i+1]);
	}
	return std::string();
}
