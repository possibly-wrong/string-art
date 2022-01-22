#include "graph_bresenham.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

int image_size = 0;
long block_size = 0;
std::vector<unsigned char> image;
std::vector<unsigned char> canvas;
std::vector<int> blocks;

// Track cumulative score for currently evaluated line.
double score_changed = 0;
int blocks_changed = 0;

// Track position and black pixel count of current block of pixels.
int block_x = 0;
int block_y = 0;
int block_pixels = 0;

// Update cumulative score for block we just left.
void accumulate_block()
{
    // Only update for block that would actually change.
    if (block_x >= 0
        && block_pixels != blocks[block_y * image_size + block_x])
    {
        // How important is continuing to darken this block?
        double current =
            static_cast<double>(block_pixels) / (block_size * block_size);
        double target = 1 - image[block_y * image_size + block_x] / 255.0;
        score_changed += target - current;
        ++blocks_changed;
    }
}

// Visit pixel on currently evaluated line.
void plot_score(int x, int y)
{
    int x0 = x / block_size;
    int y0 = y / block_size;
    if (x0 != block_x || y0 != block_y)
    {
        // We have left current block, so accumulate final block score.
        accumulate_block();

        // Move to next block.
        block_x = x0;
        block_y = y0;
        block_pixels = blocks[block_y * image_size + block_x];
    }
    if (canvas[y * image_size * block_size + x] == 255)
    {
        ++block_pixels;
    }
}

// Finalize drawing pixel on line with best score.
void plot(int x, int y)
{
    if (canvas[y * image_size * block_size + x] == 255)
    {
        canvas[y * image_size * block_size + x] = 0;
        int x0 = x / block_size;
        int y0 = y / block_size;
        ++blocks[y0 * image_size + x0];
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::fprintf(stderr, "Usage: string_art input.pgm block_size num_pins output.pgm\n");
        return 0;
    }
    std::FILE* image_file = std::fopen(argv[1], "rb");
    int height = 0, depth = 0;
    if (std::fscanf(image_file, "P5 %d%d%d%*c",
                    &image_size, &height, &depth) != 3
        || image_size != height || depth != 255)
    {
        std::fprintf(stderr,
            "Error reading square binary PGM image without comments.\n");
        return 1;
    }
    image.resize(image_size * image_size);
    std::fread(&image[0], 1, image_size * image_size, image_file);
    std::fclose(image_file);
    blocks.resize(image_size * image_size);
    block_size = std::strtol(argv[2], 0, 10);
    long num_pins = std::strtol(argv[3], 0, 10);

    // Create white canvas with pins arranged around a circle.
    canvas.resize(image_size * block_size * image_size * block_size, 255);
    struct Pin { int x, y; };
    std::vector<Pin> pins(num_pins);
    int radius = (image_size * block_size - 1) / 2;
    for (int pin = 0; pin < num_pins; ++pin)
    {
        const double pi = 3.141592653589793;
        double angle = 2 * pi / num_pins * pin;
        pins[pin].x = static_cast<int>(radius * (1 + std::cos(angle)) + 0.5);
        pins[pin].y = static_cast<int>(radius * (1 - std::sin(angle)) + 0.5);
        std::printf("%d %d\n", pins[pin].x, pins[pin].y);
    }

    // Wind thread around pins until image can't be improved.
    int current_pin = 0;
    while (true)
    {
        std::printf("%d ", current_pin);
        int best_pin = -1;
        double best_score = -1;
        for (int next_pin = 0; next_pin < num_pins; ++next_pin)
        {
            score_changed = 0;
            blocks_changed = 0;
            block_x = -1;
            graph::draw_line(pins[current_pin].x, pins[current_pin].y,
                             pins[next_pin].x, pins[next_pin].y, plot_score);
            accumulate_block();
            if (blocks_changed > 0)
            {
                double score = score_changed / blocks_changed;
                if (score > best_score)
                {
                    best_pin = next_pin;
                    best_score = score;
                }
            }
        }
        if (best_score < 0)
        {
            break;
        }
        graph::draw_line(pins[current_pin].x, pins[current_pin].y,
                            pins[best_pin].x, pins[best_pin].y, plot);
        current_pin = best_pin;
    }
    std::printf("\n");
    std::FILE* output_file = std::fopen(argv[4], "wb");
    std::fprintf(output_file, "P5\n%d %d\n255\n",
        image_size * block_size, image_size * block_size);
    std::fwrite(&canvas[0],
        image_size * block_size, image_size * block_size, output_file);
    std::fclose(output_file);
}
