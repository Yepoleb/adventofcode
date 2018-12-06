#include <fstream>
#include <string>
#include <limits.h>



class CharStream
{
public:
    CharStream(char* str, int size_) :
        buffer(str),
        size(size_),
        pos(0)
    {

    }

    char get()
    {
        if (pos >= size) {
            return -1;
        }
        return buffer[pos++];
    }

    char pop()
    {
        if (pos < 0) {
            return -1;
        }
        pos--;
        return buffer[pos - 1];
    }

    void put(char c)
    {
        buffer[pos++] = c;
    }

    char* buffer;
    int size;
    int pos;
};

int react(char* input, int input_size, char* output, char ignore)
{
    CharStream in_stream(input, input_size);
    CharStream out_stream(output, input_size);

    char c_in = in_stream.get();
    char c_out = -1;

    while (c_in != -1) {
        if (c_in == ignore || c_in == (ignore + 32)) {
            c_in = in_stream.get();
            continue;
        }
        char diff = c_in - c_out;
        if (diff == 32 || diff == -32) {
            c_out = out_stream.pop();
            c_in = in_stream.get();
        } else {
            out_stream.put(c_in);
            c_out = c_in;
            c_in = in_stream.get();
        }
    }

    return out_stream.pos;
}

int main()
{
    volatile int simple_size;
    volatile int shortest;

    for (int i = 0; i < 1000; i++) {
        std::ifstream file("input.txt");
        std::string input;
        std::getline(file, input);

        int input_size = input.size();
        std::string output(input_size, 0);
        char* input_p = &input[0];
        char* output_p = &output[0];

        simple_size = react(input_p, input_size, output_p, 0);

        shortest = INT_MAX;
        for (char c = 'A'; c <= 'Z'; c++) {
            int react_size = react(input_p, input_size, output_p, c);
            if (react_size < shortest) {
                shortest = react_size;
            }
        }
    }

    std::printf("Reacted: %d\n", simple_size);
    std::printf("Shortest %d\n", shortest);

    return 0;
}
