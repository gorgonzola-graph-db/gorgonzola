import os

def main():
    repo_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    print(f"Starting rename in: {repo_root}")

    exclude_dirs = {'.git', 'build', 'third_party'}
    exclude_files = {'rename_to_gorgonzola.py'}

    all_files = []
    all_dirs = []

    # 1. Collect all files and directories
    for root, dirs, files in os.walk(repo_root, topdown=True):
        # Modify dirs in-place to skip excluded directories
        dirs[:] = [d for d in dirs if d not in exclude_dirs]
        
        for file in files:
            if file in exclude_files:
                continue
            all_files.append(os.path.join(root, file))
        
        for d in dirs:
            all_dirs.append(os.path.join(root, d))

    # 2. Process file contents (search and replace)
    print("Processing file contents...")
    for file_path in all_files:
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
            
            new_content = content
            replacements = [
                ("kuzu", "gorgonzola"),
                ("Kuzu", "Gorgonzola"),
                ("KUZU", "GORGONZOLA")
            ]
            
            replaced = False
            for old, new in replacements:
                if old in new_content:
                    new_content = new_content.replace(old, new)
                    replaced = True
            
            if replaced:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(new_content)
        except Exception as e:
            print(f"Skipping content replacement for {file_path} due to error: {e}")

    # 3. Rename files containing 'kuzu'
    print("Renaming files...")
    renamed_files = []
    for file_path in all_files:
        dir_name, file_name = os.path.split(file_path)
        new_file_name = file_name
        replacements = [
            ("kuzu", "gorgonzola"),
            ("Kuzu", "Gorgonzola"),
            ("KUZU", "GORGONZOLA")
        ]
        replaced = False
        for old, new in replacements:
            if old in new_file_name:
                new_file_name = new_file_name.replace(old, new)
                replaced = True
        
        if replaced:
            new_path = os.path.join(dir_name, new_file_name)
            try:
                os.rename(file_path, new_path)
                print(f"Renamed file: {file_path} -> {new_path}")
            except Exception as e:
                print(f"Error renaming file {file_path}: {e}")

    # 4. Rename directories containing 'kuzu' (bottom-up to preserve paths)
    print("Renaming directories...")
    all_dirs.sort(key=len, reverse=True) # Sort by length descending to do bottom-up
    for dir_path in all_dirs:
        parent_dir, dir_name = os.path.split(dir_path)
        new_dir_name = dir_name
        replacements = [
            ("kuzu", "gorgonzola"),
            ("Kuzu", "Gorgonzola"),
            ("KUZU", "GORGONZOLA")
        ]
        replaced = False
        for old, new in replacements:
            if old in new_dir_name:
                new_dir_name = new_dir_name.replace(old, new)
                replaced = True
        
        if replaced:
            new_path = os.path.join(parent_dir, new_dir_name)
            try:
                os.rename(dir_path, new_path)
                print(f"Renamed directory: {dir_path} -> {new_path}")
            except Exception as e:
                print(f"Error renaming directory {dir_path}: {e}")

    print("Rename process completed!")

if __name__ == "__main__":
    main()
