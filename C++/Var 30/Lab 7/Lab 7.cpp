#define HAVE_STRUCT_TIMESPEC

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <windows.h>
#include <stdlib.h>
#include <pthread.h>

struct Image
{
    std::string name = "";
    std::string extension = "";
    int size = 0;
    std::string date = "";
    std::string color = "";
    std::string note = "";
};

void* del(void* arg);
void* add(void* arg);
void* writeToFile(void* arg);
void* printFilesWithExt(void* arg);
void* printFilesWithSize(void* arg);
void* printFilesWithColor(void* arg);
void* printFilesWithWord(void* arg);
Image generateImage();
void random();

std::vector<Image> vec;

pthread_rwlock_t rwlock;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    pthread_rwlock_init(&rwlock, NULL);

    for (int i = 0; i < 10; i++)
    {
        vec.push_back(generateImage());
    }

    pthread_t p[7];
    pthread_create(
        &p[0],
        NULL,
        del,
        NULL
    );
    pthread_create(
        &p[1],
        NULL,
        add,
        NULL
    );
    pthread_create(
        &p[2],
        NULL,
        writeToFile,
        NULL
    );
    pthread_create(
        &p[3],
        NULL,
        printFilesWithExt,
        NULL
    );
    pthread_create(
        &p[4],
        NULL,
        printFilesWithSize,
        NULL
    );
    pthread_create(
        &p[5],
        NULL,
        printFilesWithWord,
        NULL
    );
    pthread_create(
        &p[6],
        NULL,
        printFilesWithColor,
        NULL
    );

    Sleep(1000000);
    system("pause");

    return 0;
}

void* del(void* arg)
{
    random();

    while (true)
    {
        if (vec.size() != 0)
        {
            pthread_rwlock_wrlock(&rwlock);
            int index = rand() % vec.size();
            vec.erase(vec.begin() + index);
            pthread_rwlock_unlock(&rwlock);

            pthread_mutex_lock(&mutex);
            std::cout << "Deleted file #" << index << std::endl;
            pthread_mutex_unlock(&mutex);
        }

        Sleep(2000);
    }    
}

void* add(void* arg)
{
    random();

    while (true)
    {
        pthread_rwlock_wrlock(&rwlock);
        Image tmp = generateImage();

        pthread_mutex_lock(&mutex);
        std::cout << "Created file:" << std::endl << tmp.name << "\t" << tmp.extension << "\t" << tmp.size << "\t" << tmp.date << "\t" << tmp.color << "\t" << tmp.note << std::endl;
        pthread_mutex_unlock(&mutex);
        
        vec.push_back(tmp);
        pthread_rwlock_unlock(&rwlock);

        Sleep(2000);
    }
}

void* writeToFile(void* arg)
{
    while (true)
    {
        std::ofstream fout("gallery.txt");

        pthread_rwlock_rdlock(&rwlock);
        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            fout << it->name << " " << it->extension << "\t" << it->size << "\t" << it->date << "\t" << it->color << "\t" << it->note << std::endl;
        }
        fout.close();
        pthread_rwlock_unlock(&rwlock);

        pthread_mutex_lock(&mutex);
        std::cout << "Gallery condition was written to file" << std::endl;
        pthread_mutex_unlock(&mutex);

        Sleep(2000);
    }
}

void* printFilesWithExt(void* arg)
{
    random();

    while (true)
    {
        std::vector<std::string> extensions{ "jpg", "png", "gif" };

        pthread_rwlock_rdlock(&rwlock);
        std::string ext = extensions[rand() % 3];

        pthread_mutex_lock(&mutex);
        std::cout << ext << " files:" << std::endl;
        pthread_mutex_unlock(&mutex);

        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            if (it->extension == ext)
            {
                pthread_mutex_lock(&mutex);
                std::cout << it->name << "\t" << it->extension << "\t" << it->size << "\t" << it->date << "\t" << it->color << "\t" << it->note << std::endl;
                pthread_mutex_unlock(&mutex);
            }
        }
        pthread_rwlock_unlock(&rwlock);

        Sleep(2800);
    }
}

void* printFilesWithSize(void* arg)
{
    random();

    while (true)
    {
        int lowerThreshold = rand() % 5 + 1;
        int upperThreshold = rand() % 5 + 6;

        pthread_rwlock_rdlock(&rwlock);

        pthread_mutex_lock(&mutex);
        std::cout << "Files with size between " << lowerThreshold << " and " << upperThreshold << ":" << std::endl;
        pthread_mutex_unlock(&mutex);

        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            if (it->size >= lowerThreshold && it->size <= upperThreshold)
            {
                pthread_mutex_lock(&mutex);
                std::cout << it->name << "\t" << it->extension << "\t" << it->size << "\t" << it->date << "\t" << it->color << "\t" << it->note << std::endl;
                pthread_mutex_unlock(&mutex);
            }
        }
        pthread_rwlock_unlock(&rwlock);

        Sleep(2850);
    }
}

void* printFilesWithColor(void* arg)
{
    random();

    while (true)
    {
        std::vector<std::string> colors{ "red", "green", "blue" };

        std::string color = colors[rand() % 3];

        pthread_mutex_lock(&mutex);
        std::cout << color << " files:" << std::endl;
        pthread_mutex_unlock(&mutex);

        pthread_rwlock_rdlock(&rwlock);
        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            if (it->extension == color)
            {
                pthread_mutex_lock(&mutex);
                std::cout << it->name << "\t" << it->extension << "\t" << it->size << "\t" << it->date << "\t" << it->color << "\t" << it->note << std::endl;
                pthread_mutex_unlock(&mutex);
            }
        }
        pthread_rwlock_unlock(&rwlock);

        Sleep(2900);
    }
}

void* printFilesWithWord(void* arg)
{
    random();

    while (true)
    {
        std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::string word(1, 0);
        word = letters[rand() % letters.size()];

        pthread_rwlock_rdlock(&rwlock);

        pthread_mutex_lock(&mutex);
        std::cout << "Files with word(letter) '" << word << "':" << std::endl;
        pthread_mutex_unlock(&mutex);

        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            if (it->note.find(word) != std::string::npos)
            {
                pthread_mutex_lock(&mutex);
                std::cout << it->name << "\t" << it->extension << "\t" << it->size << "\t" << it->date << "\t" << it->color << "\t" << it->note << std::endl;
                pthread_mutex_unlock(&mutex);
            }
        }
        pthread_rwlock_unlock(&rwlock);

        Sleep(2950);
    }
}

Image generateImage()
{
    std::vector<std::string> colors{ "red", "green", "blue" };
    std::vector<std::string> extensions{ "jpg", "png", "gif" };
    std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    struct Image tmp;

    std::string name(5, 0);
    for (int i = 0; i < 5; i++)
    {
        name[i] = letters[rand() % letters.size()];
    }
    tmp.name = name;

    tmp.extension = extensions[rand() % 3];
    tmp.size = rand() % 10 + 1;
    tmp.date = std::string(std::to_string(rand() % 30 + 1) + "-" + std::to_string(rand() % 12 + 1) + "-" + std::to_string(2010 + rand() % 13));
    tmp.color = colors[rand() % 3];

    std::string note(5, 0);
    for (int i = 0; i < 5; i++)
    {
        note[i] = letters[rand() % letters.size()];
    }
    tmp.note = note;

    return tmp;
}

void random()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    srand((unsigned)li.QuadPart);
}