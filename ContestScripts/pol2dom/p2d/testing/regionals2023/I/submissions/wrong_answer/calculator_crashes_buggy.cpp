/**
 * 1) Find smallest input that doesn't immediately crash due to subtraction
 * 2) Simplify to assume division operator is last by performing circular shift
 * 3) Use rational number instead of integer division to approximate => linear function a*x + b
 * 4) Work out unique rational fixed point, smallest or largest integer fixed point is top or bottom of rational one
 * 5) Find solution by comparing 2) and 4) and checking behavior of generated sequence (convergence / divergence)
 * 6) Adjust solution for circular shift
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// rational number type
struct rational_t
{
    int64_t p, q;

    rational_t(int64_t p, int64_t q=1) : p(p), q(q) {}

    rational_t& simplify() {
        int64_t x = gcd(p, q);
        p /= x;
        q /= x;
        if (q < 0)
        {
            p = -p;
            q = -q;
        }
        return *this;
    }

    // simplify as early as possible to avoid int overflow
    rational_t operator+(rational_t other) const { int64_t x = lcm(q, other.q); return rational_t(p * (x/q) + other.p * (x/other.q), x).simplify(); }
    rational_t operator-(rational_t other) const { int64_t x = lcm(q, other.q); return rational_t(p * (x/q) - other.p * (x/other.q), x).simplify(); }
    rational_t operator*(rational_t other) const { int64_t x = gcd(p, other.q), y = gcd(other.p, q); return rational_t((p/x) * (other.p/y), (q/y) * (other.q/x)).simplify(); }
    rational_t operator/(rational_t other) const { int64_t x = gcd(p, other.p), y = gcd(other.q, q); return rational_t((p/x) * (other.q/y), (q/y) * (other.p/x)).simplify(); }

    bool operator<(rational_t other) const { return p * other.q < other.p * q; }
    bool operator>(rational_t other) const { return p * other.q > other.p * q; }
    bool operator==(rational_t other) const { return p * other.q == other.p * q; }

    int64_t floor() const { return p / q - (p % q < 0 ? 1 : 0); }
    int64_t ceil() const { return p / q + (p % q > 0 ? 1 : 0); }
};

void bad_code()
{
    cout << "BAD CODE" << endl;
    throw 1;
}

struct operator_t
{
    char op;
    int64_t val;
    int64_t operator()(int64_t x) const
    {
        switch (op)
        {
            case '+': return x + val;
            case '-': return x - val;
            case '*': return x * val;
            case '/': return x / val;
            default: exit(1);
        }
    }
    rational_t operator()(rational_t x) const
    {
        switch (op)
        {
            case '+': return x + val;
            case '-': return x - val;
            case '*': return x * val;
            case '/': return x / val;
            default: exit(1);
        }
    }
    // find minimum non-negative input to get result >= x
    int64_t min_inverse(int64_t x) const
    {
        switch (op)
        {
            case '+': return max((int64_t)0, x - val);
            case '-': return x + val;
            case '*':
                if (val == 0)
                {
                    if (x > 0)
                        bad_code();
                    else
                        return 0;
                }
                return (x + val - 1) / val;
            case '/': return x * val;
            default: exit(1);
        }
    }
};

bool is_op(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; }

istream& operator>>(istream& s, operator_t& o)
{
    while (s >> o.op)
        if (is_op(o.op))
        {
            s >> o.val;
            break;
        }
    return s;
}

istream& operator>>(istream& s, vector<operator_t>& v)
{
    operator_t o;
    for (int i=0;i<4;i++) {
        s >> o;
        v.push_back(o);
    }
    return s;
}

size_t find_op(const vector<operator_t>& v, char op)
{
    for (size_t pos = 0; pos < v.size(); pos++)
        if (v[pos].op == op)
            return pos;
    return v.size();
}

void do_case()
{
    // read input function
    vector<operator_t> f;
    cin >> f;
    // find minimal input that doesn't cause immediate crash
    int64_t min_input = 0;
    const size_t minus_pos = find_op(f, '-');
    if (minus_pos < f.size())
    {
        // work backwards from minus
        for (int pos = minus_pos; pos >= 0; pos--)
            min_input = f[pos].min_inverse(min_input);
    }
    // circular shift to make division the last operator
    const size_t div_pos = find_op(f, '/');
    if (div_pos + 1 < f.size())
    {
        // adjust min_input for shift
        for (size_t pos = 0; pos <= div_pos; pos++)
            min_input = f[pos](min_input);
        rotate(f.begin(), f.begin() + div_pos + 1, f.end());
    }
    // handle division by zero
    if (f.back().op == '/' && f.back().val == 0)
        bad_code();
    // rational version of f is linear and can be represented as x -> ax + b
    // since division is last, integer version of f is x -> floor(ax + b)
    rational_t a = 1, b = 0;
    for (operator_t o : f)
    {
        if (o.op == '*' || o.op == '/')
            a = o(a);
        b = o(b);
    }
    // if a = 1 either all or no inputs are (integer) fixed points
    if (a == 1 && b.floor() != 0)
        bad_code();
    // find rational fixed point and smallest int converging against int fixed point
    if (a > 1)
    {
        // sequence is strictly decreasing for inputs smaller than fixpoint and weakly increasing for values larger than fixpoint
        // thus either ceil(fixpoint) is smallest fixed point or none exists, and if min_input is larger it must be a fixpoint to converge
        int64_t fixpoint = (b / (rational_t(1) - a)).ceil();
        min_input = max(min_input, fixpoint);
        if ((a * min_input + b).floor() != min_input)
            bad_code();
    }
    // for a < 1 the sequences generated by inputs smaller than the rational fixed point are increasing, and for larger inputs they are decreasing
    // hence sequences converge for all inputs, and it suffices that min_input does not strictly decrease
    if (a < 1)
    {
        if ((a * min_input + b).floor() < min_input)
            bad_code();
    }
    // adjust again for circular shift - recall that f has already been shifted
    if (div_pos + 1 < f.size())
    {
        for (size_t offset = 0; offset <= div_pos; offset++)
            min_input = f[f.size() - 1 - offset].min_inverse(min_input);
    }
    // print result
    cout << min_input << endl;
}

int main() {
    int Q;
    cin >> Q;
    while(Q--) {
        try {
            do_case();
        } catch (int myNum) {}
    }
}
