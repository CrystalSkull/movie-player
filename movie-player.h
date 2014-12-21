#define DEFAULT_PATH ("/mnt/downloads/Movies")
#define AMOUNT_VALID_TYPES (2)
const std::string VALID_TYPES[] = {"mp4", "mkv"};
//will find the path to the latest downloaded movie
std::string findLatestDirectory(std::string);
//will take the largest file with a valid filetype
std::string findMovie(std::string);
