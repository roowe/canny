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

#ifndef KEYMOLEN_CANNY_HPP
#define KEYMOLEN_CANNY_HPP

namespace keymolen {
class Canny {
   public:
    enum class NoiseFilter { Gaus3x3, Gaus5x5 };

   public:
    Canny(int w, int h);
    virtual ~Canny();
    unsigned char* edges(unsigned char* dst, const unsigned char* src,
                         Canny::NoiseFilter kernel_size,
                         int weak_threshold = 50, int strong_threshold = 200);

   private:
    int w_;
    int h_;
    int size_;
    double* G_; // magnitude，强度，或称方向大小
    double* M_; // G_计算完，复制给M_
    unsigned char* s_; // 方向枚举值
};
}

#endif
