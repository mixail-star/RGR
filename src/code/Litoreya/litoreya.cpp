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
        // согласные буквы русского алфавита (21 буква)
        // Б, В, Г, Д, Ж, З, Й, К, Л, М, Н, П, Р, С, Т, Ф, Х, Ц, Ч, Ш, Щ

        static const int rus_upper_consonants[] = {
            1041, 1042, 1043, 1044, 1046, 1047, 1049, 1050, 1051, 1052,
            1053, 1055, 1056, 1057, 1058, 1060, 1061, 1062, 1063, 1064, 1065};
        // б, в, г, д, ж, з, й, к, л, м, н, п, р, с, т, ф, х, ц, ч, ш, щ
        static const int rus_lower_consonants[] = {
            1073, 1074, 1075, 1076, 1078, 1079, 1081, 1082, 1083, 1084,
            1085, 1087, 1088, 1089, 1090, 1092, 1093, 1094, 1095, 1096, 1097};
        const int rus_size = 21;

        // согласные английского алфавита (21 буква)
        static const int eng_upper_consonants[] = {
            66, 67, 68, 70, 71, 72, 74, 75, 76, 77, 78, 80, 81, 82, 83, 84, 86, 87, 88, 90};
        static const int eng_lower_consonants[] = {
            98, 99, 100, 102, 103, 104, 106, 107, 108, 109, 110, 112, 113, 114, 115, 116, 118, 119, 120, 122};
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
