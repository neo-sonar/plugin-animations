#include "TrimPathEffect.hpp"

namespace mc {

TrimPathEffect::TrimPathEffect(float start, float end) : _start{start}, _end{end}
{
    jassert(_start <= _end);
}

auto TrimPathEffect::start(float newStart) -> void { _start = newStart; }

auto TrimPathEffect::end(float newEnd) -> void { newEnd = _end; }

auto TrimPathEffect::operator()(juce::Path const& path) -> juce::Path
{
    auto const length   = path.getLength();
    auto const startPos = length * _start;
    auto const endPos   = length * _end;

    auto out       = juce::Path{};
    auto lastStart = 0.0F;
    auto iter      = juce::PathFlatteningIterator{path};

    while (iter.next()) {
        if (lastStart >= endPos) { break; }

        auto const line = juce::Line{iter.x1, iter.y1, iter.x2, iter.y2};
        auto const next = lastStart + line.getLength();
        if (next >= startPos) {
            if (out.isEmpty()) {
                out.startNewSubPath(line.withShortenedStart(startPos - lastStart).getStart());
            } else if (iter.subPathIndex == 0) {
                out.startNewSubPath(line.getStart());
            }

            if (next <= endPos) {
                out.lineTo(line.getEnd());
            } else {
                out.lineTo(line.withShortenedEnd(next - endPos).getEnd());
            }

            // if (iter.closesSubPath) { out.closeSubPath(); }
        }

        lastStart = next;
    }

    return out;
}
}  // namespace mc