#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool FileCheck(int argc, char **argv)
{
    std::ifstream txt_file;

    txt_file.open(argv[argc - 1]);

    if (!txt_file)
    {
        std::cerr << "File adress is incorrect\n";
        return 1;
    }
    else
    {
        txt_file.close();
        return 0;
    }
}

size_t WordsInFile(char c, bool &was_word, size_t words)
{
    if (c == ' ')
    {
        if (was_word)
        {
            words++;
            was_word = 0;
        }

        return words;
    }

    if (std::isprint(c))
    {
        was_word = 1; //Если была буква => было слово
        return words;
    }

    if (was_word)
    {
        words++;
        was_word = 0;
    }

    return words;
}

size_t CharsInFile(char c, size_t chars)
{

    if (std::isprint(c) && c != ' ')
    {
        chars++;
    }

    return chars;
}

void CountInFile(int argc, char **argv, size_t &lines, size_t &bytes, size_t &words, size_t &chars)
{
    int co = 0;
    std::ifstream txt_file(argv[argc - 1], std::ios::binary);

    int spaces = 0; //количество пробелов в строках, состоящих не только из пробелов
    char c;
    bool was_word = 0; // проверка, было ли уже слово в строке
    while (1)
    {
        co++;

        txt_file.get(c);
        if (txt_file.eof())
        {
            if (was_word)
                words++;
            lines++;
            break;
        }

        bytes++;

        words = WordsInFile(c, was_word, words);
        chars = CharsInFile(c, chars);
        if (c == 13)
        {
            bytes -= 2; // тк два символа для перевода строки на конце
            lines++;
            was_word = 0;
        }
    }
}

void Output(int argc, char **argv, size_t lines, size_t bytes, size_t words, size_t chars)
{

    if (argc == 3)
    {
        std::cout << lines << " " << words << " " << bytes << " " << argv[argc - 1];
        return;
    }

    for (int i = 1; i < argc - 2; i++)
    {

        if (sizeof(argv[i]) < 2)
            continue;

        if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            if (argv[i] == "--lines")
                std::cout << lines << " ";
            if (argv[i] == "--bytes")
                std::cout << bytes << " ";
            if (argv[i] == "--words")
                std::cout << words << " ";
            if (argv[i] == "--chars")
                std::cout << chars << " ";
        }
        else
        {
            for (int j = 1; j < sizeof(argv[i]); j++)
            {
                switch (argv[i][j])
                {
                case 'l':
                    std::cout << lines << " ";
                    break;
                case 'c':
                    std::cout << bytes << " ";
                    break;
                case 'w':
                    std::cout << words << " ";
                    break;
                case 'm':
                    std::cout << chars << " ";
                    break;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        std::cerr << "Arguments must have the filename\n"
                  << "WordCont.exe [OPTION] filename [filename,.....]\n";
        return 1;
    }

    size_t lines = 0;
    size_t bytes = 0;
    size_t words = 0;
    size_t chars = 0;

    if (FileCheck(argc, argv))
        return 1;

    CountInFile(argc, argv, lines, bytes, words, chars);

    Output(argc, argv, lines, bytes, words, chars);

    return 0;
}