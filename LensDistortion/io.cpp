#include "io.hpp"

using namespace std;

namespace fs = std::filesystem;


bool getFile(const string& dir_path, cv::Mat& outputImage)
{
    const fs::path filePath{ dir_path };

    try
    {
        outputImage = cv::imread(filePath.generic_string(), cv::ImreadModes::IMREAD_GRAYSCALE);
    }
    catch (const std::exception& e)
    {
        cout << e.what();
    }

    return true;
}

void getFiles(const string& dir_path, vector<cv::Mat>& outArray)
{
    const fs::path dir{ dir_path };
    cout << "Reading files..." << endl;

    if (fs::is_directory(dir))
    {
        for (const fs::directory_entry& item : fs::directory_iterator{ dir })
        {
            try
            {
                outArray.push_back(cv::imread(item.path().string(), cv::IMREAD_UNCHANGED));
            }
            catch (const exception& e)
            {
                cout << e.what();
            }
            cout << "Read file: " << item.path().string() << endl;
        }
    }

    cout << "Finished reading files..." << endl;
}


void handleOutFolder(const string& dir_path)
{
    const fs::path outputFolder{ dir_path };
    bool h_result = false;
    if (!fs::exists(dir_path))
    {
        h_result = fs::create_directory(outputFolder);
        cout << "Created folder: " << dir_path << endl;
    }
    else
    {
        h_result = true;
        cout << "Folder: " << dir_path <<" already exists" << endl;
    }
}