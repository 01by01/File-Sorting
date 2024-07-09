
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <windows.h>
#include <fileapi.h>
#include <sys/stat.h>
#include <sqlite3.h>

void insertFileInfo(sqlite3* db, const std::string& path, const std::string& name);





sqlite3* db;

namespace fs = std::filesystem;





std::string getDirectoryName(const std::string& path)
{
    fs::path dirPath(path);
    {
        return dirPath.parent_path().filename().string();
    }
    return dirPath.filename().string();
}


std::string UserInput;

//void listfiles(const fs::path& path)


int main()


    {

    std::cout << "please type in an directory address ie c:\\ \\ \\- please use double backslashes" << std::endl;

    std::getline(std::cin, UserInput);

    std::string DirectoryName = getDirectoryName(UserInput);

    std::string DBTitle = DirectoryName + ".db";

    std::cout << "Database Title:  " << DBTitle << std :: endl;




        /*char* err_msg = 0;;

        int rc = sqlite3_open("AGI_IES.db", &db);

        if (rc != SQLITE_OK)

        {
            std::cerr << "cannot open database: " << sqlite3_errmsg(db) << std::endl;

            sqlite3_close(db);
            return 1;

        }*/

        /* const char* createTABLESQL = "CREATE TABLE IF NOT EXISTS file_info (";

         "id INTEGER PRIMARY KEY AUTOINCREMENT,"
             "path TEXT NOT NULL,"
             "name TEXT NOT NULL)";

         sqlite3_exec(db, createTABLESQL, nullptr, nullptr, nullptr);

         /*if (rc != SQLITE_OK)

         {
             std::cerr << "SQL error: " << err_msg << std::endl;
             sqlite3_free(err_msg);
             sqlite3_close(db);
             return 1;

         }*/



        int rc = sqlite3_open(DBTitle.c_str(), &db);

        if (rc)

        {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;

            return rc;
        }
        const char* createTABLESQL = "CREATE TABLE IF NOT EXISTS file_info ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      "path TEXT NOT NULL,"
                                      "name TEXT NOT NULL)";

        rc = sqlite3_exec(db, createTABLESQL, nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK)

        {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;

            sqlite3_close(db);

            return rc;
        }

        const char* deleteSQL = "DELETE FROM file_info";

            rc = sqlite3_exec(db, deleteSQL, nullptr, nullptr, nullptr);

            if (rc != SQLITE_OK)

            {
                std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;

                sqlite3_close(db);

                return rc;
            }

            else

            {
                std::cout << "all old records deleted successfully." << std::endl;
            }

        const fs::path dir = UserInput;

        for (const auto& entry : fs::directory_iterator(dir))

        {

            if (entry.is_directory())

            {
                std::filesystem::recursive_directory_iterator subdir(entry.path());

                for (const auto& subentry : subdir)

                {
                    if (fs::is_regular_file(subentry.status()))

                    {
                        std::string filepath = entry.path().filename().string();
                        std::string filename = subentry.path().filename().string();
                        std::cout << "File: " << filename << " in Path: " << filepath << std::endl;



                        insertFileInfo(db, filepath, filename);
                    }



                   // listfiles(dir, db);

                    //std::cout << entry.path().filename()  

                   // <<   subentry.path().filename() << std::endl;

                }

            }

            else if (fs::is_regular_file(entry.status()))

            {
                std::string filepath = entry.path().string();
                std::string filename = entry.path().filename().string();
                std::cout << "File: " << filename << "in path " << filepath << std::endl;
            }

        }
        sqlite3_close(db);

        return 0;
    }


    void insertFileInfo(sqlite3* db, const std::string& path, const std::string& name)

    {


        sqlite3_stmt* stmt;

        //sqlite3* db;

        const char* sql = "INSERT INTO file_info (path, name) VALUES (? , ? )";

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)

        {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)

        {
            std::cerr << "failed to execute statement:  " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
  
    }





