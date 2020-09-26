#include <memory>
#include <string>
#include <utility>

#include "fsm.h"

using namespace std;

FSM::FSM(shared_ptr<std::string> buffer) : edges_(), is_halt_(false), state_(start_state), buffer_(std::move(buffer)) {
    word_.clear();
}

bool FSM::input(char chr) {
    if(is_halt_) return false;
    for(unsigned i = 0; i < edges_.getLength(); ++i) {
        Edge edge = edges_[i];
        if(edge.from == state_) {
            if(edge.check(chr)) {
                word_ += edge.convert(chr);
                state_ = edge.to;
                return true;
            }
        }
    }
    halt();
    return false;
}

bool FSM::isHalt() const { return is_halt_; }

string FSM::output() const { return word_; }

void FSM::halt() {
    *buffer_ = word_;
    is_halt_ = true;
}

string FSM::restart() {
    auto result = word_;
    is_halt_ = false;
    word_.clear();
    buffer_->clear();
    state_ = start_state;
    return result;
}

void FSM::setEdge(unsigned from, unsigned to, bool(*check)(char), string(*convert)(char)) {
    edges_.prepend(Edge(from, to, check, convert));
}

void FSM::setEdge(Edge edge) {
    edges_.prepend(edge);
}

unsigned FSM::getState() { return state_; }
