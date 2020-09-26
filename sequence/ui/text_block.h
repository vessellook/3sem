#pragma once

#include <iostream>

#include "../sequence.h"
#include "../array_sequence.h"
#include "../linked_list_sequence.h"
#include "../matrix/matrix.h"

using art::ISequence;
using art::ArraySequence;
using art::LinkedListSequence;
using art::IMatrix;

class TextBlock {
friend std::ostream& operator<<(std::ostream& os, const TextBlock& text_block);
public:
    TextBlock(): width_(0), height_(0), lines_(new LinkedListSequence<ISequence<char>*>) {};
    TextBlock(const char* text_line, unsigned N) { Create(text_line, N); };
    template <unsigned N> TextBlock(const char text_line[N]) { Create(text_line, N); };
    explicit TextBlock(const TextBlock& text_block);
    TextBlock(const ISequence<char>& text_line) { Create(text_line); };
    virtual ~TextBlock();

    unsigned getWidth() const { return width_; };
    unsigned getHeight() const { return height_; };
    char get(unsigned line_num, unsigned column_num) const { return (*((*lines_)[line_num]))[column_num]; };
    TextBlock *clone() const { return new TextBlock(*this); };

    void Create(const char* text_line, unsigned N);
    void Create(const ISequence<char>& text_line);
    void Add(const TextBlock& text_block);
    void Add(const ISequence<char>& text_line);
    void Add(const char* text_line, unsigned N);
    void Extend(const TextBlock& text_block);
    template <unsigned N> void Add(const char text_line[N]) { Add(text_line, N); };
    template <unsigned N>  void operator+=(const char text_line[N]) { Add(text_line); };
protected:
    unsigned width_;
    unsigned height_;
    ISequence<ISequence<char>*>* lines_ = nullptr;
};

TextBlock::TextBlock(const TextBlock &text_block): width_(text_block.getWidth()), height_(text_block.getHeight()) {
    lines_ = new LinkedListSequence<ISequence<char>*>();
    for(unsigned i = 0; i < height_; ++i) {
        lines_->prepend((*(text_block.lines_))[i]->clone());
    }
}

TextBlock::~TextBlock() {
    for(unsigned i = 0; i < height_; ++i) {
        delete (*lines_)[i];
    }
    delete lines_;
    lines_ = nullptr;
}

void TextBlock::Create(const char *text_line, unsigned N) {
    width_ = N;
    height_ = 1;
    lines_ = new LinkedListSequence<ISequence<char>*>();
    lines_->prepend(new ArraySequence<char>(text_line, N));
}

void TextBlock::Create(const ISequence<char> &text_line) {
    width_ = text_line.getLength();
    height_ = 1;
    lines_ = new LinkedListSequence<ISequence<char>*>();
    lines_->prepend(new ArraySequence<char>(text_line));
}

void TextBlock::Add(const TextBlock &text_block) {
    if (text_block.width_ > width_) {
        //TODO add some spaces to this
    } else if (text_block.width_ < width_) {
        //TODO add some spaces to text_block
    }
    //TODO add text_block to this. Line by line
}

void TextBlock::Add(const ISequence<char> &text_line) {
    lines_->prepend(text_line.clone());
}

void TextBlock::Add(const char *text_line, unsigned N) {
    lines_->prepend(new ArraySequence<char>(text_line, N));
}

void TextBlock::Extend(const TextBlock &text_block) {
    for(unsigned i = 0; i < height_; ++i) {
        for(unsigned j = width_; j < width_ + text_block.width_; ++j) {
            (*lines_)[i]->prepend(text_block.get(i, j));
        }
    }
    width_ += text_block.width_;
}

std::ostream &operator<<(std::ostream &os, const TextBlock &text_block) {
    for (unsigned i = 0; i < text_block.getHeight(); ++i) {
        for(unsigned j = 0; j < text_block.getHeight(); ++j) {
           os << text_block.lines_->getRef(i)->getRef(j);
        }
        os << std::endl;
    }
    return os;
}





