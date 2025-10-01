#include <bits/stdc++.h>
using namespace std;

//AST node for controlling
struct Node {
    virtual string to_cpp() = 0;
    virtual ~Node() = default;
};

// trim helper
static inline string trim_copy(const string &s) {
    const string ws = " \t\n\r";
    size_t start = s.find_first_not_of(ws);
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

struct Statement : Node {
    string content;
    bool auto_semicolon;
    Statement(string c, bool semi = true) : content(c), auto_semicolon(semi) {}
    string to_cpp() override {
        string out = content;
        if (!auto_semicolon) return out; // comment-only hoặc đã yêu cầu không thêm ;

        // ignore preprocessor lines
        if (!out.empty() && out[0] == '#') return out;

        // nếu đã có ; ở cuối, {, } thì không động đến
        string trimmed = trim_copy(out);
        if (trimmed.empty()) return out;

        // nếu có '//' (inline comment), chèn ; trước '//' nếu cần
        size_t pos_slash = out.find("//");
        if (pos_slash != string::npos) {
            // find position to insert semicolon before the comment =>skip trailing spaces----
            size_t insert_pos = pos_slash;
            while (insert_pos > 0 && isspace((unsigned char)out[insert_pos-1])) --insert_pos;
            if (insert_pos == 0 || out[insert_pos-1] != ';') {
                out.insert(insert_pos, ";");
            }
            return out;
        }

        // nếu có '{' hoặc '}', tránh thêm ;
        if (out.find('{') != string::npos || out.find('}') != string::npos) return out;

        // nếu đã kết thúc bằng ; thì ok
        size_t end = out.find_last_not_of(" \t\r\n");
        if (end != string::npos && out[end] == ';') return out;

        // nếu không, thêm ; ở cuối
        out += ";";
        return out;
    }
};

struct DumaNode : Node {
    vector<string> parts;
    DumaNode(const vector<string>& p) : parts(p) {}
    string to_cpp() override {
        stringstream ss;
        ss << "std::cout";
        for (auto &p : parts) {
            ss << " << " << p; // token or literal
        }
        ss << ";";
        return ss.str();
    }
};


struct DumeNode : Node {
    vector<string> vars;
    DumeNode(const vector<string>& v) : vars(v) {}
    string to_cpp() override {
        stringstream ss;
        ss << "std::cin";
        for (auto &v : vars) ss << " >> " << v;
        ss << ";";
        return ss.str();
    }
};


//Parser line to line
vector<string> split_parts(const string& line) {
    vector<string> parts;
    size_t i = 0;
    while (i < line.size()) {
        while (i < line.size() && isspace(line[i])) i++;

        if (i >= line.size()) break;

        if (line[i] == '"') { //literal starting with " 
            // literal bắt đầu bằng "
            size_t start = i;
            i++; // bỏ dấu " --- remove "  
            while (i < line.size() && line[i] != '"') i++;
            if (i < line.size()) i++; // bao gồm dấu " --combine ""
            parts.push_back(line.substr(start, i - start));
        } else {
            // if normal token/ bình thường: it can be a valuable or operator
            size_t start = i;
            while (i < line.size() && !isspace(line[i])) i++;
            parts.push_back(line.substr(start, i - start));
        }
    }
    return parts;
}

// helper: join remaining parts with spaces
static string join_from(const vector<string>& parts, size_t start) {
    string out;
    for (size_t k = start; k < parts.size(); ++k) {
        if (!out.empty()) out += " ";
        out += parts[k];
    }
    return out;
}

vector<Node*> parse_line(const string &line) {
    vector<Node*> nodes;
    string s = trim_copy(line);

    // empty line
    if (s.empty()) {
        nodes.push_back(new Statement(""));
        return nodes;
    }

    // if the entire line is a block comment entry like "/* ... */"
    if (s.size() >= 2 && s[0] == '/' && s[1] == '*') {
        nodes.push_back(new Statement(s, false));
        return nodes;
    }

    // quick single-line comment-only
    if (s.rfind("//", 0) == 0) {
        nodes.push_back(new Statement(s, false));
        return nodes;
    }

    auto parts = split_parts(s);
    if (parts.empty()) return nodes;

    // Pre-process tokens that contain '//' 
    // split to 2 part left is code, right is comment
    for (size_t idx = 0; idx < parts.size(); ++idx) {
        size_t pos = parts[idx].find("//");
        if (pos != string::npos) {
            string left = parts[idx].substr(0, pos);
            string comment_rest = parts[idx].substr(pos); // begins with //
            if (idx + 1 < parts.size()) {
                comment_rest += " " + join_from(parts, idx + 1);
            }
            vector<string> nparts;
            for (size_t j = 0; j < idx; ++j) nparts.push_back(parts[j]);
            if (!left.empty()) nparts.push_back(left);
            nparts.push_back(comment_rest);
            parts.swap(nparts);
            break;
        }
    }

    size_t i = 0;
    while (i < parts.size()) {
        const string &tok = parts[i];

        // if token is a comment starting here -> push comment node and finish line
        if (tok.rfind("//", 0) == 0) {
            nodes.push_back(new Statement(tok, false));
            break;
        }

        if (tok == "duma") {
            i++;
            vector<string> p;
            bool saw_comment = false;
            while (i < parts.size() && parts[i] != "duma" && parts[i] != "dume") {
                if (parts[i].rfind("//", 0) == 0) {
                    // push comment condition
                    nodes.push_back(new DumaNode(p));
                    nodes.push_back(new Statement(parts[i], false));
                    saw_comment = true;
                    i = parts.size(); // end processing line
                    break;
                }
                p.push_back(parts[i++]);
            }
            if (!saw_comment) {
                nodes.push_back(new DumaNode(p));
            }
        } else if (tok == "dume") {
            i++;
            vector<string> v;
            bool saw_comment = false;
            while (i < parts.size() && parts[i] != "duma" && parts[i] != "dume") {
                if (parts[i].rfind("//", 0) == 0) {
                    nodes.push_back(new DumeNode(v));
                    nodes.push_back(new Statement(parts[i], false));
                    saw_comment = true;
                    i = parts.size();
                    break;
                }
                v.push_back(parts[i++]);
            }
            if (!saw_comment) {
                nodes.push_back(new DumeNode(v));
            }
        } else {
            // normal statement: reconstruct remaining tokens into one statement
            string stmt;
            for (size_t j = i; j < parts.size(); ++j) {
                if (!stmt.empty()) stmt += " ";
                stmt += parts[j];
            }
            nodes.push_back(new Statement(stmt));
            break;
        }
    }

    return nodes;
}

//parse all line
vector<Node*> parse_lines(const vector<string>& lines) {
    vector<Node*> nodes;
    for (auto &line : lines) {
        auto ns = parse_line(line);
        nodes.insert(nodes.end(), ns.begin(), ns.end());
    }
    return nodes;
}

// split cmm_code into logical lines and keeping /* ... */ as single entries
vector<string> explode_lines_preserve_block_comments(const string &cmm_code) {
    vector<string> out;
    size_t i = 0, n = cmm_code.size();
    string cur;
    while (i < n) {
        // if starts block comment
        if (i + 1 < n && cmm_code[i] == '/' && cmm_code[i+1] == '*') {
            // flush current line (if any)
            if (!cur.empty()) {
                out.push_back(cur);
                cur.clear();
            }
            // capture whole block comment (may include newlines)
            size_t start = i;
            i += 2;
            while (i + 1 < n && !(cmm_code[i] == '*' && cmm_code[i+1] == '/')) i++;
            if (i + 1 < n) i += 2; // include closing */
            out.push_back(cmm_code.substr(start, i - start));
            // consume following newline if present
            if (i < n && cmm_code[i] == '\n') i++;
            continue;
        }

        // normal line accumulation until newline
        if (cmm_code[i] == '\n') {
            out.push_back(cur);
            cur.clear();
            i++;
        } else {
            cur.push_back(cmm_code[i++]);
        }
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}


// helper: last non-space char
static char last_non_space_char(const string &s) {
    for (int i = (int)s.size()-1; i >= 0; --i) {
        if (!isspace((unsigned char)s[i])) return s[i];
    }
    return '\0';
}

// helper: count unescaped double-quotes -> " <- in a string
static int count_unescaped_quotes(const string &s) {
    int cnt = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '"') {
            // count preceding backslashes
            size_t j = i;
            int back = 0;
            while (j > 0 && s[j-1] == '\\') { back++; --j; }
            if (back % 2 == 0) cnt++; // not escaped
        }
    }
    return cnt;
}


// Join physical lines into logical lines so statements that span lines are kept together
vector<string> make_logical_lines(const vector<string>& phys_lines) {
    vector<string> out;
    size_t n = phys_lines.size();
    for (size_t i = 0; i < n; ++i) {
        string cur = phys_lines[i];
        string trimmed = trim_copy(cur);

        // if this is an exact block comment entry like "/*...*/" 
        // or empty line, or preprocessor, or single-line comment, push it now skiping
        if (trimmed.empty() || (trimmed.size() >= 2 && trimmed[0] == '/' && trimmed[1] == '*') ||
            trimmed.rfind("//", 0) == 0 || trimmed.rfind("#", 0) == 0) {
            out.push_back(cur);
            continue;
        }

        // Otherwise try to extend until we hit a statement terminator
        while (true) {
            string ttrim = trim_copy(cur);

            // if contains // anywhere, treat code before // as final for statement
            size_t pos_sl = cur.find("//");
            if (pos_sl != string::npos) {
                // keep whole line
                break;
            }

            // if starts with preprocessor -> stop
            if (!ttrim.empty() && ttrim[0] == '#') break;

            char last = last_non_space_char(cur);

            // if last char is ; or { or } -> this logical line is complete
            if (last == ';' || last == '{' || last == '}') break;

            // if odd number of unescaped quotes -> unterminated string literal -> must continue
            int qcount = count_unescaped_quotes(cur);
            if (qcount % 2 == 1) {
                if (i+1 < n) {
                    // check next line before appending
                    string next_trim = trim_copy(phys_lines[i+1]);
                    if (!next_trim.empty() && (next_trim[0] == '{' || next_trim[0] == '}' ||
                        next_trim.rfind("//",0) == 0 || next_trim.rfind("/*",0) == 0 || next_trim[0] == '#')) {
                        // next line is a brace/comment/preprocessor stop
                        break;
                    }
                    cur += "\n";
                    cur += phys_lines[++i];
                    continue;
                } else break;
            }


          
            // if last non-space char is '"' (closing string) and no semicolon -> allow adjacent literal on next line
            if (last == '"') {
                if (i+1 < n) {
                    // check next line before appending
                    string next_trim = trim_copy(phys_lines[i+1]);
                    if (!next_trim.empty() && (next_trim[0] == '{' || next_trim[0] == '}' ||
                        next_trim.rfind("//",0) == 0 || next_trim.rfind("/*",0) == 0 || next_trim[0] == '#')) {
                        break;
                    }
                    cur += "\n";
                    cur += phys_lines[++i];
                    continue;
                } else break;
            }


            //if next line exists and begins with operator that continues an expression
            // (common: <<, >>, or starts with a string-literal),
            // then join it to current logical line
            if (i+1 < n) {
                string next_trim = trim_copy(phys_lines[i+1]);
                if (!next_trim.empty()) {
                    // starts with << or >> or starts with a string literal
                    // if next line is { } or comment just break it
                    if (next_trim[0] == '{' || next_trim[0] == '}' || next_trim.rfind("//",0)==0 || next_trim.rfind("/*",0)==0) break;
                    if (next_trim.rfind("<<", 0) == 0 ||
                        next_trim.rfind(">>", 0) == 0 ||
                        next_trim.rfind("\"", 0) == 0) {
                        cur += "\n";
                        cur += phys_lines[++i];
                        continue;
                    }

                }
            }

            // otherwise treat as complete logical line 
            break;
        }

        out.push_back(cur);
    }
    return out;
}


//Compiler reading code and parse to node
string compile_cmm(const string& cmm_code) {
    // 1 -----split physical lines but keeping block comments "/*...*/" is 1 entry
    auto phys_lines = explode_lines_preserve_block_comments(cmm_code);

    // 2 -----combine physical lines to logical lines (format exactly code)
    //    (keeping statement can have many line such as cout << "a" << "b"; and literal side by side)
    auto logical_lines = make_logical_lines(phys_lines);

    // 3 -----parse logical lines to nodes (this is definition to change std::cin std::cout => duma dume)
    auto nodes = parse_lines(logical_lines);

    // 4 -----export indent by seeing '{' / '}'
    stringstream out;
    int indent_level = 0;
    for (auto node : nodes) {
        string cpp_line = node->to_cpp();
        string trimmed = trim_copy(cpp_line);

        // decrease indent if line have '}'
        if (!trimmed.empty() && trimmed[0] == '}') {
            indent_level = max(0, indent_level - 1);
        }

        // prepend indent
        for (int t = 0; t < indent_level; ++t) out << "    ";

        out << cpp_line << "\n";

        // increase indent if line have '{'
        if (cpp_line.find('{') != string::npos) {
            indent_level++;
        }
    }

    // Delete node
    for (auto n : nodes) delete n;
    return out.str();
}

//Main func using method
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input.cmm> [--run] [--keep-cpp]\n";
        return 1;
    }

    string input_file = argv[1];
    bool keep_cpp = false;
    bool run_bin = false;

    for (int i = 2; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--keep-cpp") keep_cpp = true;
        if (arg == "--run") run_bin = true;
    }

    ifstream fin(input_file);
    if (!fin) {
        cerr << "Cannot open file: " << input_file << "\n";
        return 1;
    }

    string cmm_code((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();

    string cpp_code = compile_cmm(cmm_code);

    // change base extension .cmm -> .cpp
    string base = input_file;
    size_t pos = base.rfind('.');
    if (pos != string::npos) base = base.substr(0, pos);
    string output_cpp = base + ".cpp";
    string output_bin = base;

    ofstream fout(output_cpp);
    fout << cpp_code;
    fout.close();

    cout << "Generate file " << output_cpp << " complete.\n";

    // call g++ automatic
    string cmd = "g++ " + output_cpp + " -o " + output_bin;
    int ret = system(cmd.c_str());
    if (ret != 0) {
        cerr << "Compile fail.\n";
        return 1;
    }
    cout << "Compile to binary: " << output_bin << "\n";

    if (!keep_cpp) {
        remove(output_cpp.c_str());
        cout << "Delete temp_file: " << output_cpp << "\n";
    }

    if (run_bin) {
        cout << ">>> Running...\n\n";
        string run_cmd = "./" + output_bin;
        system(run_cmd.c_str());
    }

    return 0;
}
