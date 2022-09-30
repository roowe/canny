// **********************************************************************************
//
// BSD License.
// This file is part of a canny edge detection implementation.
//
// Copyright (c) 2017, Bruno Keymolen, email: bruno.keymolen@gmail.com
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification,
// are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice,
// this
// list of conditions and the following disclaimer in the documentation and/or
// other
// materials provided with the distribution.
// Neither the name of "Bruno Keymolen" nor the names of its contributors may be
// used to endorse or promote products derived from this software without
// specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// **********************************************************************************
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "canny.hpp"

extern FILE* stdin; // 声明而不是定义
extern FILE* stdout;
extern FILE* stderr;

std::string img_path;
int low_threshold = 30;
int high_threshold = 100;

const char* CW_IMG_ORIGINAL = "Original";
const char* CW_IMG_GRAY = "Grayscale";
const char* CW_IMG_EDGE = "Canny Edge Detection";

void doTransform(std::string);

void usage(char* s) {
    fprintf(stderr, "\n");
    fprintf(stderr, "canny edge detection. build: %s-%s \n", __DATE__,
            __TIME__);
    fprintf(stderr,
            "%s -s <source file> [-l <low threshold>] [-h <high threshold>] "
            "[-? help]",
            s); // s是程序名
    fprintf(stderr, "   s: path image file\n");
    fprintf(stderr, "   l: canny low threshold\n");
    fprintf(stderr, "   h: canny high threshold\n");
    fprintf(stderr,
            "\nexample:  ./canny -s img/Valve_original.PNG "
            "-l 30 -h 90\n");
    fprintf(stderr, "\n");
}

int main(int argc, char** argv) {
    int c;
    // https://blog.csdn.net/afei__/article/details/81261879
    // 该方法由 Unix 标准库提供，包含在 <unistd.h> 头文件中。
    // extern char *optarg;
    // extern int optind, opterr, optopt;  
    while (((c = getopt(argc, argv, "s:l:h:?"))) != -1) {
        switch (c) {
            case 's':
                img_path = optarg;
                break;
            case 'l':
                low_threshold = atoi(optarg);
                break;
            case 'h':
                high_threshold = atoi(optarg);
                break;
            case '?':
            default:
                usage(argv[0]);
                return -1;
        }
    }

    if (img_path.empty()) {
        usage(argv[0]);
        return -1;
    }

    cv::namedWindow(CW_IMG_ORIGINAL, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(CW_IMG_GRAY, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(CW_IMG_EDGE, cv::WINDOW_AUTOSIZE);

    cvMoveWindow(CW_IMG_ORIGINAL, 10, 10);
    cvMoveWindow(CW_IMG_GRAY, 680, 10);
    cvMoveWindow(CW_IMG_EDGE, 1350, 10);

    doTransform(img_path);

    double theta = M_PI + 2.01;
    theta = theta * (360 / (2 * M_PI));  // degrees
    int segment = ((int)((theta + 22.5) / 45)) % 8 % 4;

    std::cout << theta << " : " << segment << std::endl;

    return 0;
}

void doTransform(std::string file_path) {
    cv::Mat img_edge;
    cv::Mat img_gray;

    cv::Mat img_ori = cv::imread(file_path, 1); // 读图片
    cv::cvtColor(img_ori, img_gray, cv::COLOR_BGR2GRAY); // 转灰度

    int w = img_gray.cols;
    int h = img_ori.rows;

    keymolen::Canny::NoiseFilter filter = keymolen::Canny::NoiseFilter::Gaus3x3;

    while (1) {
        cv::Mat img_edge(h, w, CV_8UC1, cv::Scalar::all(0));

        keymolen::Canny canny(w, h);
        // 运行canny算法
        canny.edges(img_edge.data, img_gray.data, filter, low_threshold,
                    high_threshold);

        // Visualize all
        cv::imshow(CW_IMG_ORIGINAL, img_ori);
        cv::imshow(CW_IMG_GRAY, img_gray);
        cv::imshow(CW_IMG_EDGE, img_edge);

        char c = cv::waitKey(360000);

        // 动态修改参数
        if (c == 'h') {
            if (high_threshold > 10)
                high_threshold -= 5;
            else
                high_threshold -= 1;
        }
        if (c == 'H') {
            if (high_threshold >= 10)
                high_threshold += 5;
            else
                high_threshold += 1;
        }
        if (c == 'l') {
            if (low_threshold > 10)
                low_threshold -= 5;
            else
                low_threshold -= 1;
        }
        if (c == 'L') {
            if (low_threshold >= 10)
                low_threshold += 5;
            else
                low_threshold += 1;
        }
        if (c == '3') filter = keymolen::Canny::NoiseFilter::Gaus3x3;
        if (c == '5') filter = keymolen::Canny::NoiseFilter::Gaus5x5;
        if (c == 's') {
            cv::imwrite("canny.png", img_edge);
            std::cout << "write canny.png done..." << std::endl;
        }

        std::cout << low_threshold << ", " << high_threshold << std::endl;

        if (c == 27) break;
    }
}
