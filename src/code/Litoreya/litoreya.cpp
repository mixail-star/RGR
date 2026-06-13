#include <string>

using namespace std;

int find_consonant_index(int unicode, const int consonants[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (consonants[i] == unicode)
            return i;
    }
    return -1;
}

extern "C"
{
    string process_litoreya(const string &text)
    {
   
        static const int rus_upper_consonants[] = {
        static const int rus_lower_consonants[] = {
        const int rus_size = 21;

        static const int eng_upper_consonants[] = {
        static const int eng_lower_consonants[] = {
   
        const int eng_size = 20;

        string result = "";
        result.reserve(text.size());

        for (size_t i = 0; i < text.size(); ++i)
        {
            unsigned char c = text[i];

            
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            {
                int idx = -1;
                if (c >= 'A' && c <= 'Z')
                {
                    idx = find_consonant_index(c, eng_upper_consonants, eng_size);
                    if (idx != -1)
                    {
                        result += (char)eng_upper_consonants[eng_size - 1 - idx]; 
                    }
                    else
                    {
                        result += (char)c; 
                    }
                }
                else
                {
                    idx = find_consonant_index(c, eng_lower_consonants, eng_size);
                    if (idx != -1)
                    {
                        result += (char)eng_lower_consonants[eng_size - 1 - idx];
                    }
                    else
                    {
                        result += (char)c; 
                    }
                }
            }

            else if (c == 0xD0 || c == 0xD1)
            {
                if (i + 1 < text.size())
                {
                    unsigned char next_c = text[i + 1];
                    int unicode = ((c & 0x1F) << 6) | (next_c & 0x3F);

                    int idx = find_consonant_index(unicode, rus_upper_consonants, rus_size);
                    if (idx != -1)
                    {

                        unicode = rus_upper_consonants[rus_size - 1 - idx];
                    }
                    else
                    {
                        idx = find_consonant_index(unicode, rus_lower_consonants, rus_size);
                        if (idx != -1)
                        {

                            unicode = rus_lower_consonants[rus_size - 1 - idx];
                        }
                    }

                    result += (char)((unicode >> 6) | 0xC0);
                    result += (char)((unicode & 0x3F) | 0x80);
                    i++;
                }
                else
                {
                    result += (char)c;
                }
            }

            else
            {
                result += (char)c;
            }
        }
        return result;
    }
}
