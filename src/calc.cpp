#include "calc.hpp"

#include <cctype>
#include <cmath>
#include <iostream>
#include <vector>
bool is_fold = false;
namespace {

const std::size_t max_decimal_digits = 10;

enum class Op
{
    ERR,
    SET,
    ADD,
    SUB,
    MUL,
    DIV,
    REM,
    NEG,
    POW,
    SQRT
};

std::size_t arity(const Op op)
{
    switch (op) {
    // error
    case Op::ERR: return 0;
    // unary
    case Op::NEG: return 1;
    case Op::SQRT: return 1;
    // binary
    case Op::SET: return 2;
    case Op::ADD: return 2;
    case Op::SUB: return 2;
    case Op::MUL: return 2;
    case Op::DIV: return 2;
    case Op::REM: return 2;
    case Op::POW: return 2;
    }
    return 0;
}

Op parse_op(std::string_view line, std::size_t & i)
{
    const auto rollback = [&i, &line](const std::size_t n) {
        i -= n;
        std::cerr << "Unknown operation " << line << std::endl;
        return Op::ERR;
    };
    is_fold = line[i] == '(';
    if (is_fold) {
        i++;
    }
    Op current_op;
    switch (line[i++]) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        --i; // a first digit is a part of op's argument
        return Op::SET;
    case '+': {
        current_op = Op::ADD;
        break;
    }
    case '-': {
        current_op = Op::SUB;
        break;
    }
    case '*': {
        current_op = Op::MUL;
        break;
    }
    case '/': {
        current_op = Op::DIV;
        break;
    }
    case '%': {
        current_op = Op::REM;
        break;
    }
    case '_':
        return Op::NEG;
    case '^': {
        current_op = Op::POW;
        break;
    }
    case 'S':
        switch (line[i++]) {
        case 'Q':
            switch (line[i++]) {
            case 'R':
                switch (line[i++]) {
                case 'T':
                    return Op::SQRT;
                default:
                    return rollback(4);
                }
            default:
                return rollback(3);
            }
        default:
            return rollback(2);
        }
    default:
        return rollback(1);
    }
    if (is_fold && line[i++] != ')')
        return rollback(3);
    return current_op;
}

std::size_t skip_ws(const std::string & line, std::size_t i)
{
    while (i < line.size() && std::isspace(line[i])) {
        ++i;
    }
    return i;
}

double parse_arg(const std::string & line, std::size_t & i, bool & last_parsed)
{
    double res = 0;
    std::size_t count = 0;
    bool good = true;
    bool integer = true;
    double fraction = 1;
    while (good && i < line.size() && count < max_decimal_digits) {
        switch (line[i]) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (integer) {
                res *= 10;
                res += line[i] - '0';
            }
            else {
                fraction /= 10;
                res += (line[i] - '0') * fraction;
            }
            ++i;
            ++count;
            break;
        case '.':
            integer = false;
            ++i;
            break;
        default:
            good = false;
            break;
        }
    }
    if (!good && !is_fold) {
        last_parsed = false;
        std::cerr << "Argument parsing error at " << i << ": '" << line.substr(i) << "'" << std::endl;
    }
    else if (i < line.size() && !is_fold) {
        std::cerr << "Argument isn't fully parsed, suffix left: '" << line.substr(i) << "'" << std::endl;
    }
    return res;
}

double unary(const double current, const Op op)
{
    switch (op) {
    case Op::NEG:
        return -current;
    case Op::SQRT:
        if (current > 0) {
            return std::sqrt(current);
        }
        else {
            std::cerr << "Bad argument for SQRT: " << current << std::endl;
            [[fallthrough]];
        }
    default:
        return current;
    }
}

double binary(const Op op, const double left, const std::vector<double> & args)
{
    double tmp = left;
    for (double elem : args) {
        switch (op) {
        case Op::SET:
            return elem;
        case Op::ADD:
            tmp += elem;
            break;
        case Op::SUB:
            tmp -= elem;
            break;
        case Op::MUL:
            tmp *= elem;
            break;
        case Op::DIV: {
            if (elem != 0) {
                tmp /= elem;
            }
            else {
                std::cerr << "Bad right argument for division: " << elem << std::endl;
                return left;
            }
            break;
        }
        case Op::REM: {
            if (elem != 0) {
                tmp = std::fmod(tmp, elem);
            }
            else {
                std::cerr << "Bad right argument for remainder: " << elem << std::endl;
                return left;
            }
            break;
        }
        case Op::POW: {
            tmp = std::pow(tmp, elem);
            break;
        }
        default:
            return left;
        }
    }
    return tmp;
}
} // anonymous namespace
std::vector<double> parse_args_with_fold_checking(const std::string & line, std::size_t & i)
{
    std::vector<double> parsed_args;
    bool last_parsed;
    do {
        last_parsed = true;
        double arg = parse_arg(line, i, last_parsed);
        i = skip_ws(line, i);
        if (last_parsed) {
            parsed_args.push_back(arg);
        }
        if (!std::isdigit(line[i]) && is_fold && i < line.size()) {
            std::cerr << "Argument parsing error at " << i << ": '" << line.substr(i) << "'" << std::endl;
            break;
        }
    } while (is_fold && i < line.size());
    return parsed_args;
}
double process_line(double currentValue,
                    std::string const & line)
{
    std::size_t i = 0;
    const auto op = parse_op(line, i);
    switch (arity(op)) {
    case 2: {
        i = skip_ws(line, i);
        const auto old_i = i;
        std::vector<double> elements = parse_args_with_fold_checking(line, i);
        if (i == old_i) {
            std::cerr << "No argument for a binary operation" << std::endl;
            break;
        }
        else if (i < line.size()) {
            break;
        }
        return binary(op, currentValue, elements);
    }
    case 1: {
        if (i < line.size()) {
            std::cerr << "Unexpected suffix for a unary operation: '" << line.substr(i) << "'" << std::endl;
            break;
        }
        return unary(currentValue, op);
    }
    default: break;
    }
    return currentValue;
}

double process_line(double currentValue, bool &, const std::string & line)
{
    return process_line(currentValue, line);
}
