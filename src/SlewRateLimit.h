/* ============================================
    MIT License

Copyright (c) 2026 Jamal Meizongo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
===============================================
*/

#ifndef _SLEWRATE
#define _SLEWRATE

template <typename T, typename U>
class SlewRateLimiter
{
public:
    SlewRateLimiter()
        : _output{U{}}, _ratePerSecond{1000}, _maxChangeRate{1}
    {
    }

    SlewRateLimiter(uint16_t ratePerSecond, float dt)
        : _output{U{}}, _ratePerSecond{ratePerSecond}
    {
        _maxChangeRate = ratePerSecond * dt;
    }

    SlewRateLimiter(SlewRateLimiter &&) = default;
    SlewRateLimiter &operator=(SlewRateLimiter &&) = default;

    U update(T target)
    {
        const T error = target - _output;

        if (error > _maxChangeRate)
            _output += _maxChangeRate;
        else if (error < -_maxChangeRate)
            _output -= _maxChangeRate;
        else
            _output = target;

        return _output;
    }

    void setRate(uint16_t newRatePerSecond)
    {
        float dt = static_cast<float>(_maxChangeRate) / _ratePerSecond;
        _maxChangeRate = newRatePerSecond * dt;
        _ratePerSecond = newRatePerSecond;

        reset();
    }

    void reset(U prevOutput = U{}) { _output = prevOutput; }

private:
    U _output;
    uint16_t _ratePerSecond;
    uint8_t _maxChangeRate; // Under reasonable conditions, 0 < maxChangeRate < 255
};
#endif // _SLEWRATE