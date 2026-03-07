#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace toon {

class value; // forward declare

using object = std::unordered_map<std::string, value>;
using array  = std::vector<value>;

class value {
public:
    using variant = std::variant<
        std::nullptr_t,
        bool,
        double,
        std::string,
        array,
        object
    >;

private:
    variant data;

public:

    value() : data(nullptr) {}
    value(bool v) : data(v) {}
    value(int v) : data((double)v) {}
    value(double v) : data(v) {}
    value(const char* v) : data(std::string(v)) {}
    value(const std::string& v) : data(v) {}
    value(const array& v) : data(v) {}
    value(const object& v) : data(v) {}

    bool is_object() const { return std::holds_alternative<object>(data); }
    bool is_array()  const { return std::holds_alternative<array>(data); }
    bool is_string() const { return std::holds_alternative<std::string>(data); }
    bool is_number() const { return std::holds_alternative<double>(data); }
    bool is_bool()   const { return std::holds_alternative<bool>(data); }

    object& as_object() { return std::get<object>(data); }
    array& as_array() { return std::get<array>(data); }

    value& operator[](const std::string& key) {
        if (!is_object())
            data = object{};
        return std::get<object>(data)[key];
    }

    value& operator[](size_t i) {
        if (!is_array())
            throw std::runtime_error("value is not an array");
        return std::get<array>(data)[i];
    }

    void push_back(const value& v) {
        if (!is_array())
            data = array{};
        std::get<array>(data).push_back(v);
    }

    const variant& raw() const { return data; }
};


// File Helpers

inline std::string read_file(const std::string& path)
{
    std::ifstream f(path);
    if (!f)
        throw std::runtime_error("cannot open file");

    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

inline void write_file(const std::string& path, const std::string& text)
{
    std::ofstream f(path, std::ios::trunc); // overwrite existing file
    if (!f)
        throw std::runtime_error("cannot write file");

    f << text;
}


inline std::string trim(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && std::isspace((unsigned char)s[start]))
        start++;

    size_t end = s.size();
    while (end > start && std::isspace((unsigned char)s[end - 1]))
        end--;

    return s.substr(start, end - start);
}

inline value parse_scalar(const std::string& v)
{
    if (v == "true") return value(true);
    if (v == "false") return value(false);

    bool number = !v.empty();

    for (char c : v)
        if (!std::isdigit(c) && c != '.' && c != '-')
            number = false;

    if (number)
        return value(std::stod(v));

    return value(v);
}


inline value parse(const std::string& text) // parse values
{
    std::stringstream ss(text);
    std::string line;

    value root(object{});
    std::vector<object*> stack;
    stack.push_back(&root.as_object());

    std::vector<int> indent_stack;
    indent_stack.push_back(0);

    while (std::getline(ss, line))
    {
        if (trim(line).empty()) // if line is empty don't do shit
            continue;

        int indent = 0;
        while (indent < (int)line.size() && line[indent] == ' ')
            indent++;

        std::string content = trim(line);

        auto colon = content.find(':');
        if (colon == std::string::npos)
            throw std::runtime_error("invalid TOON line: " + line);

        std::string key = trim(content.substr(0, colon));
        std::string val = trim(content.substr(colon + 1));

        while (indent < indent_stack.back())
        {
            stack.pop_back();
            indent_stack.pop_back();
        }

        if (val.empty())
        {
            value& obj = (*stack.back())[key];
            obj = object{};

            stack.push_back(&obj.as_object());
            indent_stack.push_back(indent + 2);
        }
        else
        {
            (*stack.back())[key] = parse_scalar(val);
        }
    }

    return root;
}

inline value parse_file(const std::string& path)
{
    return parse(read_file(path));
}


inline void encode(const value& v, std::ostream& out, int indent); // declaration

inline void encode_object(const object& obj, std::ostream& out, int indent)
{
    for (auto& [k, val] : obj)
    {
        out << std::string(indent, ' ') << k << ":";

        if (val.is_object())
        {
            out << "\n";
            encode(val, out, indent + 2);
        }
        else
        {
            out << " ";
            encode(val, out, 0);
            out << "\n";
        }
    }
}

inline void encode_array(const array& arr, std::ostream& out, int indent)
{
    for (auto& v : arr)
    {
        out << std::string(indent, ' ') << "- ";
        encode(v, out, 0);
        out << "\n";
    }
}

inline void encode(const value& v, std::ostream& out, int indent)
{
    const auto& d = v.raw();

    if (std::holds_alternative<std::string>(d))
        out << std::get<std::string>(d);
    else if (std::holds_alternative<double>(d))
        out << std::get<double>(d);
    else if (std::holds_alternative<bool>(d))
        out << (std::get<bool>(d) ? "true" : "false");
    else if (std::holds_alternative<object>(d))
        encode_object(std::get<object>(d), out, indent);
    else if (std::holds_alternative<array>(d))
        encode_array(std::get<array>(d), out, indent);
    else
        out << "null";
}

inline std::string dump(const value& v)
{
    std::stringstream ss;
    encode(v, ss, 0);
    return ss.str();
}



inline void write_file(const std::string& path, const value& v)
{
    write_file(path, dump(v));
}

} // namespace toon