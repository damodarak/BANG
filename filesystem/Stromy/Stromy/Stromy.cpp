#include <iostream>
#include <filesystem>
#include <string>

using namespace std;

namespace fs = std::filesystem;

void dir(const string& root)
{
    fs::path treep{ root };
    for (auto&& de : fs::recursive_directory_iterator{ treep }) {
        fs::path p{ de };

        cout << p.root_path() << ":" << p.parent_path() << ":" << p.filename() << ":" << p.stem() << ":" << p.extension() << endl;
        
        for (auto&& pi : p) cout << pi;
        cout << endl;

        for (auto&& pi : p) cout << pi.string();
        cout << endl;

        /*fs::path p{ "temp" };
        p /= "user" / "data";
        fs::path p{ "temp/" };
        p += "user" + "data";*/


        /*if (fs::is_directory(de.status()))
        {
            cout << de.path << endl;
        }
        else
        {
            cout << de << endl;
        }*/
    }
}

void copy_rec(const string& from, const string& to)
{
    fs::copy(from, to, fs::copy_options::recursive);
}

void delete_dir_content(const fs::path& dir_path) {
    for (auto& path : fs::directory_iterator(dir_path)) {
        fs::remove_all(path);
    }
}

void delete_tree(const fs::path& dir_path)
{
    if (fs::is_directory(dir_path))
    {
        fs::remove_all(dir_path);
    }
}

void copy_tree(const fs::path& from, const fs::path& to)
{
    if (fs::is_directory(from) && !fs::exists(to))
    {
        fs::copy(from, to, fs::copy_options::recursive);
    }   
}

void copy_dir_tree(const fs::path& from, const fs::path& to)
{
    if (fs::is_directory(from) && !fs::exists(to))
    {
        fs::copy(from, to, fs::copy_options::directories_only | fs::copy_options::recursive);
    }
}

int main(int argc, char** argv)
{
    string f = "D:/from";
    string t = "D:/to";
    copy_tree(f, t);
    copy_tree(f, t);
    delete_tree(t);
    delete_tree(t);
    copy_dir_tree(f, t);
}