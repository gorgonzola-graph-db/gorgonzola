import re
import sys
from collections import defaultdict

def fix_issues(log_file):
    # Parse log file
    file_lines_to_fix = defaultdict(list)
    with open(log_file, 'r') as f:
        for line in f:
            match = re.match(r'^(.*?):(\d+):(\d+): (.*?): (.*?) \[(.*?)\]$', line.strip())
            if match:
                filepath, lineno, col, severity, msg, rule = match.groups()
                file_lines_to_fix[filepath].append({
                    'line': int(lineno),
                    'col': int(col),
                    'rule': rule,
                    'msg': msg
                })

    for filepath, issues in file_lines_to_fix.items():
        try:
            with open(filepath, 'r') as f:
                content = f.readlines()
        except FileNotFoundError:
            continue

        changed = False
        # Process from bottom to top to avoid messing up line numbers if we were to add/remove lines (we don't, but it's safe)
        issues.sort(key=lambda x: x['line'], reverse=True)

        for issue in issues:
            lineno = issue['line'] - 1  # 0-indexed
            if lineno >= len(content):
                continue
                
            line_content = content[lineno]
            
            if issue['rule'] == 'cstyleCast':
                # e.g., (Type*)ptr -> reinterpret_cast<Type*>(ptr)
                # Need a smart regex for C-style cast.
                # Common patterns in this codebase: ((LEFT_TYPE*)left.getData())[lPos], *(common::ValueVector*)resultValueVector
                # (void*)&inputVector, (unary_udf_func)udfFunc
                new_line = re.sub(r'\(\s*([a-zA-Z0-9_:]+\s*\*)\s*\)\s*([a-zA-Z0-9_&>]+(?:\.getData\(\))?)', 
                                  r'reinterpret_cast<\1>(\2)', 
                                  line_content)
                if new_line != line_content:
                    content[lineno] = new_line
                    changed = True

            elif issue['rule'] == 'returnByReference':
                # e.g., std::string getName() const -> const std::string& getName() const
                new_line = re.sub(r'\b(std::string|std::vector<[\w:]+>|expression_vector)\s+([a-zA-Z0-9_]+)\(', 
                                  r'const \1& \2(', 
                                  line_content)
                if new_line != line_content:
                    content[lineno] = new_line
                    changed = True

            elif issue['rule'] == 'passedByValue':
                # e.g., std::string name -> const std::string& name
                new_line = re.sub(r'\b(std::string|std::vector<[\w:]+>)\s+([a-zA-Z0-9_]+)\b(?!\s*\()', 
                                  r'const \1& \2', 
                                  line_content)
                if new_line != line_content:
                    content[lineno] = new_line
                    changed = True

            elif issue['rule'] == 'noExplicitConstructor':
                # e.g. int128_t(int64_t value); -> explicit int128_t(int64_t value);
                new_line = re.sub(r'^\s*([a-zA-Z0-9_]+)\(', r'    explicit \1(', line_content)
                if new_line != line_content:
                    content[lineno] = new_line
                    changed = True

            elif issue['rule'] == 'useInitializationList':
                # It's tricky to automatically move to initialization list, so we skip automation for this for now.
                pass

        if changed:
            with open(filepath, 'w') as f:
                f.writelines(content)
            print(f"Fixed issues in {filepath}")

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python fix_cppcheck.py <log_file>")
        sys.exit(1)
    fix_issues(sys.argv[1])
