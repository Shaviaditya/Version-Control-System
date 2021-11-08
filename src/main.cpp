#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <unistd.h>
#include <string>
#include <fstream>
#include <stdbool.h>
#include <string.h>
#include <vector>
#include <ios>
#include <utility>
#include <map>
#include <unordered_map>
namespace fs = std::filesystem;
std::string setPath;
void addToCache(std::string addPath, char fileType)
{
    struct stat p;
    if (stat((setPath + "/.imperium/.add").c_str(), &p) != 0)
        mkdir((setPath + "/.imperium/.add").c_str(), 0777);
    addPath = addPath.substr(1, addPath.length() - 1);
    if (fileType == 'f')
    {
        struct stat r;
        std::string src = setPath + "/" + addPath;
        std::string fileAdd, dest = setPath + "/.imperium/.add/" + addPath;
        if (addPath.find_last_of('/') != std::string::npos)
        {
            fileAdd = setPath + "/.imperium/.add/" + addPath.substr(0, addPath.find_last_of('/'));
            std::cout << fileAdd << "\n";
            if (stat(fileAdd.c_str(), &r) != 0)
            {
                fs::create_directories(fileAdd);
            }
        }
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
    }
    else if (fileType == 'd')
    {
        struct stat r;
        std::string src = setPath + "/" + addPath;
        std::string fileAdd = setPath + "/.imperium/.add/" + addPath.substr(0, addPath.find_last_of('/'));
        std::string dest = setPath + "/.imperium/.add/" + addPath;
        if (stat(fileAdd.c_str(), &r) != 0)
        {
            fs::create_directories(fileAdd);
        }
    }
}
bool ignoreCheck(std::string pathFile)
{
    std::string getData;
    std::ifstream readData(".imperiumIgnore");
    while (getline(readData, getData))
    {
        if (pathFile.find(getData) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}
bool toBeIgnored(std::string pathFile)
{
    std::string getData;
    std::ifstream readData(setPath + "/.imperium/add.log");
    while (getline(readData, getData))
    {
        if (pathFile.find(getData) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}
bool fileDirException(std::string getPath)
{
    std::string getData;
    std::ifstream readData(setPath + "/.imperium/add.log");
    while (getline(readData, getData))
    {
        if ((getPath + "/").substr(0, (getData + '/').length()) == (getData + "/"))
        {
            return false;
        }
    }
    return true;
}
void add(std::vector<std::string> paths)
{
    std::string root = setPath;
    if (paths[0] == ".")
    {
        std::cout << "Relative Paths added include : "
                  << "\n";
        for (auto &it : fs::recursive_directory_iterator(root))
        {
            std::string fileCheck = it.path();
            std::string relativePath = "/" + fileCheck.substr(root.length() + 1, fileCheck.length() - (root.length() + 1));
            if (!ignoreCheck(fileCheck) && !toBeIgnored(fileCheck.substr(root.length() + 1, fileCheck.length() - (root.length() + 1))))
            {
                std::ofstream writeData;
                writeData.open(setPath + "/.imperium/add.log", std::ios_base::app);
                struct stat checkType;
                {
                    if (stat(fileCheck.c_str(), &checkType) == 0)
                    {
                        if (checkType.st_mode & S_IFDIR)
                        {
                            addToCache(relativePath, 'd');
                        }
                        else if (checkType.st_mode & S_IFREG)
                        {
                            addToCache(relativePath, 'f');
                        }
                    }
                    writeData << relativePath << "\n";
                }
                writeData.close();
            }
        }
    }
    else
    {
        std::cout << "Relative Paths added include : "
                  << "\n";
        for (auto it : paths)
        {
            struct stat checkType;
            {
                if (stat((setPath + "/" + it).c_str(), &checkType) == 0)
                {
                    if (checkType.st_mode & S_IFDIR)
                    {
                        for (auto &it2 : fs::recursive_directory_iterator(setPath + "/" + it))
                        {
                            std::string relpath = it2.path();
                            relpath = relpath.substr(setPath.length() + 1, relpath.length() - setPath.length() - 1);
                            struct stat checkType2;
                            {
                                if (stat((relpath).c_str(), &checkType2) == 0)
                                {
                                    if (checkType2.st_mode & S_IFDIR)
                                    {
                                        addToCache("/" + relpath, 'd');
                                    }
                                    else if (checkType2.st_mode & S_IFREG)
                                    {
                                        std::cout<<"No error here"<<"\n";
                                        addToCache("/" + relpath, 'f');
                                    }
                                }
                            }
                        }
                    }
                    else if (checkType.st_mode & S_IFREG)
                    {
                        addToCache("/" + it, 'f');
                    }
                }
            }
            struct stat *buf;
            if (stat((setPath + "/" + it).c_str(), buf))
            {
                std::cout << setPath + "/" + it << "\n";

                if (ignoreCheck(setPath + "/" + it) == false)
                {
                    if (toBeIgnored(it) == false)
                    {
                        std::ofstream writeData;
                        writeData.open(setPath + "/.imperium/add.log", std::ios_base::app);
                        struct stat checkType;
                        {
                            if (stat((setPath + "/" + it).c_str(), &checkType) == 0)
                            {
                                if (checkType.st_mode & S_IFDIR)
                                {
                                    addToCache("/" + it, 'd');
                                }
                                else if (checkType.st_mode & S_IFREG)
                                {
                                    addToCache("/" + it, 'f');
                                }
                            }
                        }
                        writeData << "/" + it << "\n";
                        writeData.close();
                    }
                }
            }
            else
            {
                std::cout << "Wrong file chosen "
                          << "\n";
            }
        }
    }
}
void init(std::string path)
{
    struct stat *buf;
    const char *cstr = (path + "/.imperium").c_str();
    if (stat(cstr, buf))
    {
        int check = mkdir(".imperium", 0777);
        if (!check)
        {
            int checkdir = chdir("./.imperium");
            if (!checkdir)
            {
                std::ofstream add_file("add.log");
                std::ofstream commit_file(".commit.log");
                std::ofstream conflict("conflict");
                mkdir(".add", 0777);
                conflict << "false\n";
                add_file.close();
                commit_file.close();
                conflict.close();
                chdir("..");
            }
        }
        if (access(".imperiumIgnore", F_OK))
        {
            std::ofstream add_ignore(".imperiumIgnore");
            add_ignore << "/.imperium"
                       << "\n";
            add_ignore << "/.imperiumIgnore"
                       << "\n";
            add_ignore << "/node_modules"
                       << "\n";
            add_ignore << "/.env"
                       << "\n";
            add_ignore.close();
        }
    }
    else
    {
        {
            printf("Folder exists\n");
            chdir("./imperium");
            if (access("add.log", F_OK) && access(".commit.log", F_OK))
            {
                std::ofstream add_file("add.log");
                std::ofstream commit_file(".commit.log");
                commit_file.close();
                add_file.close();
                chdir("..");
            }
            else
            {
                std::cout << "Logs exist\n";
            }
        }
    }
    printf("Repositiory Initialized\n");
}
int main(int argc, char const *argv[])
{
    chdir(getenv("dir"));
    std::string getpath = fs::current_path();
    setPath = getpath;
    if (!strcmp("init", argv[1]))
    {
        std::cout << "\nProcessing....................\n";
        std::string ins_var = (getenv("dir"));
        init(ins_var);
    }
    else if (!strcmp("add", argv[1]))
    {
        std::vector<std::string> argVars;
        for (int i = 2; i < argc; i++)
        {
            argVars.push_back(argv[i]);
        }
        add(argVars);
    }
    else
    {
        std::cout << "Command entered is wrong\n";
    }
    return 0;
}